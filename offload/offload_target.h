#ifndef OFFLOAD_TARGET_H_
#define OFFLOAD_TARGET_H_

#ifdef __cplusplus
extern "C" {
#endif

void data_map_target(
  char * in_buffer,
  char * out_buffer,
  int    size);

void sym_alloc_target(
  int    size_a,
  int    size_b);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // OFFLOAD_TARGET_H_
