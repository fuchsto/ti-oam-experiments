L 1 "static_vars_target.c"
N
N#include "static_vars_target.h"
L 1 "static_vars_target.h" 1
N#ifndef COMM_ASYNC_TARGET_H_
N#define COMM_ASYNC_TARGET_H_
N
N#include <base/oam_types.h>
L 1 "/home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_types.h" 1
N#ifndef BASE__OAM_TYPES_H__INCLUDED
N#define BASE__OAM_TYPES_H__INCLUDED
N
N#include <base/oam_config.h>
L 1 "/home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_config.h" 1
N#ifndef DSP_IMG_FILTER__CONFIG_H_
N#define DSP_IMG_FILTER__CONFIG_H_
N
N#ifdef ENV_TARGET
S#  define ARCH_BOGOMIPS  700
N#else
N#  define ARCH_BOGOMIPS 1229
N#endif
N
N#define OAM_TASK__MAX_THREADS 2
N
N/* Note:
N *
N * MSMC for host/target shared memory is supported for K2x devices only.
N * See:
N * http://downloads.ti.com/mctools/esd/docs/opencl/memory/memory-model.html
N *
N * For a detailed reference of the zero-copy heap extensions, see:
N *
N * http://downloads.ti.com/mctools/esd/docs/opencl/memory/host-malloc-extension.html
N * http://processors.wiki.ti.com/index.php/OpenMP_OpenCL_DSP_Heap_Management
N */
N#define SYMMETRIC_MEMORY__USE_DDR
N
N#endif // DSP_IMG_FILTER__CONFIG_H_
L 5 "/home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_types.h" 2
N#include <base/macro.h>
L 1 "/home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/macro.h" 1
N#ifndef DSP_IMG_FILTER__MACRO_H__INCLUDED
N#define DSP_IMG_FILTER__MACRO_H__INCLUDED
N
N// #include <string.h>
N
N/**
N * To avoid compiler warning Wunused-var.
N */
N#define wno_unused_var_(var) (void)(var)
N
N/**
N * Wondering every time why swap() is not part of the C standard lib ...
N */
N#define swap_(x,y) do { \
N  unsigned char tmp[sizeof(x) == sizeof(y) ? (signed)(sizeof(x)) : -1]; \
N  memcpy(tmp, &y,  sizeof(x)); \
N  memcpy(&y,  &x,  sizeof(x)); \
N  memcpy(&x,  tmp, sizeof(x)); \
N} while (0)
X#define swap_(x,y) do {   unsigned char tmp[sizeof(x) == sizeof(y) ? (signed)(sizeof(x)) : -1];   memcpy(tmp, &y,  sizeof(x));   memcpy(&y,  &x,  sizeof(x));   memcpy(&x,  tmp, sizeof(x)); } while (0)
N
N/**
N * TI's implementation of cmath has no definition of rint().
N */
N#define rint_(f) ((int)(f))
N
N#define min_(a, b) ((a) < (b) ? (a) : (b))
N
N#endif /* DSP_IMG_FILTER__MACRO_H__INCLUDED */
L 6 "/home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_types.h" 2
N
Ntypedef enum OMPAccTaskRet_t
N{
N  OMPACC_TASK__OK      = 0,
N  OMPACC_TASK__CANCEL,
N  OMPACC_TASK__TIMEOUT
N} OMPAccTaskRet;
N
Ntypedef struct HostMessage_t
N{
N  int           cancel;
N  int           time_start_ms;
N  int           timeout_after_ms;
N  int           poll_interval_ms;
N  int           last_poll_time_ms[OAM_TASK__MAX_THREADS];
X  int           last_poll_time_ms[2];
N  int           param;
N  OMPAccTaskRet ret;
N} HostMessage;
N
N#endif // BASE__OAM_TYPES_H__INCLUDED
L 5 "static_vars_target.h" 2
N#include <base/oam_task.h>
L 1 "/home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_task.h" 1
N#ifndef BASE__OAM_TASK_H__INCLUDED
N#define BASE__OAM_TASK_H__INCLUDED
N
N#include <base/oam_types.h>
N#include <base/macro.h>
N
N/* ======================================================================== *
N * Host Functions                                                           *
N * ======================================================================== */
N
N#ifdef __cplusplus
Sextern "C" {
N#endif
N
N/**
N * Register the start of a target task.
N * To be called before first target offload region.
N */
Nvoid oam_task__enter(
N  HostMessage * host_signals);
N
N/**
N * Send cancellation request signal to all targets listening on the
N * specified host signals handle.
N */
Nvoid oam_task__request_cancel(
N  HostMessage * host_signals);
N
N/**
N * Synchronize signals handle between host and target.
N * To be called between target offload regions.
N */
Nvoid oam_task__step(
N  HostMessage * host_signals);
N
N/**
N * Whether the specified host signals handle requested cancellation of
N * the target task.
N */
Nstatic inline int oam_task__aborted(
N  HostMessage * host_signals)
N{
N  return (1 == host_signals->cancel ||
N          host_signals->ret == OMPACC_TASK__CANCEL ||
N          host_signals->ret == OMPACC_TASK__TIMEOUT);
N}
N
N#ifdef __cplusplus
S} /* extern "C" */
N#endif
N
N#endif // BASE__OAM_TASK_H__INCLUDED
L 6 "static_vars_target.h" 2
N
N#ifdef __cplusplus
Sextern "C" {
N#endif
N
Nvoid target_init(float value);
N
Nint target_function(
N  char        * in_buffer,
N  char        * out_buffer,
N  int           size,
N  int           repeats,
N  HostMessage * host_message_buf);
N
N#ifdef __cplusplus
S} // extern "C"
N#endif
N
N#endif // COMM_ASYNC_TARGET_H_
L 3 "static_vars_target.c" 2
N
N#include <base/oam_config.h>
N#include <base/oam_types.h>
N#include <base/oam_task.h>
N#include <base/oam_task_target.h>
L 1 "/home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/oam_task_target.h" 1
N#ifndef BASE__OAM_TASK_TARGET_H__INCLUDED
N#define BASE__OAM_TASK_TARGET_H__INCLUDED
N
N#include <base/oam_task.h>
N#include <base/oam_types.h>
N#include <base/macro.h>
N
N/* ======================================================================== *
N * Target Functions                                                         *
N * ======================================================================== */
N#pragma omp declare target
N
N// See:
N// http://downloads.ti.com/mctools/esd/docs/opencl/memory/cache-operations.html
Nvoid __cache_l2_flush(void);
Nvoid __cache_l1d_flush(void);
N
N/**
N * Poll host signals handle to test if the current thread's task is to be
N * aborted.
N *
N * Parameter \c aborted must be a thread-local variable.
N */
Nint oam_task__poll_cancel_request(
N  /// The task's start timestamp in seconds, usually obtained from
N  /// \c omp_get_wtime().
N  double        time_start,
N  /// The task's host signals handle.
N  HostMessage * host_signals,
N  /// Thread-local variable, set to \c 1 if the task has been aborted.
N  int         * aborted);
N
N/**
N * Whether the specified host signals handle requested cancellation of
N * the target task.
N */
Nstatic inline int oam_task__aborted(
N  HostMessage * host_signals);
N
N#pragma omp end declare target
N
N
N#endif // BASE__OAM_TASK_TARGET_H__INCLUDED
L 8 "static_vars_target.c" 2
N#include <base/logging.h>
L 1 "/home/fuchsto/workspaces/sieka.git/MV460-OAM/base/_inc/base/logging.h" 1
N#ifndef DSP_IMG_FILTER__LOGGING_H__INCLUDED
N#define DSP_IMG_FILTER__LOGGING_H__INCLUDED
N
N#define LOG_HOST__FILE_FIELD_WIDTH       32
N#define LOG_HOST__LINE_FIELD_WIDTH        5
N#define LOG_HOST__MAX_MESSAGE_SIZE      128
N
N// #ifdef ENABLE_LOGGING
N#define LOG_HOST_DEBUG(...) \
N  do { \
N    char msg_buf[LOG_HOST__MAX_MESSAGE_SIZE+1]; \
N    int  sn_ret = snprintf(msg_buf, LOG_HOST__MAX_MESSAGE_SIZE, __VA_ARGS__); \
N    if (sn_ret < 0) { \
N      break; \
N    } \
N    const char * path = __FILE__; \
N    char       * base = strrchr(path, '/'); \
N    printf( \
N      "[ DEBUG ] [  HOST  ] %-*s:%-*d | %s\n", \
N      LOG_HOST__FILE_FIELD_WIDTH, (base ? base + 1 : path), \
N      LOG_HOST__LINE_FIELD_WIDTH, __LINE__, \
N      msg_buf); \
N  } while (0)
X#define LOG_HOST_DEBUG(...)   do {     char msg_buf[LOG_HOST__MAX_MESSAGE_SIZE+1];     int  sn_ret = snprintf(msg_buf, LOG_HOST__MAX_MESSAGE_SIZE, __VA_ARGS__);     if (sn_ret < 0) {       break;     }     const char * path = __FILE__;     char       * base = strrchr(path, '/');     printf(       "[ DEBUG ] [  HOST  ] %-*s:%-*d | %s\n",       LOG_HOST__FILE_FIELD_WIDTH, (base ? base + 1 : path),       LOG_HOST__LINE_FIELD_WIDTH, __LINE__,       msg_buf);   } while (0)
N// #else
N// #define LOG_HOST_DEBUG(...) do { } while(0);
N// #endif
N
N#endif // DSP_IMG_FILTER_LOGGING_H__INCLUDED
L 9 "static_vars_target.c" 2
N#include <base/macro.h>
N
N#define NValues 100
N
N/* ======================================================================== *
N * Begin of Target Function Declarations and Includes                       *
N * ------------------------------------------------------------------------ */
N#pragma omp declare target
N
N#include <stdlib.h>
L 1 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h" 1
N/*****************************************************************************/
N/* stdlib.h   v8.1.0                                                         */
N/*                                                                           */
N/* Copyright (c) 1993-2015 Texas Instruments Incorporated                    */
N/* http://www.ti.com/                                                        */
N/*                                                                           */
N/*  Redistribution and  use in source  and binary forms, with  or without    */
N/*  modification,  are permitted provided  that the  following conditions    */
N/*  are met:                                                                 */
N/*                                                                           */
N/*     Redistributions  of source  code must  retain the  above copyright    */
N/*     notice, this list of conditions and the following disclaimer.         */
N/*                                                                           */
N/*     Redistributions in binary form  must reproduce the above copyright    */
N/*     notice, this  list of conditions  and the following  disclaimer in    */
N/*     the  documentation  and/or   other  materials  provided  with  the    */
N/*     distribution.                                                         */
N/*                                                                           */
N/*     Neither the  name of Texas Instruments Incorporated  nor the names    */
N/*     of its  contributors may  be used to  endorse or  promote products    */
N/*     derived  from   this  software  without   specific  prior  written    */
N/*     permission.                                                           */
N/*                                                                           */
N/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS    */
N/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT    */
N/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
N/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT    */
N/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
N/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT    */
N/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
N/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
N/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT    */
N/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
N/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
N/*                                                                           */
N/*****************************************************************************/
N
N#ifndef _STDLIB
N#define _STDLIB
N
N
N#pragma diag_push
N#pragma CHECK_MISRA("-6.3") /* standard types required for standard headers */
N#pragma CHECK_MISRA("-8.5") /* need to define inline function */
N#pragma CHECK_MISRA("-19.1") /* #includes required for implementation */
N#pragma CHECK_MISRA("-19.7") /* need function-like macros */
N#pragma CHECK_MISRA("-20.1") /* standard headers must define standard names */
N#pragma CHECK_MISRA("-20.2") /* standard headers must define standard names */
N
N/*---------------------------------------------------------------------------*/
N/* Attributes are only available in relaxed ANSI mode.                       */
N/*---------------------------------------------------------------------------*/
N#ifndef __ATTRIBUTE
N#if __TI_STRICT_ANSI_MODE__
X#if 0
S#define __ATTRIBUTE(attr)
N#else
N#define __ATTRIBUTE(attr) __attribute__(attr)
N#endif
N#endif
N
N
N#ifdef __cplusplus
S/*---------------------------------------------------------------------------*/
S/* <cstdlib> IS RECOMMENDED OVER <stdlib.h>.  <stdlib.h> IS PROVIDED FOR     */
S/* COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++                  */
S/*---------------------------------------------------------------------------*/
Sextern "C" namespace std {
N#endif /* !__cplusplus */
N
N#pragma diag_push
N#pragma CHECK_MISRA("-5.7") /* keep names intact */
N
Ntypedef struct { int quot, rem; } div_t;
N
Ntypedef struct { long quot, rem; } ldiv_t;
N
N#define _LLONG_AVAILABLE 1
Ntypedef struct { long long quot, rem; } lldiv_t;
N
N#pragma diag_pop
N
N#define MB_CUR_MAX    1
N
N#ifndef NULL
N#define NULL          0
N#endif
N
N#ifndef _SIZE_T
N#define _SIZE_T
Ntypedef __SIZE_T_TYPE__ size_t;
Xtypedef unsigned size_t;
N#endif
N
N#ifndef __cplusplus
N#ifndef _WCHAR_T
N#define _WCHAR_T
Ntypedef __WCHAR_T_TYPE__ wchar_t;
Xtypedef unsigned short wchar_t;
N#endif
N#endif
N
N#define EXIT_FAILURE  1
N#define EXIT_SUCCESS  0
N
N#define RAND_MAX      32767
N
N#include <linkage.h>
L 1 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/linkage.h" 1
N/*****************************************************************************/
N/* linkage.h   v8.1.0                                                        */
N/*                                                                           */
N/* Copyright (c) 1998-2015 Texas Instruments Incorporated                    */
N/* http://www.ti.com/                                                        */
N/*                                                                           */
N/*  Redistribution and  use in source  and binary forms, with  or without    */
N/*  modification,  are permitted provided  that the  following conditions    */
N/*  are met:                                                                 */
N/*                                                                           */
N/*     Redistributions  of source  code must  retain the  above copyright    */
N/*     notice, this list of conditions and the following disclaimer.         */
N/*                                                                           */
N/*     Redistributions in binary form  must reproduce the above copyright    */
N/*     notice, this  list of conditions  and the following  disclaimer in    */
N/*     the  documentation  and/or   other  materials  provided  with  the    */
N/*     distribution.                                                         */
N/*                                                                           */
N/*     Neither the  name of Texas Instruments Incorporated  nor the names    */
N/*     of its  contributors may  be used to  endorse or  promote products    */
N/*     derived  from   this  software  without   specific  prior  written    */
N/*     permission.                                                           */
N/*                                                                           */
N/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS    */
N/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT    */
N/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
N/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT    */
N/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
N/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT    */
N/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
N/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
N/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT    */
N/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
N/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
N/*                                                                           */
N/*****************************************************************************/
N
N#ifndef _LINKAGE
N#define _LINKAGE
N
N#pragma diag_push
N#pragma CHECK_MISRA("-19.4") /* macros required for implementation */
N
N/*--------------------------------------------------------------------------*/
N/* Define _CODE_ACCESS ==> how to call RTS functions                        */
N/*--------------------------------------------------------------------------*/
N#ifndef _FAR_RTS
N#define _CODE_ACCESS
N#elif _FAR_RTS == 0
S#define _CODE_ACCESS __near
S#else
S#define _CODE_ACCESS __far
N#endif
N
N/*--------------------------------------------------------------------------*/
N/* Define _DATA_ACCESS ==> how to access RTS global or static data          */
N/*--------------------------------------------------------------------------*/
N#define _DATA_ACCESS __far
N/*--------------------------------------------------------------------------*/
N/* Define _DATA_ACCESS_NEAR ==> some C6000 RTS data must always be near     */
N/*--------------------------------------------------------------------------*/
N#define _DATA_ACCESS_NEAR __near
N
N/*--------------------------------------------------------------------------*/
N/* Define _IDECL ==> how inline functions are declared                      */
N/*--------------------------------------------------------------------------*/
N#ifdef _INLINE
N#define _IDECL static __inline
N#define _IDEFN static __inline
N#else
S#ifdef __cplusplus
S#define _IDECL extern "C" _CODE_ACCESS
S#else
S#define _IDECL extern _CODE_ACCESS
S#endif
S#define _IDEFN _CODE_ACCESS
N#endif
N
N/*--------------------------------------------------------------------------*/
N/* If compiling with non-TI compiler (e.g. GCC), nullify any TI-specific    */
N/* language extensions.                                                     */
N/*--------------------------------------------------------------------------*/
N
N#pragma diag_pop
N
N#endif /* ifndef _LINKAGE */
L 106 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h" 2
N
N/*---------------------------------------------------------------*/
N/* NOTE - Normally, abs, labs, and fabs are expanded inline, so  */
N/*        no formal definition is really required. However, ANSI */
N/*        requires that they exist as separate functions, so     */
N/*        they are supplied in the library.  The prototype is    */
N/*        here mainly for documentation.                         */
N/*---------------------------------------------------------------*/
N#pragma diag_push
N#pragma CHECK_MISRA("-16.4") /* false positives due to builtin declarations */
N    _CODE_ACCESS  int       abs(int _val); 
X      int       abs(int _val); 
N    _CODE_ACCESS  long      labs(long _val);
X      long      labs(long _val);
N#if defined(_LLONG_AVAILABLE)
X#if 1L
N    _CODE_ACCESS  long long llabs(long long _val);
X      long long llabs(long long _val);
N#endif
N#pragma diag_pop
N
N    _CODE_ACCESS int       atoi(const char *_st);
X     int       atoi(const char *_st);
N    _CODE_ACCESS long      atol(const char *_st);
X     long      atol(const char *_st);
N#if defined(_LLONG_AVAILABLE)
X#if 1L
N    _CODE_ACCESS long long atoll(const char *_st);
X     long long atoll(const char *_st);
N#endif
N    _CODE_ACCESS int       ltoa(long val, char *buffer);
X     int       ltoa(long val, char *buffer);
N          _IDECL double    atof(const char *_st);
X          static __inline double    atof(const char *_st);
N
N    _CODE_ACCESS long      strtol(const char *_st, char **_endptr, int _base);
X     long      strtol(const char *_st, char **_endptr, int _base);
N    _CODE_ACCESS unsigned long strtoul(const char *_st, char **_endptr,
X     unsigned long strtoul(const char *_st, char **_endptr,
N    					  int _base);
N#if defined(_LLONG_AVAILABLE)
X#if 1L
N    _CODE_ACCESS long long strtoll(const char *_st, char **_endptr, int _base);
X     long long strtoll(const char *_st, char **_endptr, int _base);
N    _CODE_ACCESS unsigned long long strtoull(const char *_st, char **_endptr,
X     unsigned long long strtoull(const char *_st, char **_endptr,
N					     int _base);
N#endif
N    _CODE_ACCESS float     strtof(const char * _st, 
X     float     strtof(const char * _st, 
N                                  char ** _endptr);
N    _CODE_ACCESS double    strtod(const char * _st, 
X     double    strtod(const char * _st, 
N                                  char ** _endptr);
N    _CODE_ACCESS long double strtold(const char * _st, 
X     long double strtold(const char * _st, 
N                                     char ** _endptr);
N    
N    _CODE_ACCESS int    rand(void);
X     int    rand(void);
N    _CODE_ACCESS void   srand(unsigned _seed);
X     void   srand(unsigned _seed);
N    
N    _CODE_ACCESS void  *calloc(size_t _num, size_t _size)
X     void  *calloc(size_t _num, size_t _size)
N               __ATTRIBUTE((malloc));
X               __attribute__((malloc));
N    _CODE_ACCESS void  *malloc(size_t _size)
X     void  *malloc(size_t _size)
N               __ATTRIBUTE((malloc));
X               __attribute__((malloc));
N    _CODE_ACCESS void  *realloc(void *_ptr, size_t _size)
X     void  *realloc(void *_ptr, size_t _size)
N               __ATTRIBUTE((malloc));
X               __attribute__((malloc));
N    _CODE_ACCESS void   free(void *_ptr);
X     void   free(void *_ptr);
N    _CODE_ACCESS void  *memalign(size_t _aln, size_t _size)
X     void  *memalign(size_t _aln, size_t _size)
N               __ATTRIBUTE((malloc));
X               __attribute__((malloc));
N    
N    _CODE_ACCESS void   abort(void); 
X     void   abort(void); 
N
N    typedef void (*__TI_atexit_fn)(void);
N    _CODE_ACCESS int    atexit(__TI_atexit_fn _func);
X     int    atexit(__TI_atexit_fn _func);
N
N    typedef int (*__TI_compar_fn)(const void *_a,const void *_b);
N    _CODE_ACCESS void  *bsearch(const void *_key, const void *_base,
X     void  *bsearch(const void *_key, const void *_base,
N                                size_t _nmemb, size_t _size, 
N                                __TI_compar_fn compar);
N    _CODE_ACCESS void   qsort(void *_base, size_t _nmemb, size_t _size, 
X     void   qsort(void *_base, size_t _nmemb, size_t _size, 
N                              __TI_compar_fn compar);
N
N    _CODE_ACCESS void   exit(int _status);
X     void   exit(int _status);
N    
N    _CODE_ACCESS div_t  div(int _numer, int _denom);
X     div_t  div(int _numer, int _denom);
N    _CODE_ACCESS ldiv_t ldiv(long _numer, long _denom);
X     ldiv_t ldiv(long _numer, long _denom);
N#if defined(_LLONG_AVAILABLE)
X#if 1L
N    _CODE_ACCESS lldiv_t lldiv(long long _numer, long long _denom);
X     lldiv_t lldiv(long long _numer, long long _denom);
N#endif
N
N    _CODE_ACCESS char  *getenv(const char *_string);
X     char  *getenv(const char *_string);
N    _CODE_ACCESS int    system(const char *_name);
X     int    system(const char *_name);
N
N    _CODE_ACCESS int    mblen(const char *_s, size_t _n);
X     int    mblen(const char *_s, size_t _n);
N    _CODE_ACCESS size_t mbstowcs(wchar_t *_dest, const char *_src, size_t _n);
X     size_t mbstowcs(wchar_t *_dest, const char *_src, size_t _n);
N    _CODE_ACCESS int    mbtowc(wchar_t *_dest, const char *_src, size_t _n);
X     int    mbtowc(wchar_t *_dest, const char *_src, size_t _n);
N
N    _CODE_ACCESS size_t wcstombs(char *_dest, const wchar_t *_src, size_t _n);
X     size_t wcstombs(char *_dest, const wchar_t *_src, size_t _n);
N    _CODE_ACCESS int    wctomb(char *_s, wchar_t _wc);
X     int    wctomb(char *_s, wchar_t _wc);
N
N#ifdef __cplusplus
S} /* extern "C" namespace std */
N#endif /* __cplusplus */
N
N
N#ifdef _INLINE
N
N#ifdef __cplusplus
Snamespace std {
N#endif
N
Nstatic __inline double atof(const char *_st) 
N{
N  return strtod(_st, (char **)0); 
N}
N
N#ifdef __cplusplus
S} /* namespace std */
N#endif
N
N#endif  /* _INLINE */
N
N#ifdef __cplusplus
S#if __TI_STRICT_ANSI_MODE__
Snamespace std {
S    _CODE_ACCESS inline int atexit(void (*func)(void))
S        {
S            return atexit((__TI_atexit_fn)func);
S        }
S
S    _CODE_ACCESS inline void  *bsearch(const void *_key, const void *_base,
S                                       size_t _nmemb, size_t _size, 
S                                       int (*compar)(const void *,const void *))
S        {
S            return bsearch(_key, _base, _nmemb, _size, (__TI_compar_fn)compar);
S        }
S
S    _CODE_ACCESS inline void   qsort(void *_base, size_t _nmemb, size_t _size, 
S                                     int (*_compar)(const void *, const void *))
S        {
S            return qsort(_base, _nmemb, _size, (__TI_compar_fn)_compar);
S        }
S}
S#endif
N#endif
N
N#pragma diag_pop
N
N#endif  /* ! _STDLIB */
N
N#pragma diag_push
N
N/* using declarations must occur outside header guard to support including both
N   C and C++-wrapped version of header; see _CPP_STYLE_HEADER check */
N/* this code is for C++ mode only and thus also not relevant for MISRA */
N#pragma CHECK_MISRA("-19.15")
N
N#if defined(__cplusplus) && !defined(_CPP_STYLE_HEADER)
X#if 0L && !0L
Susing std::div_t;
Susing std::ldiv_t;
S#if defined(_LLONG_AVAILABLE)
Susing std::lldiv_t;
S#endif
Susing std::size_t;
Susing std::abs;
Susing std::labs;
Susing std::atoi;
Susing std::atol;
S#if defined(_LLONG_AVAILABLE)
Susing std::llabs;
Susing std::atoll;
S#endif
Susing std::atof;
Susing std::strtol;
Susing std::strtoul;
S#if defined(_LLONG_AVAILABLE)
Susing std::strtoll;
Susing std::strtoull;
S#endif
Susing std::strtof;
Susing std::strtod;
Susing std::strtold;
Susing std::rand;
Susing std::srand;
Susing std::calloc;
Susing std::malloc;
Susing std::realloc;
Susing std::free;
Susing std::memalign;
Susing std::abort;
Susing std::atexit;
Susing std::bsearch;
Susing std::qsort;
Susing std::exit;
Susing std::div;
Susing std::ldiv;
S#if defined(_LLONG_AVAILABLE)
Susing std::lldiv;
S#endif
Susing std::getenv;
Susing std::system;
Susing std::mblen;
Susing std::mbtowc;
Susing std::wctomb;
Susing std::mbstowcs;
Susing std::wcstombs;
N#endif /* ! _CPP_STYLE_HEADER */
N
N#pragma diag_pop
N
N#pragma diag_push
N
N/* C2000-specific additions to header implemented with #include */
N#pragma CHECK_MISRA("-19.1")
N#pragma CHECK_MISRA("-19.15")
N
N
N#pragma diag_pop
L 19 "static_vars_target.c" 2
N#include <stdio.h>
L 1 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h" 1
N/*****************************************************************************/
N/* STDIO.H v8.1.0                                                            */
N/*                                                                           */
N/* Copyright (c) 1993-2015 Texas Instruments Incorporated                    */
N/* http://www.ti.com/                                                        */
N/*                                                                           */
N/*  Redistribution and  use in source  and binary forms, with  or without    */
N/*  modification,  are permitted provided  that the  following conditions    */
N/*  are met:                                                                 */
N/*                                                                           */
N/*     Redistributions  of source  code must  retain the  above copyright    */
N/*     notice, this list of conditions and the following disclaimer.         */
N/*                                                                           */
N/*     Redistributions in binary form  must reproduce the above copyright    */
N/*     notice, this  list of conditions  and the following  disclaimer in    */
N/*     the  documentation  and/or   other  materials  provided  with  the    */
N/*     distribution.                                                         */
N/*                                                                           */
N/*     Neither the  name of Texas Instruments Incorporated  nor the names    */
N/*     of its  contributors may  be used to  endorse or  promote products    */
N/*     derived  from   this  software  without   specific  prior  written    */
N/*     permission.                                                           */
N/*                                                                           */
N/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS    */
N/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT    */
N/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
N/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT    */
N/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
N/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT    */
N/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
N/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
N/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT    */
N/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
N/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
N/*                                                                           */
N/*****************************************************************************/
N#ifndef _STDIO 
N#define _STDIO
N
N
N#include <linkage.h>
N#include <stdarg.h>
L 1 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdarg.h" 1
N/*****************************************************************************/
N/* stdarg.h   v8.1.0                                                         */
N/*                                                                           */
N/* Copyright (c) 1993-2015 Texas Instruments Incorporated                    */
N/* http://www.ti.com/                                                        */
N/*                                                                           */
N/*  Redistribution and  use in source  and binary forms, with  or without    */
N/*  modification,  are permitted provided  that the  following conditions    */
N/*  are met:                                                                 */
N/*                                                                           */
N/*     Redistributions  of source  code must  retain the  above copyright    */
N/*     notice, this list of conditions and the following disclaimer.         */
N/*                                                                           */
N/*     Redistributions in binary form  must reproduce the above copyright    */
N/*     notice, this  list of conditions  and the following  disclaimer in    */
N/*     the  documentation  and/or   other  materials  provided  with  the    */
N/*     distribution.                                                         */
N/*                                                                           */
N/*     Neither the  name of Texas Instruments Incorporated  nor the names    */
N/*     of its  contributors may  be used to  endorse or  promote products    */
N/*     derived  from   this  software  without   specific  prior  written    */
N/*     permission.                                                           */
N/*                                                                           */
N/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS    */
N/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT    */
N/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
N/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT    */
N/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
N/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT    */
N/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
N/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
N/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT    */
N/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
N/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
N/*                                                                           */
N/*****************************************************************************/
N
N#ifndef _STDARG
N#define _STDARG
N
N#pragma diag_push
N#pragma CHECK_MISRA("-20.1") /* standard headers must define standard names */
N#pragma CHECK_MISRA("-20.2") /* standard headers must define standard names */
N#pragma CHECK_MISRA("-19.7") /* macros required for implementation */
N#pragma CHECK_MISRA("-19.10") /* need types as macro arguments */
N
N#ifdef __cplusplus
Snamespace std
S{
N#endif
N
N#ifdef __TI_LLVM__
S    typedef __builtin_va_list va_list;
N#else
N    typedef char *va_list;
N#endif
N
N#ifdef __cplusplus
S}
N#endif
N
N/*****************************************************************************/
N/* LLVM VARARGS HANDLING - Use builtin operations.                           */
N/*****************************************************************************/
N#if defined(__TI_LLVM__)
X#if 0L
S#define va_start(_ap, _parmN)                                   \
S         __builtin_va_start((_ap), (_parmN))
X#define va_start(_ap, _parmN)                                            __builtin_va_start((_ap), (_parmN))
S#define va_arg(_ap, _type)                                      \
S         __builtin_va_arg((_ap), _type)
X#define va_arg(_ap, _type)                                               __builtin_va_arg((_ap), _type)
S#define va_copy(_dst, _src)                                     \
S         __builtin_va_copy((_dst), (_src))
X#define va_copy(_dst, _src)                                              __builtin_va_copy((_dst), (_src))
S#define va_end(_ap)                                             \
S         __builtin_va_end(_ap) 
X#define va_end(_ap)                                                      __builtin_va_end(_ap) 
N#else
N
N/*****************************************************************************/
N/* VA_END - Reclaim resources used by varargs handling.                      */
N/*                                                                           */
N/* No action needed                                                          */
N/*****************************************************************************/
N#define va_end(_ap) ((void)0)
N
N/*****************************************************************************/
N/* VA_START - Set up the va_list pointer.                                    */
N/*****************************************************************************/
N
N#if !defined(__TI_EABI__)
X#if !1L
S
S/*---------------------------------------------------------------------------*/
S/* COFF ABI convention:                                                      */
S/* - va_list is kept aligned to 4 bytes.                                     */
S/* - va_list pointer points one word beyond the start of the last argument.  */
S/*---------------------------------------------------------------------------*/
S#define va_start(_ap, _parmN)                                    \
S         (_ap = ((char *)__va_parmadr(_parmN)) +                 \
S                (__va_argref(_parmN) ? sizeof(&_parmN) :         \
S                        (sizeof(_parmN) < 4 ? 4 : sizeof(_parmN))))
X#define va_start(_ap, _parmN)                                             (_ap = ((char *)__va_parmadr(_parmN)) +                                 (__va_argref(_parmN) ? sizeof(&_parmN) :                                 (sizeof(_parmN) < 4 ? 4 : sizeof(_parmN))))
S
N#else
N
N/*---------------------------------------------------------------------------*/
N/* ELF ABI convention:                                                       */
N/* - va_list pointer points one byte beyond the last argument.               */
N/*---------------------------------------------------------------------------*/
N#define va_start(_ap, _parmN)                                    \
N         (_ap = ((char *)__va_parmadr(_parmN)) +                 \
N                (__va_argref(_parmN) ? sizeof(&_parmN) :         \
N                        sizeof(_parmN)))
X#define va_start(_ap, _parmN)                                             (_ap = ((char *)__va_parmadr(_parmN)) +                                 (__va_argref(_parmN) ? sizeof(&_parmN) :                                 sizeof(_parmN)))
N
N#endif /* defined(__TI_EABI__) */
N
N
N/*****************************************************************************/
N/* VA_ARG - Return the next argument, adjust va_list pointer                 */
N/*                                                                           */
N/* Some arguments passed by value are turned into pass-by-reference by       */
N/* making a temporary object and passing a pointer to this temporary.  For   */
N/* such an argument (indicated by __va_argref(_type)) the actual argument    */
N/* passed is a pointer, so it must be dealt with specially.                  */
N/*                                                                           */
N/* When an argument is larger than the maximum alignment (8 bytes for double */
N/* or long long), we only align to 8 bytes.                                  */
N/*****************************************************************************/
N
N/*---------------------------------------------------------------------------*/
N/* What happens on every va_arg(_ap, _type) call is:                         */
N/* 1) Align the value of _ap (the va_list pointer) appropriately for _type   */
N/*    (the requested type).                                                  */
N/* 2) Increment _ap appropriately for _type.                                 */
N/* 3) Return the value desired by dereferencing _ap.                         */
N/*---------------------------------------------------------------------------*/
N
N#if !defined(__TI_EABI__)
X#if !1L
S
S/*---------------------------------------------------------------------------*/
S/* The big- and little-endian variants are different only because we are     */
S/* trying to support the case of the user asking for "char" or "short",      */
S/* which is actually undefined behavior (See ISO/IEC 9899:1999 7.15.1.1),    */
S/* but we are trying to be friendly.                                         */
S/*---------------------------------------------------------------------------*/
S
S#ifdef _BIG_ENDIAN
S#define va_arg(_ap, _type)                                       \
S        (__va_argref(_type)                                      \
S         ? ((_ap += 4), (**(_type **)(_ap-4)))                   \
S         : ((sizeof(_type) == sizeof(double)                     \
S             ? ((_ap = (char *)(((int)_ap + 7) & ~7)),           \
S                (_ap += 8), (*(_type *)(_ap - 8)))               \
S             : ((_ap += 4), (*(_type *)(_ap - (sizeof(_type))))))))
X#define va_arg(_ap, _type)                                               (__va_argref(_type)                                               ? ((_ap += 4), (**(_type **)(_ap-4)))                            : ((sizeof(_type) == sizeof(double)                                  ? ((_ap = (char *)(((int)_ap + 7) & ~7)),                           (_ap += 8), (*(_type *)(_ap - 8)))                            : ((_ap += 4), (*(_type *)(_ap - (sizeof(_type))))))))
S#else
S#define va_arg(_ap, _type)                                       \
S        (__va_argref(_type)                                      \
S         ? ((_ap += 4), (**(_type **)(_ap - 4)))                 \
S         : ((sizeof(_type) == sizeof(double)                     \
S             ? ((_ap = (char *)(((int)_ap + 7) & ~7)),           \
S                (_ap += 8), (*(_type *)(_ap - 8)))               \
S             : ((_ap += 4), (*(_type *)(_ap - 4))))))
X#define va_arg(_ap, _type)                                               (__va_argref(_type)                                               ? ((_ap += 4), (**(_type **)(_ap - 4)))                          : ((sizeof(_type) == sizeof(double)                                  ? ((_ap = (char *)(((int)_ap + 7) & ~7)),                           (_ap += 8), (*(_type *)(_ap - 8)))                            : ((_ap += 4), (*(_type *)(_ap - 4))))))
S#endif
S
N#else /* defined(__TI_EABI__) */
N
N/*---------------------------------------------------------------------------*/
N/* 1) Align _ap to satisfy alignment of _type.                               */
N/* 2) Increment _ap by sizeof(_type)                                         */
N/* 3) return *(ap-sizeof(_type))                                             */
N/*---------------------------------------------------------------------------*/
N
N#define __TI_SIZEOF_SCALAR_STR(_type)                                    \
N        ((sizeof(_type) > 8) ? 16 :                                      \
N         (sizeof(_type) > 4) ? 8 :                                       \
N         (sizeof(_type) > 2) ? 4 :                                       \
N         (sizeof(_type) > 1) ? 2 : 1)
X#define __TI_SIZEOF_SCALAR_STR(_type)                                            ((sizeof(_type) > 8) ? 16 :                                               (sizeof(_type) > 4) ? 8 :                                                (sizeof(_type) > 2) ? 4 :                                                (sizeof(_type) > 1) ? 2 : 1)
N
N#define __TI_ALIGNOF_SCALAR_STR(_type)                                   \
N        ((sizeof(_type) > 8) ? 16 :                                      \
N         (sizeof(_type) > 4) ? 8 :                                       \
N         (sizeof(_type) > 2) ? 4 :                                       \
N         (sizeof(_type) > 1) ? 2 : 1)
X#define __TI_ALIGNOF_SCALAR_STR(_type)                                           ((sizeof(_type) > 8) ? 16 :                                               (sizeof(_type) > 4) ? 8 :                                                (sizeof(_type) > 2) ? 4 :                                                (sizeof(_type) > 1) ? 2 : 1)
N
N#define __TI_ALIGNOF_PROMOTED_TYPE(_type)                                \
N        (__va_argref(_type) ? __alignof__(_type *) :                     \
N         __va_argcsu(_type) ? __TI_ALIGNOF_SCALAR_STR(_type) :           \
N         (sizeof(_type) < 4) ? __alignof__(int) :                        \
N                               __alignof__(_type))
X#define __TI_ALIGNOF_PROMOTED_TYPE(_type)                                        (__va_argref(_type) ? __alignof__(_type *) :                              __va_argcsu(_type) ? __TI_ALIGNOF_SCALAR_STR(_type) :                    (sizeof(_type) < 4) ? __alignof__(int) :                                                       __alignof__(_type))
N
N#define __TI_ALIGN(e, _type)                                                \
N        ((char *)(((unsigned)(e) + __TI_ALIGNOF_PROMOTED_TYPE(_type) - 1) & \
N                                 ~(__TI_ALIGNOF_PROMOTED_TYPE(_type) - 1)))
X#define __TI_ALIGN(e, _type)                                                        ((char *)(((unsigned)(e) + __TI_ALIGNOF_PROMOTED_TYPE(_type) - 1) &                                  ~(__TI_ALIGNOF_PROMOTED_TYPE(_type) - 1)))
N
N#define __TI_SIZEOF_PROMOTED_TYPE(_type)                                 \
N        (__va_argref(_type) ? sizeof(_type *) :                          \
N         __va_argcsu(_type) ? __TI_SIZEOF_SCALAR_STR(_type) :            \
N         (sizeof(_type) < 4) ? sizeof(int) : sizeof(_type))
X#define __TI_SIZEOF_PROMOTED_TYPE(_type)                                         (__va_argref(_type) ? sizeof(_type *) :                                   __va_argcsu(_type) ? __TI_SIZEOF_SCALAR_STR(_type) :                     (sizeof(_type) < 4) ? sizeof(int) : sizeof(_type))
N
N#define __TI_DEREF(e, _type)                                             \
N        (__va_argref(_type) ? **(_type **)(e) : *(_type *)(e))
X#define __TI_DEREF(e, _type)                                                     (__va_argref(_type) ? **(_type **)(e) : *(_type *)(e))
N
N/*---------------------------------------------------------------------------*/
N/* The big- and little-endian variants are different only because we are     */
N/* trying to support the case of the user asking for "char" or "short",      */
N/* which is actually undefined behavior (See ISO/IEC 9899:1999 7.15.1.1),    */
N/* but we are trying to be friendly.                                         */
N/*---------------------------------------------------------------------------*/
N
N#ifdef _BIG_ENDIAN
S
S#define __TI_BE_SIZEOF_UNPROMOTED_TYPE(_type)                            \
S        (__va_argref(_type) ? sizeof(_type *) :                          \
S         __va_argcsu(_type) ? __TI_SIZEOF_SCALAR_STR(_type) :            \
S         (sizeof(_type)))
X#define __TI_BE_SIZEOF_UNPROMOTED_TYPE(_type)                                    (__va_argref(_type) ? sizeof(_type *) :                                   __va_argcsu(_type) ? __TI_SIZEOF_SCALAR_STR(_type) :                     (sizeof(_type)))
S
S#define va_arg(_ap, _type)                                               \
S        (_ap = __TI_ALIGN(_ap, _type),                                   \
S         _ap += __TI_SIZEOF_PROMOTED_TYPE(_type),                        \
S         __TI_DEREF(_ap - __TI_BE_SIZEOF_UNPROMOTED_TYPE(_type), _type))
X#define va_arg(_ap, _type)                                                       (_ap = __TI_ALIGN(_ap, _type),                                            _ap += __TI_SIZEOF_PROMOTED_TYPE(_type),                                 __TI_DEREF(_ap - __TI_BE_SIZEOF_UNPROMOTED_TYPE(_type), _type))
S
N#else
N
N#define va_arg(_ap, _type)                                               \
N        (_ap = __TI_ALIGN(_ap, _type),                                   \
N         _ap += __TI_SIZEOF_PROMOTED_TYPE(_type),                        \
N         __TI_DEREF(_ap - __TI_SIZEOF_PROMOTED_TYPE(_type), _type))
X#define va_arg(_ap, _type)                                                       (_ap = __TI_ALIGN(_ap, _type),                                            _ap += __TI_SIZEOF_PROMOTED_TYPE(_type),                                 __TI_DEREF(_ap - __TI_SIZEOF_PROMOTED_TYPE(_type), _type))
N
N#endif /* _BIG_ENDIAN */
N
N#endif /* defined(__TI_EABI__) */
N
N#endif /* defined(__TI_LLVM__) */
N
N#pragma diag_pop
N
N#endif /* _STDARG */
N
N#pragma diag_push
N
N/* using declarations must occur outside header guard to support including both
N   C and C++-wrapped version of header; see _CPP_STYLE_HEADER check */
N/* this code is for C++ mode only and thus also not relevant for MISRA */
N#pragma CHECK_MISRA("-19.15")
N
N#if defined(__cplusplus) && !defined(_CPP_STYLE_HEADER)
X#if 0L && !0L
Susing std::va_list;
N#endif /* _CPP_STYLE_HEADER */
N
N#pragma diag_pop
L 43 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdio.h" 2
N
N/*---------------------------------------------------------------------------*/
N/* Attributes are only available in relaxed ANSI mode.                       */
N/*---------------------------------------------------------------------------*/
N#ifndef __ATTRIBUTE
S#if __TI_STRICT_ANSI_MODE__
S#define __ATTRIBUTE(attr)
S#else
S#define __ATTRIBUTE(attr) __attribute__(attr)
S#endif
N#endif
N
N
N#ifdef __cplusplus
S//----------------------------------------------------------------------------
S// <cstdio> IS RECOMMENDED OVER <stdio.h>.  <stdio.h> IS PROVIDED FOR
S// COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++
S//----------------------------------------------------------------------------
Sextern "C" namespace std {
N#endif
N
N/****************************************************************************/
N/* TYPES THAT ANSI REQUIRES TO BE DEFINED                                   */
N/****************************************************************************/
N#ifndef _SIZE_T
S#define _SIZE_T
Stypedef __SIZE_T_TYPE__ size_t;
N#endif
N
Ntypedef struct {
N      int fd;                    /* File descriptor */
N      unsigned char* buf;        /* Pointer to start of buffer */
N      unsigned char* pos;        /* Position in buffer */
N      unsigned char* bufend;     /* Pointer to end of buffer */
N      unsigned char* buff_stop;  /* Pointer to last read char in buffer */
N      unsigned int   flags;      /* File status flags (see below) */
N} FILE;
N
N#ifndef _FPOS_T
N#define _FPOS_T
Ntypedef int fpos_t; 
N#endif /* _FPOS_T */
N
N/****************************************************************************/
N/* DEVICE AND STREAM RELATED MACROS                                         */
N/****************************************************************************/
N/****************************************************************************/
N/* MACROS THAT DEFINE AND USE FILE STATUS FLAGS                             */
N/****************************************************************************/
N
N#define _IOFBF       0x0001
N#define _IOLBF       0x0002
N#define _IONBF       0x0004
N#define _BUFFALOC    0x0008
N#define _MODER       0x0010
N#define _MODEW       0x0020
N#define _MODERW      0x0040
N#define _MODEA       0x0080
N#define _MODEBIN     0x0100
N#define _STATEOF     0x0200
N#define _STATERR     0x0400
N#define _UNGETC      0x0800
N#define _TMPFILE     0x1000
N
N#define _SET(_fp, _b)      (((_fp)->flags) |= (_b))
N#define _UNSET(_fp, _b)    (((_fp)->flags) &= ~(_b))
N#define _STCHK(_fp, _b)    (((_fp)->flags) & (_b))
N#define _BUFFMODE(_fp)     (((_fp)->flags) & (_IOFBF | _IOLBF | _IONBF))
N#define _ACCMODE(_fp)      (((_fp)->flags) & (_MODER | _MODEW))
N
N/****************************************************************************/
N/* MACROS THAT ANSI REQUIRES TO BE DEFINED                                  */
N/****************************************************************************/
N#define BUFSIZ          256 
N
N#define FOPEN_MAX       _NFILE
N#define FILENAME_MAX    256  
N#define TMP_MAX         65535
N
N#ifdef __cplusplus
S#define stdin     (&std::_ftable[0])      
S#define stdout    (&std::_ftable[1])
S#define stderr    (&std::_ftable[2])
N#else
N#define stdin     (&_ftable[0])      
N#define stdout    (&_ftable[1])
N#define stderr    (&_ftable[2])
N#endif
N
N#define L_tmpnam  _LTMPNAM
N
N
N#define SEEK_SET  (0x0000)
N#define SEEK_CUR  (0x0001)
N#define SEEK_END  (0x0002)
N
N#ifndef NULL
S#define NULL 0
N#endif
N
N#ifndef EOF
N#define EOF    (-1)
N#endif
N
N/******** END OF ANSI MACROS ************************************************/
N
N#define P_tmpdir        ""                   /* Path for temp files         */
N
N/****************************************************************************/
N/* DEVICE AND STREAM RELATED DATA STRUCTURES AND MACROS                     */
N/****************************************************************************/
N#define _NFILE           20                   /* Max number of files open   */
N#define _LTMPNAM         16                   /* Length of temp name        */
N
Nextern _DATA_ACCESS FILE _ftable[_NFILE];
Xextern __far FILE _ftable[20];
Nextern _DATA_ACCESS char __TI_tmpnams[_NFILE][_LTMPNAM];
Xextern __far char __TI_tmpnams[20][16];
N
N/****************************************************************************/
N/*   FUNCTION DEFINITIONS  - ANSI                                           */
N/****************************************************************************/
N/****************************************************************************/
N/* OPERATIONS ON FILES                                                      */
N/****************************************************************************/
Nextern _CODE_ACCESS int     remove(const char *_file);
Xextern  int     remove(const char *_file);
Nextern _CODE_ACCESS int     rename(const char *_old, const char *_new);
Xextern  int     rename(const char *_old, const char *_new);
Nextern _CODE_ACCESS FILE   *tmpfile(void);
Xextern  FILE   *tmpfile(void);
Nextern _CODE_ACCESS char   *tmpnam(char *_s);
Xextern  char   *tmpnam(char *_s);
N
N/****************************************************************************/
N/* FILE ACCESS FUNCTIONS                                                    */
N/****************************************************************************/
Nextern _CODE_ACCESS int     fclose(FILE *_fp); 
Xextern  int     fclose(FILE *_fp); 
Nextern _CODE_ACCESS FILE   *fopen(const char *_fname, const char *_mode);
Xextern  FILE   *fopen(const char *_fname, const char *_mode);
Nextern _CODE_ACCESS FILE   *freopen(const char *_fname, const char *_mode,
Xextern  FILE   *freopen(const char *_fname, const char *_mode,
N			            register FILE *_fp);
Nextern _CODE_ACCESS void    setbuf(register FILE *_fp, char *_buf);
Xextern  void    setbuf(register FILE *_fp, char *_buf);
Nextern _CODE_ACCESS int     setvbuf(register FILE *_fp, register char *_buf, 
Xextern  int     setvbuf(register FILE *_fp, register char *_buf, 
N			            register int _type, register size_t _size);
Nextern _CODE_ACCESS int     fflush(register FILE *_fp); 
Xextern  int     fflush(register FILE *_fp); 
N
N/****************************************************************************/
N/* FORMATTED INPUT/OUTPUT FUNCTIONS                                         */
N/****************************************************************************/
Nextern _CODE_ACCESS int fprintf(FILE *_fp, const char *_format, ...)
Xextern  int fprintf(FILE *_fp, const char *_format, ...)
N               __ATTRIBUTE ((__format__ (__printf__, 2, 3)));
X               __attribute__((__format__ (__printf__, 2, 3)));
Nextern _CODE_ACCESS int fscanf(FILE *_fp, const char *_fmt, ...)
Xextern  int fscanf(FILE *_fp, const char *_fmt, ...)
N               __ATTRIBUTE ((__format__ (__scanf__, 2, 3)));
X               __attribute__((__format__ (__scanf__, 2, 3)));
Nextern _CODE_ACCESS int printf(const char *_format, ...)
Xextern  int printf(const char *_format, ...)
N               __ATTRIBUTE ((__format__ (__printf__, 1, 2)));
X               __attribute__((__format__ (__printf__, 1, 2)));
Nextern _CODE_ACCESS int scanf(const char *_fmt, ...)
Xextern  int scanf(const char *_fmt, ...)
N               __ATTRIBUTE ((__format__ (__scanf__, 1, 2)));
X               __attribute__((__format__ (__scanf__, 1, 2)));
Nextern _CODE_ACCESS int sprintf(char *_string, const char *_format, ...)
Xextern  int sprintf(char *_string, const char *_format, ...)
N               __ATTRIBUTE ((__format__ (__printf__, 2, 3)));
X               __attribute__((__format__ (__printf__, 2, 3)));
Nextern _CODE_ACCESS int snprintf(char *_string, size_t _n, 
Xextern  int snprintf(char *_string, size_t _n, 
N				 const char *_format, ...)
N               __ATTRIBUTE ((__format__ (__printf__, 3, 4)));
X               __attribute__((__format__ (__printf__, 3, 4)));
Nextern _CODE_ACCESS int sscanf(const char *_str, const char *_fmt, ...)
Xextern  int sscanf(const char *_str, const char *_fmt, ...)
N               __ATTRIBUTE ((__format__ (__scanf__, 2, 3)));
X               __attribute__((__format__ (__scanf__, 2, 3)));
Nextern _CODE_ACCESS int vfprintf(FILE *_fp, const char *_format, va_list _ap)
Xextern  int vfprintf(FILE *_fp, const char *_format, va_list _ap)
N               __ATTRIBUTE ((__format__ (__printf__, 2, 0)));
X               __attribute__((__format__ (__printf__, 2, 0)));
Nextern _CODE_ACCESS int vfscanf(FILE *_fp, const char *_fmt, va_list _ap)
Xextern  int vfscanf(FILE *_fp, const char *_fmt, va_list _ap)
N               __ATTRIBUTE ((__format__ (__scanf__, 2, 0)));
X               __attribute__((__format__ (__scanf__, 2, 0)));
Nextern _CODE_ACCESS int vprintf(const char *_format, va_list _ap)
Xextern  int vprintf(const char *_format, va_list _ap)
N               __ATTRIBUTE ((__format__ (__printf__, 1, 0)));
X               __attribute__((__format__ (__printf__, 1, 0)));
Nextern _CODE_ACCESS int vscanf(const char *_format, va_list _ap)
Xextern  int vscanf(const char *_format, va_list _ap)
N               __ATTRIBUTE ((__format__ (__scanf__, 1, 0)));
X               __attribute__((__format__ (__scanf__, 1, 0)));
Nextern _CODE_ACCESS int vsprintf(char *_string, const char *_format,
Xextern  int vsprintf(char *_string, const char *_format,
N				 va_list _ap)
N               __ATTRIBUTE ((__format__ (__printf__, 2, 0)));
X               __attribute__((__format__ (__printf__, 2, 0)));
Nextern _CODE_ACCESS int vsnprintf(char *_string, size_t _n, 
Xextern  int vsnprintf(char *_string, size_t _n, 
N				  const char *_format, va_list _ap)
N               __ATTRIBUTE ((__format__ (__printf__, 3, 0)));
X               __attribute__((__format__ (__printf__, 3, 0)));
Nextern _CODE_ACCESS int vsscanf(const char *_str, const char *_fmt, va_list _ap)
Xextern  int vsscanf(const char *_str, const char *_fmt, va_list _ap)
N               __ATTRIBUTE ((__format__ (__scanf__, 2, 0)));
X               __attribute__((__format__ (__scanf__, 2, 0)));
N
N/****************************************************************************/
N/* CHARACTER INPUT/OUTPUT FUNCTIONS                                         */
N/****************************************************************************/
Nextern _CODE_ACCESS int     fgetc(register FILE *_fp);
Xextern  int     fgetc(register FILE *_fp);
Nextern _CODE_ACCESS char   *fgets(char *_ptr, register int _size,
Xextern  char   *fgets(char *_ptr, register int _size,
N				  register FILE *_fp);
Nextern _CODE_ACCESS int     fputc(int _c, register FILE *_fp);
Xextern  int     fputc(int _c, register FILE *_fp);
Nextern _CODE_ACCESS int     fputs(const char *_ptr, register FILE *_fp);
Xextern  int     fputs(const char *_ptr, register FILE *_fp);
Nextern _CODE_ACCESS int     getc(FILE *_p);
Xextern  int     getc(FILE *_p);
Nextern _CODE_ACCESS int     getchar(void);
Xextern  int     getchar(void);
Nextern _CODE_ACCESS char   *gets(char *_ptr); 
Xextern  char   *gets(char *_ptr); 
Nextern _CODE_ACCESS int     putc(int _x, FILE *_fp);
Xextern  int     putc(int _x, FILE *_fp);
Nextern _CODE_ACCESS int     putchar(int _x);
Xextern  int     putchar(int _x);
Nextern _CODE_ACCESS int     puts(const char *_ptr); 
Xextern  int     puts(const char *_ptr); 
Nextern _CODE_ACCESS int     ungetc(int _c, register FILE *_fp);
Xextern  int     ungetc(int _c, register FILE *_fp);
N
N/****************************************************************************/
N/* DIRECT INPUT/OUTPUT FUNCTIONS                                            */
N/****************************************************************************/
Nextern _CODE_ACCESS size_t  fread(void *_ptr, size_t _size, size_t _count,
Xextern  size_t  fread(void *_ptr, size_t _size, size_t _count,
N				  FILE *_fp);
Nextern _CODE_ACCESS size_t  fwrite(const void *_ptr, size_t _size,
Xextern  size_t  fwrite(const void *_ptr, size_t _size,
N				   size_t _count, register FILE *_fp); 
N
N/****************************************************************************/
N/* FILE POSITIONING FUNCTIONS                                               */
N/****************************************************************************/
Nextern _CODE_ACCESS int     fgetpos(FILE *_fp, fpos_t *_pos);
Xextern  int     fgetpos(FILE *_fp, fpos_t *_pos);
Nextern _CODE_ACCESS int     fseek(register FILE *_fp, long _offset,
Xextern  int     fseek(register FILE *_fp, long _offset,
N				  int _ptrname);
Nextern _CODE_ACCESS int     fsetpos(FILE *_fp, const fpos_t *_pos);
Xextern  int     fsetpos(FILE *_fp, const fpos_t *_pos);
Nextern _CODE_ACCESS long    ftell(FILE *_fp);
Xextern  long    ftell(FILE *_fp);
Nextern _CODE_ACCESS void    rewind(register FILE *_fp); 
Xextern  void    rewind(register FILE *_fp); 
N
N/****************************************************************************/
N/* ERROR-HANDLING FUNCTIONS                                                 */
N/****************************************************************************/
Nextern _CODE_ACCESS void    clearerr(FILE *_fp);
Xextern  void    clearerr(FILE *_fp);
Nextern _CODE_ACCESS int     feof(FILE *_fp);
Xextern  int     feof(FILE *_fp);
Nextern _CODE_ACCESS int     ferror(FILE *_fp);
Xextern  int     ferror(FILE *_fp);
Nextern _CODE_ACCESS void    perror(const char *_s);
Xextern  void    perror(const char *_s);
N
N#define _getchar()      getc(stdin)
N#define _putchar(_x)    putc((_x), stdout)
N#define _clearerr(_fp)   ((void) ((_fp)->flags &= ~(_STATERR | _STATEOF)))
N
N#define _ferror(_x)     ((_x)->flags & _STATERR)
N
N#define _remove(_fl)    (unlink((_fl)))
N
N#ifdef __cplusplus
S} /* extern "C" namespace std */
N#endif  /* __cplusplus */
N
N#endif  /* #ifndef _STDIO */
N
N#if defined(__cplusplus) && !defined(_CPP_STYLE_HEADER)
X#if 0L && !0L
Susing std::size_t;
Susing std::FILE;
Susing std::fpos_t;
Susing std::_ftable;
Susing std::__TI_tmpnams;
Susing std::remove;
Susing std::rename;
Susing std::tmpfile;
Susing std::tmpnam;
Susing std::fclose;
Susing std::fopen;
Susing std::freopen;
Susing std::setbuf;
Susing std::setvbuf;
Susing std::fflush;
Susing std::fprintf;
Susing std::fscanf;
Susing std::printf;
Susing std::scanf;
Susing std::sprintf;
Susing std::snprintf;
Susing std::sscanf;
Susing std::vfprintf;
Susing std::vprintf;
Susing std::vsprintf;
Susing std::vsnprintf;
Susing std::fgetc;
Susing std::fgets;
Susing std::fputc;
Susing std::fputs;
Susing std::getc;
Susing std::getchar;
Susing std::gets;
Susing std::putc;
Susing std::putchar;
Susing std::puts;
Susing std::ungetc;
Susing std::fread;
Susing std::fwrite;
Susing std::fgetpos;
Susing std::fseek;
Susing std::fsetpos;
Susing std::ftell;
Susing std::rewind;
Susing std::clearerr;
Susing std::feof;
Susing std::ferror;
Susing std::perror;
S
N#endif  /* _CPP_STYLE_HEADER */
N
N
L 20 "static_vars_target.c" 2
N#include <omp.h>
L 1 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/openmpacc/dsp/include/omp.h" 1
N/******************************************************************************
N * Copyright (c) 2014, Texas Instruments Incorporated - http://www.ti.com/
N *   All rights reserved.
N *
N *   Redistribution and use in source and binary forms, with or without
N *   modification, are permitted provided that the following conditions are met:
N *       * Redistributions of source code must retain the above copyright
N *         notice, this list of conditions and the following disclaimer.
N *       * Redistributions in binary form must reproduce the above copyright
N *         notice, this list of conditions and the following disclaimer in the
N *         documentation and/or other materials provided with the distribution.
N *       * Neither the name of Texas Instruments Incorporated nor the
N *         names of its contributors may be used to endorse or promote products
N *         derived from this software without specific prior written permission.
N *
N *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
N *   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
N *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
N *   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
N *   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
N *   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
N *   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
N *   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
N *   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
N *   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
N *   THE POSSIBILITY OF SUCH DAMAGE.
N *****************************************************************************/
N/**
N * \file omp.h
N *
N * \brief OMP header file to include in OpenMPAcc user applications
N */
N
N#ifndef OMP_H_
N#define OMP_H_
N
N#ifdef __cplusplus
Sextern "C"
S{
N#endif /* __cplusplus */
N
N#include <stdint.h>
L 1 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdint.h" 1
N/*****************************************************************************/
N/* STDINT.H v8.1.0                                                           */
N/*                                                                           */
N/* Copyright (c) 2002-2015 Texas Instruments Incorporated                    */
N/* http://www.ti.com/                                                        */
N/*                                                                           */
N/*  Redistribution and  use in source  and binary forms, with  or without    */
N/*  modification,  are permitted provided  that the  following conditions    */
N/*  are met:                                                                 */
N/*                                                                           */
N/*     Redistributions  of source  code must  retain the  above copyright    */
N/*     notice, this list of conditions and the following disclaimer.         */
N/*                                                                           */
N/*     Redistributions in binary form  must reproduce the above copyright    */
N/*     notice, this  list of conditions  and the following  disclaimer in    */
N/*     the  documentation  and/or   other  materials  provided  with  the    */
N/*     distribution.                                                         */
N/*                                                                           */
N/*     Neither the  name of Texas Instruments Incorporated  nor the names    */
N/*     of its  contributors may  be used to  endorse or  promote products    */
N/*     derived  from   this  software  without   specific  prior  written    */
N/*     permission.                                                           */
N/*                                                                           */
N/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS    */
N/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT    */
N/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
N/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT    */
N/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
N/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT    */
N/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
N/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
N/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT    */
N/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
N/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
N/*                                                                           */
N/*****************************************************************************/
N#ifndef _STDINT_H_
N#define _STDINT_H_
N
N/* 7.18.1.1 Exact-width integer types */
N
N    typedef   signed char   int8_t;
N    typedef unsigned char  uint8_t;
N    typedef          short  int16_t;
N    typedef unsigned short uint16_t;
N    typedef          int    int32_t;
N    typedef unsigned int   uint32_t;
N
N    typedef          __int40_t  int40_t;
N    typedef unsigned __int40_t uint40_t;
N
N    typedef          long long  int64_t;
N    typedef unsigned long long uint64_t;
N
N/* 7.18.1.2 Minimum-width integer types */
N
N    typedef  int8_t   int_least8_t;
N    typedef uint8_t  uint_least8_t;
N
N    typedef  int16_t  int_least16_t;
N    typedef uint16_t uint_least16_t;
N    typedef  int32_t  int_least32_t;
N    typedef uint32_t uint_least32_t;
N
N    typedef  int40_t  int_least40_t;
N    typedef uint40_t uint_least40_t;
N
N    typedef  int64_t  int_least64_t;
N    typedef uint64_t uint_least64_t;
N
N/* 7.18.1.3 Fastest minimum-width integer types */
N
N    typedef  int32_t  int_fast8_t;
N    typedef uint32_t uint_fast8_t;
N    typedef  int32_t  int_fast16_t;
N    typedef uint32_t uint_fast16_t;
N
N    typedef  int32_t  int_fast32_t;
N    typedef uint32_t uint_fast32_t;
N
N    typedef  int40_t  int_fast40_t;
N    typedef uint40_t uint_fast40_t;
N
N    typedef  int64_t  int_fast64_t;
N    typedef uint64_t uint_fast64_t;
N
N/* 7.18.1.4 Integer types capable of holding object pointers */
N    typedef          int intptr_t;
N    typedef unsigned int uintptr_t;
N
N/* 7.18.1.5 Greatest-width integer types */
N    typedef          long long intmax_t;
N    typedef unsigned long long uintmax_t;
N
N/* 
N   According to footnotes in the 1999 C standard, "C++ implementations
N   should define these macros only when __STDC_LIMIT_MACROS is defined
N   before <stdint.h> is included." 
N*/
N#if !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS)
X#if !0L || 0L
N
N/* 7.18.2 Limits of specified width integer types */
N
N    #define  INT8_MAX   0x7f
N    #define  INT8_MIN   (-INT8_MAX-1)
N    #define UINT8_MAX   0xff
N
N    #define  INT16_MAX  0x7fff
N    #define  INT16_MIN  (-INT16_MAX-1)
N    #define UINT16_MAX  0xffff
N
N    #define  INT32_MAX  0x7fffffff
N    #define  INT32_MIN  (-INT32_MAX-1)
N    #define UINT32_MAX  0xffffffff
N
N    #define  INT40_MAX  0x7fffffffff
N    #define  INT40_MIN  (-INT40_MAX-1)
N    #define UINT40_MAX  0xffffffffff
N
N    #define  INT64_MAX  0x7fffffffffffffff
N    #define  INT64_MIN  (-INT64_MAX-1)
N    #define UINT64_MAX  0xffffffffffffffff
N
N    #define  INT_LEAST8_MAX   (INT8_MAX)
N    #define  INT_LEAST8_MIN   (INT8_MIN)
N    #define UINT_LEAST8_MAX   (UINT8_MAX)
N
N    #define  INT_LEAST16_MAX  (INT16_MAX)
N    #define  INT_LEAST16_MIN  (INT16_MIN)
N    #define UINT_LEAST16_MAX  (UINT16_MAX)
N    #define  INT_LEAST32_MAX  (INT32_MAX)
N    #define  INT_LEAST32_MIN  (INT32_MIN)
N    #define UINT_LEAST32_MAX  (UINT32_MAX)
N
N    #define  INT_LEAST40_MAX  (INT40_MAX)
N    #define  INT_LEAST40_MIN  (INT40_MIN)
N    #define UINT_LEAST40_MAX  (UINT40_MAX)
N
N    #define  INT_LEAST64_MAX  (INT64_MAX)
N    #define  INT_LEAST64_MIN  (INT64_MIN)
N    #define UINT_LEAST64_MAX  (UINT64_MAX)
N
N    #define  INT_FAST8_MAX   (INT32_MAX)
N    #define  INT_FAST8_MIN   (INT32_MIN)
N    #define UINT_FAST8_MAX   (UINT32_MAX)
N    #define  INT_FAST16_MAX  (INT32_MAX)
N    #define  INT_FAST16_MIN  (INT32_MIN)
N    #define UINT_FAST16_MAX  (UINT32_MAX)
N
N    #define  INT_FAST32_MAX  (INT32_MAX)
N    #define  INT_FAST32_MIN  (INT32_MIN)
N    #define UINT_FAST32_MAX  (UINT32_MAX)
N
N    #define  INT_FAST40_MAX  (INT40_MAX)
N    #define  INT_FAST40_MIN  (INT40_MIN)
N    #define UINT_FAST40_MAX  (UINT40_MAX)
N
N    #define  INT_FAST64_MAX  (INT64_MAX)
N    #define  INT_FAST64_MIN  (INT64_MIN)
N    #define UINT_FAST64_MAX  (UINT64_MAX)
N
N    #define INTPTR_MAX   (INT32_MAX)
N    #define INTPTR_MIN   (INT32_MIN)
N    #define UINTPTR_MAX  (UINT32_MAX)
N
N    #define INTMAX_MIN   (INT64_MIN)
N    #define INTMAX_MAX   (INT64_MAX)
N    #define UINTMAX_MAX  (UINT64_MAX)
N
N/* 7.18.3 Limits of other integer types */
N
N    #define PTRDIFF_MAX (INT32_MAX)
N    #define PTRDIFF_MIN (INT32_MIN)
N
N    #define SIG_ATOMIC_MIN (INT32_MIN)
N    #define SIG_ATOMIC_MAX (INT32_MAX)
N
N    #define SIZE_MAX (UINT32_MAX)
N
N#ifndef WCHAR_MAX
N#if !defined(__TI_WCHAR_T_BITS__) || __TI_WCHAR_T_BITS__ == 16
X#if !1L || 16 == 16
N#define WCHAR_MAX 0xffffu
N#else 
S#define WCHAR_MAX 0xffffffffu
N#endif
N#endif
N
N#ifndef WCHAR_MIN
N#define WCHAR_MIN 0
N#endif
N
N    #define WINT_MIN (INT32_MIN)
N    #define WINT_MAX (INT32_MAX)
N
N/* 7.18.4.1 Macros for minimum-width integer constants */
N
N/*
N   There is a defect report filed against the C99 standard concerning how 
N   the (U)INTN_C macros should be implemented.  Please refer to --
N   http://wwwold.dkuug.dk/JTC1/SC22/WG14/www/docs/dr_209.htm 
N   for more information.  These macros are implemented according to the
N   suggestion given at this web site.
N*/
N
N    #define  INT8_C(value)  ((int_least8_t)(value))
N    #define UINT8_C(value)  ((uint_least8_t)(value))
N    #define  INT16_C(value) ((int_least16_t)(value))
N    #define UINT16_C(value) ((uint_least16_t)(value))
N    #define  INT32_C(value) ((int_least32_t)(value))
N    #define UINT32_C(value) ((uint_least32_t)(value))
N
N    #define  INT40_C(value) ((int_least40_t)(value))
N    #define UINT40_C(value) ((uint_least40_t)(value))
N
N    #define  INT64_C(value) ((int_least64_t)(value))
N    #define UINT64_C(value) ((uint_least64_t)(value))
N
N/* 7.18.4.2 Macros for greatest-width integer constants */
N
N    #define  INTMAX_C(value) ((intmax_t)(value))
N    #define UINTMAX_C(value) ((uintmax_t)(value))
N
N#endif /* !defined(__cplusplus) || defined(__STDC_LIMIT_MACROS) */
N
N#endif /* _STDINT_H_ */
L 43 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/openmpacc/dsp/include/omp.h" 2
N#include <stdlib.h>
L 1 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/cgt-c6x/include/stdlib.h" 1
N/*****************************************************************************/
N/* stdlib.h   v8.1.0                                                         */
N/*                                                                           */
N/* Copyright (c) 1993-2015 Texas Instruments Incorporated                    */
N/* http://www.ti.com/                                                        */
N/*                                                                           */
N/*  Redistribution and  use in source  and binary forms, with  or without    */
N/*  modification,  are permitted provided  that the  following conditions    */
N/*  are met:                                                                 */
N/*                                                                           */
N/*     Redistributions  of source  code must  retain the  above copyright    */
N/*     notice, this list of conditions and the following disclaimer.         */
N/*                                                                           */
N/*     Redistributions in binary form  must reproduce the above copyright    */
N/*     notice, this  list of conditions  and the following  disclaimer in    */
N/*     the  documentation  and/or   other  materials  provided  with  the    */
N/*     distribution.                                                         */
N/*                                                                           */
N/*     Neither the  name of Texas Instruments Incorporated  nor the names    */
N/*     of its  contributors may  be used to  endorse or  promote products    */
N/*     derived  from   this  software  without   specific  prior  written    */
N/*     permission.                                                           */
N/*                                                                           */
N/*  THIS SOFTWARE  IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS    */
N/*  "AS IS"  AND ANY  EXPRESS OR IMPLIED  WARRANTIES, INCLUDING,  BUT NOT    */
N/*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR    */
N/*  A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT    */
N/*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,    */
N/*  SPECIAL,  EXEMPLARY,  OR CONSEQUENTIAL  DAMAGES  (INCLUDING, BUT  NOT    */
N/*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,    */
N/*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    */
N/*  THEORY OF  LIABILITY, WHETHER IN CONTRACT, STRICT  LIABILITY, OR TORT    */
N/*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE    */
N/*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.     */
N/*                                                                           */
N/*****************************************************************************/
N
N#ifndef _STDLIB
S#define _STDLIB
S
S
S#pragma diag_push
S#pragma CHECK_MISRA("-6.3") /* standard types required for standard headers */
S#pragma CHECK_MISRA("-8.5") /* need to define inline function */
S#pragma CHECK_MISRA("-19.1") /* #includes required for implementation */
S#pragma CHECK_MISRA("-19.7") /* need function-like macros */
S#pragma CHECK_MISRA("-20.1") /* standard headers must define standard names */
S#pragma CHECK_MISRA("-20.2") /* standard headers must define standard names */
S
S/*---------------------------------------------------------------------------*/
S/* Attributes are only available in relaxed ANSI mode.                       */
S/*---------------------------------------------------------------------------*/
S#ifndef __ATTRIBUTE
S#if __TI_STRICT_ANSI_MODE__
S#define __ATTRIBUTE(attr)
S#else
S#define __ATTRIBUTE(attr) __attribute__(attr)
S#endif
S#endif
S
S
S#ifdef __cplusplus
S/*---------------------------------------------------------------------------*/
S/* <cstdlib> IS RECOMMENDED OVER <stdlib.h>.  <stdlib.h> IS PROVIDED FOR     */
S/* COMPATIBILITY WITH C AND THIS USAGE IS DEPRECATED IN C++                  */
S/*---------------------------------------------------------------------------*/
Sextern "C" namespace std {
S#endif /* !__cplusplus */
S
S#pragma diag_push
S#pragma CHECK_MISRA("-5.7") /* keep names intact */
S
Stypedef struct { int quot, rem; } div_t;
S
Stypedef struct { long quot, rem; } ldiv_t;
S
S#define _LLONG_AVAILABLE 1
Stypedef struct { long long quot, rem; } lldiv_t;
S
S#pragma diag_pop
S
S#define MB_CUR_MAX    1
S
S#ifndef NULL
S#define NULL          0
S#endif
S
S#ifndef _SIZE_T
S#define _SIZE_T
Stypedef __SIZE_T_TYPE__ size_t;
S#endif
S
S#ifndef __cplusplus
S#ifndef _WCHAR_T
S#define _WCHAR_T
Stypedef __WCHAR_T_TYPE__ wchar_t;
S#endif
S#endif
S
S#define EXIT_FAILURE  1
S#define EXIT_SUCCESS  0
S
S#define RAND_MAX      32767
S
S#include <linkage.h>
S
S/*---------------------------------------------------------------*/
S/* NOTE - Normally, abs, labs, and fabs are expanded inline, so  */
S/*        no formal definition is really required. However, ANSI */
S/*        requires that they exist as separate functions, so     */
S/*        they are supplied in the library.  The prototype is    */
S/*        here mainly for documentation.                         */
S/*---------------------------------------------------------------*/
S#pragma diag_push
S#pragma CHECK_MISRA("-16.4") /* false positives due to builtin declarations */
S    _CODE_ACCESS  int       abs(int _val); 
S    _CODE_ACCESS  long      labs(long _val);
S#if defined(_LLONG_AVAILABLE)
S    _CODE_ACCESS  long long llabs(long long _val);
S#endif
S#pragma diag_pop
S
S    _CODE_ACCESS int       atoi(const char *_st);
S    _CODE_ACCESS long      atol(const char *_st);
S#if defined(_LLONG_AVAILABLE)
S    _CODE_ACCESS long long atoll(const char *_st);
S#endif
S    _CODE_ACCESS int       ltoa(long val, char *buffer);
S          _IDECL double    atof(const char *_st);
S
S    _CODE_ACCESS long      strtol(const char *_st, char **_endptr, int _base);
S    _CODE_ACCESS unsigned long strtoul(const char *_st, char **_endptr,
S    					  int _base);
S#if defined(_LLONG_AVAILABLE)
S    _CODE_ACCESS long long strtoll(const char *_st, char **_endptr, int _base);
S    _CODE_ACCESS unsigned long long strtoull(const char *_st, char **_endptr,
S					     int _base);
S#endif
S    _CODE_ACCESS float     strtof(const char * _st, 
S                                  char ** _endptr);
S    _CODE_ACCESS double    strtod(const char * _st, 
S                                  char ** _endptr);
S    _CODE_ACCESS long double strtold(const char * _st, 
S                                     char ** _endptr);
S    
S    _CODE_ACCESS int    rand(void);
S    _CODE_ACCESS void   srand(unsigned _seed);
S    
S    _CODE_ACCESS void  *calloc(size_t _num, size_t _size)
S               __ATTRIBUTE((malloc));
S    _CODE_ACCESS void  *malloc(size_t _size)
S               __ATTRIBUTE((malloc));
S    _CODE_ACCESS void  *realloc(void *_ptr, size_t _size)
S               __ATTRIBUTE((malloc));
S    _CODE_ACCESS void   free(void *_ptr);
S    _CODE_ACCESS void  *memalign(size_t _aln, size_t _size)
S               __ATTRIBUTE((malloc));
S    
S    _CODE_ACCESS void   abort(void); 
S
S    typedef void (*__TI_atexit_fn)(void);
S    _CODE_ACCESS int    atexit(__TI_atexit_fn _func);
S
S    typedef int (*__TI_compar_fn)(const void *_a,const void *_b);
S    _CODE_ACCESS void  *bsearch(const void *_key, const void *_base,
S                                size_t _nmemb, size_t _size, 
S                                __TI_compar_fn compar);
S    _CODE_ACCESS void   qsort(void *_base, size_t _nmemb, size_t _size, 
S                              __TI_compar_fn compar);
S
S    _CODE_ACCESS void   exit(int _status);
S    
S    _CODE_ACCESS div_t  div(int _numer, int _denom);
S    _CODE_ACCESS ldiv_t ldiv(long _numer, long _denom);
S#if defined(_LLONG_AVAILABLE)
S    _CODE_ACCESS lldiv_t lldiv(long long _numer, long long _denom);
S#endif
S
S    _CODE_ACCESS char  *getenv(const char *_string);
S    _CODE_ACCESS int    system(const char *_name);
S
S    _CODE_ACCESS int    mblen(const char *_s, size_t _n);
S    _CODE_ACCESS size_t mbstowcs(wchar_t *_dest, const char *_src, size_t _n);
S    _CODE_ACCESS int    mbtowc(wchar_t *_dest, const char *_src, size_t _n);
S
S    _CODE_ACCESS size_t wcstombs(char *_dest, const wchar_t *_src, size_t _n);
S    _CODE_ACCESS int    wctomb(char *_s, wchar_t _wc);
S
S#ifdef __cplusplus
S} /* extern "C" namespace std */
S#endif /* __cplusplus */
S
S
S#ifdef _INLINE
S
S#ifdef __cplusplus
Snamespace std {
S#endif
S
Sstatic __inline double atof(const char *_st) 
S{
S  return strtod(_st, (char **)0); 
S}
S
S#ifdef __cplusplus
S} /* namespace std */
S#endif
S
S#endif  /* _INLINE */
S
S#ifdef __cplusplus
S#if __TI_STRICT_ANSI_MODE__
Snamespace std {
S    _CODE_ACCESS inline int atexit(void (*func)(void))
S        {
S            return atexit((__TI_atexit_fn)func);
S        }
S
S    _CODE_ACCESS inline void  *bsearch(const void *_key, const void *_base,
S                                       size_t _nmemb, size_t _size, 
S                                       int (*compar)(const void *,const void *))
S        {
S            return bsearch(_key, _base, _nmemb, _size, (__TI_compar_fn)compar);
S        }
S
S    _CODE_ACCESS inline void   qsort(void *_base, size_t _nmemb, size_t _size, 
S                                     int (*_compar)(const void *, const void *))
S        {
S            return qsort(_base, _nmemb, _size, (__TI_compar_fn)_compar);
S        }
S}
S#endif
S#endif
S
S#pragma diag_pop
S
N#endif  /* ! _STDLIB */
N
N#pragma diag_push
N
N/* using declarations must occur outside header guard to support including both
N   C and C++-wrapped version of header; see _CPP_STYLE_HEADER check */
N/* this code is for C++ mode only and thus also not relevant for MISRA */
N#pragma CHECK_MISRA("-19.15")
N
N#if defined(__cplusplus) && !defined(_CPP_STYLE_HEADER)
X#if 0L && !0L
Susing std::div_t;
Susing std::ldiv_t;
S#if defined(_LLONG_AVAILABLE)
Susing std::lldiv_t;
S#endif
Susing std::size_t;
Susing std::abs;
Susing std::labs;
Susing std::atoi;
Susing std::atol;
S#if defined(_LLONG_AVAILABLE)
Susing std::llabs;
Susing std::atoll;
S#endif
Susing std::atof;
Susing std::strtol;
Susing std::strtoul;
S#if defined(_LLONG_AVAILABLE)
Susing std::strtoll;
Susing std::strtoull;
S#endif
Susing std::strtof;
Susing std::strtod;
Susing std::strtold;
Susing std::rand;
Susing std::srand;
Susing std::calloc;
Susing std::malloc;
Susing std::realloc;
Susing std::free;
Susing std::memalign;
Susing std::abort;
Susing std::atexit;
Susing std::bsearch;
Susing std::qsort;
Susing std::exit;
Susing std::div;
Susing std::ldiv;
S#if defined(_LLONG_AVAILABLE)
Susing std::lldiv;
S#endif
Susing std::getenv;
Susing std::system;
Susing std::mblen;
Susing std::mbtowc;
Susing std::wctomb;
Susing std::mbstowcs;
Susing std::wcstombs;
N#endif /* ! _CPP_STYLE_HEADER */
N
N#pragma diag_pop
N
N#pragma diag_push
N
N/* C2000-specific additions to header implemented with #include */
N#pragma CHECK_MISRA("-19.1")
N#pragma CHECK_MISRA("-19.15")
N
N
N#pragma diag_pop
L 44 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/openmpacc/dsp/include/omp.h" 2
N
N/* Lock type */
Ntypedef struct _omp_lock
N{
N    volatile int  entered;
N    void         *owner_task;
N    volatile int  count;
N    int           hw_sem;
N} *omp_lock_t;
N
Ntypedef struct _omp_lock *omp_nest_lock_t;
N
N/* Schedule kinds */
Ntypedef enum omp_sched_t
N{
N  omp_sched_static = 1,
N  omp_sched_dynamic = 2,
N  omp_sched_guided = 3,
N  omp_sched_auto = 4
N} omp_sched_t;
N
N/* Execution environment routines */
Nvoid omp_set_num_threads (int nthreads);
Nint omp_get_num_threads (void);
Nint omp_get_max_threads (void);
Nint omp_get_thread_num (void);
Nint omp_get_num_procs(void);
Nint omp_in_parallel(void);
Nvoid omp_set_dynamic (int nthreads);
Nint omp_get_dynamic (void);
Nvoid omp_set_nested (int val);
Nint omp_get_nested (void);
Nvoid omp_set_schedule (omp_sched_t kind, int modifier);
Nvoid omp_get_schedule (omp_sched_t *kind, int *modifier);
Nint omp_get_thread_limit (void);
Nvoid omp_set_max_active_levels(int val);
Nint omp_get_max_active_levels(void);
Nint omp_get_level (void);
Nint omp_get_ancestor_thread_num (int level);
Nint omp_get_team_size (int level);
Nint omp_get_active_level (void);
Nvoid omp_set_default_device (int device_num);
Nint omp_get_default_device (void);
Nint omp_get_num_devices (void);
Nint omp_is_initial_device (void);
N#if 0
Sint omp_in_final (void);
N#endif
N
N/* Lock Routines */
Nvoid omp_init_lock (omp_lock_t* lock);
Nvoid omp_destroy_lock (omp_lock_t *lock);
Nvoid omp_set_lock (omp_lock_t* lock);
Nint  omp_test_lock (omp_lock_t* lock);
Nvoid omp_unset_lock (omp_lock_t* lock);
N
Nvoid omp_init_nest_lock (omp_nest_lock_t* lock);
Nvoid omp_destroy_nest_lock (omp_nest_lock_t *lock);
Nvoid omp_set_nest_lock (omp_nest_lock_t* lock);
Nint  omp_test_nest_lock (omp_nest_lock_t* lock);
Nvoid omp_unset_nest_lock (omp_nest_lock_t* lock);
N
N/* Timing Routines */
Ndouble omp_get_wtime (void);
Ndouble omp_get_wtick (void);
N
N/*-----------------------------------------------------------------------------
N* The extended TI built-in function set
N*----------------------------------------------------------------------------*/
Nvoid     __touch           (const char *p, uint32_t size);
Nuint32_t __core_num        (void);
Nuint32_t __clock           (void);
Nuint64_t __clock64         (void);
Nvoid     __cycle_delay     (uint64_t cyclesToDelay);
Nvoid     __mfence          (void);
N
Nvoid     __cache_l1d_none  (void);
Nvoid     __cache_l1d_all   (void);
Nvoid     __cache_l1d_4k    (void);
Nvoid     __cache_l1d_8k    (void);
Nvoid     __cache_l1d_16k   (void);
Nvoid     __cache_l1d_flush (void);
N
Nvoid     __cache_l2_none   (void);
Nvoid     __cache_l2_128k   (void);
Nvoid     __cache_l2_256k   (void);
Nvoid     __cache_l2_512k   (void);
Nvoid     __cache_l2_flush  (void);
N
Nvoid     __heap_init_ddr  (void *ptr, size_t size);
Nvoid*    __malloc_ddr     (size_t size);
Nvoid*    __calloc_ddr     (size_t num, size_t size);
Nvoid*    __realloc_ddr    (void *ptr,  size_t size);
Nvoid     __free_ddr       (void *ptr);
Nvoid*    __memalign_ddr   (size_t alignment, size_t size);
N
Nvoid     __heap_init_msmc (void *ptr, size_t size);
Nvoid*    __malloc_msmc    (size_t size);
Nvoid*    __calloc_msmc    (size_t num, size_t size);
Nvoid*    __realloc_msmc   (void *ptr, size_t size);
Nvoid     __free_msmc      (void *ptr);
Nvoid*    __memalign_msmc  (size_t alignment, size_t size);
N
Nvoid     __heap_init_l2   (void *ptr, size_t size);
Nvoid*    __malloc_l2      (size_t size);
N
N#ifdef __cplusplus
S} /* extern "C" */
N#endif /* __cplusplus */
N
N#endif /* OMP_H_ */
L 21 "static_vars_target.c" 2
N
N#include "ti_omp_device.h"
L 1 "/opt/ti-processor-sdk-linux-am57xx-evm-03.00.00.04/targetNFS/usr/share/ti/openmpacc/dsp/include/ti_omp_device.h" 1
N/******************************************************************************
N * Copyright (c) 2014, Texas Instruments Incorporated - http://www.ti.com/
N *   All rights reserved.
N *
N *   Redistribution and use in source and binary forms, with or without
N *   modification, are permitted provided that the following conditions are met:
N *       * Redistributions of source code must retain the above copyright
N *         notice, this list of conditions and the following disclaimer.
N *       * Redistributions in binary form must reproduce the above copyright
N *         notice, this list of conditions and the following disclaimer in the
N *         documentation and/or other materials provided with the distribution.
N *       * Neither the name of Texas Instruments Incorporated nor the
N *         names of its contributors may be used to endorse or promote products
N *         derived from this software without specific prior written permission.
N *
N *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
N *   AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
N *   IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
N *   ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
N *   LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
N *   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
N *   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
N *   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
N *   CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
N *   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
N *   THE POSSIBILITY OF SUCH DAMAGE.
N *****************************************************************************/
N#ifndef _TI_OMP_DEVICE_H_
N#define _TI_OMP_DEVICE_H_
N
N/*-----------------------------------------------------------------------------
N* While this file may also be included when compiling device code, the 
N* following check ensures that these definitions are only included when 
N* compiling for the host.
N*----------------------------------------------------------------------------*/
N#ifdef __arm__
S#ifdef __cplusplus
S#include <stdint.h>
S#include <cstdlib>
S#define _QUALIFY    std::
Sextern "C" {
S#else
S#include <stdint.h>
S#include <stdlib.h>
S#define _QUALIFY
S#endif /* __cplusplus */
S
Svoid*  __malloc_ddr  (_QUALIFY size_t size);
Svoid*  __malloc_msmc (_QUALIFY size_t size);
Svoid   __free_ddr    (void* ptr);
Svoid   __free_msmc   (void* ptr);
S
Svoid omp_set_default_device (int device_num);
Sint  omp_get_default_device (void);
Sint  omp_get_num_devices    (void);
Sint  omp_is_initial_device  (void);
S
S/*-----------------------------------------------------------------------------
S* This following are definitions for host versions of some functions that are 
S* typically applicable to executions wihin DSP environments. They include 
S* built-in DSP functions as well as functions for cache and dynamic memory 
S* management. Host versions of these functions are provided to facilitate easy 
S* compilation and execution of target regions that may end up being executed 
S* on the host. 
S*----------------------------------------------------------------------------*/
Svoid     __touch           (const char *p, uint32_t size);
Suint32_t __core_num        (void);
Suint32_t __clock           (void);
Suint64_t __clock64         (void);
Svoid     __cycle_delay     (uint64_t cyclesToDelay);
Svoid     __mfence          (void);
S
Svoid     __cache_l1d_none  (void);
Svoid     __cache_l1d_all   (void);
Svoid     __cache_l1d_4k    (void);
Svoid     __cache_l1d_8k    (void);
Svoid     __cache_l1d_16k   (void);
Svoid     __cache_l1d_flush (void);
S
Svoid     __cache_l2_none   (void);
Svoid     __cache_l2_128k   (void);
Svoid     __cache_l2_256k   (void);
Svoid     __cache_l2_512k   (void);
Svoid     __cache_l2_flush  (void);
S
Svoid     __heap_init_ddr  (void *ptr, size_t size);
Svoid*    __calloc_ddr     (size_t num, size_t size);
Svoid*    __realloc_ddr    (void *ptr,  size_t size);
Svoid*    __memalign_ddr   (size_t alignment, size_t size);
S
Svoid     __heap_init_msmc (void *ptr, size_t size);
Svoid*    __calloc_msmc    (size_t num, size_t size);
Svoid*    __realloc_msmc   (void *ptr, size_t size);
Svoid*    __memalign_msmc  (size_t alignment, size_t size);
S
Svoid     __heap_init_l2   (void *ptr, size_t size);
Svoid*    __malloc_l2      (size_t size);
S
S#ifdef __cplusplus
S} /* extern "C" */
S#endif /* __cplusplus */
S
N#endif /* __arm__ */
N
N#endif /* _TI_OMP_DEVICE_H_ */
L 23 "static_vars_target.c" 2
N
Nstatic inline int add_static(int a, int b) {
N  return a + b;
N}
N
Nstatic float static_buffer[100];
N
N#pragma omp end declare target
N/* ------------------------------------------------------------------------ *
N * End of Target Function Declarations and Includes                         *
N * ======================================================================== */
N
Nvoid target_init(float value)
N{
N  #pragma omp target data map(to: value)
N  {
N    int i;
N    #pragma omp parallel for schedule(static)
N    for (i = 0; i < NValues; i++) {
X    for (i = 0; i < 100; i++) {
N      static_buffer[i] = value * (i+1);
N    }
N  }
N}
N
Nint target_function(
N  char        * in_buffer,
N  char        * out_buffer,
N  int           size,
N  int           repeats,
N  HostMessage * host_signals)
N{
N  wno_unused_var_(repeats);
X  (void)(repeats);
N
N  int acc;
N  #pragma omp target data map(to:     in_buffer[0:size], size) \
N                          map(from:   out_buffer[0:size], acc) \
N                          map(tofrom: host_signals[0:1])
X  #pragma omp target data map(to:     in_buffer[0:size], size)                           map(from:   out_buffer[0:size], acc)                           map(tofrom: host_signals[0:1])
N  {
N    oam_task__enter(host_signals);
N
N    if (!oam_task__aborted(host_signals)) {
N      #pragma omp target map(to:     in_buffer[0:size], size) \
N                         map(from:   out_buffer[0:size], acc) \
N                         map(tofrom: host_signals[0:1])
X      #pragma omp target map(to:     in_buffer[0:size], size)                          map(from:   out_buffer[0:size], acc)                          map(tofrom: host_signals[0:1])
N      {
N        acc = 0;
N        double time_start = omp_get_wtime();
N        double limit_ms = (double)(host_signals->timeout_after_ms);
N        double time_max = time_start + (limit_ms / 1000.0);
N        printf("target time start: %.3f max. ms: %d -> time limit: %.3f\n",
N               time_start, host_signals->timeout_after_ms, time_max);
N
N        #pragma omp parallel \
N                    shared(host_signals, acc)
X        #pragma omp parallel                     shared(host_signals, acc)
N        {
N          int aborted = 0;
N          int i;
N          #pragma omp for schedule(static)
N          for (i = 0; i < size; i++) {
N            // Poll cancellation request:
N            if (0 != i % 10 ||
N                0 == oam_task__poll_cancel_request(
N                       time_start, host_signals, &aborted)) {
N              // Work step implementation here
N              out_buffer[i] = static_buffer[i % NValues] += in_buffer[i];
X              out_buffer[i] = static_buffer[i % 100] += in_buffer[i];
N            }
N          }
N        }
N        printf("target section 1: leaving\n");
N      } // omp target
N    }
N
N    // Synchronize with host:
N    oam_task__step(host_signals);
N
N    printf("target data section: leaving\n");
N  } // omp target data
N
N  return acc;
N}
N
W "static_vars_target.c" 24 19 function "add_static" was declared but never referenced
