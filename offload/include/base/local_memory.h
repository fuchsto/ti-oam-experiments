#ifndef DSP_IMG_FILTER__LOCAL_MEMORY_H__INCLUDED
#define DSP_IMG_FILTER__LOCAL_MEMORY_H__INCLUDED

#pragma omp declare target
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "ti_omp_device.h"
#pragma omp end declare target

typedef enum LOCAL_MEMORY__ALLOC_TYPE_t
{
  LOCAL_MEMORY__ALLOC_TARGET = 0,
  LOCAL_MEMORY__ALLOC_CORE
}
LOCAL_MEMORY__ALLOC_TYPE;

/*
 * For a detailed reference of the target-local heap extensions, see:
 *
 * http://downloads.ti.com/mctools/esd/docs/opencl/memory/host-malloc-extension.html
 * http://processors.wiki.ti.com/index.php/OpenMP_OpenCL_DSP_Heap_Management
 *
 */

/**
 * User-controlled DSP heaps in L2 SRAM are local to the core that initialized
 * the heap. Heaps initialized in L2 SRAM do not persist across target regions
 * or kernels.
 */
static inline void * local_malloc(int nbytes, LOCAL_MEMORY__ALLOC_TYPE atype)
{
  char * local_pmem = NULL;
  if (atype == LOCAL_MEMORY__ALLOC_TARGET) {
    #pragma omp target map(to:nbytes) map(local:local_pmem[0:nbytes])
    {
      local_pmem = (char *)(__malloc_l2(nbytes));
    }
  } else {
    #pragma omp target map(to:nbytes) map(from:local_pmem[0:nbytes])
    {
      local_pmem = (char *)(__malloc_l2(nbytes));
    }
  }
  return local_pmem;
}

static inline void local_free(void * pmem)
{
  return __free_l2(pmem);
}

static inline void local_heap_init(void * pmem, int nbytes)
{
  // Unused dummy pointer required for function interface.
  char * p = NULL;
  #pragma omp target map(to:nbytes) map(local:p[0:nbytes])
  {
    __heap_init_l2((char *)(pmem), nbytes);
  }
}

#endif // DSP_IMG_FILTER__LOCAL_MEMORY_H__INCLUDED
