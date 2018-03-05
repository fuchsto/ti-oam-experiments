
#include <omp.h>

#include <oam/oam_types.h>
#include <oam/oam_task.h>

#include "dsp_freq_types.h"
#include "dsp_freq_target.h"

#include <time.h>
// #include <sched.h>
#include <sys/time.h>

static inline double clock_realtime_ts() {
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts);
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return (double)(timestamp * 1.0e-6);
}


void measurement_task(
  target_measurements * measurements,
  HostMessage         * host_signals)
{
  measurements->host_realtime_start  = clock_realtime_ts();

  target_task(
      measurements,
      host_signals);

  measurements->host_realtime_end    = clock_realtime_ts();
}


