
#include <base/oam_comm.h>
#include <base/oam_vsmem.h>
#include <base/oam_types.h>

#include <base/macro.h>

#include <stdio.h>
#include <omp.h>

/* ======================================================================== *
 * Host Functions                                                           *
 * ======================================================================== */

HostMessage * oam_comm__host_signals_new(
  int poll_interval_ms,
  int timeout_after_ms)
{
  int t;
  HostMessage * host_mesg =
    (HostMessage *)(oam_vsmem__symmetric_malloc(sizeof(HostMessage)));
  host_mesg->cancel            = 0;
  host_mesg->time_start_ms     = 0;
  host_mesg->poll_interval_ms  = poll_interval_ms;
  host_mesg->timeout_after_ms  = timeout_after_ms;
  host_mesg->param             = 0;
  host_mesg->ret               = OMPACC_TASK__OK;

  for (t = 0; t < OAM_TASK__MAX_THREADS; t++) {
    host_mesg->last_poll_time_ms[t] = 0;
  }

  return host_mesg;
}

void oam_comm__host_signals_delete(
  HostMessage * host_signals)
{
  oam_vsmem__symmetric_free(host_signals);
}

void oam_comm__flush_signals(
  HostMessage * host_signals)
{
  #pragma omp target data map(to: host_signals[0:1])
  {
    #pragma omp target update to (host_signals[0:1])
    ;
  }

  printf("[ %.3fs ] >> oam_task__flush_signals\n", omp_get_wtime());
  wno_unused_var_(host_signals);
}

void oam_comm__update_target_data(
  char         * host_data,
  unsigned int   num_bytes)
{
  wno_unused_var_(host_data);
  #pragma omp target update to (host_data[0:num_bytes])
}

void oam_comm__update_host_data(
  char         * target_data,
  unsigned int   num_bytes)
{
  wno_unused_var_(target_data);
  #pragma omp target update from (target_data[0:num_bytes])
}

/* ======================================================================== *
 * Target Functions                                                         *
 * ======================================================================== */

int oam_comm__poll_interval(
  HostMessage * host_signals)
{
  int time_now_ms       = (int)(omp_get_wtime() * 1000);
  int tid               = omp_get_thread_num();
  int last_poll_elapsed = time_now_ms - host_signals->last_poll_time_ms[tid];
  if (host_signals->last_poll_time_ms[tid] == 0 ||
      last_poll_elapsed >= host_signals->poll_interval_ms) {
    host_signals->last_poll_time_ms[tid] = time_now_ms;
    return 1;
  }
  return 0;
}

int oam_comm__poll_message(
  double        time_start,
  HostMessage * host_signals)
{
  int do_abort = 0;

// __cache_l1d_flush();
// __cache_l2_flush();

  #pragma omp flush
  if (0 != host_signals->cancel) {
    do_abort = 1;
    host_signals->ret = OMPACC_TASK__CANCEL;
//  printf("[ %.3fs ] !! oam_task__poll_message: cancel\n", omp_get_wtime());
  } else if (host_signals->timeout_after_ms <=
              (omp_get_wtime() - time_start) * 1000) {
    do_abort = 1;
    host_signals->ret = OMPACC_TASK__TIMEOUT;
    printf("[ %.3fs ] !! oam_task__poll_message: timeout\n", omp_get_wtime());
  }
  return do_abort;
}

