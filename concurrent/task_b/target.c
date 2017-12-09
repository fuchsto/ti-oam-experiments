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


int task_b_target(
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
    int    i;
    int    aborted;
    double ts_start = omp_get_wtime();

    acc = 0;
    #pragma omp parallel \
                private(i, aborted) \
                shared(host_signals, acc)
    {
      aborted = 0;
      #pragma omp for
      for (i = 0; i < repeats; i++) {
        // Poll cancellation request:
        if (0 == oam_task__poll_cancel_request(
                   ts_start, host_signals, &aborted)) {
          // Print progress:
          if (i % (repeats / 10) == (int)(__core_num())) {
            #pragma omp atomic
            acc += ACC_INCREMENT(acc);
          }
          out_buffer[size  / (i+1)] = in_buffer[repeats / i] + 100;
        }
      }
    }
  } // omp target

  return acc;
}

