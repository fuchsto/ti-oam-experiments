#define SYMMETRIC_MEMORY__USE_DDR

#include "offload_target.h"

/*
 * Type definitions and macros do not have to be included in a
 * `omp declare target` region.
 */
#include <oam/logging.h>
#include <oam/oam_vsmem.h>


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
    int   size  = 16; // size_a; // + size_b;

    if (size < sizeof(int)) { size = sizeof(int); }

    int   nelem = size / sizeof(int);

    int * buffer = oam_vsmem__symmetric_malloc(size);
    printf("target: oam_vsmem__symmetric_malloc(%d) -> %p\n",
           size, (void *)buffer);

    int   i;
    for (i = 0; i < nelem; i++) {
      buffer[i] = i + 1;
    }
    for (i = 0; i < nelem; i++) {
      int expect = i + 1;
      if (buffer[i] != expect) {
        printf("Test of buffer value at offset %d failed: got %d, "
               "expected %d\n", i, (int)buffer[i], expect);
        break;
      }
    }
    oam_vsmem__symmetric_free(buffer);
  }
}


