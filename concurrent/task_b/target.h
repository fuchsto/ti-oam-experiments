#ifndef CONCURRENT__TASK_B__TARGET_TASK_H_
#define CONCURRENT__TASK_B__TARGET_TASK_H_

#include <oam/oam_types.h>

#ifdef __cplusplus
extern "C" {
#endif

int task_b_target(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_message_buf);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // CONCURRENT__TASK_B__TARGET_TASK_H_
