#ifndef DSP_FREQ_TYPES_H__INCLUDED
#define DSP_FREQ_TYPES_H__INCLUDED


#ifdef __cplusplus
extern "C" {
#endif

#pragma omp declare target

typedef long long chrono_ts;

typedef struct target_measurements_s {
  uint64_t  target_clock64_count;
  uint64_t  host_clock64_count;
  double    target_omp_wtime_start;
  double    target_omp_wtime_end;
  double    host_omp_wtime_start;
  double    host_omp_wtime_end;
  chrono_ts host_chrono_ts_start;
  chrono_ts host_chrono_ts_end;
} target_measurements;

#pragma omp end declare target

#ifdef __cplusplus
} // extern "C"
#endif

#endif // DSP_FREQ_TYPES_H__INCLUDED
