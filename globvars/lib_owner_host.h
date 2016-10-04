#ifndef OWNER_HOST_H_
#define OWNER_HOST_H_

#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target
char global_host_string[32];
#pragma omp end declare target

#ifdef __cplusplus
} // extern "C"
#endif

#endif
