#ifndef GAUSSIAN_H__INCLUDED
#define GAUSSIAN_H__INCLUDED

int GaussianFilterKernel(
  char * image_in_buf,
  char * image_out_buf,
  int    img_rows,
  int    img_cols,
  int    iterations);

#endif // GAUSSIAN_H__INCLUDED
