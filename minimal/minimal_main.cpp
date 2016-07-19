
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

#include <base/types.h>
#include <base/symmetric_memory.h>
#include <base/timestamp.h>
#include <base/oam_task.h>

#include "minimal_target.h"

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
    if (i % 23) {
      result += (i * result) % 11;
    }
  }
}

int main(int argc, char *argv[])
{
  int  array_size         = 1024000;
  int  num_host_repeat    = 100;
  int  num_target_repeat  = 10;
  int  timeout_after_ms   = 10000;
  int  poll_interval_ms   = 500;
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
      timeout_after_ms  = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-pi") == 0) {
      poll_interval_ms  = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
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

  char * array_a = (char *)(symmetric_malloc(array_size));
  char * array_b = (char *)(symmetric_malloc(array_size));

  for (int i = 0; i < array_size; i++) {
    array_a[i] = (char)(i % 32);
    array_b[i] = 0;
  }

  HostMessage * host_signals = oam_task__host_signals_new(
                                 poll_interval_ms,
                                 timeout_after_ms);

  /* ---------------------------------------------------------------------- *
   * Run kernels on DSPs and record time to completion at host:             *
   * ---------------------------------------------------------------------- */

  /* Kernel execution time in single repeats for median and stddev: */
  std::vector<double> target_durations;
  std::future<int>    target_future;

  ts_t target_start = timestamp();

  target_future = std::async(std::launch::async,
                             target_function,
                               array_a,
                               array_b,
                               array_size,
                               num_target_repeat,
                               host_signals);

  computation_at_host(array_size / 2 * num_host_repeat);

  if (cancel_task) {
    oam_task__request_cancel(host_signals);
  }

  computation_at_host(array_size / 2 * num_host_repeat);

  int result = target_future.get();

  std::cout << "Task exit code:        " << host_signals->ret << std::endl;
  std::cout << "The irrelevant result: " << result            << std::endl;

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

  symmetric_free(array_a);
  symmetric_free(array_b);

  oam_task__host_signals_delete(host_signals);

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

