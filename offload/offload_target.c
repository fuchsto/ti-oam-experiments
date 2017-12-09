#define SYMMETRIC_MEMORY__USE_DDR

#include "offload_target.h"

/*
 * Type definitions and macros do not have to be included in a
 * `omp declare target` region.
 */
#include <oam/logging.h>
#include <oam/oam_vsmem.h>

// #define min_(a,b) ((a)<(b) ? (a) : (b))


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

typedef struct _Inner {
  int    nums[10];
  int  * pInts;
  int    nInts;
} Inner;

typedef struct _SomeObject {
  int     value;
  Inner * inner_t;
  Inner * inner_h;
} SomeObject;

void Inner_init(Inner * inner, int n, int baseval) {
  inner->nInts = 5;
  inner->pInts = oam_vsmem__symmetric_malloc(
                   inner->nInts * sizeof(int)); 
  int i;
  for (i = 0; i < inner->nInts; i++) {
    inner->pInts[i] = (i + 1) * baseval;
  }
  for (i = 0; i < 10; i++) {
    inner->nums[i]   = (i + 1) * baseval;
  }
}

#pragma omp end declare target

void data_map_target(
  char * in_buffer,
  char * out_buffer,
  int    size)
{
  SomeObject * pObj    = oam_vsmem__symmetric_malloc(sizeof(SomeObject));
  pObj->value          = size;

  // inner_h allocated at host:
  pObj->inner_h        = oam_vsmem__symmetric_malloc(sizeof(Inner));
  Inner_init(pObj->inner_h, 5, 10);

  // inner_t allocated in target section:
  pObj->inner_t        = NULL;

  // ------------------------------------------------------------------------
  // Parameter packing:

  Inner * pObj_inner_h   = pObj->inner_h;
  int   * pInner_h_pInts = pObj->inner_h->pInts;
  int     pInner_h_nInts = pObj->inner_h->nInts;
  Inner * pObj_inner_t   = pObj->inner_t;
  int   * pInner_t_pInts = pObj->inner_t->pInts;
  int     pInner_t_nInts = pObj->inner_t->nInts;

  #pragma omp target map(to:     in_buffer[0:size], size)          \
                     map(tofrom: pObj[0:1],                        \
                                 pInner_h_pInts[0:pInner_h_nInts], \
                                 pInner_t_pInts[0:pInner_t_nInts]) \
                     map(from:   out_buffer[0:size])
  {
    #pragma omp single
    {
      pObj->inner_t = oam_vsmem__symmetric_malloc(sizeof(Inner));
      Inner_init(pObj->inner_t, 5, 6);

      out_buffer[0] += in_buffer[0];
      int i;
      for (i = 0; i < min_(size, pObj->inner_h->nInts); i++) {
        out_buffer[i] += pObj->inner_h->pInts[i];
      }

      pInner_h_pInts = pObj->inner_h->pInts;
      pInner_t_pInts = pObj->inner_t->pInts;
    }
  }
  pObj->inner_h = pInner_h;
  pObj->inner_t = pInner_t;
}

void sym_alloc_target(
  int    size_a,
  int    size_b)
{
  #pragma omp target map(to: size_a, size_b)
  {
    int   size  = size_a; // + size_b;

    if (size < sizeof(int)) { size = sizeof(int); }

    int   nelem = size / sizeof(int);

    int * buffer = oam_vsmem__symmetric_malloc(size);
 // printf("target: oam_vsmem__symmetric_malloc(%d) -> %p\n",
 //        size, (void *)buffer);

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


