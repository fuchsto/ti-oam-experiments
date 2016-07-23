
#include "ext_interface.h"
#include "nested_interface.h"

#pragma omp declare target
static inline char ext_sil(char c);

#include "check_compiler_flag.h"

#include "inl_interface.h"
#pragma omp end declare target

#define EXT_MACRO(x) ((x) / 2 * 5)

double ext_foo(double d) {
  return 1.23 * EXT_MACRO(d);
}

int    ext_bar(int    n) {
  return compute(ext_sil(n) * 2, 3);
}

static inline char ext_sil(char c) {
  return _sinl_char(c + 10);
}

