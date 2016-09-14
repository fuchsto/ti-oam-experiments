
#include <oam/oam_task.h>
#include <oam/oam_comm.h>
#include <oam/oam_types.h>
#include <oam/macro.h>

#include <stdio.h>
#include <omp.h>

/* ======================================================================== *
 * Host Functions                                                           *
 * ======================================================================== */

void oam_task__enter(
  HostMessage * host_signals)
{
  host_signals->time_start_us = (int)(omp_get_wtime() * 1000);
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
    host_signals->timeout_after_us = 0;

    #pragma omp target update to (host_signals[0:1])
    ;
  }
}

void oam_task__step(
  HostMessage * host_signals)
{
  oam_comm__flush_signals(host_signals);

  int time_start_us = host_signals->time_start_us;
  int time_now_us   = (int)(omp_get_wtime() * 1000);
  int elapsed_us    = (time_now_us - time_start_us);

  #pragma omp atomic
  host_signals->timeout_after_us -= elapsed_us;

// printf("[ %.3fs ] >> oam_task__step: elapsed: %dms\n",
//        omp_get_wtime(), elapsed_us);
}

