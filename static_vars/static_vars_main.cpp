
#include <unistd.h>

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

#include <base/oam_types.h>
#include <base/oam_vsmem.h>
#include <base/oam_time.h>
#include <base/oam_task.h>
#include <base/oam_comm.h>

#include "static_vars_target.h"

#define HI_BYTE(num) (((num) & 0x0000FF00) >> 8)
#define LO_BYTE(num)  ((num) & 0x000000FF)

static void print_kernel_durations(
  const std::string   & name,
  std::vector<double> & durations);


int computation_at_host(int size)
{
  int result = 0;
  for (int i = 0; i < size; i++) {
    if (i % (size / 10) == 0) {
      printf("[ host ] at index %d / %d\n", i, size - 1);
    }
    result += (i * result) % 11;
  }
  return result;
}

int main(int argc, char *argv[])
{
  int  array_size         = 10240;
  int  num_host_repeat    = 30000;
  int  num_target_repeat  = 50000;
  int  timeout_after_us   = 10000000;
  int  poll_interval_us   = 30;
  bool cancel_task        = false;

  for (int argi = 1; argi < argc; argi += 2) {
    if (strcmp(argv[argi], "-b") == 0) {
      array_size        = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-tr") == 0) {
      num_target_repeat = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-hr") == 0) {
      num_host_repeat   = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-to") == 0) {
      timeout_after_us  = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-pi") == 0) {
      poll_interval_us  = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-c") == 0) {
      cancel_task       = true;
      argi++;
    }
  }

  /* ---------------------------------------------------------------------- *
   * Symmetric allocation of arrays:                                        *
   * ---------------------------------------------------------------------- */
  std::cout << "Allocating arrays (2 x " << array_size << " bytes = "
            << std::setprecision(2) << std::fixed
            << static_cast<double>(2 * array_size) / 1024 / 1024 << " MB)"
            << std::endl;

  char * array_a = (char *)(oam_vsmem__symmetric_malloc(array_size));
  char * array_b = (char *)(oam_vsmem__symmetric_malloc(array_size));

  for (int i = 0; i < array_size; i++) {
    array_a[i] = (char)(i % 32);
    array_b[i] = 0;
  }

  std::cout << "Writing values to static array (base value: 1):"
            << std::endl;
  target_init(1);
  target_print_nvalues(10);

  std::cout << "Writing values to static array (base value: 3):"
            << std::endl;
  target_init(3);
  target_print_nvalues(10);

  HostMessage * host_signals = oam_comm__host_signals_new(
                                 poll_interval_us,
                                 timeout_after_us);

  /* ---------------------------------------------------------------------- *
   * Run kernels on DSPs and record time to completion at host:             *
   * ---------------------------------------------------------------------- */

  /* Kernel execution time in single repeats for median and stddev: */
  std::vector<double> target_durations;
  std::future<int>    target_future;

  target_future = std::async(std::launch::async,
                             target_function,
                               array_a,
                               array_b,
                               array_size,
                               num_target_repeat,
                               host_signals);

  computation_at_host(array_size * num_host_repeat);

  ts_t ts_cancel_req_start = oam_timestamp();
  ts_t ts_cancel_req_end   = oam_timestamp();
  if (cancel_task) {
    ts_cancel_req_start = oam_timestamp();
    oam_task__request_cancel(host_signals);
    oam_comm__flush_signals(host_signals);
    ts_cancel_req_end   = oam_timestamp();
    std::cout << "[ host ] -- [ "
              << std::setprecision(3) << std::fixed
              << oam_timestamp() << "s ] "
              << "Cancel request sent"  << std::endl;
  }

  int  result      = target_future.get();
  ts_t ts_task_end = oam_timestamp();

  if (cancel_task) {
    std::cout << "[ host ] -- [ "
              << std::setprecision(3) << std::fixed
              << oam_timestamp() << "s ] "
              << "request latency:      "
              << std::setprecision(3) << std::fixed
              << (ts_cancel_req_end - ts_cancel_req_start) * 1.0e6 << "us "
              << std::endl;
    std::cout << "[ host ] -- [ "
              << std::setprecision(3) << std::fixed
              << oam_timestamp() << "s ] "
              << "cancellation latency: "
              << std::setprecision(3) << std::fixed
              << (ts_task_end - ts_cancel_req_start) * 1.0e6 << "us"
              << std::endl;
  }

  std::cout << "Task exit code:        " << host_signals->ret << std::endl;
  std::cout << "The irrelevant result: " << result            << std::endl;

  target_print_nvalues(10);

  if (host_signals->ret == OMPACC_TASK__OK) {
    // Verify output array if task ran to completion:
    for (int i = 0; i < array_size; i++) {
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

  oam_vsmem__symmetric_free(array_a);
  oam_vsmem__symmetric_free(array_b);

  oam_comm__host_signals_delete(host_signals);

  return EXIT_SUCCESS;
}

std::ostream & operator<<(
  std::ostream & os,
  OMPAccTaskRet  ret)
{
  os << "(" << ret << "):";
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

