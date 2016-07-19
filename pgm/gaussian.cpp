
#include "gaussian.h"

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#define wno_unused_var_(v) (void)(v)

#define swap(x,y) do \
  { unsigned char tmp[sizeof(x) == sizeof(y) ? (signed)(sizeof(x)) : -1]; \
    memcpy(tmp, &y,  sizeof(x)); \
    memcpy(&y,  &x,  sizeof(x)); \
    memcpy(&x,  tmp, sizeof(x)); \
  } while(0)


/**
 * Applies gaussian stencil with specified radius in output image data at
 * position (row_index, col_index).
 */
inline static void gauss_stencil(
  char * image_in_buf,
  char * image_out_buf,
  int    img_rows,
  int    img_cols,
  int    row_index,
  int    col_index);

inline static int coords_canonical_2d(
  int i,     int j,
  int rows,  int cols);


/* ======================================================================== *
 * DSP Kernel Implementation                                                *
 * ======================================================================== */

int GaussianFilterKernel(
  char * image_in_data,
  char * image_out_data,
  int    img_rows,
  int    img_cols,
  int    iterations)
{
  char * image_buf_a = (char *) malloc(img_rows * img_cols * sizeof(char));
  char * image_buf_b = image_out_data;

  memcpy(image_buf_a, image_in_data, img_rows * img_cols * sizeof(char));

  /* Loop nesting for row-major storage order. */
  int n;
  int i;
  int j;
  for (n = 0; n < iterations; n++) {
    for (i = 1; i < img_rows - 1; i++) {
      for (j = 1; j < img_cols - 1; j++) {
        gauss_stencil(image_buf_a,
                      image_buf_b,
                      img_rows,
                      img_cols,
                      i, j);
      }
    }
    swap(image_buf_a, image_buf_b);
  }
  if (image_buf_a != image_out_data) {
    free(image_buf_a);
  } else if(image_buf_b != image_out_data) {
    free(image_buf_b);
  }

  return 0;
}

inline static int coords_canonical_2d(
  int i,     int j,
  int rows,  int cols)
{
  wno_unused_var_(rows);

  return (i * cols) + j;
}

inline static void gauss_stencil(
  char * image_in_buf,
  char * image_out_buf,
  int    nrows,
  int    ncols,
  int    i,
  int    j)
{
  char * center = image_out_buf +
                  coords_canonical_2d(i, j, nrows, ncols);

  static const float stencil_c = 0.28;
  static const float stencil_n = 0.18;
  static const float stencil_e = 0.18;
  static const float stencil_s = 0.18;
  static const float stencil_w = 0.18;

  char value_c = image_in_buf[coords_canonical_2d(i,   j  , nrows, ncols)];
  char value_n = image_in_buf[coords_canonical_2d(i,   j-1, nrows, ncols)];
  char value_w = image_in_buf[coords_canonical_2d(i-1, j  , nrows, ncols)];
  char value_s = image_in_buf[coords_canonical_2d(i,   j+1, nrows, ncols)];
  char value_e = image_in_buf[coords_canonical_2d(i+1, j  , nrows, ncols)];

  *center = (char)(rint(stencil_c * value_c +
                        stencil_n * value_n +
                        stencil_w * value_w +
                        stencil_s * value_s +
                        stencil_e * value_e));
}

