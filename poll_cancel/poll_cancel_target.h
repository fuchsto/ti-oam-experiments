#ifndef COMM_ASYNC_TARGET_H_
#define COMM_ASYNC_TARGET_H_

#include <base/oam_types.h>
#include <base/oam_task.h>

#ifdef __cplusplus
extern "C" {
#endif

int target_function(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_message_buf);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // COMM_ASYNC_TARGET_H_
