
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

#include <oam/oam_types.h>
#include <oam/oam_vsmem.h>
#include <oam/oam_time.h>
#include <oam/oam_task.h>
#include <oam/oam_comm.h>

#include "lib_client_target.h"

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
  return result;
}

int main(int argc, char *argv[])
{
  int array_size       = 128;
  int poll_interval_us = 300;
  int timeout_after_us = 30 * 1.0e6;

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
                               host_signals);

  int result = target_future.get();

  std::cout << "Task exit code:        " << host_signals->ret << std::endl;
  std::cout << "The irrelevant result: " << result            << std::endl;

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

