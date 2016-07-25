
#include <base/oam_task.h>
#include <base/oam_comm.h>
#include <base/oam_types.h>

#include <base/macro.h>

#include <stdio.h>
#include <omp.h>

/* ======================================================================== *
 * Host Functions                                                           *
 * ======================================================================== */

void oam_task__enter(
  HostMessage * host_signals)
{
  host_signals->time_start_ms = (int)(omp_get_wtime() * 1000);
  printf("[ %.3fs ] >> oam_task__start\n", omp_get_wtime());

  oam_comm__flush_signals(host_signals);
}

void oam_task__request_cancel(
  HostMessage * host_signals)
{
  #pragma omp target data map(to: host_signals[0:1])
  {
    host_signals->cancel           = 1;
    host_signals->ret              = OMPACC_TASK__CANCEL;
    host_signals->timeout_after_ms = 0;

    #pragma omp target update to (host_signals[0:1])
    ;
  }
}

void oam_task__step(
  HostMessage * host_signals)
{
  oam_comm__flush_signals(host_signals);

  int time_start_ms = host_signals->time_start_ms;
  int time_now_ms   = (int)(omp_get_wtime() * 1000);
  int elapsed_ms    = (time_now_ms - time_start_ms);

  #pragma omp atomic
  host_signals->timeout_after_ms -= elapsed_ms;

// printf("[ %.3fs ] >> oam_task__step: elapsed: %dms\n",
//        omp_get_wtime(), elapsed_ms);
}

