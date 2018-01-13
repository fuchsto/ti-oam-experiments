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


// -----------------------------------------------------------------------
// -----------------------------------------------------------------------

#define VSMEM_MAX_NRHEAPS 5

typedef struct _VSMEM_heapMemStatObj
{
    unsigned int totalMem;
    unsigned int freeMem;
} VSMEM_heapMemStatObj;


/* ******************************************************************** */
/* Type:       VSMEM_sTagName                                           */
/*                                                                      */
/* Module:     Memory Interface                                         */
/*                                                                      */
/* ==================================================================== */
/* Description:                                                         */
/* strings corresponding to memory tags                                 */
/*                                                                      */
/* ******************************************************************** */
/*! strings corresponding to memory tags                                 */
typedef struct _VSMEM_sTagName
{
    char tagname[12];
} VSMEM_sTagName;

/* ******************************************************************** */
/* Constants:  VSMEM_eMemTag                                            */
/*                                                                      */
/* Module:     Memory Interface                                         */
/*                                                                      */
/* ==================================================================== */
/* Description:                                                         */
/* memory tags for global use                                           */
/*                                                                      */
/* ******************************************************************** */
/*! memory tags for global use                                           */
typedef enum _VSMEM_eMemTag
{
    DIAGNOSE_MEM,
    XMLUTIL_MEM,
    IMAGE_MEM,
    IMAGE_FRAME_MEM,
    AUTOMAT_MEM,
    DELAY_MEM,            //5
    FILEIF_MEM,
    IRQDISPATCH_MEM,
    MOVIEUP_MEM,
    BMP_MEM,
    NDK_MEM,            //10
    STSWEBIF_MEM,
    FSWEB_MEM,
    LOGWEBIF_MEM,
    RS232TCP_MEM,
    PROFINET_MEM,        //15
    PROFIBUS_MEM,
    JPEG_MEM,
    XCMD_MEM,
    HTMLUTIL_MEM,
    LOOKUP_MEM,            //20
    CSV_PARSER_MEM,
    ZIPARCHIVE_MEM,
    IPEXCHANGE_MEM,
    KERNEL_MEM,
    VERIFICATION_MEM,    //25
    RS232IO_MEM,
    MOBY_MEM,
    RSYNC_MEM,
    LICMGMT_MEM,
    LICTRANS_MEM,        //30
    PIB_MEM,
    TOOLBOX_MEM,
    GLOBAL_CONSTR_MEM,
    AUTO_FOCUS_MEM,
    CHUNKED_MEM, 		// 35
    COMPRESSION_MEM,
    XFERMGMT_MEM,

    NUM_BASEFW_TAGS,    //38

    VSMEM_TAG_USERBASE = NUM_BASEFW_TAGS
} VSMEM_eMemTag;

typedef enum _VSMEM_eFunction{
    E_ALLOC,
    E_FREE,
    E_REALLOC,
    E_HEAP_SHIFT
} VSMEM_eFunction;


/* ******************************************************************** */
/* Constants:  VSMEM_status                                             */
/*                                                                      */
/* Module:     Memory Interface                                         */
/*                                                                      */
/* ==================================================================== */
/* Description:                                                         */
/* memory status                                                        */
/*                                                                      */
/* ******************************************************************** */
/*! memory status                                                       */
typedef enum _VSMEM_status
{
    VSMEM_STATUS_OK,
    VSMEM_STATUS_CRITICAL,
    VSMEM_STATUS_EXHAUSTED
} VSMEM_status;

// -----------------------------------------------------------------------
// -----------------------------------------------------------------------

#pragma omp declare target

#define ALGOHEAP_SIZE 0x2000000
//#define ALGOHEAP_SIZE 0x200
typedef enum _VSMEM130_Tags
{
	RUN_MEM = VSMEM_TAG_USERBASE,	//36,
	IDLE_MEM,
	WEBCOMMU_MEM,
	EDIT_MEM,
	FORMATTING_MEM,	//40
	ALGO_MEM,
	SELFTEST_MEM,
	VSXUPD_MEM,
	XCMD130_MEM,
	DECODEIF_MEM,	//45
	OCV_MEM,
	XMATCH_MEM,
	AP4HMI_MEM,
	FONT_MEM,
	READ_MEM,		//50
	PARAMETER_MEM,
	LOCATOR_MEM,
	STDLIB_DYNAMIC_MEM,
				//!<< If you add a tag here be sure to add the name in vsmem130.c AND vsmem.c.
	LAST_130_MEMTAG //54
} VSMEM130_Tags;

extern const VSMEM_sTagName VSMEM130_TAG_NAMES[LAST_130_MEMTAG - RUN_MEM];

extern int VSMEM130_checkFreeImages(VSMEM_eMemTag tag);

extern void VS130MEM_initVSMEM(void);

extern void VS130MEM_finalizeVSMEM(void);

extern int vsmemReady;

#pragma omp end declare target

// -----------------------------------------------------------------------
// -----------------------------------------------------------------------

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

#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target

extern int   EXTERNAL_ALGO_MEMORY;

#include <string.h>
#if defined(OMPACC)
void * memcpy(
  void                * dst,
  const void          * src,
  unsigned int          nbytes);
#endif

void * oam_vsmem__symmetric_malloc(
  unsigned long         nbytes);

void * oam_vsmem__symmetric_memalign(
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

void * oam_vsmem__getMem(
  int                   segid,
  unsigned long         size,
  oam_vsmem__eMemTag    tag);

static inline void * oam_vsmem__cgetMem(
  int                   segid,
  unsigned long         size,
  oam_vsmem__eMemTag    tag);

void * oam_vsmem__getAlignedMem(
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

#pragma omp end declare target

static inline void * oam_vsmem__copy(
  void                * dst,
  const void          * src,
  unsigned long         nbytes)
{
  return memcpy(dst, src, nbytes);
}

static inline void * oam_vsmem__cgetMem(
  int                   segid,
  unsigned long         size,
  oam_vsmem__eMemTag    tag)
{
  void * ptr = oam_vsmem__getMem(segid, size, tag);
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
