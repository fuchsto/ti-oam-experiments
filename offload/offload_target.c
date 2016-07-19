#define SYMMETRIC_MEMORY__USE_DDR

#include "offload_target.h"

/*
 * Type definitions and macros do not have to be included in a
 * `omp declare target` region.
 */
#include <base/config.h>
#include <base/types.h>
#include <base/macro.h>
#include <base/logging.h>


/* ======================================================================== *
 * Begin of Target Function Declarations and Includes                       *
 * ------------------------------------------------------------------------ *
 *                                                                          *
 * Definition of functions and variables used within the offloaded DSP      *
 * kernel routine.                                                          *
 *                                                                          *
 * See TI documentation:                                                    *
 * downloads.ti.com/mctools/esd/docs/openmpacc/supported_constructs.html    *
 *                                                                          *
 *      "The 'declare target' construct specifies that variables and        *
 *       functions are mapped to a device. Each function specified in a     *
 *       declare target region must have definitions for both the host      *
 *       and target device."                                                *
 *                                                                          *
 * ------------------------------------------------------------------------ */
#pragma omp declare target

  /*
   * Place includes of function definition headers in `omp declare target`
   * region to map included definitions to target devices.
   */
# include <stdlib.h>
# include <stdio.h>
# include <omp.h>

#pragma omp end declare target

void offload_target(
  char * in_buffer,
  char * out_buffer,
  int    size)
{
  #pragma omp target map(to:   in_buffer[0:size], size) \
                     map(from: out_buffer[0:size])
  {
    #pragma omp single
    {
      out_buffer[0]        += in_buffer[0];
//    out_buffer[size - 1] += in_buffer[size - 1];
    }
  }
}

