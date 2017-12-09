
#include "decoder_target_fun_a.h"

#pragma omp declare target
// #include "../ext/ext.h"
int ext__foo(int value);
#pragma omp end declare target

int decoder_fun_a(int value)
{
  return ext__foo(value * 2);
}

