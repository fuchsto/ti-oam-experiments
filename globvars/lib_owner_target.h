#ifndef OWNER_TARGET_H_
#define OWNER_TARGET_H_

#include <oam/oam_types.h>
#include <oam/oam_task.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target
// Size actually not required to be specified for target-target
// linkage of global variables.
extern char global_target_string[128];
#pragma omp end declare target

#ifdef __cplusplus
} // extern "C"
#endif

#endif
