#ifndef DSP_IMG_FILTER_TARGET_H__INCLUDED
#define DSP_IMG_FILTER_TARGET_H__INCLUDED

#include <dsp_img_filter/types.h>

#ifdef __cplusplus
extern "C" {
#endif

DSP_TARGET__RET GaussianFilter(
  RunParameters * run_params,
  int             iterations,
  double        * elapsed_target_out,
  double        * elapsed_kernel_out);

DSP_TARGET__RET InvertFilter(
  RunParameters * run_params,
  int             iterations,
  double        * elapsed_target_out,
  double        * elapsed_kernel_out);

DSP_TARGET__RET DifferenceReduceOperation(
  RunParameters * run_params,
  int             iterations,
  double        * elapsed_target_out,
  double        * elapsed_kernel_out);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // DSP_IMG_FILTER_TARGET_H__INCLUDED
