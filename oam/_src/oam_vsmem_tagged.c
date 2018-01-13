
#include <oam/logging.h>
#include <oam/oam_vsmem.h>
#include <oam/oam_config.h>
#include <oam/macro.h>

#if !defined(OMPACC)
#include <basefw/vsmem.h>
#endif

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
#include <stdarg.h>

#include <oam/logging.h>

#include <omp.h>

#if defined(OMPACC)
#include "ti_omp_device.h"
#endif

#if defined(OMPACC)
int EXTERNAL_ALGO_MEMORY = EXTERNAL_MEMORY;
#endif

#if defined(OMPACC)
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
#endif

#pragma omp end declare target

#if defined(MV_LOG__OAM__VSMEM)
  #warning Excessive OMP memory management debug output enabled
#else
  #undef  LOG_DEBUG
  #define LOG_DEBUG(...) do { } while(0)
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

int oam_vsmem__strcmp(
  const char          * str1,
  const char          * str2)
{
#if defined(OMPACC)
  return strcmp(str1, str2);
#else
  return VSMEM_strcmp(str1, str2);
#endif
}

int oam_vsmem__compare(
  const void * pData1,
  const void * pData2,
  unsigned int nbytes)
{
#if defined(OMPACC)
	if(memcmp(pData1, pData2, nbytes)) {
		return -1;
  }
  return 0;
#else
  return VSMEM_compare(pData1, pData2, nbytes);
#endif
}

int oam_vsmem__freeMem(
  void                * pMem)
{
  return VSMEM_freeMem(pMem);

  // No alternative target implementation required so far
#if 0
  if (NULL == pMem) {
    LOG_DEBUG("oam_vsmem__freeMem(%p) -> failed (-1)", pMem);
    return -1;
  }
#if defined(OMPACC_TARGET)
  oam_vsmem__symmetric_free(pMem);
#else
  return VSMEM_freeMem(pMem);
#endif
  return 1;
#endif
}

#if MEMORY_DEBUG_SUPPORTED
int oam_vsmem__freeMemDebug(
  void                * pMem)
{
  return VSMEM_freeMemDebug(pMem);

  // No alternative target implementation required so far
#if 0
  LOG_DEBUG("oam_vsmem__freeMem(%p)", pMem);
#if defined(OMPACC_TARGET)
  oam_vsmem__symmetric_free(pMem);
#else
  VSMEM_freeMemDebug(pMem);
#endif
  return 1;
}
#endif

int oam_vsmem__freeTag(
  oam_vsmem__eMemTag    tag)
{
  LOG_DEBUG("oam_vsmem__freeTag: tag:(%dp)", tag);
#if 0 && defined(OMPACC)
  return 1;
#else
  return VSMEM_freeTag(tag);
#endif
}

void * oam_vsmem__getMem(
  int                   segid,
  unsigned int          size,
  oam_vsmem__eMemTag    tag)
{
  void* pmem = 0;

#if defined(OMPACC_TARGET)
  pmem = oam_vsmem__getAlignedMem(segid, size, tag, 8, NULL);
#else
  pmem = VSMEM_getMem(segid, size, tag);
#endif
  LOG_DEBUG("oam_vsmem__getMem > (%p)", pmem);
  return pmem;
}

void * oam_vsmem__getAlignedMem(
  int                   segid,
  unsigned int          size,
  oam_vsmem__eMemTag    tag,
  unsigned int          align,
  const char          * pinitializer)
{
  LOG_DEBUG("oam_vsmem__getAlignedMem() "
            "segId:(%d) size:(%d) tag:(%d) align:(%d), pInit:(%p)",
            segid, size, tag, align, pinitializer);
  void * pmem = NULL;
#if defined(OMPACC_TARGET)
  pmem = oam_vsmem__symmetric_malloc(size);
#else
  pmem = VSMEM_getAlignedMem(segid, size, tag, align, pinitializer);
#endif
  LOG_DEBUG("oam_vsmem__getAlignedMem > (%p)", pmem);
  return pmem;
}

void * oam_vsmem__getMultiMem(
  int                   segid,
  oam_vsmem__eMemTag    tag,
  unsigned int          num,
  unsigned int          firstSize, ...)
{
  LOG_DEBUG("oam_vsmem__getMultiMem() "
            "segId:(%d) tag:(%d) num:(%d) firstSize:(%d)",
            segid, tag, num, firstSize);

  va_list      args;
  unsigned int totalsize = (firstSize + 7) & 0xFFFFFFF8;
  void *       baseptr;
  unsigned int i;

  va_start(args, firstSize);
  for (i = 1; i < num; i++) {
    unsigned int len = va_arg(args, unsigned int);
    void **      p0  = va_arg(args, void*);
    *p0              = NULL;
    totalsize       += ((len + 7) & 0xFFFFFFF8);
  }
  va_end(args);

  baseptr = oam_vsmem__getMem(segid, totalsize, tag);

  if (!baseptr) { return NULL; }

  va_start(args, firstSize);
  totalsize = firstSize;
  for (i = 1; i < num; i++) {
    unsigned int len = va_arg(args, unsigned int);
    void **      p0  = va_arg(args, void*);
    *p0              = (void *)(
                         ((unsigned int)baseptr + totalsize + 7) & 0xFFFFFFF8
                       );
    totalsize       += ((len + 7) & 0xFFFFFFF8);
  }
  va_end(args);

  LOG_DEBUG("oam_vsmem__getMultiMem > (%p)", baseptr);
  return baseptr;
}

int oam_vsmem__strEq(
  const char * restrict string1,
  const char * restrict string2)
{
	if (!string1 || !string2) {
		return 0;
	}
	while ((*string1) && (*string2) && !(*string1 - *string2)) {
		string1++;
		string2++;
	}
	return (!(*string1) && !(*string2));
}

/* ======================================================================= *
 * Symmetric memory shared between host and targets                        *
 * ======================================================================= */

void * oam_vsmem__symmetric_malloc(
  unsigned int   nbytes)
{
  void * pmem;
  LOG_DEBUG("oam_vsmem__symmetric_malloc(nbytes:%d)", nbytes);
#if defined(OMPACC)
# ifdef SYMMETRIC_MEMORY__USE_DDR
  pmem = __malloc_ddr(nbytes);
# else
  pmem = __malloc_msmc(nbytes);
# endif
#else
  pmem = malloc(nbytes);
#endif
  LOG_DEBUG("oam_vsmem__symmetric_malloc(nbytes:%d) -> 0x%p", nbytes, pmem);
  return pmem;
}

void oam_vsmem__symmetric_free(
  void         * pmem)
{
  LOG_DEBUG("oam_vsmem__symmetric_free(0x%p)", pmem);
  if (pmem == NULL) { return; }
#if defined(OMPACC)
# ifdef SYMMETRIC_MEMORY__USE_DDR
  __free_ddr(pmem);
# else
  __free_msmc(pmem);
# endif
#else
  LOG_DEBUG("oam_vsmem__symmetric_free(0x%p) -> free", pmem);
  free(pmem);
#endif
}

void oam_vsmem__symmetric_heap_init(
  void         * pmem,
  unsigned int   nbytes)
{
#if defined(OMPACC)
  char * pmem_c = (char *)(pmem);
  #pragma omp target map(to: nbytes, pmem_c[0:nbytes])
  {
    LOG_DEBUG("oam_vsmem__heap_init(0x%p, %d)", pmem, nbytes);
# ifdef SYMMETRIC_MEMORY__USE_DDR
    __heap_init_ddr((char *)(pmem_c), nbytes);
# else
    __heap_init_msmc((char *)(pmem_c), nbytes);
# endif
  }
#endif
}

/* ======================================================================= *
 * Local memory in target L2 cache                                         *
 * ======================================================================= */

void * oam_vsmem__local_malloc(
  unsigned int   nbytes,
  OAM_VSMEM__LOCAL_ALLOC_TYPE atype)
{
  char * local_pmem = NULL;
#if defined(OMPACC)
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
#else
  local_pmem = malloc(nbytes);
#endif
  LOG_DEBUG("oam_vsmem__local_malloc > (%p)", local_pmem);
  return local_pmem;
}

void oam_vsmem__local_heap_init(
  void         * pmem,
  unsigned int   nbytes)
{
#if defined(OMPACC)
  // Unused dummy pointer required for function interface.
  char * p = NULL;
  #pragma omp target map(to:nbytes) map(local:p[0:nbytes])
  {
    __heap_init_l2((char *)(pmem), nbytes);
  }
  wno_unused_var_(p);
#endif
}

