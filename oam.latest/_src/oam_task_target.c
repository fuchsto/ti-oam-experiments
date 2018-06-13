
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
  if (!host_signals) {
	  return;
  }
  if (host_signals->time_start_us <= 0) {
    host_signals->time_start_us = (long)(oam_timestamp() * 1000000L);
  }
#pragma omp flush
  host_signals->targets_entered++;
}



