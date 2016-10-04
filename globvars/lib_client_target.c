
#include "lib_client_target.h"
#include "lib_owner_host.h"
#include "lib_owner_target.h"

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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <omp.h>

#include "ti_omp_device.h"

#pragma omp end declare target
/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */

int target_function(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  HostMessage * host_signals)
{
  int acc = 0;
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
        printf("client target section:\n");
        printf("client target global_host_string: '%s':\n",
               global_host_string);
        printf("client target global_target_string: '%s'\n",
               global_target_string);
        strcat(global_target_string, "-X");
      } // omp target
    }

    printf("target data section: leaving\n");
  } // omp target data

  return acc;
}

