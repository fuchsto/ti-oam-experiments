
#include "host_fun.h"

int host_function(
    char      * array_a,
    char      * array_b,
    int         nelem)
{
  int result = 0;
  int i;
  for (i = 0; i < nelem; i++) {
    array_a[i] += array_b[i];
    result     += array_a[i];
  }
  return result;
}

