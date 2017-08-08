#ifndef LIBWRAPPER_HOST_H__INCLUDED
#define LIBWRAPPER_HOST_H__INCLUDED

#include <oam/oam_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int hybrid_wrapper(
    char        * array_a,
    char        * array_b,
    int           nelem,
    int           nrepeat,
    HostMessage * signals);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // LIBWRAPPER_HOST_H__INCLUDED
