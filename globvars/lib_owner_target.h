#ifndef OWNER_TARGET_H_
#define OWNER_TARGET_H_

#include <oam/oam_types.h>
#include <oam/oam_task.h>

#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target
extern char global_string[32];
#pragma omp end declare target

#ifdef __cplusplus
} // extern "C"
#endif

#endif
