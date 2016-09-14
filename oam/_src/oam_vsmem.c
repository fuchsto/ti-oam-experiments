
#include <oam/oam_vsmem.h>
#include <oam/oam_config.h>
#include <oam/macro.h>

#include <string.h>

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
 */
#define SYMMETRIC_MEMORY__USE_DDR

#pragma omp declare target
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <oam/logging.h>

#include <omp.h>

#include "ti_omp_device.h"

int    snprintf(char * , size_t, const char *, ...);
int    printf(const char *, ...);
char * strrchr(const char *, int);

void * memcpy(
  void                * dst,
  const void          * src,
  unsigned int          nbytes);

void * memset(
  void                * dst,
  int                   value,
  size_t                nbytes);

#pragma omp end declare target

#ifndef MV_LOG__OAM__VSMEM
#  undef  LOG_DEBUG
#  define LOG_DEBUG(...) do { } while(0)
#endif

void * oam_vsmem__set(
  void                * dst,
  const unsigned char   value,
  unsigned int          nbytes)
{
  LOG_DEBUG("oam_vsmem__set: dst:(%p) value:(%d) nbytes:(%d)",
            dst, value, nbytes);

  return memset(dst, value, nbytes);
}

/* ======================================================================= *
 * Symmetric memory shared between host and targets                        *
 * ======================================================================= */

void * oam_vsmem__symmetric_malloc(
  unsigned int   nbytes)
{
  void * pmem;
  LOG_DEBUG("oam_vsmem__symmetric_malloc(nbytes:%d)", nbytes);
#ifdef SYMMETRIC_MEMORY__USE_DDR
  pmem = __malloc_ddr(nbytes);
#else
  pmem = __malloc_msmc(nbytes);
#endif
  LOG_DEBUG("oam_vsmem__symmetric_malloc(nbytes:%d) -> 0x%p", nbytes, pmem);
  return pmem;
}

void oam_vsmem__symmetric_free(
  void         * pmem)
{
  LOG_DEBUG("oam_vsmem__symmetric_free(0x%p)\n", pmem);
#ifdef SYMMETRIC_MEMORY__USE_DDR
  __free_ddr(pmem);
#else
  __free_msmc(pmem);
#endif
}

void oam_vsmem__symmetric_heap_init(
  void         * pmem,
  unsigned int   nbytes)
{
  char * pmem_c = (char *)(pmem);
  #pragma omp target map(to: nbytes, pmem_c[0:nbytes])
  {
    LOG_DEBUG("oam_vsmem__heap_init(0x%p, %d)\n", pmem, nbytes);
#ifdef SYMMETRIC_MEMORY__USE_DDR
    __heap_init_ddr((char *)(pmem_c), nbytes);
#else
    __heap_init_msmc((char *)(pmem_c), nbytes);
#endif
  }
}

/* ======================================================================= *
 * Local memory in target L2 cache                                         *
 * ======================================================================= */

void * oam_vsmem__local_malloc(
  unsigned int   nbytes,
  OAM_VSMEM__LOCAL_ALLOC_TYPE atype)
{
  char * local_pmem = NULL;
  if (atype == ALLOC_TARGET) {
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

void oam_vsmem__local_heap_init(
  void         * pmem,
  unsigned int   nbytes)
{
  // Unused dummy pointer required for function interface.
  char * p = NULL;
  #pragma omp target map(to:nbytes) map(local:p[0:nbytes])
  {
    __heap_init_l2((char *)(pmem), nbytes);
  }
  wno_unused_var_(p);
}

