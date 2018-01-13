#define SYMMETRIC_MEMORY__USE_DDR

#include "sym_heap_reset_target.h"

#include <oam/macro.h>
#include <oam/logging.h>

#include <oam/oam_config.h>
#include <oam/oam_types.h>
#include <oam/oam_vsmem.h>
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


int target_task(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           leak_nbytes,
  int           repeats,
  HostMessage * host_signals);

int async_target(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           leak_nbytes,
  int           repeats,
  HostMessage * host_signals)
{
  int acc;
  #pragma omp target data map(to:   in_buffer[0:size], size) \
                          map(from: out_buffer[0:size], acc) \
                          map(to:   host_signals[0:1])
  {
    int i;
    int repeat = size / 100;
    acc = 0;
    oam_task__enter(host_signals);

    for (i = 0; i < repeat; i++) {
      acc += target_task(in_buffer, out_buffer, size, leak_nbytes,
                         repeats, host_signals);
      /* Send progress events after every 10%: */
      if (i % (repeat / 10) == (int)(__core_num())) {
//      oam_task__step(host_signals);
        printf("[ %.3fs ] ++ section: %d / %d, acc:%d, param:%d\n",
               omp_get_wtime(), i, repeat-1, acc,
               host_signals->param);
      }
      #pragma omp flush
      if (1 == host_signals->cancel) {
        printf("[ %.3fs ] ++ aborted at %d / %d, acc:%d, param:%d "
               "cancel:%d ret:%d\n",
               omp_get_wtime(), i, repeat-1, acc,
               host_signals->param, host_signals->cancel, host_signals->ret);
        break;
      }
    }
    printf("target data section: leaving\n");
  } // omp target data

  return acc;
}

int target_task(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           leak_nbytes,
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

    if (leak_nbytes > 0) {
      oam_vsmem__symmetric_malloc(leak_nbytes);
    }

    acc = 0;
    #pragma omp parallel \
                private(i, aborted) \
                shared(host_signals, acc)
    {
      aborted = 0;
      #pragma omp for
      for (i = 0; i < size; i++) {
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

