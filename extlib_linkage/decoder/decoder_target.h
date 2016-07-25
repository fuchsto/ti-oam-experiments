#ifndef TASK_EXTLIB__DECODER__DECODER_TARGET_H__INCLUDED
#define TASK_EXTLIB__DECODER__DECODER_TARGET_H__INCLUDED

#include <base/oam_task.h>


#ifdef __cplusplus
extern "C" {
#endif

unsigned int decode_target(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_signals);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* TASK_EXTLIB__DECODER__DECODER_TARGET_H__INCLUDED */
