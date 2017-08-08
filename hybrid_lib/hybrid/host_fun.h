#ifndef __HOST_ONLY_LIB_H__INCLUDED
#define __HOST_ONLY_LIB_H__INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target

int host_function(
    char      * array_a,
    char      * array_b,
    int         nelem);

#pragma omp end declare target

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* __HOST_ONLY_LIB_H__INCLUDED */
