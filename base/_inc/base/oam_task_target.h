#ifndef BASE__OAM_TASK_TARGET_H__INCLUDED
#define BASE__OAM_TASK_TARGET_H__INCLUDED

#include <base/oam_task.h>
#include <base/oam_types.h>
#include <base/macro.h>

/* ======================================================================== *
 * Target Functions                                                         *
 * ======================================================================== */
#pragma omp declare target

// See:
// http://downloads.ti.com/mctools/esd/docs/opencl/memory/cache-operations.html
void __cache_l2_flush(void);
void __cache_l1d_flush(void);

/**
 * Poll host signals handle to test if the current thread's task is to be
 * aborted.
 *
 * Parameter \c aborted must be a thread-local variable.
 */
int oam_task__poll_cancel_request(
  /// The task's start timestamp in seconds, usually obtained from
  /// \c omp_get_wtime().
  double        time_start,
  /// The task's host signals handle.
  HostMessage * host_signals,
  /// Thread-local variable, set to \c 1 if the task has been aborted.
  int         * aborted);

/**
 * Whether the specified host signals handle requested cancellation of
 * the target task.
 */
static inline int oam_task__aborted(
  HostMessage * host_signals);

#pragma omp end declare target


#endif // BASE__OAM_TASK_TARGET_H__INCLUDED
