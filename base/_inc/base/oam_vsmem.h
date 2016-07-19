#ifndef BASE__OAM_VSMEM_H__INCLUDED
#define BASE__OAM_VSMEM_H__INCLUDED

typedef enum OAM_VSMEM__LOCAL_ALLOC_TYPE_t
{
  ALLOC_TARGET = 0,
  ALLOC_CORE
}
OAM_VSMEM__LOCAL_ALLOC_TYPE;

#ifdef __cplusplus
extern "C" {
#endif

void * oam_vsmem__copy(
  void         * dst,
  const void   * src,
  unsigned int   nbytes);

/* ======================================================================= *
 * Symmetric memory shared between host and targets                        *
 * ======================================================================= */

void * oam_vsmem__symmetric_malloc(
  unsigned int   nbytes);

void oam_vsmem__symmetric_free(
  void         * pmem);

void oam_vsmem__symmetric_heap_init(
  void         * pmem,
  unsigned int   nbytes);

/* ======================================================================= *
 * Local memory in target L2 cache                                         *
 * ======================================================================= */

void * oam_vsmem__local_malloc(
  unsigned int   nbytes,
  OAM_VSMEM__LOCAL_ALLOC_TYPE atype);

void oam_vsmem__local_heap_init(
  void         * pmem,
  unsigned int   nbytes);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // BASE__OAM_VSMEM_H__INCLUDED
