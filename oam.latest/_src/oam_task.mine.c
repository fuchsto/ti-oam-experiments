
#include <oam/oam_task.h>
#include <oam/oam_comm.h>
#include <oam/oam_types.h>
#include <oam/macro.h>
#include <oam/oam_time.h>
#include <oam/logging.h>

#include <omp.h>

/* ======================================================================== *
 * Host Functions                                                           *
 * ======================================================================== */

void oam_task__request_cancel(
  HostMessage * host_signals)
{
  if (!host_signals) {
      return;
  }
  // Prevent double cancellation: the DSP might already
  // be in the process of unloading mapped data, including
  // host_signals
  if (host_signals->cancel == 1) {
      LOG_WARN("oam_task__request_cancel: Ignored, cancellation already requested");
      return;
  }

  #pragma omp target map(from: host_signals[0:1])
  {
    LOG_TARGET_WARN(
        "oam_task__request_cancel: target start time: %ld us",
        host_signals->time_start_us);
    host_signals->cancel           = 1;
    host_signals->ret              = OMPACC_TASK__CANCEL;
    host_signals->timeout_after_us = 1;
  }
}

void oam_task__request_timeout(
  HostMessage * host_signals,
  long          timeout_us)
{
  if (!host_signals) {
      return;
  }
  // Prevent double cancellation: the DSP might already
  // be in the process of unloading mapped data, including
  // host_signals
  if (host_signals->cancel == 1) {
      LOG_WARN("oam_task__request_timeout: Ignored, cancel request active");
      return;
  }
  #pragma omp target map(from: host_signals[0:1])
  {
    host_signals->timeout_after_us = timeout_us;
    LOG_INFO(
        "oam_task__request_timeout: %ld us, target start time: %ld us",
        host_signals->timeout_after_us,
        host_signals->time_start_us);
  }
}

void oam_task__step(
  HostMessage * host_signals)
{
#ifdef OMPACC
  oam_comm__flush_signals(host_signals);

  long time_start_us = host_signals->time_start_us;
  long time_now_us   = (long)(oam_timestamp() * 1e6);
  long elapsed_us    = (time_now_us - time_start_us);

  #pragma omp atomic
  host_signals->timeout_after_us -= elapsed_us;

// printf("[ %.3fs ] >> oam_task__step: elapsed: %dms\n",
//        omp_get_wtime(), elapsed_us);
#endif
}

void oam_task__finalize(
  HostMessage * host_signals)
{
#if defined(MV_LOG__ENABLED)
  ts_t target_time_now_s = 0;
#endif

  if (!host_signals) {
    return;
  }

  host_signals->targets_entered--;
}

