
#include "static_vars_target.h"

#include <base/oam_config.h>
#include <base/oam_types.h>
#include <base/oam_task.h>
#include <base/oam_task_target.h>
#include <base/logging.h>
#include <base/macro.h>

#define NValues 100

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

static float static_buffer[100];

#pragma omp end declare target
/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */

void target_init(float value)
{
  #pragma omp target data map(to: value)
  {
    int i;
    #pragma omp parallel for schedule(static)
    for (i = 0; i < NValues; i++) {
      static_buffer[i] = value * (i+1);
    }
  }
}

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
        double limit_ms = (double)(host_signals->timeout_after_ms);
        double time_max = time_start + (limit_ms / 1000.0);
        printf("target time start: %.3f max. ms: %d -> time limit: %.3f\n",
               time_start, host_signals->timeout_after_ms, time_max);

        #pragma omp parallel \
                    shared(host_signals, acc)
        {
          int aborted = 0;
          int i;
          #pragma omp for schedule(static)
          for (i = 0; i < size; i++) {
            // Poll cancellation request:
            if (0 != i % 10 ||
                0 == oam_task__poll_cancel_request(
                       time_start, host_signals, &aborted)) {
              // Work step implementation here
              out_buffer[i] = static_buffer[i % NValues] += in_buffer[i];
            }
          }
        }
        printf("target section 1: leaving\n");
      } // omp target
    }

    // Synchronize with host:
    oam_task__step(host_signals);

    printf("target data section: leaving\n");
  } // omp target data

  return acc;
}

