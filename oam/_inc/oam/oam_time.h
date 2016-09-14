#ifndef OAM__OAM_TIME_H__INCLUDED
#define OAM__OAM_TIME_H__INCLUDED

#include <oam/oam_types.h>
#include <omp.h>

typedef double ts_t;

/**
 * Returns a current time stamp.
 * Note that time measurements at host and target are unrelated and cannot
 * be compared.
 */
static inline ts_t oam_timestamp()
{
  /* Alternative methods to obtain timestamp: */

#if 0
  /* - Using clock_gettime
   *
   * ! NOTE that this requires linker flag -lrt which is not supported by
   * ! cl6x target compiler.
   */
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts); /* or: CLOCK_MONOTONIC */
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
#endif

#if 0
  /* - Using CPU cycle counter (TLSC on ARM, RDTSC on x64) wrapper provided
   *   by underlying OpenCL runtime.
   *   Divide elapsed ticks by bogomips for unit conversion to seconds.
   *
   * ! NOTE that this yields measurements suited for performance comparison
   * ! but not actual elapsed wall-clock time.
   */
  return __clock64();
#endif

#if 1
  /* - Using the wrapper provided by OpenMP interface.
   *
   * ! NOTE that clock resolutions (see omp_get_wtick) differ between
   * ! host and target. Comparison of elapsed time spans measured with
   * ! omp_get_wtime will not give meaningful results.
   */
  return omp_get_wtime();
#endif
}

/**
 * Elapsed time since specified time stamp, in seconds.
 */
static inline double oam_elapsed_since(ts_t ts_start)
{
  /* Apply frequency scaling and/or unit conversion to seconds here if
   * necessary, depending on implementation of timestamp(). */

#if 1
  return (double)(oam_timestamp() - ts_start);
#else
  return (double)((timestamp() - ts_start) / ARCH_BOGOMIPS);
#endif
}

#endif /* OAM__OAM_TIME_H__INCLUDED */
