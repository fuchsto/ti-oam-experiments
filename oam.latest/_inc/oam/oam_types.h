#ifndef OAM__OAM_TYPES_H__INCLUDED
#define OAM__OAM_TYPES_H__INCLUDED

/**
 * \file oam_types.h
 *
 * Public OAM type definitions.
 */

#include <oam/oam_config.h>
#include <oam/macro.h>

/**
 * Return code of a target task to be obtained from `HostMessage`
 * object.
 *
 */
typedef enum OMPAccTaskRet_t
{
  /// Unspecified return code, indicates that the target task has not
  /// been completed yet.
  OMPACC_TASK__UNDEFINED = -1,
  /// Target task completed successfully.
  OMPACC_TASK__OK        =  0,
  /// Target task has been canceled.
  OMPACC_TASK__CANCEL,
  /// Target task timed out.
  OMPACC_TASK__TIMEOUT
} OMPAccTaskRet;

/**
 * Message object for signalling between host and target regions.
 *
 * \see oam/oam_comm.h
 * \see oam/oam_task.h
 */
typedef struct HostMessage_t
{
  /// Optional general-purpose value transmitted between host and target
  /// with this signal.
  int           param;
  /// Flag, set to 1 to issue cancellation request.
  int           cancel;
  /// Current depth of nested target regions. Incremented by 1 when entering
  /// a target region, decremented by 1 after leaving a target region.
  /// Value 0 indicates that no OMPACC target section is currently executed.
  int           targets_entered;
  /// Timeout period in microseconds.
  long          timeout_after_us;
  /// Start timestamp of the toplevel target region in microseconds.
  long          time_start_us;
  /// Message poll interval in microseconds. Tradeoff between cancellation
  /// latency and polling overhead.
  long          poll_interval_us;
  /// Internal: timestamp of last message poll by OAM thread, in microseconds.
  long          last_poll_time_us[OAM_TASK__MAX_THREADS];
  /// Return code of the OMPACC task.
  OMPAccTaskRet ret;
} HostMessage;

#endif // OAM__OAM_TYPES_H__INCLUDED
