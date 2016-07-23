#ifndef DECLARE_TARGET_H_
#define DECLARE_TARGET_H_

#include <base/oam_types.h>

#include "check_compiler_flag.h"

// Uncomment this include to produce an example where a header that should
// be included in a target declaration region has been previously included:
//
// include "inl_interface.h"

#ifdef __cplusplus
extern "C" {
#endif

int async_target(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_message_buf);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // DECLARE_TARGET_H_
