
#include "dsp_freq_target.h"

#include <oam/oam_config.h>
#include <oam/oam_types.h>
#include <oam/oam_task.h>
#include <oam/oam_task_target.h>
#include <oam/logging.h>
#include <oam/macro.h>

/* ======================================================================== *
 * Begin of Target Function Declarations and Includes                       *
 * ------------------------------------------------------------------------ */
#pragma omp declare target

#include "dsp_freq_types.h"

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

// #include <unistd.h>

#include "ti_omp_device.h"

#pragma omp end declare target
/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */



void target_task(
  target_measurements * measurements,
  HostMessage         * host_signals)
{
  measurements->host_omp_wtime_start = omp_get_wtime();

  #pragma omp target map(tofrom: host_signals[0:1]) \
                     map(tofrom: measurements[0:1])
  {
    ts_t               oam_ts_start      = oam_timestamp();
    unsigned long long clk_ts_start      = __clock64();
    long               ts_start_us       = oam_ts_start * 1000 * 1000;
    int                aborted           = 0;
    measurements->target_omp_wtime_start = omp_get_wtime();

    while (!aborted) {
      measurements->target_clock64_count = __clock64() - clk_ts_start;
      measurements->target_omp_wtime_end = omp_get_wtime();

      oam_task__poll_cancel_request(ts_start_us, host_signals, &aborted);
    }
  }

  measurements->host_omp_wtime_end = omp_get_wtime();
}

