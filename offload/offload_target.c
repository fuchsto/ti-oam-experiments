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
  inner->nInts = n;
  inner->pInts = (int *)(oam_vsmem__symmetric_malloc(
                           inner->nInts * sizeof(int)));
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
  int i;
  SomeObject * pObj    = (SomeObject*)(
                            oam_vsmem__symmetric_malloc(sizeof(SomeObject)));
  if (pObj == NULL) {
    printf("!!! Failed to allocate offload object\n");
    return;
  }

  pObj->value          = size;

  // inner_h allocated at host:
  pObj->inner_h        = (Inner*)(
                            oam_vsmem__symmetric_malloc(sizeof(Inner)));
  Inner_init(pObj->inner_h, 5, 10);

  // inner_t allocated in target section:
  pObj->inner_t        = (Inner*)(
                            oam_vsmem__symmetric_malloc(sizeof(Inner)));
  Inner_init(pObj->inner_t, 5, 10);

  for (i = 0; i < pObj->inner_t->nInts; i++) {
    pObj->inner_t->pInts[i] = 9;
  }

  Inner * host__pObj_inner_t_pInts = pObj->inner_t->pInts;

  // ------------------------------------------------------------------------
  // Parameter packing:

  Inner * pObj_inner_h       = pObj->inner_h;
  int   * pObj_inner_h_pInts = pObj->inner_h->pInts;
  int     pObj_inner_h_nInts = pObj->inner_h->nInts;
  Inner * pObj_inner_t       = pObj->inner_t;
  int   * pObj_inner_t_pInts = pObj->inner_t->pInts;
  int     pObj_inner_t_nInts = pObj->inner_t->nInts;

  printf("data_map_target: in_buffer     |> %p[%d]\n", in_buffer, size);
  printf("data_map_target: inner_h       |> %p\n",     pObj_inner_h);
  printf("data_map_target: inner_h_pInts |> %p[%d]\n", pObj_inner_h_pInts,
                                                       pObj_inner_h_nInts);
  printf("data_map_target: inner_t       |> %p\n",     pObj_inner_t);
  printf("data_map_target: inner_t_pInts |> %p[%d]\n", pObj_inner_t_pInts,
                                                       pObj_inner_t_nInts);
  #pragma omp target map(to:     in_buffer[0:size], size)                  \
                     map(tofrom: pObj[0:1],                                \
                                 pObj_inner_h[0:1],                        \
                                 pObj_inner_h_pInts[0:pObj_inner_h_nInts], \
                                 out_buffer[0:size],                       \
                                 pObj_inner_t[0:1],                        \
                                 pObj_inner_t_pInts[0:pObj_inner_t_nInts])
  {
//  #pragma omp single
    {
      printf("data_map_target: in_buffer     -> %p[%d]\n", in_buffer, size);
      printf("data_map_target: inner_h       -> %p\n",     pObj_inner_h);
      printf("data_map_target: inner_h_pInts -> %p[%d]\n", pObj_inner_h_pInts,
                                                           pObj_inner_h_nInts);
      printf("data_map_target: inner_t       -> %p\n",     pObj_inner_t);
      printf("data_map_target: inner_t_pInts -> %p[%d]\n", pObj_inner_t_pInts,
                                                           pObj_inner_t_nInts);
      pObj->inner_h        = pObj_inner_h;
      pObj->inner_h->pInts = pObj_inner_h_pInts;
      pObj->inner_t        = pObj_inner_t;
      pObj->inner_t->pInts = pObj_inner_t_pInts;

      Inner_init(pObj->inner_t, 5, 6);

#if 0
      out_buffer[0] += in_buffer[0];
      int i;
      for (i = 0; i < min_(size, pObj->inner_h->nInts); i++) {
        out_buffer[i] += pObj->inner_h->pInts[i];
      }
#endif

      pObj_inner_h       = pObj->inner_h;
      pObj_inner_h_pInts = pObj->inner_h->pInts;
      pObj_inner_t       = pObj->inner_t;
      pObj_inner_t_pInts = pObj->inner_t->pInts;

      printf("data_map_target: in_buffer     <- %p[%d]\n", in_buffer, size);
      printf("data_map_target: inner_h       <- %p\n",     pObj_inner_h);
      printf("data_map_target: inner_h_pInts <- %p[%d]\n", pObj_inner_h_pInts,
                                                           pObj_inner_h_nInts);
      printf("data_map_target: inner_t       <- %p\n",     pObj_inner_t);
      printf("data_map_target: inner_t_pInts <- %p[%d]\n", pObj_inner_t_pInts,
                                                           pObj_inner_t_nInts);
    }
  }
  printf("data_map_target: in_buffer     <| %p[%d]\n", in_buffer, size);
  printf("data_map_target: inner_h       <| %p\n",     pObj_inner_h);
  printf("data_map_target: inner_h_pInts <| %p[%d]\n", pObj_inner_h_pInts,
                                                       pObj_inner_h_nInts);
  printf("data_map_target: inner_t       <| %p\n",     pObj_inner_t);
  printf("data_map_target: inner_t_pInts <| %p[%d]\n", pObj_inner_t_pInts,
                                                       pObj_inner_t_nInts);
  pObj->inner_h        = pObj_inner_h;
  pObj->inner_h->pInts = pObj_inner_h_pInts;
  pObj->inner_t        = pObj_inner_t;
  pObj->inner_t->pInts = pObj_inner_t_pInts;

  if (host__pObj_inner_t_pInts) {
    oam_vsmem__symmetric_free(host__pObj_inner_t_pInts);
  }

  printf("data_map_target: inner_h values:\n");
  for (i = 0; i < pObj->inner_h->nInts; i++) {
     printf("data_map_target:      %d", (int)(pObj->inner_h->pInts[i]));
  }
  printf("data_map_target: inner_t values:\n");
  for (i = 0; i < pObj->inner_t->nInts; i++) {
     printf("data_map_target:      %d", (int)(pObj->inner_t->pInts[i]));
  }
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


