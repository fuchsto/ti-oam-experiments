#ifndef OAM__CONFIG_H_
#define OAM__CONFIG_H_

/**
 * \file oam_config.h
 *
 * Configuration interface of the OAM module.
 */

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
#define OAM__SYMMETRIC_MEMORY__USE_DDR



#ifdef __cplusplus
extern "C" {
#endif

/**
 * OAM module configuration settings.
 */
typedef struct OMPAccConfig_s {
  /// Whether offloading is enabled. Default-initialized to `1`.
  int offloading_enabled;
  /// Frequency scaling of the target clock to adjust for host clock frequency
  /// in time measurements.
  int target_freq_scale;
  /// Base frequency of the target clock to adjust for host clock frequency
  /// in time measurements.
  int target_base_freq;
} OMPAccConfig;

/**
 * Read-only access to OAM configuration object.
 */
const OMPAccConfig * oam_config();

/**
 * Initializes the OAM configuration, calculates configuration settings from
 * target system and build configuration (i.e. clock scaling).
 */
void                 oam_config__init();

#ifdef __cplusplus
} /* extern "C" */
#endif


#endif // OAM__CONFIG_H_
