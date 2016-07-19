
#include <base/oam_types.h>
#include <base/oam_time.h>
#include <base/oam_vsmem.h>
#include <base/oam_comm.h>
#include <base/oam_task.h>

#include "poll_cancel_target.h"

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>

#include <omp.h>
#include <unistd.h>


#define HI_BYTE(num) (((num) & 0x0000FF00) >> 8)
#define LO_BYTE(num)  ((num) & 0x000000FF)

typedef struct run_params_t
{
  int  array_size;
  int  num_host_repeat;
  int  num_target_repeat;
  int  timeout_after_ms;
  int  poll_interval_ms;
  bool cancel_task;
}
RunParams;

static void print_usage(const RunParams * params);

std::ostream & operator<<(
  std::ostream & os,
  OMPAccTaskRet  ret);

void parse_args(int argc, char * argv[], RunParams * pout)
{
  pout->array_size        = 10240;
  pout->num_host_repeat   = 100;
  pout->num_target_repeat = 10;
  pout->timeout_after_ms  = 10000;
  pout->poll_interval_ms  = 500;
  pout->cancel_task       = false;

  for (int argi = 1; argi < argc; argi += 2) {
    if (strcmp(argv[argi], "-b") == 0) {
      pout->array_size =
        std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-tr") == 0) {
      pout->num_target_repeat =
        std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-hr") == 0) {
      pout->num_host_repeat =
        std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-to") == 0) {
      pout->timeout_after_ms =
        std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-pi") == 0) {
      pout->poll_interval_ms =
        std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-c") == 0) {
      pout->cancel_task = true;
      argi++;
    }
  }
}

static inline double timestamp()
{
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts); /* or: CLOCK_MONOTONIC */
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return (timestamp / 1.0e9);
}


int computation_at_host(int size)
{
  int result = 0;
  for (int i = 0; i < size; i++) {
    if (i % (size / 10) == 0) {
      int perc_complete =
        (static_cast<double>(i) / static_cast<double>(size)) * 100.0;
      std::cout << "[ host ] [ "
                << std::setprecision(3) << std::fixed
                << oam_timestamp() << "s ] "
                << std::setw(3) << perc_complete << "% completed"
                << std::endl;
    }
    if (i % 23) {
      result += (i * result) % 11;
    }
  }
  return result;
}

int main(int argc, char *argv[])
{
  RunParams params;
  parse_args(argc, argv, &params);

  print_usage(&params);

  /* ---------------------------------------------------------------------- *
   * Symmetric allocation of arrays:                                        *
   * ---------------------------------------------------------------------- */
  char * array_a = (char *)(oam_vsmem__symmetric_malloc(params.array_size));
  char * array_b = (char *)(oam_vsmem__symmetric_malloc(params.array_size));

  for (int i = 0; i < params.array_size; i++) {
    array_a[i] = (char)(i % 32);
    array_b[i] = 0;
  }

  HostMessage * host_signals = oam_comm__host_signals_new(
                                 params.poll_interval_ms,
                                 params.timeout_after_ms);

  /* ---------------------------------------------------------------------- *
   * Run kernels on DSPs and record time to completion at host:             *
   * ---------------------------------------------------------------------- */

  /* Kernel execution time in single repeats for median and stddev: */
  std::future<int> target_future;

  /* Start target task asynchronously: */
  target_future = std::async(std::launch::async,
                             target_function,
                               array_a,
                               array_b,
                               params.array_size,
                               params.num_target_repeat,
                               host_signals);

  /* Computation at host parallel to target task: */
  computation_at_host(params.array_size / 2 * params.num_host_repeat);

  std::cout << "[ host ] -- [ "
            << std::setprecision(3) << std::fixed
            << oam_timestamp() << "s ] "
            << "Setting param to 999" << std::endl;
  host_signals->param  = 999;

  /* Asynchronous cancellation of target task, if requested: */
  ts_t ts_cancel = oam_timestamp();
  if (params.cancel_task) {
    std::cout << "[ host ] -- [ "
              << std::setprecision(3) << std::fixed
              << oam_timestamp() << "s ] "
              << "Canceling target task"
              << std::endl;
    ts_cancel = oam_timestamp();
    oam_task__request_cancel(host_signals);
    usleep(10);
    oam_comm__flush_signals(host_signals);
  }

  /* Block until completion of target task: */
  int result = target_future.get();

  ts_t ts_target_ret = oam_timestamp();

  if (params.cancel_task) {
    std::cout << "[ host ] -- [ "
              << std::setprecision(3) << std::fixed
              << ts_target_ret << "s ] "
              << "Cancellation latency: "
              << std::setprecision(3) << std::fixed
              << (ts_target_ret - ts_cancel) * 1.0e6 << "us"
              << std::endl;
  }
  std::cout << "[ host ] -- [ "
            << std::setprecision(3) << std::fixed
            << oam_timestamp() << "s ] "
            << "Completed target task"
            << std::endl;

  std::cout << "[ host ] -- Task exit code:        " << host_signals->ret
            << std::endl;
  std::cout << "[ host ] -- The irrelevant result: " << result
            << std::endl;

  if (host_signals->ret == OMPACC_TASK__OK) {
    std::cout << "[ host ] -- Verifying result ..." << std::endl;
    // Verify output array if task ran to completion:
    for (int i = 0; i < params.array_size; i++) {
      if (array_b[i] != array_a[i] + 100) {
        std::cout << "array_b[" << i << "] mismatch: "
                  << (int)(array_b[i]) << " != "
                  << (int)(array_a[i]) << " + 100"
                  << std::endl;
        break;
      }
    }
  }

  /* ---------------------------------------------------------------------- *
   * Finalize:                                                              *
   * ---------------------------------------------------------------------- */
  oam_comm__host_signals_delete(host_signals);

  oam_vsmem__symmetric_free(array_a);
  oam_vsmem__symmetric_free(array_b);

  return EXIT_SUCCESS;
}

static void print_usage(const RunParams * params)
{
  std::cout << "Options:"
            << std::endl
            << "  -b <buffer size> Bytes to allocate per repetition"
            << std::endl
            << "  -hr <num rep>    Number of repetitions at host"
            << std::endl
            << "  -tr <num rep>    Number of repetitions at target"
            << std::endl
            << "  -to <ms>         Timeout duration of target task in ms"
            << std::endl
            << "  -pi <ms>         Polling interval at target task in ms"
            << std::endl
            << std::endl
            << "Running:"
            << std::endl
            << "  poll_cancel.bin"
            << " -b "  << params->array_size
            << " -hr " << params->num_host_repeat
            << " -tr " << params->num_target_repeat
            << " -to " << params->timeout_after_ms
            << " -pi " << params->poll_interval_ms;
  if (params->cancel_task) {
    std::cout << " -c";
  }
  std::cout << std::endl
            << std::endl;
}

std::ostream & operator<<(
  std::ostream & os,
  OMPAccTaskRet  ret)
{
  os << "(" << static_cast<int>(ret) << "):";
  switch (ret) {
    case OMPACC_TASK__OK:
      os << "OK"; break;
    case OMPACC_TASK__CANCEL:
      os << "CANCEL"; break;
    case OMPACC_TASK__TIMEOUT:
      os << "TIMEOUT"; break;
    default:
      os << "unknown"; break;
  }
  os << std::endl;
  return os;
}

