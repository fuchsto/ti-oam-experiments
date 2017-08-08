
#include "hybrid_wrapper.h"

#include "hybrid/target_fun.h"
#include "hybrid/host_fun.h"

#include <oam/oam_comm.h>
#include <oam/oam_types.h>
#include <oam/macro.h>

#include <omp.h>
#include <ti_omp_device.h>


int hybrid_wrapper(
    char        * array_a,
    char        * array_b,
    int           nelem,
    int           nrepeat,
    HostMessage * signals)
{
  wno_unused_var_(signals);

  int result = 0;
  
  result += host_function(array_a, array_b, nelem);

  result += target_function(array_a, array_b, nelem);

  #pragma omp target map(tofrom: array_a[0:nelem], \
                                 array_b[0:nelem], \
                                 nelem) \
                     map(from:   result)
  {
    int r;
    for (r = 0; r < nrepeat; r++) {
      result = target_function(array_a, array_b, nelem);
    }
  }
  return result;
}

