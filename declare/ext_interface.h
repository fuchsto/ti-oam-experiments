#ifndef EXT_INTERFACE_H__INCLUDED
#define EXT_INTERFACE_H__INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target
#include "ext_interface.decl.h"
#pragma omp end declare target

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* EXT_INTERFACE_H__INCLUDED */
