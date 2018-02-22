
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>
#include <algorithm>
#include <numeric>
#include <future>
#include <chrono>

#include <omp.h>

#include <unistd.h>

#include <oam/oam_types.h>
#include <oam/oam_vsmem.h>
#include <oam/oam_time.h>
#include <oam/oam_task.h>
#include <oam/oam_comm.h>

#include "dsp_freq_target.h"
#include "dsp_freq_types.h"

#define HI_BYTE(num) (((num) & 0x0000FF00) >> 8)
#define LO_BYTE(num)  ((num) & 0x000000FF)

static void print_kernel_durations(
  const std::string   & name,
  std::vector<double> & durations);


int main(int argc, char *argv[])
{
  int  sym_heap_mb        = 32;
  int  num_host_repeat    = 100;
  int  num_target_repeat  = 10;
  int  poll_interval_ms   = 500;
  int  timeout_after_ms   = 10000;
  int  cancel_after_ms    = 0;

  for (int argi = 1; argi < argc; argi += 2) {
    if (strcmp(argv[argi], "-mb") == 0) {
      sym_heap_mb       = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
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
      cancel_after_ms   = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
  }

  long sym_heap_size = 1024 * 1024 * sym_heap_mb;

  /* ---------------------------------------------------------------------- *
   * Symmetric allocation of arrays:                                        *
   * ---------------------------------------------------------------------- */
  std::cout << "Allocating symmetric heap (" << sym_heap_size
            << " bytes = " << sym_heap_mb << " MB)"
            << std::endl;

  char * sym_heap = (char *)(oam_vsmem__symmetric_malloc(sym_heap_size));

  HostMessage * host_signals = oam_comm__host_signals_new(
                                 poll_interval_ms * 1000,
                                 timeout_after_ms * 1000);

  /* ---------------------------------------------------------------------- *
   * Run kernels on DSPs and record time to completion at host:             *
   * ---------------------------------------------------------------------- */

  /* Kernel execution time in single repeats for median and stddev: */
  std::vector<double> target_durations;
  std::future<int>    target_future;

  std::cout << "---- cancellation ms: -c  " << cancel_after_ms  << '\n';
  std::cout << "---- timeout ms:      -to " << timeout_after_ms << '\n';

  if (timeout_after_ms) {
     oam_task__set_default_timeout(timeout_after_ms * 1000);
  }

  target_measurements msr = { };

  auto ts_start = std::chrono::system_clock::now();

  std::thread target_thread(
                target_task
                , &msr
                , host_signals);

  if (cancel_after_ms) {
    std::cout << "Cancel request in " << cancel_after_ms << " ms ..."
              << std::endl;

    usleep(cancel_after_ms * 1000);
    oam_task__request_cancel(host_signals);

    std::cout << "Cancel request sent" << std::endl;
  }

  target_thread.join();

  auto ts_end = std::chrono::system_clock::now();

  auto target_lat_ms = std::chrono::duration_cast<std::chrono::microseconds>(
                          ts_end - ts_start
                       ).count();

  auto target_duration = (msr.host_omp_wtime_end - msr.host_omp_wtime_start);

  std::cout << "Target exit code:           " << host_signals->ret
            << '\n';
  std::cout << "Task latency:               " << target_lat_ms
            << '\n';
     
  std::cout << "---> target_clock64_count   " << msr.target_clock64_count
            << '\n';
  std::cout << "---> target_omp_wtime_start " << msr.target_omp_wtime_start
            << '\n';
  std::cout << "---> target_omp_wtime_end   " << msr.target_omp_wtime_end
            << '\n';
  std::cout << "---> host_omp_wtime_start   " << msr.host_omp_wtime_start
            << '\n';
  std::cout << "---> host_omp_wtime_end     " << msr.host_omp_wtime_end
            << '\n';

  std::cout << "----------------------------\n";
  std::cout << "task execution time measured on target: "
            << (msr.target_omp_wtime_end - msr.target_omp_wtime_start)
            << '\n';
  std::cout << "task execution time measured on host:   "
            << (msr.host_omp_wtime_end - msr.host_omp_wtime_start)
            << '\n';
  std::cout << "target CPU frequency:                   "
            << ( (static_cast<double>(msr.target_clock64_count)
                   / target_duration)
                 * 1.0e-6 )
            << std::endl;

  /* ---------------------------------------------------------------------- *
   * Finalize:                                                              *
   * ---------------------------------------------------------------------- */

  oam_vsmem__symmetric_free(sym_heap);

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

