
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

/* Note:
 *
 * MSMC for host/target shared memory is supported for K2x devices only.
 * See:
 * http://downloads.ti.com/mctools/esd/docs/opencl/memory/memory-model.html
 */
#define SYMMETRIC_MEMORY__USE_DDR

#include <oam/oam_types.h>
#include <oam/oam_time.h>
#include <oam/oam_vsmem.h>
#include <oam/oam_comm.h>
#include <oam/oam_task.h>

#include "sym_heap_reset_target.h"

#define HI_BYTE(num) (((num) & 0x0000FF00) >> 8)
#define LO_BYTE(num)  ((num) & 0x000000FF)

typedef struct run_params_t
{
  int  array_size;
  int  num_host_repeat;
  int  num_target_repeat;
  int  timeout_after_us;
  int  poll_interval_us;
  int  nbytes_leak_target;
  bool cancel_task;
}
RunParams;

static void print_usage(const RunParams * params);

std::ostream & operator<<(
  std::ostream & os,
  OMPAccTaskRet  ret);

void parse_args(int argc, char * argv[], RunParams * pout)
{
  pout->array_size         = 1024 * 1024 * 10;
  pout->num_host_repeat    = 100;
  pout->num_target_repeat  = 1;
  pout->timeout_after_us   = 3000000;
  pout->poll_interval_us   = 200;
  pout->cancel_task        = false;
  pout->nbytes_leak_target = 0;

  for (int argi = 1; argi < argc; argi += 2) {
    if (strcmp(argv[argi], "-b") == 0) {
      pout->array_size =
        std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    if (strcmp(argv[argi], "-l") == 0) {
      pout->nbytes_leak_target =
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
      pout->timeout_after_us =
        std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-pi") == 0) {
      pout->poll_interval_us =
        std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-c") == 0) {
      pout->cancel_task = true;
      argi++;
    }
  }
}


int main(int argc, char *argv[])
{
  RunParams params;
  parse_args(argc, argv, &params);

  print_usage(&params);

  int sym_heap_size = (params.array_size * 2) +
                      (1024 * 1024 * 1) +
                      params.nbytes_leak_target;
  int iterations    = params.num_host_repeat;

  bool passed       = false;

  while (iterations--) {
    passed                = false;
    char * symmetric_heap = (char *)(oam_vsmem__symmetric_malloc(
                                      sym_heap_size));

    if (symmetric_heap == NULL) {
      passed = false;
      std::cerr << "Failed to allocate symmetric heap" << std::endl;
      break;
    }

    oam_vsmem__symmetric_heap_init(symmetric_heap, sym_heap_size);

    /* ---------------------------------------------------------------------- *
     * Symmetric allocation of arrays:                                        *
     * ---------------------------------------------------------------------- */
    std::cout << "[host] -- Allocating arrays (2 x " << params.array_size
              << " bytes = "
              << std::setprecision(2) << std::fixed
              << static_cast<double>(2 * params.array_size) / 1024 / 1024
              << " MB)"
              << std::endl;

    char * array_a = (char *)(oam_vsmem__symmetric_malloc(params.array_size));
    char * array_b = (char *)(oam_vsmem__symmetric_malloc(params.array_size));

    for (int i = 0; i < params.array_size; i++) {
      array_a[i] = (char)(i % 32);
      array_b[i] = 0;
    }

    HostMessage * host_signals = oam_comm__host_signals_new(
                                   params.poll_interval_us,
                                   params.timeout_after_us);

    /* ---------------------------------------------------------------------- *
     * Run kernels on DSPs and record time to completion at host:             *
     * ---------------------------------------------------------------------- */

    /* Block until completion of target task: */
    int result = target_task(
                   array_a,
                   array_b,
                   params.array_size,
                   params.nbytes_leak_target,
                   params.num_target_repeat,
                   host_signals);

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

    oam_vsmem__symmetric_free(symmetric_heap);
    symmetric_heap = NULL;

    passed = true;
  }

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
            << "  -c               Cancel request after host task completion"
            << std::endl
            << std::endl
            << "Running:"
            << std::endl
            << "  sym_heap_reset.bin"
            << " -b "  << params->array_size
            << " -hr " << params->num_host_repeat
            << " -tr " << params->num_target_repeat
            << " -to " << params->timeout_after_us
            << " -pi " << params->poll_interval_us;
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

