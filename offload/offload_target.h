#ifndef OFFLOAD_TARGET_H_
#define OFFLOAD_TARGET_H_

#ifdef __cplusplus
extern "C" {
#endif

void offload_target(
  char * in_buffer,
  char * out_buffer,
  int    buffer_size);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // OFFLOAD_TARGET_H_
