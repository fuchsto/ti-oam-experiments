#ifndef DSP_IMG_FILTER__CONFIG_H_
#define DSP_IMG_FILTER__CONFIG_H_

#ifdef ENV_TARGET
#  define ARCH_BOGOMIPS  700
#else
#  define ARCH_BOGOMIPS 1229
#endif

#define OAM_TASK__MAX_THREADS 2

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

#endif // DSP_IMG_FILTER__CONFIG_H_
