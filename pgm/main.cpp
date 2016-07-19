
#include <iostream>
#include <cstdlib>
#include <cstring>

#include "pgm.h"
#include "gaussian.h"

#define HI_BYTE(num) (((num) & 0x0000FF00) >> 8)
#define LO_BYTE(num)  ((num) & 0x000000FF)

static void gen_test_input(
  PGMData * pgm);

static void print_usage();

int main(int argc, char *argv[])
{
  int           num_iter        = 64;
  int           num_rows        = 512;
  int           num_cols        = 512;
  PGMData *     input_pgm       = NULL;
  PGMData *     output_pgm      = NULL;
  int           gen_input_file  = 1;

  if (argc < 2) {
    print_usage();
  }

  for (int argi = 1; argi < argc; argi += 2) {
    if (strcmp(argv[argi], "-n") == 0) {
      num_iter = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-s") == 0) {
      num_rows = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
      num_cols = std::max<int>(1, strtoll(argv[argi + 2], NULL, 10));
      argi++;
    }
    else if (strcmp(argv[argi], "-i") == 0) {
      const char * input_filename = argv[argi + 1];
      std::cout << "Reading input file " << input_filename << " ..."
                << std::endl;
      input_pgm      = pgm_read(input_filename);
      num_rows       = input_pgm->rows;
      num_cols       = input_pgm->cols;
      gen_input_file = 0;
    }
  }

  std::cout << "Input data "
            << "cols: "       << num_cols << " "
            << "rows: "       << num_rows << " "
            << "iterations: " << num_iter
            << std::endl;

  if (NULL == input_pgm) {
    input_pgm = pgm_new(num_rows, num_cols, 255);
    gen_test_input(input_pgm);
  }

  char * image_buf_in  = (char *) malloc(num_rows * num_cols * sizeof(char));
  char * image_buf_out = (char *) malloc(num_rows * num_cols * sizeof(char));

  /* ---------------------------------------------------------------------- *
   * Copy image input data to data heap:                                    *
   * ---------------------------------------------------------------------- */
  if (NULL != input_pgm) {
    int i, j, offset;
    for (i = 0; i < input_pgm->rows; i++) {
      for (j = 0; j < input_pgm->cols; j++) {
        offset               = (i * input_pgm->cols) + j;
        image_buf_in[offset] = LO_BYTE(input_pgm->data[i][j]);
      }
    }
  }

  /* ---------------------------------------------------------------------- *
   * Apply first kernel:                                                    *
   * ---------------------------------------------------------------------- */
  GaussianFilterKernel(
    image_buf_in,
    image_buf_out,
    num_rows,
    num_cols,
    num_iter);

  /* ---------------------------------------------------------------------- *
   * Invert output image:                                                   *
   * ---------------------------------------------------------------------- */
  for (int p = 0; p < input_pgm->rows * input_pgm->cols; p++) {
    image_buf_out[p] = input_pgm->thld - image_buf_out[p];
  }

  /* ---------------------------------------------------------------------- *
   * Write data in output heap to image file:                               *
   * ---------------------------------------------------------------------- */
  output_pgm = pgm_new(num_rows, num_cols, 255);
  int i, j, offset;
  for (i = 0; i < input_pgm->rows; i++) {
    for (j = 0; j < input_pgm->cols; j++) {
      offset                 = (i * input_pgm->cols) + j;
      output_pgm->data[i][j] = image_buf_out[offset];
    }
  }

  /* ---------------------------------------------------------------------- *
   * Free resources:                                                        *
   * ---------------------------------------------------------------------- */
  if (NULL != input_pgm) {
    if (gen_input_file) {
      pgm_write(input_pgm, "gen.in.pgm");
    }
    pgm_delete(input_pgm);
  }
  if (NULL != output_pgm) {
    pgm_write(output_pgm, "out.pgm");
    pgm_delete(output_pgm);
  }

  free(image_buf_in);
  free(image_buf_out);

  return EXIT_SUCCESS;
}

static void gen_test_input(
  PGMData * pgm)
{
  if (pgm->rows < 1 || pgm->cols < 1 || pgm->thld < 1) {
    std::cerr << "Generating input test data failed: "
              << "invalid PGM attributes: "
              << "rows = " << pgm->rows << " "
              << "cols = " << pgm->cols << " "
              << "thld = " << pgm->thld
              << std::endl;
    return;
  }

  static const int fpx = 16;
  /* Draw a checker board pattern with field size (fpx x fpx) :
   */
  int checker_rows = pgm->rows / fpx;
  int checker_cols = pgm->cols / fpx;
  int checker_idx, i, j;
  if (checker_rows < 1) { checker_rows = 1; }
  if (checker_cols < 1) { checker_cols = 1; }
  for (i = 0; i < pgm->rows; i++) {
    for (j = 0; j < pgm->cols; j++) {
      pgm->data[i][j] = 0;
      checker_idx     = pgm->thld * (
                          ((j / checker_cols) % 2) +
                          ((i / checker_rows) % 2));
      if (checker_idx % 2) {
        pgm->data[i][j] = (pgm->thld / 2);
      }
    }
  }
}

static void print_usage()
{
  std::cout << "Options:"
            << std::endl
            << "  -i <input filename>       image input data from PGM file"
            << std::endl
            << "  -s <rows> <cols>          image input test data size"
            << std::endl
            << std::endl;
}

