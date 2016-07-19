#define SYMMETRIC_MEMORY__USE_DDR

#include "dsp_img_filter_target.h"

/*
 * Type definitions and macros do not have to be included in a
 * `omp declare target` region.
 */
#include <dsp_img_filter/config.h>
#include <dsp_img_filter/types.h>
#include <dsp_img_filter/macro.h>
#include <dsp_img_filter/logging.h>


/* ======================================================================== *
 * Begin of Target Function Declarations and Includes                       *
 * ------------------------------------------------------------------------ *
 *                                                                          *
 * Definition of functions and variables used within the offloaded DSP      *
 * kernel routine.                                                          *
 *                                                                          *
 * See TI documentation:                                                    *
 * downloads.ti.com/mctools/esd/docs/openmpacc/supported_constructs.html    *
 *                                                                          *
 *      "The 'declare target' construct specifies that variables and        *
 *       functions are mapped to a device. Each function specified in a     *
 *       declare target region must have definitions for both the host      *
 *       and target device."                                                *
 *                                                                          *
 * ------------------------------------------------------------------------ */
#pragma omp declare target

  /*
   * Place includes of function definition headers in `omp declare target`
   * region to map included definitions to target devices.
   */
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include <omp.h>

# include <dsp_img_filter/symmetric_memory.h>
# include <dsp_img_filter/target_logging.h>

# include "ti_omp_device.h"

  uint32_t __core_num(void);

  /**
   * Invert value in stencil cell via (cell <- threshold - cell).
   */
  inline static void invert_stencil_op(
    const CPicturePixel * image_in_buf,
    CPicturePixel       * image_out_buf,
    CPicturePixel         threshold,
    int                   nrows,
    int                   ncols,
    int                   i,
    int                   j,
    int                   core_id);

  /**
   * Applies single iteration of 5-point gaussian stencil in output image
   * data at position (row_index, col_index).
   */
  inline static void gauss_stencil_op(
    const CPicturePixel * image_in_buf,
    CPicturePixel       * image_out_buf,
    CPicturePixel         threshold,
    int                   img_rows,
    int                   img_cols,
    int                   row_index,
    int                   col_index,
    int                   core_id);

  inline static void difference_reduce_op(
    const CPicturePixel * image_in_buf,
    const CPicturePixel * image_op_buf,
    CPicturePixel       * image_out_buf,
    CPicturePixel         threshold,
    int                   nrows,
    int                   ncols,
    int                   i,
    int                   j,
    int                   core_id);

  /**
   * Canonical offset of two-dimensional Cartesian point.
   */
  inline static int canonical_2d(
    int i,     int j,
    int rows,  int cols);

  /**
   * Current time.
   */
  static inline ts_t timestamp();

  /**
   * Time elapsed since specified time stamp, in seconds.
   */
  static inline double time_elapsed_since(
    ts_t ts_start);

  DSP_TARGET__RET UnbufferedStencilKernel(
    RunParameters    * run_params,
    int                iterations,
    StencilOperator    stencil_op);

  DSP_TARGET__RET DoubleBufferedStencilKernel(
    RunParameters    * run_params,
    int                iterations,
    StencilOperator    stencil_op);

  DSP_TARGET__RET ReduceOperationKernel(
    RunParameters    * run_params,
    int                iterations,
    ReduceOperator     reduce_op);

#pragma omp end declare target
/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */



/* ======================================================================== *
 * Internal Function Declarations                                           *
 * ======================================================================== */


/* ======================================================================== *
 * Public Function Definitions: Filters                                     *
 * ------------------------------------------------------------------------ *
 * To add a new filter kernel, just define the stencil function operating   *
 * on a single pixel and implement the kernel function as a wrapper of      *
 * `UnbufferedStencilKernel` or `DoubleBufferedStencilKernel` as required.  *
 * ======================================================================== */

DSP_TARGET__RET InvertFilter(
  RunParameters    * run_params,
  int                iterations,
  double           * elapsed_target_out,
  double           * elapsed_kernel_out)
{
  LOG_HOST_DEBUG("InvertFilter()");

  CPicturePixel * image_in_data   = run_params->image_in.data;
  CPicturePixel * image_out_data  = run_params->image_out.data;

  LOG_HOST_DEBUG("run_params.image_in.data:   %p", image_in_data);
  LOG_HOST_DEBUG("run_params.image_out.data:  %p", image_out_data);

  int    img_rows        = run_params->image_in.rows;
  int    img_cols        = run_params->image_in.cols;
  int    img_size        = img_rows * img_cols;

  DSP_TARGET__RET ret    = DSP_TARGET__OK;

  ts_t ts_kernel_start   = timestamp();
  double elapsed_target;

  LOG_HOST_DEBUG("ts_start:  %.6f", ts_kernel_start);

  RunParameters t_run_params = *run_params;

  /* ================================================================== *
   * Begin of offloaded section                                         *
   * ------------------------------------------------------------------ */
  #pragma omp target map(to:   image_in_data[0:img_size],     \
                               t_run_params, iterations)      \
                     map(from: image_out_data[0:img_size],    \
                               elapsed_target)
  {
    LOG_TARGET_DEBUG("target:      %d", omp_get_thread_num());
    LOG_TARGET_DEBUG("num targets: %d", omp_get_num_threads());
    /* Map image buffers in run params to target address space: */
    t_run_params.image_in.data  = image_in_data;
    t_run_params.image_out.data = image_out_data;

    elapsed_target        = 0;
    ts_t ts_target_start  = timestamp();

    LOG_TARGET_DEBUG("ts_start:  %.6f", ts_target_start);

    UnbufferedStencilKernel(
      &t_run_params,
      iterations,
      invert_stencil_op);

    #pragma omp single
    {
      elapsed_target = time_elapsed_since(ts_target_start);
    }
  }
  /* ------------------------------------------------------------------ *
   * End of offloaded section                                           *
   * ================================================================== */

  *elapsed_target_out = elapsed_target;
  *elapsed_kernel_out = time_elapsed_since(ts_kernel_start);

  LOG_HOST_DEBUG("InvertFilter > %d", ret);
  return ret;
}

DSP_TARGET__RET GaussianFilter(
  RunParameters    * run_params,
  int                iterations,
  double           * elapsed_target_out,
  double           * elapsed_kernel_out)
{
  LOG_HOST_DEBUG("GaussianFilter()");

  CPicturePixel * image_in_data   = run_params->image_in.data;
  CPicturePixel * image_out_data  = run_params->image_out.data;

  LOG_HOST_DEBUG("run_params.image_in.data:   %p", image_in_data);
  LOG_HOST_DEBUG("run_params.image_out.data:  %p", image_out_data);

  int    img_rows        = run_params->image_in.rows;
  int    img_cols        = run_params->image_in.cols;
  int    img_size        = img_rows * img_cols;

  DSP_TARGET__RET ret    = DSP_TARGET__OK;

  ts_t ts_kernel_start   = timestamp();
  double elapsed_target;

  LOG_HOST_DEBUG("ts_start:  %.6f", ts_kernel_start);

  RunParameters t_run_params = *run_params;

  /* ================================================================== *
   * Begin of offloaded section                                         *
   * ------------------------------------------------------------------ */
  #pragma omp target map(to:   image_in_data[0:img_size],     \
                               t_run_params, iterations)      \
                     map(from: image_out_data[0:img_size],    \
                               elapsed_target)
  {
    LOG_TARGET_DEBUG("target:      %d", omp_get_thread_num());
    LOG_TARGET_DEBUG("num targets: %d", omp_get_num_threads());
    /* Map image buffers in run params to target address space: */
    t_run_params.image_in.data  = image_in_data;
    t_run_params.image_out.data = image_out_data;

    elapsed_target        = 0;
    ts_t ts_target_start  = timestamp();

    LOG_TARGET_DEBUG("ts_start:  %.6f", ts_target_start);

    DoubleBufferedStencilKernel(
      &t_run_params,
      iterations,
      gauss_stencil_op);

    #pragma omp single
    {
      elapsed_target = time_elapsed_since(ts_target_start);
    }
  }
  /* ------------------------------------------------------------------ *
   * End of offloaded section                                           *
   * ================================================================== */

  *elapsed_target_out = elapsed_target;
  *elapsed_kernel_out = time_elapsed_since(ts_kernel_start);

  LOG_HOST_DEBUG("GaussianFilter > %d", ret);
  return ret;
}

/* ======================================================================== *
 * Public Function Definitions: Reduce Operations                           *
 * ------------------------------------------------------------------------ *
 * To add a new reduce operation kernel, just define the reduce function    *
 * operating on a single pixel and implement the kernel as a wrapper of     *
 * `ReduceOperationKernel`.                                                 *
 * ======================================================================== */

DSP_TARGET__RET DifferenceReduceOperation(
  RunParameters    * run_params,
  int                iterations,
  double           * elapsed_target_out,
  double           * elapsed_kernel_out)
{
  LOG_HOST_DEBUG("DifferenceReduceOperation()");

  CPicturePixel * image_in_data   = run_params->image_in.data;
  CPicturePixel * image_out_data  = run_params->image_out.data;

  LOG_HOST_DEBUG("run_params.image_in.data:   %p", image_in_data);
  LOG_HOST_DEBUG("run_params.image_out.data:  %p", image_out_data);

  int    img_rows        = run_params->image_in.rows;
  int    img_cols        = run_params->image_in.cols;
  int    img_size        = img_rows * img_cols;

  DSP_TARGET__RET ret    = DSP_TARGET__OK;

  ts_t ts_kernel_start   = timestamp();
  double elapsed_target;

  LOG_HOST_DEBUG("ts_start:  %.6f", ts_kernel_start);

  RunParameters t_run_params = *run_params;

  /* ================================================================== *
   * Begin of offloaded section                                         *
   * ------------------------------------------------------------------ */
  #pragma omp target map(to:   image_in_data[0:img_size],     \
                               t_run_params, iterations)      \
                     map(from: image_out_data[0:img_size],    \
                               elapsed_target)
  {
    LOG_TARGET_DEBUG("target:      %d", omp_get_thread_num());
    LOG_TARGET_DEBUG("num targets: %d", omp_get_num_threads());
    /* Map image buffers in run params to target address space: */
    t_run_params.image_in.data  = image_in_data;
    t_run_params.image_out.data = image_out_data;

    elapsed_target        = 0;
    ts_t ts_target_start  = timestamp();

    LOG_TARGET_DEBUG("ts_start:  %.6f", ts_target_start);

    ReduceOperationKernel(
      &t_run_params,
      iterations,
      difference_reduce_op);

    #pragma omp single
    {
      elapsed_target = time_elapsed_since(ts_target_start);
    }
  }
  /* ------------------------------------------------------------------ *
   * End of offloaded section                                           *
   * ================================================================== */

  *elapsed_target_out = elapsed_target;
  *elapsed_kernel_out = time_elapsed_since(ts_kernel_start);

  LOG_HOST_DEBUG("DifferenceReduceOperation > %d", ret);
  return ret;
}

/* ======================================================================== *
 * Internal Function Definitions                                            *
 * ======================================================================== */

DSP_TARGET__RET UnbufferedStencilKernel(
  RunParameters    * run_params,
  int                iterations,
  StencilOperator    stencil_op)
{
  LOG_TARGET_DEBUG("UnbufferedStencilKernel()");
  LOG_TARGET_DEBUG("targets:                    %d", omp_get_num_threads());

  CPicturePixel * image_in_data   = run_params->image_in.data;
  CPicturePixel * image_out_data  = run_params->image_out.data;
  CPicturePixel   image_threshold = run_params->image_in.thld;

  LOG_TARGET_DEBUG("run_params.image_in.data:   %p", image_in_data);
  LOG_TARGET_DEBUG("run_params.image_out.data:  %p", image_out_data);
  LOG_TARGET_DEBUG("iterations:                 %d", iterations);

  int img_rows = run_params->image_in.rows;
  int img_cols = run_params->image_in.cols;
  int img_size = img_rows * img_cols;

  int n, i, j;
  for (n = 0; n < iterations; n++) {
    /* Distribute work load as chunks of consecutive rows: */
    #pragma omp parallel for
    /* Canonical iteration order for row-major storage: */
    for (i = 1; i < img_rows - 1; i++) {
      for (j = 1; j < img_cols - 1; j++) {
        (*stencil_op)(image_in_data,
                      image_out_data,
                      image_threshold,
                      img_rows,
                      img_cols,
                      i, j,
                      omp_get_thread_num());
      }
    }
  }

  wno_unused_var_(img_size);
  wno_unused_var_(iterations);

  LOG_TARGET_DEBUG("UnbufferedStencilKernel >");

  return DSP_TARGET__OK;
}

DSP_TARGET__RET DoubleBufferedStencilKernel(
  RunParameters    * run_params,
  int                iterations,
  StencilOperator    stencil_op)
{
  LOG_TARGET_DEBUG("DoubleBufferedStencilKernel()");
  LOG_TARGET_DEBUG("targets:                    %d", omp_get_num_threads());

  CPicturePixel * image_in_data   = run_params->image_in.data;
  CPicturePixel * image_out_data  = run_params->image_out.data;
  CPicturePixel   image_threshold = run_params->image_in.thld;

  LOG_TARGET_DEBUG("run_params.image_in.data:   %p", image_in_data);
  LOG_TARGET_DEBUG("run_params.image_out.data:  %p", image_out_data);
  LOG_TARGET_DEBUG("iterations:                 %d", iterations);

  int img_rows  = run_params->image_in.rows;
  int img_cols  = run_params->image_in.cols;
  int img_size  = img_rows * img_cols;

  /* Allocate additional local back buffer: */
  CPicturePixel * image_buf_a = (char *) symmetric_malloc(
                                           img_rows * img_cols *
                                           sizeof(char));
  CPicturePixel * image_buf_b = image_out_data;

  memcpy(image_buf_a, image_in_data, img_rows * img_cols * sizeof(char));

  int n, i, j;
  for (n = 0; n < iterations; n++) {
    /* Distribute work load as chunks of consecutive rows: */
    #pragma omp parallel for
    /* Canonical iteration order for row-major storage: */
    for (i = 1; i < img_rows - 1; i++) {
      if (i == 1 && n == 0) {
        LOG_TARGET_DEBUG("omp for thread id: %d", omp_get_thread_num());
      }
      for (j = 1; j < img_cols - 1; j++) {
        (*stencil_op)(image_buf_a,
                      image_buf_b,
                      image_threshold,
                      img_rows,
                      img_cols,
                      i, j,
                      omp_get_thread_num());
      }
    }
    /* Swap back- and front buffer for next iteration: */
    swap(image_buf_a, image_buf_b);
  }

  /* Free additional local back buffer: */
  if (image_buf_a != image_out_data) {
    symmetric_free(image_buf_a);
  } else if(image_buf_b != image_out_data) {
    symmetric_free(image_buf_b);
  }

  wno_unused_var_(image_in_data);
  wno_unused_var_(image_out_data);
  wno_unused_var_(img_size);
  wno_unused_var_(iterations);

  LOG_TARGET_DEBUG("DoubleBufferedStencilKernel >");

  return DSP_TARGET__OK;
}

DSP_TARGET__RET ReduceOperationKernel(
  RunParameters    * run_params,
  int                iterations,
  ReduceOperator     reduce_op)
{
  CPicturePixel * image_in_data   = run_params->image_in.data;
  CPicturePixel * image_op_data   = run_params->image_op.data;
  CPicturePixel * image_out_data  = run_params->image_out.data;
  CPicturePixel   image_threshold = run_params->image_in.thld;

  LOG_TARGET_DEBUG("run_params.image_in.data:   %p", image_in_data);
  LOG_TARGET_DEBUG("run_params.image_out.data:  %p", image_out_data);
  LOG_TARGET_DEBUG("iterations:                 %d", iterations);

  int img_rows  = run_params->image_in.rows;
  int img_cols  = run_params->image_in.cols;
  int img_size  = img_rows * img_cols;

  int n, i, j;
  for (n = 0; n < iterations; n++) {
    /* Distribute work load as chunks of consecutive rows: */
    #pragma omp parallel for
    /* Canonical iteration order for row-major storage: */
    for (i = 1; i < img_rows - 1; i++) {
      for (j = 1; j < img_cols - 1; j++) {
        (*reduce_op)(image_in_data,
                     image_op_data,
                     image_out_data,
                     image_threshold,
                     img_rows,
                     img_cols,
                     i, j,
                     omp_get_thread_num());
      }
    }
  }

  wno_unused_var_(img_size);
  wno_unused_var_(iterations);

  return DSP_TARGET__OK;
}

inline static int canonical_2d(
  int i,     int j,
  int rows,  int cols)
{
  wno_unused_var_(rows);
  return (i * cols) + j;
}

inline static void invert_stencil_op(
  const CPicturePixel * image_in_buf,
  CPicturePixel       * image_out_buf,
  CPicturePixel         threshold,
  int                   nrows,
  int                   ncols,
  int                   i,
  int                   j,
  int                   core_id)
{
  wno_unused_var_(core_id);

  const CPicturePixel * pixel_in  = image_in_buf +
                                    canonical_2d(i, j, nrows, ncols);
  CPicturePixel       * pixel_out = image_out_buf +
                                    canonical_2d(i, j, nrows, ncols);
  *pixel_out = threshold - *pixel_in;
}

inline static void gauss_stencil_op(
  const CPicturePixel * image_in_buf,
  CPicturePixel       * image_out_buf,
  CPicturePixel         threshold,
  int                   nrows,
  int                   ncols,
  int                   i,
  int                   j,
  int                   core_id)
{
  wno_unused_var_(threshold);

  CPicturePixel * pixel_out = image_out_buf +
                              canonical_2d(i, j, nrows, ncols);

  static const float stencil_c = 0.28;
  static const float stencil_n = 0.18;
  static const float stencil_e = 0.18;
  static const float stencil_s = 0.18;
  static const float stencil_w = 0.18;

  CPicturePixel value_c = image_in_buf[canonical_2d(i,   j  , nrows, ncols)];
  CPicturePixel value_n = image_in_buf[canonical_2d(i,   j-1, nrows, ncols)];
  CPicturePixel value_w = image_in_buf[canonical_2d(i-1, j  , nrows, ncols)];
  CPicturePixel value_s = image_in_buf[canonical_2d(i,   j+1, nrows, ncols)];
  CPicturePixel value_e = image_in_buf[canonical_2d(i+1, j  , nrows, ncols)];

  *pixel_out = (int)(rint_(stencil_c * value_c +
                           stencil_n * value_n +
                           stencil_w * value_w +
                           stencil_s * value_s +
                           stencil_e * value_e));
  wno_unused_var_(core_id);
}

inline static void difference_reduce_op(
  const CPicturePixel * image_in_buf,
  const CPicturePixel * image_op_buf,
  CPicturePixel       * image_out_buf,
  CPicturePixel         threshold,
  int                   nrows,
  int                   ncols,
  int                   i,
  int                   j,
  int                   core_id)
{
  wno_unused_var_(core_id);
  wno_unused_var_(threshold);

  const CPicturePixel * pixel_in  = image_in_buf +
                                    canonical_2d(i, j, nrows, ncols);
  const CPicturePixel * pixel_op  = image_op_buf +
                                    canonical_2d(i, j, nrows, ncols);
  CPicturePixel       * pixel_out = image_out_buf +
                                    canonical_2d(i, j, nrows, ncols);
  *pixel_out = *pixel_in - *pixel_op;
}

static inline ts_t timestamp()
{
  /* Alternative methods to obtain timestamp: */

#if 0
  /* - Using clock_gettime
   *
   * ! NOTE that this requires linker flag -lrt which is not supported by
   * ! cl6x target compiler.
   */
  struct timespec ts;
  long long timestamp;
  clock_gettime(CLOCK_REALTIME, &ts); /* or: CLOCK_MONOTONIC */
  timestamp = ts.tv_sec * 1000000000LL + ts.tv_nsec;
  return timestamp;
#endif

#if 1
  /* - Using CPU cycle counter (TLSC on ARM, RDTSC on x64) wrapper provided
   *   by underlying OpenCL runtime.
   *   Divide elapsed ticks by bogomips for unit conversion to seconds.
   *
   * ! NOTE that this yields measurements suited for performance comparison
   * ! but not actual elapsed wall-clock time.
   */
  return __clock64();
#endif

#if 0
  /* - Using the wrapper provided by OpenMP interface.
   *
   * ! NOTE that clock resolutions (see omp_get_wtick) differ between
   * ! host and target. Comparison of elapsed time spans measured with
   * ! omp_get_wtime will not give meaningful results.
   */
  return omp_get_wtime();
#endif
}

static inline double time_elapsed_since(ts_t ts_start)
{
  /* Apply frequency scaling and/or unit conversion to seconds here if
   * necessary, depending on implementation of timestamp(). */

#if 0
  return (double)(timestamp() - ts_start);
#else
  return (double)((timestamp() - ts_start) / ARCH_BOGOMIPS);
#endif
}

