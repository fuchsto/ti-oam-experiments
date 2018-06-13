
#pragma omp declare target
#  include <omp.h>
#  if defined(OMPACC_TARGET)
#    include "ti_omp_device.h"
#  endif

unsigned long long __clock64(void);

#pragma omp end declare target


#include <oam/oam_config.h>
#include <oam/oam_task.h>


static OMPAccConfig s_oam_config = {
#if defined(OMPACC)
  .offloading_enabled = 1,
#else
  .offloading_enabled = 0,
#endif
  // Milliseconds passed on DSP in one
  // second passed on host, 1000 indicating
  // no clock scaling:
  .target_freq_scale  = 1000,
  // Clock cycles on target measured during one
  // second passed on host:
  .target_base_freq   = 750 * 1e6
};

const OMPAccConfig * oam_config() {
  return &s_oam_config;
}

void oam_config__init() {
#if defined(OMPACC)
  const int device_num = 0;
  ts_t t0, t1;
  int num_target_cycles = 0;

  // Set timeout to 1 second and measure actual
  // elapsed time:
  const long second_ms = 1000;

  s_oam_config.target_base_freq  = 750 * 1e6;
  s_oam_config.target_freq_scale = 1000;

  // Do not use oam_task__set_default_timeout as it
  // again relies on a calibrated oam_config:
  __ti_set_default_timeout(device_num, second_ms);

  t0 = oam_timestamp();
#pragma omp target map (from: num_target_cycles)
  {
      unsigned long nclk0 = __clock64();
      unsigned long nclk1;

      while (1) {
         nclk1 = __clock64();
         num_target_cycles = nclk1 - nclk0;
      }
  }
  t1 = oam_timestamp();

  s_oam_config.target_base_freq  = num_target_cycles;
  s_oam_config.target_freq_scale = ((t1 - t0) * 1000);

  LOG_HOST_INFO("oam_config__init: target base freq:  %d", s_oam_config.target_base_freq);
  LOG_HOST_INFO("oam_config__init: target freq scale: %d", s_oam_config.target_freq_scale);
#endif
}
