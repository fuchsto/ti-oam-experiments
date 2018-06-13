#ifndef OAM__OAM_TIME_H__INCLUDED
#define OAM__OAM_TIME_H__INCLUDED

#pragma omp declare target
#  include <omp.h>
#  if defined(OMPACC)
#    include "ti_omp_device.h"
#  endif
#pragma omp end declare target

#if !defined(_OPENMP) && defined(ARMLINUX)
// #pragma message "Using clock_gettime in oam_timestamp"
#  include <time.h>
#  include <sched.h>
#  include <sys/time.h>
#endif

#include <oam/oam_types.h>

#pragma omp declare target
double omp_get_wtime(void);

typedef double        ts_t;
typedef unsigned long ts_clk_t;

static inline ts_t     oam_timestamp();
static inline ts_clk_t oam_clocktick();


#if !defined(OMPACC_TARGET)
#include <time.h>
static struct timespec clkbase = { 0, 0 };
static unsigned long long baseTime_us=0;

#define OAM_TIME__TIME_VAL_2_NS(sec,nsec) \
    (((unsigned long long)(sec)) * \
     ((unsigned long long)1000000000u) + \
     ((unsigned long long)(nsec)))
#endif


#pragma omp end declare target

/**
 * Returns a current time stamp in seconds.
 * Note that time measurements at host and target are unrelated and cannot
 * be compared.
 */
static inline ts_t oam_timestamp()
{
  /* Alternative methods to obtain timestamp: */

#if defined(_OPENMP)
  /* - Any target with OpenMP support, or OpenMP accelerator model explicitly
   *   enabled in build. Using the wrapper provided by OpenMP interface.
   *
   * ! NOTE that clock resolutions (see omp_get_wtick) differ between
   * ! host and target. Comparison of elapsed time spans measured with
   * ! omp_get_wtime will not give meaningful results.
   */
  return omp_get_wtime();
#elif defined(OMPACC_TARGET)
  /* - Non-Linux target without OpenMP support, assuming DSP build.
   *   Using CPU cycle counter (TLSC on ARM, RDTSC on x64) wrapper provided
   *   by underlying OpenCL runtime.
   *   Divide elapsed ticks by bogomips for unit conversion to seconds.
   *
   * ! NOTE that this yields measurements suited for performance comparison
   * ! but not actual elapsed wall-clock time.
   */
  return __clock64();
#elif defined(ARMLINUX)
  /* - ARM target but no OpenMP support, using clock_gettime provided by
   *   Linux base system.
   *
   * ! NOTE that this requires linker flag -lrt which is not supported by
   * ! cl6x target compiler.
   */
//# if TODO__FIX_ME
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts); /* or: CLOCK_MONOTONIC */
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return (double)(timestamp) * 1.0e-6;
//# else
//   return 0;
//# endif
#else
# error "Could not resolve implementation for oam_timestamp()"
#endif
}

static inline ts_clk_t oam_clocktick()
{
#if defined(OMPACC_TARGET)
  /* - Non-Linux target without OpenMP support, assuming DSP build.
   *   Using CPU cycle counter (TLSC on ARM, RDTSC on x64) wrapper provided
   *   by underlying OpenCL runtime.
   *   Divide elapsed ticks by bogomips for unit conversion to seconds.
   *
   * ! NOTE that this yields measurements suited for performance comparison
   * ! but not actual elapsed wall-clock time.
   */
  return __clock();
#elif defined(ARMLINUX) || defined(OMPACC)
  /* - ARM target but no OpenMP support, using clock_gettime provided by
   *   Linux base system.
   *
   * ! NOTE that this requires linker flag -lrt which is not supported by
   * ! cl6x target compiler.
   */
  struct timespec tv;
  unsigned long long ltvTime;

  clock_gettime(CLOCK_MONOTONIC, &tv);

  ltvTime = OAM_TIME__TIME_VAL_2_NS(tv.tv_sec,tv.tv_nsec);

  return (ts_clk_t)((ltvTime-baseTime_us)/((unsigned long long)1000000));

#else
# error "Could not resolve implementation for oam_clocktick()"
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
