#ifndef OAM__LOGGING_H__INCLUDED
#define OAM__LOGGING_H__INCLUDED

#ifdef OMPACC

#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target
#include <omp.h>
// #include <stdio.h>
#include <string.h>
int    snprintf(char *, size_t, const char *, ...);
int    printf(const char *, ...);
char * strrchr(const char *, int);
#include <oam/oam_time.h>
#pragma omp end declare target

#ifdef __cplusplus
} /* extern "C" */
#endif

#else

#include <log.h>

#endif /* OMPACC */



#include <stdio.h>
#include <string.h>

#include <oam/oam_time.h>

#define LOG_HOST__FILE_FIELD_WIDTH          49
#define LOG_HOST__LINE_FIELD_WIDTH           5
#define LOG_HOST__MAX_MESSAGE_SIZE         256

#if defined(OMPACC) && \
    defined(MV_LOG__ENABLED) && \
    defined(MV_LOG__MEMORY)
#  define MV_LOG__OAM__VSMEM                 1
#endif

#define LOG_HOST_MSG__(level, ...) \
  do { \
    const char * path; \
    char       * base; \
    char msg_buf[LOG_HOST__MAX_MESSAGE_SIZE+1]; \
    int  sn_ret = snprintf(msg_buf, LOG_HOST__MAX_MESSAGE_SIZE, __VA_ARGS__); \
    if (sn_ret < 0) { \
      break; \
    } \
    path = __FILE__; \
    base = strrchr(path, '/'); \
    printf( \
      "[ host ] %-5s  [ %011.6f ] %-*s:%-*d %s | %s\n", \
      level, \
      oam_timestamp(), \
      LOG_HOST__FILE_FIELD_WIDTH, (base ? base + 1 : path), \
      LOG_HOST__LINE_FIELD_WIDTH, __LINE__, \
      __DATE__ "-" __TIME__, \
      msg_buf); \
  } while (0)

#define LOG_TARGET_MSG__(level, ...) \
  do { \
    const char * path; \
    char       * base; \
    char msg_buf[LOG_HOST__MAX_MESSAGE_SIZE+1]; \
    int  sn_ret = snprintf(msg_buf, LOG_HOST__MAX_MESSAGE_SIZE, __VA_ARGS__); \
    if (sn_ret < 0) { \
      break; \
    } \
    path = __FILE__; \
    base = strrchr(path, '/'); \
    printf( \
      "%-5s  [ %011.6f ] %-*s:%-*d %s | %s\n", \
      level, \
      oam_timestamp(), \
      LOG_HOST__FILE_FIELD_WIDTH, (base ? base + 1 : path), \
      LOG_HOST__LINE_FIELD_WIDTH, __LINE__, \
      __DATE__ "-" __TIME__, \
      msg_buf); \
  } while (0)


/* Warning and Error log levels are always enabled */
#define LOG_TARGET_ERROR(...)  LOG_TARGET_MSG__("ERROR", __VA_ARGS__)
#define LOG_TARGET_WARN(...)   LOG_TARGET_MSG__("WARN",  __VA_ARGS__)
#define LOG_HOST_ERROR(...)    LOG_HOST_MSG__("ERROR", __VA_ARGS__)
#define LOG_HOST_WARN(...)     LOG_HOST_MSG__("WARN",  __VA_ARGS__)

#define LOG_TARGET_INFO(...)  LOG_TARGET_MSG__("INFO",  __VA_ARGS__)
#define LOG_HOST_INFO(...)    LOG_HOST_MSG__("INFO",  __VA_ARGS__)

#if defined(MV_LOG__ENABLED)
#  define LOG_HOST_DEBUG(...)   LOG_HOST_MSG__("DEBUG", __VA_ARGS__)
#  define LOG_HOST_TRACE(...)   LOG_HOST_MSG__("TRACE", __VA_ARGS__)
#  define LOG_TARGET_DEBUG(...) LOG_TARGET_MSG__("DEBUG", __VA_ARGS__)
#  define LOG_TARGET_TRACE(...) LOG_TARGET_MSG__("TRACE", __VA_ARGS__)
#else

#  define LOG_HOST_DEBUG(...)   do { } while(0)
#  define LOG_HOST_TRACE(...)   do { } while(0)

#  define LOG_TARGET_DEBUG(...) do { } while(0)
#  define LOG_TARGET_TRACE(...) do { } while(0)
#endif /* defined(MV_LOG__ENABLED) */

#if defined(OMPACC_TARGET)
#  define LOG_TRACE(...)       LOG_TARGET_TRACE(__VA_ARGS__)
#  define LOG_DEBUG(...)       LOG_TARGET_DEBUG(__VA_ARGS__)
#  define LOG_INFO(...)        LOG_TARGET_INFO(__VA_ARGS__)
#  define LOG_WARN(...)        LOG_TARGET_WARN(__VA_ARGS__)
#  define LOG_ERROR(...)       LOG_TARGET_ERROR(__VA_ARGS__)
#else
#  define LOG_TRACE(...)       LOG_HOST_TRACE(__VA_ARGS__)
#  define LOG_DEBUG(...)       LOG_HOST_DEBUG(__VA_ARGS__)
#  define LOG_INFO(...)        LOG_HOST_INFO(__VA_ARGS__)
#  define LOG_WARN(...)        LOG_HOST_WARN(__VA_ARGS__)
#  define LOG_ERROR(...)       LOG_HOST_ERROR(__VA_ARGS__)
#endif

#endif // OAM__LOGGING_H__INCLUDED
