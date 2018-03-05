#ifndef DSP_FREQ_HOST_H__INCLUDED
#ifndef DSP_FREQ_HOST_H__INCLUDED

#include "dsp_freq_types.h"

#include <oam/oam_types.h>


#ifdef __cplusplus
extern "C" {
#endif


void measurement_task(
  target_measurements * measurements,
  HostMessage         * host_signals);


#ifdef __cplusplus
} // extern "C"
#endif

#endif //  DSP_FREQ_HOST_H__INCLUDED
