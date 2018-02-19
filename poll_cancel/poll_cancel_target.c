
#include "poll_cancel_target.h"

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

#include <base/oam_time.h>

#include "ti_omp_device.h"

#pragma omp end declare target
/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */
extern void __ti_set_default_timeout(int device_num, int timeout_in_ms);
extern int  __ti_get_timeout_status(int device_num);

int target_function(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_signals)
{
  const int device_num = 0;
  int timeout_in_ms    = host_signals->timeout_after_us / 1000;

  wno_unused_var_(repeats);
  
  __ti_set_default_timeout(device_num, timeout_in_ms);

  LOG_DEBUG("target_function: timeout: %d ms", timeout_in_ms);

  int acc;
  int i;
  int r;
  #pragma omp target data map(to:     in_buffer[0:size], size) \
                          map(from:   out_buffer[0:size], acc) \
                          map(tofrom: host_signals[0:1])
  {
//  oam_task__enter(host_signals);

//  if (!oam_task__aborted(host_signals))
    {
      #pragma omp target map(to:     in_buffer[0:size], size) \
                         map(from:   out_buffer[0:size], acc) \
                         map(tofrom: host_signals[0:1])
      {
//      __ti_set_default_timeout(device_num, timeout_in_ms);

        for (r = 0; r < repeats; r++) {
          ts_t ts_start = oam_timestamp();
          int  aborted  = 0;
          acc           = 0;
          for (i = 0; i < size; i++) {
            // Poll for cancel request around single MAC operation:
            if (1 || 
                0 == oam_task__poll_cancel_request(
                       ts_start, host_signals, &aborted))
            {
              out_buffer[0] *= in_buffer[0] + 100;
            } else {
              acc++;
            }
          }
        }
      } // omp target
    }
//  oam_task__step(host_signals);
  } // omp target data

  return __ti_get_timeout_status(device_num);
}

