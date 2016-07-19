#ifndef BASE__OAM_TASK_H__INCLUDED
#define BASE__OAM_TASK_H__INCLUDED

#include <base/oam_types.h>
#include <base/macro.h>

/* ======================================================================== *
 * Host Functions                                                           *
 * ======================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Register the start of a target task.
 * To be called before first target offload region.
 */
void oam_task__enter(
  HostMessage * host_signals);

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

#endif // BASE__OAM_TASK_H__INCLUDED
