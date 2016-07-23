#ifndef INL_INTERFACE_H__INCLUDED
#define INL_INTERFACE_H__INCLUDED

#ifdef __cplusplus
extern "C" {
#endif

static inline int   _sinl_int(int a) {
  return a + 1;
}

static inline char  _sinl_char(char a) {
  return a + 10;
}

static inline float _sinl_float(float a) {
  return a + 100.0;
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* INL_INTERFACE_H__INCLUDED */
