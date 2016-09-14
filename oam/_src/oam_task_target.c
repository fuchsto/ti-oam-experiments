
#include <oam/oam_task_target.h>
#include <oam/oam_comm.h>
#include <oam/oam_types.h>

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
  }
  return *aborted;
}

