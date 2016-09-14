#ifndef OAM__OAM_VSMEM_H__INCLUDED
#define OAM__OAM_VSMEM_H__INCLUDED

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

// #include <basefw/vsmem_types.h>
// #include <vs130/vsmem130.h>

typedef enum {
  EXTERNAL_MEMORY,
  INTERNAL_MEMORY,
  EXTERNAL_IMAGE_MEMORY
} oam_vsmem__eMemTag;

#ifndef EXTERNAL_MEMORY
#define EXTERNAL_MEMORY 1
#endif

#ifndef INTERNAL_MEMORY
#define INTERNAL_MEMORY 2
#endif

#ifndef ALGO_MEM
#define ALGO_MEM 10
#endif

#ifndef MEM_ILLEGAL
#define MEM_ILLEGAL NULL
#endif


#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target

#include <string.h>
void * memcpy(
  void                * dst,
  const void          * src,
  unsigned int          nbytes);

void * oam_vsmem__symmetric_malloc(
  unsigned int          nbytes);

void oam_vsmem__symmetric_free(
  void                * pmem);

static inline void * oam_vsmem__copy(
  void                * dst,
  const void          * src,
  unsigned int          nbytes);

void * oam_vsmem__set(
  void                * dst,
  const unsigned char   value,
  unsigned int          nbytes);

#pragma omp end declare target

static inline void * oam_vsmem__copy(
  void                * dst,
  const void          * src,
  unsigned int          nbytes)
{
  return memcpy(dst, src, nbytes);
}

/* ======================================================================= *
 * Symmetric memory shared between host and targets                        *
 * ======================================================================= */

void oam_vsmem__symmetric_heap_init(
  void                * pmem,
  unsigned int          nbytes);

/* ======================================================================= *
 * Local memory in target L2 cache                                         *
 * ======================================================================= */

void * oam_vsmem__local_malloc(
  unsigned int          nbytes,
  OAM_VSMEM__LOCAL_ALLOC_TYPE atype);

void oam_vsmem__local_heap_init(
  void                * pmem,
  unsigned int          nbytes);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* OAM__OAM_VSMEM_H__INCLUDED */
