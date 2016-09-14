#ifndef DSP_IMG_FILTER__SYMMETRIC_MEMORY_H__INCLUDED
#define DSP_IMG_FILTER__SYMMETRIC_MEMORY_H__INCLUDED


#define SYMMETRIC_MEMORY__USE_DDR

#pragma omp declare target
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>
#include "ti_omp_device.h"
#pragma omp end declare target

/* Note:
 *
 * MSMC for host/target shared memory is supported for K2x devices only.
 * See:
 * http://downloads.ti.com/mctools/esd/docs/opencl/memory/memory-model.html
 *
 * For a detailed reference of the zero-copy heap extensions, see:
 *
 * http://downloads.ti.com/mctools/esd/docs/opencl/memory/host-malloc-extension.html
 * http://processors.wiki.ti.com/index.php/OpenMP_OpenCL_DSP_Heap_Management
 *
 */

static inline void * symmetric_malloc(int nbytes)
{
#ifdef SYMMETRIC_MEMORY__USE_DDR
  return __malloc_ddr(nbytes);
#else
  return __malloc_msmc(nbytes);
#endif
}

static inline void symmetric_free(void * pmem)
{
#ifdef SYMMETRIC_MEMORY__USE_DDR
  __free_ddr(pmem);
#else
  __free_msmc(pmem);
#endif
}

static inline void symmetric_heap_init(void * pmem, size_t nbytes)
{
  char * pmem_c = (char *)(pmem);

  #pragma omp target map(to: nbytes, pmem_c[0:nbytes])
  {
#ifdef SYMMETRIC_MEMORY__USE_DDR
    __heap_init_ddr((char *)(pmem_c), nbytes);
#else
    __heap_init_msmc((char *)(pmem_c), nbytes);
#endif
  }
}

#endif // DSP_IMG_FILTER__SYMMETRIC_MEMORY_H__INCLUDED
