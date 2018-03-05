#ifndef OAM__OAM_TASK_H__INCLUDED
#define OAM__OAM_TASK_H__INCLUDED

#include <oam/oam_types.h>
#include <oam/oam_comm.h>
#include <oam/logging.h>
#include <oam/macro.h>

/* ======================================================================== *
 * Host Functions                                                           *
 * ======================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

extern void __ti_set_default_timeout(int device_num, int timeout_in_ms);
extern int  __ti_get_timeout_status(int device_num);


/**
 * Send cancellation request signal to all targets listening on the
 * specified host signals handle.
 */
void oam_task__request_cancel(
  HostMessage * host_signals);

/**
 * Set timeout for all targets listening on the specified host signals
 * handle.
 */
void oam_task__request_timeout(
  HostMessage * host_signals,
  long          timeout_us);

/**
 * Synchronize signals handle between host and target.
 * To be called between target offload regions.
 */
void oam_task__step(
  HostMessage * host_signals);

/**
 * Finalize the target offload task, states that the target code's owner
 * thread can safely be joined.
 * To be called in host code when the target offload region returned.
 */
void oam_task__finalize(
  HostMessage * host_signals);

/**
 * Whether the offloaded task has been finalized and the code's owner
 * thread can safely be joined.
 */
static inline int oam_task__finalized(
  HostMessage * host_signals)
{
  if (!host_signals) {
      return -1;
  }
  if (host_signals->targets_entered <= 0) {
      return 1;
  }
  return 0;
}


void oam_task__set_default_timeout(long timeout_us);


#pragma omp declare target

/**
 * Whether the task referenced by the specified host signals handle
 * has been signalled for cancellation or timeout.
 */
static inline int oam_task__aborted(
  HostMessage * host_signals)
{
  if (!host_signals) {
      return -1;
  }
  return (1 == host_signals->cancel ||
          host_signals->ret == OMPACC_TASK__CANCEL ||
          host_signals->ret == OMPACC_TASK__TIMEOUT);
}

#pragma omp end declare target

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // OAM__OAM_TASK_H__INCLUDED
