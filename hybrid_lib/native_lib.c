
#include "host_only_lib.h"

#include <math.h>

int native_function(
    double      * array_a,
    double      * array_b,
    int           nelem)
{
  int result = 0;
  for (int i = 0; i < nelem; i++) {
    array_a[i] += array_b[i];
    result     += array_a[i];
  }
  return result;
}

