
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
  #pragma omp target data map(to: host_signals[0:1])
  {
    // LOG_WARN("oam_task__request_cancel: "
    //          "target start time: %lu us, current time: %lu us",
    //          host_signals->time_start_us,
    //          (long)(oam_timestamp() * 1e6));

    host_signals->cancel           = 1;
    host_signals->ret              = OMPACC_TASK__CANCEL;
    host_signals->timeout_after_us = 0;

    // #pragma omp target update to (host_signals[0:1])
    // ;
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
  if (!host_signals) {
    return;
  }
  host_signals->targets_entered--;

#if 1 || \
    defined(MV_LOG__ENABLED) && \
    defined(MV_LOG__PROFILING)
  ts_t target_time_now_s = 0;
  #pragma omp target data map(from: target_time_now_s)
  {
    target_time_now_s = oam_timestamp();
  }
  LOG_INFO("oam_task__finalize: duration:%.2f ms, target task level:%d",
           ( (target_time_now_s * 1.0e3) -             // s  to ms
             (host_signals->time_start_us * 1.0e-3) ), // us to ms
           host_signals->targets_entered);
#endif
}

