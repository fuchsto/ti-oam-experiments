#ifndef CHECK_COMPILER_FLAG_H__INCLUDED
#define CHECK_COMPILER_FLAG_H__INCLUDED

#ifndef _OA_MODE
#warn !!! _OA_MODE flag not set
#endif

#ifdef _OA_TARGET_MODE
#warn >>> _OA_TARGET_MODE flag set
#endif
#ifdef _OA_HOST_MODE
#warn >>> _OA_HOST_MODE flag set
#endif

#endif /* CHECK_COMPILER_FLAG_H__INCLUDED */
