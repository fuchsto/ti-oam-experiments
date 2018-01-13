#ifndef OAM__OAM_TASK_H__INCLUDED
#define OAM__OAM_TASK_H__INCLUDED

#include <oam/oam_types.h>
#include <oam/macro.h>

/* ======================================================================== *
 * Host Functions                                                           *
 * ======================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Send cancellation request signal to all targets listening on the
 * specified host signals handle.
 */
void oam_task__request_cancel(
  HostMessage * host_signals);

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
#ifdef OMPACC
  return host_signals->targets_entered == 0;
#else
  return 1;
#endif
}

/**
 * Whether the specified host signals handle requested cancellation of
 * the target task.
 */
static inline int oam_task__aborted(
  HostMessage * host_signals)
{
  return (1 == host_signals->cancel ||
          host_signals->ret == OMPACC_TASK__CANCEL ||
          host_signals->ret == OMPACC_TASK__TIMEOUT);
}

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif // OAM__OAM_TASK_H__INCLUDED
