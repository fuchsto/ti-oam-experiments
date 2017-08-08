
#include "target_fun.h"

int hybrid__dummy_target_fun() {
  int rc;
  #pragma omp target map (from: rc)
  {
    rc = 1;
  }
  return rc;
}

int target_function(
    char      * array_a,
    char      * array_b,
    int         nelem)
{
  int i;
  int result = 0;
  for (i = 0; i < nelem; i++) {
    array_a[i] += array_b[i];
    result     += array_a[i];
  }
  return result;
}
