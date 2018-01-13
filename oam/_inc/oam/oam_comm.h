#ifndef OAM__OAM_COMM_H__INCLUDED
#define OAM__OAM_COMM_H__INCLUDED

#include <oam/oam_types.h>

/* ======================================================================== *
 * Host Functions                                                           *
 * ======================================================================== */

#ifdef __cplusplus
extern "C" {
#endif


HostMessage * oam_comm__host_signals_init(
  HostMessage * host_mesg,
  long poll_interval_us,
  long timeout_after_us);

/**
 * Creates a new host signals descriptor for a target task with specified
 * poll interval and timeout, in milliseconds.
 */
HostMessage * oam_comm__host_signals_new(
  long poll_interval_us,
  long timeout_after_us);


/**
 * Deletes the specified host signals descriptor.
 */
void oam_comm__host_signals_delete(
  HostMessage * host_signals);

/**
 * Notify targets of changes on the referenced host message.
 */
void oam_comm__flush_signals(
  HostMessage * host_signals);

/**
 * Synchronize host data at target.
 * To be called between offload sections in target tasks.
 */
void oam_comm__update_target_data(
  char         * host_data,
  unsigned int   num_bytes);

/**
 * Synchronize target data to host.
 * To be called between offload sections in target tasks.
 */
void oam_comm__update_host_data(
  char         * target_data,
  unsigned int   num_bytes);

#ifdef __cplusplus
} /* extern "C" */
#endif

/* ======================================================================== *
 * Target Functions                                                         *
 * ======================================================================== */
#pragma omp declare target

#include <omp.h>
// #include <stdio.h>

// See:
// http://downloads.ti.com/mctools/esd/docs/opencl/memory/cache-operations.html
void __cache_l2_flush(void);
void __cache_l1d_flush(void);

/**
 * Tests if the current thread's poll interval has been reached.
 *
 * If at least <tt>host_signals->poll_interval_ms</tt> ms have been elapsed
 * since the last poll at thread \c t, returns \c 1 and
 * <tt>host_signals->last_poll_time_ms[t]</tt> is set to the current time.
 */
int oam_comm__poll_interval(
  HostMessage * host_signals);

int oam_comm__poll_message(
  /// The task's start timestamp in seconds, usually obtained from
  /// \c omp_get_wtime().
  double        time_start,
  /// The task's host signals handle.
  HostMessage * host_signals);

#pragma omp end declare target

#endif // OAM__OAM_COMM_H__INCLUDED
