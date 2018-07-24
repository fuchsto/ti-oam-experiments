#ifndef OAM__OAM_VSMEM_H__INCLUDED
#define OAM__OAM_VSMEM_H__INCLUDED

/**
 * \file oam_vsmem.h
 *
 * Unified memory management routines usable in host- and target code.
 */

#include <sysdefs/qualifier.h>


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

typedef enum OAM_VSMEM__LOCAL_ALLOC_TYPE_t
{
  ALLOC_TARGET = 0,
  ALLOC_CORE
}
OAM_VSMEM__LOCAL_ALLOC_TYPE;

#include <basefw/vsmem_types.h>
#include <vs130/vsmem130.h>

typedef VSMEM130_Tags oam_vsmem__eMemTag;

#if defined(OMPACC)

# ifndef EXTERNAL_MEMORY
# define EXTERNAL_MEMORY 1
# endif

# ifndef INTERNAL_MEMORY
# define INTERNAL_MEMORY 2
# endif

# ifndef ALGO_MEM
# define ALGO_MEM 10
# endif

# ifndef MEM_ILLEGAL
# define MEM_ILLEGAL NULL
# endif

#else  /* OMPACC */
# include <basefw/vsmem.h>
#endif /* OMPACC */

#pragma omp declare target
// C++ linkage only
extern int   EXTERNAL_ALGO_MEMORY;
#pragma omp end declare target

#ifdef __cplusplus
extern "C" {
#endif


#if defined(OMPACC)
#pragma omp declare target
#endif


#include <string.h>
#if defined(OMPACC)
void * memcpy(
  void                * dst,
  const void          * src,
  unsigned int          nbytes);
#endif

void * oam_vsmem__symmetric_malloc(
  unsigned long         nbytes);

void * MV__ALIGNED_POINTER(MV__MIN_ALIGN) oam_vsmem__symmetric_memalign(
  unsigned int          align,
  unsigned long         nbytes);

void * oam_vsmem__symmetric_scratch_malloc(
  unsigned long         nbytes);


void oam_vsmem__symmetric_free(
  void                * pmem);

void oam_vsmem__symmetric_scratch_free(
  void                * pmem);


static inline void * oam_vsmem__copy(
  void                * dst,
  const void          * src,
  unsigned long         nbytes);

int oam_vsmem__compare(
  const void          * pData1,
  const void          * pData2,
  unsigned int          nbytes);

void * oam_vsmem__set(
  void                * dst,
  const unsigned char   value,
  unsigned long         nbytes);

int oam_vsmem__strcmp(
  const char          * str1,
  const char          * str2);

int oam_vsmem__freeMem(
  void                * paddr);

#if MEMORY_DEBUG_SUPPORTED
int oam_vsmem__freeMemDebug(
  void                * paddr);
#endif

int oam_vsmem__freeTag(
  oam_vsmem__eMemTag    tag);

void * MV__ALIGNED_POINTER(MV__MIN_ALIGN) oam_vsmem__getMem(
  int                   segid,
  unsigned long         size,
  oam_vsmem__eMemTag    tag);

static inline void * MV__ALIGNED_POINTER(MV__MIN_ALIGN) oam_vsmem__cgetMem(
  int                   segid,
  unsigned long         size,
  oam_vsmem__eMemTag    tag);

void * MV__ALIGNED_POINTER(MV__MIN_ALIGN) oam_vsmem__getAlignedMem(
  int                   segid,
  unsigned long         size,
  oam_vsmem__eMemTag    tag,
  unsigned int          align,
  const char          * pinitializer);

void * oam_vsmem__getMultiMem(
  int                   segid,
  oam_vsmem__eMemTag    tag,
  unsigned int          num,
  unsigned int          firstSize, ...);

int oam_vsmem__strEq(
  const char          * string1,
  const char          * string2);

#if defined(OMPACC)
#pragma omp end declare target
#endif

static inline void * oam_vsmem__copy(
  void                * dst,
  const void          * src,
  unsigned long         nbytes)
{
  return memcpy(dst, src, nbytes);
}

static inline void * MV__ALIGNED_POINTER(8) oam_vsmem__cgetMem(
  int                   segid,
  unsigned long         size,
  oam_vsmem__eMemTag    tag)
{
  void * MV__ALIGNED_POINTER(8) ptr = oam_vsmem__getMem(segid, size, tag);
  oam_vsmem__set(ptr, 0, size);
  return ptr;
}

/* ======================================================================= *
 * Symmetric memory shared between host and targets                        *
 * ======================================================================= */

void oam_vsmem__symmetric_heap_init(
  void                * pmem,
  unsigned long         nbytes);

void * oam_vsmem__symmetric_heap_create(
  unsigned long         nbytes);

void oam_vsmem__symmetric_heap_destroy(
  void                * pmem);

/* ======================================================================= *
 * Local memory in target L2 cache                                         *
 * ======================================================================= */

void * oam_vsmem__local_malloc(
  unsigned long         nbytes,
  OAM_VSMEM__LOCAL_ALLOC_TYPE atype);

void oam_vsmem__local_heap_init(
  void                * pmem,
  unsigned long         nbytes);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OAM__OAM_VSMEM_H__INCLUDED */
