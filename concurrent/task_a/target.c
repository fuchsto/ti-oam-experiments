#define SYMMETRIC_MEMORY__USE_DDR

#include "target.h"

#include <oam/macro.h>
#include <oam/logging.h>
#include <oam/oam_config.h>
#include <oam/oam_types.h>
#include <oam/oam_task_target.h>


/* ======================================================================== *
 * Begin of Target Function Declarations and Includes                       *
 * ------------------------------------------------------------------------ */
#pragma omp declare target

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include "ti_omp_device.h"

uint32_t __core_num(void);

#pragma omp end declare target
/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */

#define ACC_INCREMENT(x) 1


int task_a_target(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_signals)
{
  int acc;
  #pragma omp target map(to:     in_buffer[0:size], size) \
                     map(tofrom: out_buffer[0:size], acc, \
                                 host_signals[0:1])
  {
    int    i, r;
    int    aborted;
    double ts_start = omp_get_wtime();

    LOG_DEBUG(">>> task_a_target()");

    acc = 0;
//  pragma omp parallel \
//             private(i, aborted) \
//             shared(host_signals, acc)
    {
      aborted = 0;
      for (r = 0; r < repeats; r++) {
//      pragma omp for
        for (i = 0; i < size; i++) {
          // Poll cancellation request:
          if (0 == oam_task__poll_cancel_request(
                     ts_start, host_signals, &aborted)) {
            out_buffer[i] = in_buffer[i] + 100;
          }
        }
      }
    }
    LOG_DEBUG("<<< task_a_target()");
  } // omp target

  return acc;
}

