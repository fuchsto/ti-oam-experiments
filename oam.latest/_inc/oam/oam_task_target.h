#ifndef OAM__OAM_TASK_TARGET_H__INCLUDED
#define OAM__OAM_TASK_TARGET_H__INCLUDED

/**
 * \file oam_task_target.h
 *
 * OAM target-side task interface.
 */

#include <oam/oam_task.h>
#include <oam/oam_types.h>
#include <oam/macro.h>

#if DOXYGEN
/**
 * Cancellation checkpoint in decoder task section.
 *
 * \code
 *   int  aborted    = 0;
 *   ts_t time_start = host_signals->time_start_us / 1.0e6;
 *
 *   // ...
 *
 *   OAM_TASK__CANCELLATION_CHECK(
 *     "NameOfScope",
 *     time_start,
 *     host_signals,
 *     aborted_flag,
 *     GOTO_LABEL_IF_CANCELLED);
 * \endcode
 */
#  define OAM_TASK__CANCELLATION_CHECK(scope, ts,hs,a, goto_label)

#else // DOXYGEN

#if defined(OMPACC_TARGET)
#  define OAM_TASK__CANCELLATION_CHECK(scope, ts,hs,a, goto_label) \
    { \
      if (NULL != (hs) && \
          ( 0 != (a) || \
            0 != (hs)->cancel)) { \
        LOG_TARGET_INFO("=//= " scope ": (r:%d c:%d l:%d s:%ld t:%ld)", \
                        (hs)->ret, \
                        ((hs)->cancel), \
                        ((hs)->targets_entered), \
                        (long)((hs)->time_start_us), \
                        (long)((hs)->timeout_after_us)); \
        goto goto_label; \
      } \
    }
#else
#  define OAM_TASK__CANCELLATION_CHECK(scope, ts,hs,a, goto_label) do { } while(0)
#endif

#endif // DOXYGEN

/* ======================================================================== *
 * Target Functions                                                         *
 * ======================================================================== */
#pragma omp declare target

// See:
// - http://downloads.ti.com/mctools/esd/docs/opencl/memory/cache-operations.html
// - http://downloads.ti.com/mctools/esd/docs/opencl/extensions/bios-apis.html
void __cache_l2_flush(void);
void __cache_l1d_flush(void);


// See:
// - http://downloads.ti.com/mctools/esd/docs/opencl/multiprocess.html
#if 0
static inline unsigned oam_task__sleep(
    unsigned ms)
{

    unsigned t0      = __clock(); // value of DSP's TSCL register
    Task_sleep(ms);
    unsigned t1      = __clock(); // value of DSP's TSCL register
    unsigned elapsed = t1-t0;     // cycles elapsed between __clock() calls
    return elapsed;
}
#endif

/**
 * Register the start of a target task.
 * To be called at beginning of target offload region.
 */
void oam_task__enter(
  HostMessage * host_signals);

/**
 * Poll host signals handle to test if the current thread's task is to be
 * aborted.
 *
 * Parameter \c aborted must be a thread-local variable.
 */
static inline int oam_task__poll_cancel_request(
  /// The task's start timestamp in seconds, usually obtained from
  /// \c omp_get_wtime().
  long          time_start_us,
  /// The task's host signals handle.
  HostMessage * host_signals,
  /// Thread-local variable, set to \c 1 if the task has been aborted.
  int         * aborted);

#pragma omp end declare target


/**
 * Host-side variant of `oam_task__poll_cancel_request`.
 */
static inline int oam_task__poll_cancel_request(
  long          time_start_us,
  HostMessage * host_signals,
  int         * aborted)
{
  if (0 != *aborted) {
    return 1;
  }
  else if (host_signals->ret == OMPACC_TASK__TIMEOUT ||
           host_signals->cancel != 0) {
    *aborted = 1;
  }
#if 1
  else { // if (oam_comm__poll_interval(host_signals)) {
    int timeout = oam_comm__poll_message(time_start_us, host_signals);
    *aborted = (timeout > 0) || (0 != host_signals->cancel);
  }
#endif
  return *aborted;
}


#endif // OAM__OAM_TASK_TARGET_H__INCLUDED
