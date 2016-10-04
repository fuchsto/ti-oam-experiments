
#include <oam/oam_task_target.h>
#include <oam/oam_comm.h>
#include <oam/oam_types.h>
#include <oam/logging.h>

/* ======================================================================== *
 * Target Functions                                                         *
 * ======================================================================== */

int oam_task__poll_cancel_request(
  double        time_start,
  HostMessage * host_signals,
  int         * aborted)
{
  if (0 != *aborted) {
    return 1;
  }
  if (oam_comm__poll_interval(host_signals)) {
    int timeout = oam_comm__poll_message(time_start, host_signals);
//  *aborted = oam_task__aborted(host_signals);
    *aborted = (timeout > 0) || (0 != host_signals->cancel);
#if MV_LOG__OAM__COMM
    if (host_signals->timeout_after_us > 0) {
      /* cannot use (host_signals->time_start_us - time_now_us) as host
       * and targets have different epochs, but target epoch always starts
       * at the beginnign of the task region: */
      int time_now_us     = omp_get_wtime() * 1e6;
      int task_elapsed_us = time_now_us - (time_start * 1e6);
      int num_poll_logs   = 10;
      int log_interval_us = host_signals->timeout_after_us / num_poll_logs;

      if (task_elapsed_us % log_interval_us < host_signals->poll_interval_us)
      {
        LOG_DEBUG("oam_comm__poll_interval: at %d%% of max. task duration",
                  (task_elapsed_us * 100) / host_signals->timeout_after_us);
      }
    }
#endif
  }
  return *aborted;
}

