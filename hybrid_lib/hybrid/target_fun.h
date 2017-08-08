#ifndef __TARGET_FUN_H__INCLUDED
#define __TARGET_FUN_H__INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target

int target_function(
    char      * array_a,
    char      * array_b,
    int         nelem);

#pragma omp end declare target

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
