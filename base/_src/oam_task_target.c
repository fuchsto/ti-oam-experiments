
#include <base/oam_task_target.h>
#include <base/oam_comm.h>
#include <base/oam_types.h>

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
    oam_comm__poll_message(time_start, host_signals);
//  *aborted = oam_task__aborted(host_signals);
    *aborted = (0 != host_signals->cancel);
  }
  return *aborted;
}

