#ifndef COMM_ASYNC_TARGET_H_
#define COMM_ASYNC_TARGET_H_

#include <oam/oam_types.h>
#include <oam/oam_task.h>

#include "dsp_freq_types.h"

#ifdef __cplusplus
extern "C" {
#endif

void target_task(
  target_measurements * measurements,
  HostMessage         * host_signals);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // COMM_ASYNC_TARGET_H_
