#ifndef NESTED_INTERFACE_H__INCLUDED
#define NESTED_INTERFACE_H__INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target
#include "nested_interface.decl.h"
#pragma omp end declare target

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* NESTED_INTERFACE_H__INCLUDED */
