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

#include <base/symmetric_memory.h>

#pragma omp end declare target

void data_map_target(
  char * in_buffer,
  char * out_buffer,
  int    size)
{
  #pragma omp target map(to:   in_buffer[0:size], size) \
                     map(from: out_buffer[0:size])
  {
    #pragma omp single
    {
      out_buffer[0] += in_buffer[0];
    }
  }
}

void sym_alloc_target(
  int    size_a,
  int    size_b)
{
  #pragma omp target map(to: size_a, size_b)
  {
    int size = 5; // size_a; // + size_b;

    int    i, k;
    char * buffer = symmetric_malloc(size);

    printf("Target: symmetric_malloc(%d) -> %p\n", size, (void *)buffer);

    for (i = 0; i < size; i++) {
      buffer[i] = 100 + (i % 10);
      printf("buffer[%d] = %d\n", i, buffer[i]);
    }
    for (k = 0; k < size; k++) {
      char expect = 100 + (k % 10);
      if (buffer[k] != expect) {
        printf("Test of buffer value at offset %d failed: got %d, "
               "expected %d\n", k, (int)buffer[k], expect);
        break;
      }
    }
    symmetric_free(buffer);
  }
}


