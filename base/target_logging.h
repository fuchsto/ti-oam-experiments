#ifndef DSP_IMG_FILTER__TARGET_LOGGING_H__INCLUDED
#define DSP_IMG_FILTER__TARGET_LOGGING_H__INCLUDED


#define LOGGING__FILE_FIELD_WIDTH       32
#define LOGGING__LINE_FIELD_WIDTH        5
#define LOGGING__MAX_MESSAGE_SIZE      128

#ifdef ENABLE_LOGGING
#define LOG_TARGET_DEBUG(...) \
  do { \
    char msg_buf[LOGGING__MAX_MESSAGE_SIZE+1]; \
    int  sn_ret = snprintf(msg_buf, LOGGING__MAX_MESSAGE_SIZE, __VA_ARGS__); \
    if (sn_ret < 0) { \
      break; \
    } \
    const char * path = __FILE__; \
    char       * base = strrchr(path, '/'); \
    printf( \
      "[ DEBUG ] [ TARGET ] %-*s:%-*d | %s\n", \
      LOGGING__FILE_FIELD_WIDTH, (base ? base + 1 : path), \
      LOGGING__LINE_FIELD_WIDTH, __LINE__, \
      msg_buf); \
  } while (0)
#else
#define LOG_TARGET_DEBUG(...) do { } while(0);
#endif

#endif // DSP_IMG_FILTER__TARGET_LOGGING_H__INCLUDED
