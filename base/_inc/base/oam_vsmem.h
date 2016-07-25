#ifndef BASE__OAM_VSMEM_H__INCLUDED
#define BASE__OAM_VSMEM_H__INCLUDED

typedef enum OAM_VSMEM__LOCAL_ALLOC_TYPE_t
{
  ALLOC_TARGET = 0,
  ALLOC_CORE
}
OAM_VSMEM__LOCAL_ALLOC_TYPE;

typedef enum oam_vsmem__eMemTag
{
#if 0
    DIAGNOSE_MEM,
    XMLUTIL_MEM,
    IMAGE_MEM,
    IMAGE_FRAME_MEM,
    AUTOMAT_MEM,
    DELAY_MEM,          // 5
    FILEIF_MEM,
    IRQDISPATCH_MEM,
    MOVIEUP_MEM,
    BMP_MEM,
    NDK_MEM,            // 10
    STSWEBIF_MEM,
    FSWEB_MEM,
    LOGWEBIF_MEM,
    RS232TCP_MEM,
    PROFINET_MEM,       // 15
    PROFIBUS_MEM,
    JPEG_MEM,
    XCMD_MEM,
    HTMLUTIL_MEM,
    LOOKUP_MEM,         // 20
    CSV_PARSER_MEM,
    ZIPARCHIVE_MEM,
    IPEXCHANGE_MEM,
    KERNEL_MEM,
    VERIFICATION_MEM,   // 25
    RS232IO_MEM,
    MOBY_MEM,
    RSYNC_MEM,
    LICMGMT_MEM,
    LICTRANS_MEM,       // 30
    PIB_MEM,
    TOOLBOX_MEM,
    GLOBAL_CONSTR_MEM,

    NUM_BASEFW_TAGS,    // 34

    VSMEM_TAG_USERBASE = NUM_BASEFW_TAGS
#endif
    OAM_VSMEM__E_MEM_TAG_DUMMY
} oam_vsmem__eMemTag;

#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target

void * oam_vsmem__copy(
  void                * dst,
  const void          * src,
  unsigned int          nbytes);

void * oam_vsmem__set(
  void                * dst,
  const unsigned char   value,
  unsigned int          nbytes);

int oam_vsmem__freeMem(
  void                * paddr);

int oam_vsmem__freeTag(
  oam_vsmem__eMemTag    tag);

void * oam_vsmem__getMem(
  int                   segid,
  unsigned int          size,
  oam_vsmem__eMemTag    tag);

void * oam_vsmem__getAlignedMem(
  int                   segid,
  unsigned int          size,
  oam_vsmem__eMemTag    tag,
  unsigned int          align,
  const char          * pinitializer);

#pragma omp end declare target

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
