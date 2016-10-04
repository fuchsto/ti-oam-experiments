#ifndef LIBWRAPPER_HOST_H__INCLUDED
#define LIBWRAPPER_HOST_H__INCLUDED

#include <oam/oam_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int native_lib_wrapper(
    double      * array_a,
    double      * array_b,
    int           nelem,
    int           nrepeat,
    HostMessage * signals);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBWRAPPER_HOST_H__INCLUDED
