
#include <oam/oam_comm.h>
#include <oam/oam_vsmem.h>
#include <oam/oam_types.h>
#include <oam/oam_time.h>

#include <oam/logging.h>
#include <oam/macro.h>

#include <omp.h>

/* ======================================================================== *
 * Host Functions                                                           *
 * ======================================================================== */

HostMessage * oam_comm__host_signals_init(
  HostMessage * host_mesg,
  long poll_interval_us,
  long timeout_us)
{

  if (host_mesg != NULL) {
      int t;

      host_mesg->cancel            = 0;
      host_mesg->time_start_us     = 0;
      host_mesg->poll_interval_us  = poll_interval_us;
      host_mesg->timeout_after_us  = timeout_us;
      host_mesg->param             = 0;
      host_mesg->ret               = OMPACC_TASK__UNDEFINED;
      host_mesg->targets_entered   = 0;

      for (t = 0; t < OAM_TASK__MAX_THREADS; t++) {
        host_mesg->last_poll_time_us[t] = 0;
      }
  }
  return host_mesg;
}

HostMessage * oam_comm__host_signals_new(
  long poll_interval_us,
  long timeout_after_us)
{
  HostMessage * host_mesg =
    (HostMessage *)(oam_vsmem__symmetric_malloc(sizeof(HostMessage)));

  oam_comm__host_signals_init(
      host_mesg,
      poll_interval_us,
      timeout_after_us);

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
#if 0
  oam_comm__update_host_data(
      (char *)(host_signals),
      sizeof(HostMessage);
#endif

  if (host_signals == NULL) {
    return;
  }

#if defined(OMPACC_HOST)
  #pragma omp target update from (host_signals[0:1])
  ;
#elif defined(OMPACC_TARGET)
  #pragma omp flush(host_signals)
  ;
#endif
}

void oam_comm__update_target_data(
  char         * host_data,
  unsigned int   num_bytes)
{
  if (host_data == NULL) {
    return;
  }

  wno_unused_var_(host_data);
  #pragma omp target update to (host_data[0:num_bytes])
  ;
}

void oam_comm__update_host_data(
  char         * target_data,
  unsigned int   num_bytes)
{
  if (target_data == NULL) {
    return;
  }

  wno_unused_var_(target_data);
  #pragma omp target update from (target_data[0:num_bytes])
  ;
}

/* ======================================================================== *
 * Target Functions                                                         *
 * ======================================================================== */

int oam_comm__poll_interval(
  HostMessage * host_signals)
{
#if 0
  int  tid                = 0;
  long time_now_us        = oam_timestamp() * 1e6;
  long last_poll_elapsed;

  if (host_signals == NULL) {
    return 0;
  }

#if defined(_OPENMP)
  tid               = omp_get_thread_num();
#endif
  last_poll_elapsed = time_now_us - host_signals->last_poll_time_us[tid];
  if (host_signals->last_poll_time_us[tid] == 0 ||
      last_poll_elapsed >= host_signals->poll_interval_us) {
    host_signals->last_poll_time_us[tid] = time_now_us;
    return 1;
  }
  return 0;
#else
  return 1;
#endif
}

int oam_comm__poll_message(
  long          time_start_us,
  HostMessage * host_signals)
{
  int do_abort = 0;

  if (host_signals == NULL) {
    return -1;
  }

// __cache_l1d_flush();
// __cache_l2_flush();

// #pragma omp flush
  if (0 != host_signals->cancel) {
    do_abort = 1;
    host_signals->ret = OMPACC_TASK__CANCEL;
  } else if (host_signals->timeout_after_us > 0) {
    long time_now_us     = oam_timestamp() * 1e6;
    long task_elapsed_us = time_now_us - time_start_us;
    if (host_signals->timeout_after_us <= task_elapsed_us) {
      do_abort = 1;
      host_signals->ret = OMPACC_TASK__TIMEOUT;
    }
  }
  return do_abort;
}

