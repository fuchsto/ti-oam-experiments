
#include "native_lib_wrapper.h"
#include "native_lib.h"

#include <oam/oam_comm.h>
#include <oam/oam_types.h>
#include <oam/macro.h>

#include <omp.h>
#include <ti_omp_device.h>


int native_lib_wrapper(
    double      * array_a,
    double      * array_b,
    int           nelem,
    int           nrepeat,
    HostMessage * signals)
{
  wno_unused_var_(signals);

  int result = 0;
  #pragma omp target map(tofrom: array_a[0:nelem], \
                                 array_b[0:nelem], \
                                 nelem) \
                     map(from:   result)
  {
    for (int r = 0; r < nrepeat; r++) {
      result = native_function(array_a, array_b, nelem);
    }
  }
  return result;
}

