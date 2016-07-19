#ifndef DSP_IMG_FILTER__MACRO_H__INCLUDED
#define DSP_IMG_FILTER__MACRO_H__INCLUDED

#include <string.h>

/**
 * To avoid compiler warning Wunused-var.
 */
#define wno_unused_var_(var) (void)(var)

/**
 * Wondering every time why swap() is not part of the C standard lib ...
 */
#define swap_(x,y) do { \
  unsigned char tmp[sizeof(x) == sizeof(y) ? (signed)(sizeof(x)) : -1]; \
  memcpy(tmp, &y,  sizeof(x)); \
  memcpy(&y,  &x,  sizeof(x)); \
  memcpy(&x,  tmp, sizeof(x)); \
} while (0)

/**
 * TI's implementation of cmath has no definition of rint().
 */
#define rint_(f) ((int)(f))

#define min_(a, b) ((a) < (b) ? (a) : (b))

#endif /* DSP_IMG_FILTER__MACRO_H__INCLUDED */
