
#include "minimal_target.h"

#include <base/oam_config.h>
#include <base/oam_types.h>
#include <base/oam_task.h>
#include <base/oam_task_target.h>
#include <base/logging.h>
#include <base/macro.h>

/* ======================================================================== *
 * Begin of Target Function Declarations and Includes                       *
 * ------------------------------------------------------------------------ */
#pragma omp declare target

#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include "ti_omp_device.h"

static inline int add_static(int a, int b) {
  return a + b;
}

#pragma omp end declare target
/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */

int target_function(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_signals)
{
  wno_unused_var_(repeats);

  int acc;
  #pragma omp target data map(to:     in_buffer[0:size], size) \
                          map(from:   out_buffer[0:size], acc) \
                          map(tofrom: host_signals[0:1])
  {
    oam_task__enter(host_signals);

    if (!oam_task__aborted(host_signals)) {
      #pragma omp target map(to:     in_buffer[0:size], size) \
                         map(from:   out_buffer[0:size], acc) \
                         map(tofrom: host_signals[0:1])
      {
        acc = 0;
        double time_start = omp_get_wtime();
        double limit_ms = (double)(host_signals->timeout_after_us) / 1000;
        double time_max = time_start + (limit_ms / 1000.0);
        printf("target time start: %.3f max. ms: %d -> time limit: %.3f\n",
               time_start, host_signals->timeout_after_us, time_max);

        #pragma omp parallel \
                    shared(host_signals, acc)
        {
          int aborted = 0;
          // Poll cancellation request:
          if (0 == oam_task__poll_cancel_request(
                     time_start, host_signals, &aborted)) {
            // Work step implementation here
            out_buffer[0] = add_static(in_buffer[0], 100);
            // ...
          }
          if (0 == oam_task__poll_cancel_request(
                     time_start, host_signals, &aborted)) {
            // Work step implementation here
            out_buffer[0] = add_static(in_buffer[0], 100);
            // ...
          }
          #pragma omp atomic
          acc += out_buffer[0];
        }
        printf("target section 1: leaving\n");
      } // omp target
    }

    // Synchronize with host:
    oam_task__step(host_signals);

    if (!oam_task__aborted(host_signals)) {
      #pragma omp target map(to:     in_buffer[0:size], size) \
                         map(tofrom: out_buffer[0:size], acc, \
                                     host_signals[0:1])
      {
        double time_start = omp_get_wtime();
        double limit_ms = (double)(host_signals->timeout_after_us) / 1000;
        double time_max = time_start + (limit_ms / 1000.0);
        printf("target time start: %.3f max. ms: %d -> time limit: %.3f\n",
               time_start, host_signals->timeout_after_us, time_max);

        #pragma omp parallel \
                    shared(host_signals, acc)
        {
          int aborted = 0;
          // Poll cancellation request:
          if (0 == oam_task__poll_cancel_request(
                     time_start, host_signals, &aborted)) {
            // Work step implementation here
            out_buffer[0] = add_static(in_buffer[0], 100);
            // ...
          }
          if (0 == oam_task__poll_cancel_request(
                     time_start, host_signals, &aborted)) {
            // Work step implementation here
            out_buffer[0] = add_static(in_buffer[0], 100);
            // ...
          }
          #pragma omp atomic
          acc += out_buffer[0];
        }
        printf("target section 2: leaving\n");
      } // omp target
    }

    printf("target data section: leaving\n");
  } // omp target data

  return acc;
}

