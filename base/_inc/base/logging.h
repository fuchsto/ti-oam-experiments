#ifndef DSP_IMG_FILTER__LOGGING_H__INCLUDED
#define DSP_IMG_FILTER__LOGGING_H__INCLUDED

#include <stdio.h>
#include <string.h>

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
      "[ DEBUG ] [  HOST  ] %-*s:%-*d | %s\n", \
      LOG_HOST__FILE_FIELD_WIDTH, (base ? base + 1 : path), \
      LOG_HOST__LINE_FIELD_WIDTH, __LINE__, \
      msg_buf); \
  } while (0)
// #else
// #define LOG_HOST_DEBUG(...) do { } while(0);
// #endif

#endif // DSP_IMG_FILTER_LOGGING_H__INCLUDED
