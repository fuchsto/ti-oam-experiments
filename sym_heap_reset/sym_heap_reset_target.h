#ifndef SYM_HEAP_RESET_TARGET_H_
#define SYM_HEAP_RESET_TARGET_H_

#include <oam/oam_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int async_target(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           leak_nbytes,
  int           repeats,
  HostMessage * host_message_buf);

int target_task(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           leak_nbytes,
  int           repeats,
  HostMessage * host_signals);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // SYM_HEAP_RESET_TARGET_H_
