
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>

#include <omp.h>

/* Note:
 *
 * MSMC for host/target shared memory is supported for K2x devices only.
 * See:
 * http://downloads.ti.com/mctools/esd/docs/opencl/memory/memory-model.html
 */
#define SYMMETRIC_MEMORY__USE_DDR

#include <dsp_img_filter/types.h>
#include <dsp_img_filter/pgm.h>
#include <dsp_img_filter/symmetric_memory.h>

#define HI_BYTE(num) (((num) & 0x0000FF00) >> 8)
#define LO_BYTE(num)  ((num) & 0x000000FF)

#include "dsp_img_filter_target.h"


static void gen_test_input(
  PGMData * pgm);

static void print_usage();

static void print_kernel_durations(
  const std::string &         name,
  const std::vector<double> & durations);


int main(int argc, char *argv[])
{
  RunParameters run_params;
  /* Number of iterations within a DSP kernel: */
  int           num_kernel_iter = 1;
  /* Number of repeats of DSP kernel at host:  */
  int           num_kernel_reps = 1;
  /* Total number of DSP kernel iterations:    */
  int           total_iter      = 1;
  int           num_rows        = 512;
  int           num_cols        = 512;
  PGMData *     input_pgm       = NULL;
  PGMData *     output_pgm      = NULL;
  int           gen_input_file  = 1;

  if (argc < 3) {
    print_usage();
    return EXIT_FAILURE;
  }

  for (int argi = 1; argi < argc; argi += 2) {
    if (strcmp(argv[argi], "-ki") == 0) {
      num_kernel_iter = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-kr") == 0) {
      num_kernel_reps = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
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
  total_iter = num_kernel_iter * num_kernel_reps;

  std::cout << "Input data:"
            << std::endl
            << "  cols: " << num_cols << " rows: " << num_rows
            << std::endl
            << "Iterations:"
            << std::endl
            << "  in single kernel:   " << num_kernel_iter
            << std::endl
            << "  * kernel repeats:   " << num_kernel_reps
            << std::endl
            << "  = total iterations: " << total_iter
            << std::endl;

  if (NULL == input_pgm) {
    input_pgm = pgm_new(num_rows, num_cols, 255);
    gen_test_input(input_pgm);
  }

  run_params.image_in.rows  = num_rows;
  run_params.image_in.cols  = num_cols;
  run_params.image_in.thld  = 255;
  run_params.image_in.data  = NULL;

  run_params.image_out.rows = num_rows;
  run_params.image_out.cols = num_cols;
  run_params.image_out.thld = 255;
  run_params.image_out.data = NULL;

  /* ---------------------------------------------------------------------- *
   * Symmetric allocation of heaps:                                         *
   * ---------------------------------------------------------------------- */
  int    image_num_px       = num_rows * num_cols;
  int    image_size         = image_num_px * sizeof(CPicturePixel);
  /* Heap capacity for input and output image with front- and back buffers
   * for both: */
  int    sym_heap_size      = 2 * 2 * image_size;

  /* Data of input and output image are contiguous in heap, starting with
   * input data:
   */
  char * symmetric_heap     = (char *)(symmetric_malloc(sym_heap_size));
  char * symmetric_heap_in  = symmetric_heap;
  char * symmetric_heap_out = symmetric_heap_in + image_size;

  /* ---------------------------------------------------------------------- *
   * Copy image input data to data heap:                                    *
   * ---------------------------------------------------------------------- */
  if (NULL != input_pgm) {
    int i, j, offset;
    for (i = 0; i < input_pgm->rows; i++) {
      for (j = 0; j < input_pgm->cols; j++) {
        offset                    = (i * input_pgm->cols) + j;
        symmetric_heap_in[offset] = LO_BYTE(input_pgm->data[i][j]);
      }
    }
  }

  /* ---------------------------------------------------------------------- *
   * Initialize the pre-allocated buffers as new DDR and MSMC heaps         *
   * accesible to DSP cores.                                                *
   * ---------------------------------------------------------------------- */
  std::cout << "Initialize symmetric heap (" << sym_heap_size << " bytes)"
            << std::endl;

  symmetric_heap_init(symmetric_heap, sym_heap_size);

  run_params.image_in.data  = symmetric_heap_in;
  run_params.image_out.data = symmetric_heap_out;

  /* ---------------------------------------------------------------------- *
   * Run kernels on DSPs and record time to completion at host:             *
   * ---------------------------------------------------------------------- */

  /* Kernel execution time in single repeats for median and stddev: */
  std::vector<double> target_durations;
  std::vector<double> kernel_durations;

  for (int i = 0; i < num_kernel_reps; i++) {
    double target_elapsed = 0;
    double kernel_elapsed = 0;
#if 0
    InvertFilter(
      &run_params,
      num_kernel_iter,
      &target_elapsed,
      &kernel_elapsed);
    std::swap(run_params.image_in.data, run_params.image_out.data);
#endif

#if 1
    GaussianFilter(
      &run_params,
      num_kernel_iter,
      &target_elapsed,
      &kernel_elapsed);
    std::swap(run_params.image_in.data, run_params.image_out.data);
#endif

#if 0
    InvertFilter(
      &run_params,
      num_kernel_iter,
      &target_elapsed,
      &kernel_elapsed);
    std::swap(run_params.image_in.data, run_params.image_out.data);
#endif

    target_durations.push_back(target_elapsed);
    kernel_durations.push_back(kernel_elapsed);
  }

  /* ---------------------------------------------------------------------- *
   * Print time measurements summary:                                       *
   * ---------------------------------------------------------------------- */
  print_kernel_durations("target (DSP)",  target_durations);
  print_kernel_durations("host (ARM)", kernel_durations);

  /* ---------------------------------------------------------------------- *
   * Write data in output heap to image file:                               *
   * ---------------------------------------------------------------------- */
  output_pgm = pgm_new(num_rows, num_cols, 255);
  int i, j, offset;
  for (i = 0; i < input_pgm->rows; i++) {
    for (j = 0; j < input_pgm->cols; j++) {
      offset                 = (i * input_pgm->cols) + j;
      output_pgm->data[i][j] = symmetric_heap_out[offset];
    }
  }

  /* ---------------------------------------------------------------------- *
   * Finalize:                                                              *
   * ---------------------------------------------------------------------- */

  symmetric_free(symmetric_heap);

  if (NULL != input_pgm) {
    if (gen_input_file) {
      const char filename[] = "in.gen.pgm";
      std::cout << "Writing generated input image to " << filename
                << std::endl;
      pgm_write(input_pgm, filename);
    }
    pgm_delete(input_pgm);
  }
  if (NULL != output_pgm) {
    const char filename[] = "out.gen.pgm";
    std::cout << "Writing generated output image to " << filename
              << std::endl;
    pgm_write(output_pgm, filename);
    pgm_delete(output_pgm);
  }

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
            << "  -i  <input filename>   load input image data from PGM file"
            << std::endl
            << "  -s  <rows> <cols>      size of test data to generate as"
            << std::endl
            << "                         input if no input file (-i) is"
            << std::endl
            << "                         specified"
            << std::endl
            << "  -ki <num iterations>   number of iterations in a single"
            << std::endl
            << "                         execution of the target kernel"
            << std::endl
            << "  -kr <num repeats>      number of repeats of the target"
            << std::endl
            << "                         kernel at the host"
            << std::endl
            << std::endl;
}

static void print_kernel_durations(
  const std::string       & name,
  const std::vector<double> & durations)
{
  typedef const std::vector<double> vec_t;

  using std::cout;
  using std::endl;
  using std::setw;
  using std::setprecision;

  vec_t::const_iterator it  = durations.begin();
  vec_t::const_iterator end = durations.end();

  double median = durations[durations.size() / 2];
  /* Equivalent to std::minmax available since C++11: */
  double min    = std::numeric_limits<double>::max();
  double max    = std::numeric_limits<double>::min();
  for (; it != end; ++it) {
    double dur = *it;
    if (dur < min) { min = dur; }
    if (dur > max) { max = dur; }
  }

  double min_ms    = min    * 1000;
  double median_ms = median * 1000;
  double max_ms    = max    * 1000;

  cout << "kernel execution times at " << name << ":"
       << endl
       << "  min:    " << setw(12) << setprecision(6) << min_ms    << " ms"
       << endl
       << "  median: " << setw(12) << setprecision(6) << median_ms << " ms"
       << endl
       << "  max:    " << setw(12) << setprecision(6) << max_ms    << " ms"
       << endl;
}

