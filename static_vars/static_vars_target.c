
#include "static_vars_target.h"

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

static float static_buffer[100];

#pragma omp end declare target
/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */

void target_init(float value)
{
  #pragma omp target map(to:     value) \
                     map(tofrom: static_buffer)
  {
    int i;
    #pragma omp parallel for schedule(static)
    for (i = 0; i < StaticArraySize; i++) {
      float v = value * (i+1);
      static_buffer[i] += v;
    }
  }
}

void target_print_nvalues(int nvalues)
{
  #pragma omp target map(to: nvalues) \
                     map(to: static_buffer)
  {
    int i;
    int nv = nvalues < StaticArraySize
             ? nvalues
             : StaticArraySize;
    for (i = 0; i < nv; i++) {
      printf("%.4f ", static_buffer[i]);
    }
    printf("\n");
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
                         map(tofrom: host_signals[0:1], static_buffer)
      {
        acc = 0;
        double time_start = omp_get_wtime();
        double limit_ms = (double)(host_signals->timeout_after_ms);
        double time_max = time_start + (limit_ms / 1000.0);
        printf("target time start: %.3f max. ms: %d -> time limit: %.3f\n",
               time_start, host_signals->timeout_after_ms, time_max);

        int c_id;
        #pragma omp parallel \
                    private(c_id) \
                    shared(host_signals, acc)
        {
          c_id = omp_get_thread_num();
          int aborted = 0;
          int i, r;
          #pragma omp for schedule(static)
          for (i = 0; i < size; i++) {
            if (i % (size / 10) == c_id) {
              printf("[ target ] at index %d / %d\n", i, size - 1);
            }
            for (r = 0; r < repeats; r++) {
              // Poll cancellation request:
              if (0 == aborted &&
                  (c_id != i+r % 10 ||
                   0 == oam_task__poll_cancel_request(
                          time_start, host_signals, &aborted))) {
                // Work step implementation here
                static_buffer[i % 64] =
                   ((float)i) + ((float)r / 10.0);

                out_buffer[i] = static_buffer[i];
              } else {
                break;
              }
            }
          }
          printf("target section: leaving, canceled: %d\n",
                 host_signals->cancel);
        }
      } // omp target
    }

    // Synchronize with host:
    oam_task__step(host_signals);

    printf("target data section: leaving\n");
  } // omp target data

  return acc;
}

