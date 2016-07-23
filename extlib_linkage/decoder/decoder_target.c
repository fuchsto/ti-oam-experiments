
#include "decoder_target.h"

#include <base/oam_config.h>
#include <base/oam_types.h>
#include <base/oam_comm.h>
#include <base/oam_task.h>
#include <base/oam_task_target.h>

#include <base/macro.h>
#include <base/logging.h>


#include "decoder_target_fun_a.h"

/* ======================================================================== *
 * Begin of Target Function Declarations and Includes                       *
 * ------------------------------------------------------------------------ */
#pragma omp declare target

#  include <stdlib.h>
#  include <stdio.h>
#  include <omp.h>

#  include "ti_omp_device.h"

#pragma omp end declare target
/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */

int decoder_body(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_signals);

/**
 * Adapter function executed at host.
 * Maps buffers from host address space to targets and implements program
 * flow control.
 */
unsigned int decode_target(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_signals)
{
  int acc;

  printf("[ %.3fs ] >> decode(): "
         "devices: %d default device: %d initial device: %s\n",
         omp_get_wtime(),
         omp_get_num_devices(), omp_get_default_device(),
         omp_is_initial_device() ? "yes" : "no");

  #pragma omp target data map(to:     in_buffer[0:size], size) \
                          map(from:   out_buffer[0:size], acc) \
                          map(to:     host_signals[0:1])
  {
    oam_task__enter(host_signals);

    acc = 0;

    if (0 == oam_task__aborted(host_signals)) {
//      printf("[ %.3fs ] << target data section: step 1\n", omp_get_wtime());
      acc += decoder_body(in_buffer, out_buffer, size, repeats, host_signals);
      oam_task__step(host_signals);
    if (0 == oam_task__aborted(host_signals)) {
//      printf("[ %.3fs ] << target data section: step 2\n", omp_get_wtime());
      acc += decoder_body(in_buffer, out_buffer, size, repeats, host_signals);
      oam_task__step(host_signals);
    if (0 == oam_task__aborted(host_signals)) {
//      printf("[ %.3fs ] << target data section: step 3\n", omp_get_wtime());
      acc += decoder_body(in_buffer, out_buffer, size, repeats, host_signals);
      oam_task__step(host_signals);
    if (0 == oam_task__aborted(host_signals)) {
//      printf("[ %.3fs ] << target data section: step 4\n", omp_get_wtime());
      acc += decoder_body(in_buffer, out_buffer, size, repeats, host_signals);
      oam_task__step(host_signals);
    }
    }
    }
    }

    printf("[ %.3fs ] << target data section: leaving, acc:%d, param:%d\n",
           omp_get_wtime(), acc, host_signals->param);
  } // omp target data

  return (unsigned int)(acc);
}

/**
 * Implementation of a single task step, executed at target.
 *
 * Buffers passed as arguments (here: \c in_buffer, \c out_buffer) must
 * have be mapped to target address space in an enclosing omp target data
 * region.
 */
int decoder_body(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_signals)
{
  int acc;

  if (oam_task__aborted(host_signals)) {
    return 0;
  }

  #pragma omp target map(to:     in_buffer[0:size], size) \
                     map(from:   out_buffer[0:size], acc) \
                     map(tofrom: host_signals[0:1])
  {
    int i;
    int r;
    double ts_start = omp_get_wtime();
    double limit_ms = (double)(host_signals->timeout_after_ms);
    double time_max = (host_signals->time_start_ms + limit_ms) * 1.0e-3;
    acc = 0;

//  printf("[ %.3fs ] >> decoder_body: %.3f max.ms: %d "
//         "-> time limit: %.3f repeats: %d\n",
//         ts_start, host_signals->time_start_ms * 1.0e-3,
//         host_signals->timeout_after_ms,
//         time_max, repeats);
//
//  printf("[ %.3fs ] >> decoder_body: processor cores: %d "
//         "max. number of OpenMP threads: %d\n",
//         omp_get_wtime(), omp_get_num_procs(), omp_get_max_threads());

    int aborted;
/*
    #pragma omp parallel \
                private(i, r, aborted) \
                shared(host_signals, acc)
*/
    {
      aborted = 0;
//    #pragma omp for
      for (i = 0; i < size * repeats; i++) {
        // Poll cancellation request:
        if (0 == aborted &&
            0 == oam_task__poll_cancel_request(
                   ts_start, host_signals, &aborted)) {
          // Print progress:
          if (i % ((size * repeats) / 10) == (int)(__core_num())) {
            printf("[ %.3fs ] ++ decoder_body: %d / %d, acc:%d, param:%d\n",
                   omp_get_wtime(), i, size-1, acc, host_signals->param);
          }
          acc += 1;
        }
      } // omp for
    } // omp parallel
//  printf("[ %.3fs ] << target section: leaving, acc:%d, param:%d\n",
//         omp_get_wtime(), acc, host_signals->param);
  } // omp target

  return acc;
}

