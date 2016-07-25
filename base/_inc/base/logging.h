#ifndef DSP_IMG_FILTER__LOGGING_H__INCLUDED
#define DSP_IMG_FILTER__LOGGING_H__INCLUDED

// #include <sysdefs/qualifier.h>

#ifdef OMPACC
#pragma omp declare target
#include <omp.h>
#include <stdio.h>
#include <string.h>
int    snprintf(char * , size_t, const char *, ...);
int    printf(const char *, ...);
char * strrchr(const char *, int);
#include <base/oam_time.h>
#pragma omp end declare target
#endif /* OMPACC */

#include <stdio.h>
#include <string.h>

#include <base/oam_time.h>

#define LOG_HOST__FILE_FIELD_WIDTH       32
#define LOG_HOST__LINE_FIELD_WIDTH        5
#define LOG_HOST__MAX_MESSAGE_SIZE      128

// #ifdef ENABLE_LOGGING

#define LOG_HOST_DEBUG(...) \
  do { \
    char msg_buf[LOG_HOST__MAX_MESSAGE_SIZE+1]; \
    int  sn_ret = snprintf(msg_buf, LOG_HOST__MAX_MESSAGE_SIZE, __VA_ARGS__); \
    if (sn_ret < 0) { \
      break; \
    } \
    const char * path = __FILE__; \
    char       * base = strrchr(path, '/'); \
    printf( \
      "[ host ] DEBUG   [ %09.6f ] %-*s:%-*d | %s\n", \
      oam_timestamp(), \
      LOG_HOST__FILE_FIELD_WIDTH, (base ? base + 1 : path), \
      LOG_HOST__LINE_FIELD_WIDTH, __LINE__, \
      msg_buf); \
  } while (0)

#define LOG_TARGET_DEBUG(...) \
  do { \
    char msg_buf[LOG_HOST__MAX_MESSAGE_SIZE+1]; \
    int  sn_ret = snprintf(msg_buf, LOG_HOST__MAX_MESSAGE_SIZE, __VA_ARGS__); \
    if (sn_ret < 0) { \
      break; \
    } \
    const char * path = __FILE__; \
    char       * base = strrchr(path, '/'); \
    printf( \
      "DEBUG   [ %09.6f ] %-*s:%-*d | %s\n", \
      oam_timestamp(), \
      LOG_HOST__FILE_FIELD_WIDTH, (base ? base + 1 : path), \
      LOG_HOST__LINE_FIELD_WIDTH, __LINE__, \
      msg_buf); \
  } while (0)

#if defined(OMPACC_TARGET)
#  define LOG_DEBUG(...) do { LOG_TARGET_DEBUG(__VA_ARGS__); } while(0)
#else
#  define LOG_DEBUG(...) do { LOG_HOST_DEBUG(__VA_ARGS__);   } while(0)
#endif

#endif // DSP_IMG_FILTER_LOGGING_H__INCLUDED
