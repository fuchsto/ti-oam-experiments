
#include <oam/oam_task_target.h>
#include <oam/oam_comm.h>
#include <oam/oam_types.h>
#include <oam/oam_time.h>
#include <oam/logging.h>

/* ======================================================================== *
 * Target Functions                                                         *
 * ======================================================================== */

void oam_task__enter(
  HostMessage * host_signals)
{
  if (host_signals->time_start_us == 0) {
    host_signals->time_start_us = (long)(oam_timestamp() * 1e6);
  }
  host_signals->targets_entered++;

  LOG_INFO("oam_task__enter: target task level:%d",
           host_signals->targets_entered);
}

int oam_task__poll_cancel_request(
  double        time_start,
  HostMessage * host_signals,
  int         * aborted)
{
  if (0 != *aborted) {
    return 1;
  }
  if (host_signals->ret == OMPACC_TASK__TIMEOUT) {
    *aborted = 1;
  } else if (oam_comm__poll_interval(host_signals)) {
    int timeout = oam_comm__poll_message(time_start, host_signals);
//  *aborted = oam_task__aborted(host_signals);
    *aborted = (timeout > 0) || (0 != host_signals->cancel);
#if MV_LOG__OAM__COMM
    if (host_signals->timeout_after_us > 0) {
      /* cannot use (host_signals->time_start_us - time_now_us) as host
       * and targets have different epochs, but target epoch always starts
       * at the beginning of the task region: */
      long time_now_us     = oam_timestamp() * 1e6;
      long task_elapsed_us = time_now_us - (time_start * 1e6);
      long num_poll_logs   = 10;
      long log_interval_us = host_signals->timeout_after_us / num_poll_logs;

      if (task_elapsed_us % log_interval_us < host_signals->poll_interval_us)
      {
        LOG_DEBUG("oam_comm__poll_cancel_request: at %d%% of max. task duration",
                  (task_elapsed_us * 100) / host_signals->timeout_after_us);
      }
    }
#endif
  }
  return *aborted;
}

