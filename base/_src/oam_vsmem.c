
#include <base/oam_vsmem.h>
#include <base/oam_config.h>
#include <base/macro.h>

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

#pragma omp declare target
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <omp.h>

#include "ti_omp_device.h"

void * memcpy(
  void         * dst,
  const void   * src,
  unsigned int   nbytes);

void * memset(
  void         * dst,
  int            value,
  size_t         nbytes);

#pragma omp end declare target

void * oam_vsmem__copy(
  void         * dst,
  const void   * src,
  unsigned int   nbytes)
{
  return memcpy(dst, src, nbytes);
}

void * oam_vsmem__set(
  void                * dst,
  const unsigned char   value,
  unsigned int          nbytes)
{
  return memset(dst, value, nbytes);
}

int oam_vsmem__freeMem(
  void                * paddr)
{
  free(paddr);
  return 1;
}

int oam_vsmem__freeTag(
  oam_vsmem__eMemTag    tag)
{
  return 1;
}

void * oam_vsmem__getMem(
  int                   segid,
  unsigned int          size,
  oam_vsmem__eMemTag    tag)
{
  return NULL;
}

void * oam_vsmem__getAlignedMem(
  int                   segid,
  unsigned int          size,
  oam_vsmem__eMemTag    tag,
  unsigned int          align,
  const char          * pinitializer)
{
  return NULL;
}

/* ======================================================================= *
 * Symmetric memory shared between host and targets                        *
 * ======================================================================= */

void * oam_vsmem__symmetric_malloc(
  unsigned int   nbytes)
{
  void * pmem;
#ifdef SYMMETRIC_MEMORY__USE_DDR
  pmem = __malloc_ddr(nbytes);
#else
  pmem = __malloc_msmc(nbytes);
#endif
  printf("[ %.3fs ] ** oam_vsmem__symmetric_malloc(%d) -> 0x%p\n",
         omp_get_wtime(), nbytes, pmem);
  return pmem;
}

void oam_vsmem__symmetric_free(
  void         * pmem)
{
  printf("[ %.3fs ] ** oam_vsmem__free(0x%p)\n",
         omp_get_wtime(), pmem);
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
    printf("[ %.3fs ] ** oam_vsmem__heap_init(0x%p, %d)\n",
           omp_get_wtime(), pmem, nbytes);
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

