
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <vector>
#include <algorithm>

#include <omp.h>

/* Note:
 *
 * MSMC for host/target shared memory is supported for K2x devices only.
 * See:
 * http://downloads.ti.com/mctools/esd/docs/opencl/memory/memory-model.html
 */
#define SYMMETRIC_MEMORY__USE_DDR

#include <oam/oam_vsmem.h>
#include <oam/oam_time.h>

#include "offload_target.h"


static void print_usage();

static void print_kernel_durations(
  const std::string   & name,
  std::vector<double> & durations);


int main(int argc, char *argv[])
{
  int  kb_alloc_a   = 1024 * 8;
  int  kb_alloc_b   = 1024 * 1;
  int  num_repeat   = 1;
  bool use_sym_heap = true;

  if (argc < 3) {
    print_usage();
    return EXIT_FAILURE;
  }

  for (int argi = 1; argi < argc; argi += 2) {
    if (strcmp(argv[argi], "-na") == 0) {
      kb_alloc_a = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-nb") == 0) {
      kb_alloc_b = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-r") == 0) {
      num_repeat = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
    }
    else if (strcmp(argv[argi], "-cp") == 0) {
      use_sym_heap = false;
      argi++;
    }
  }

  /* ---------------------------------------------------------------------- *
   * Symmetric allocation of heap:                                          *
   * ---------------------------------------------------------------------- */
  int    buffer_size_a    = 1024 * kb_alloc_a;
  int    buffer_size_b    = 1024 * kb_alloc_b;
  int    sym_heap_a_size  = 4 * buffer_size_a; /* size of in + out buffers */
  char * symmetric_heap_a;

  /* ---------------------------------------------------------------------- *
   * Initialize the pre-allocated buffers as new DDR and MSMC heaps         *
   * accesible to DSP cores.                                                *
   * ---------------------------------------------------------------------- */
  std::cout << "sym. heap size: " << sym_heap_a_size << " bytes = "
            << std::setprecision(2) << std::fixed
            << static_cast<double>(sym_heap_a_size) / 1024 / 1024 << " MB"
            << std::endl;
  std::cout << "buffer size:    " << buffer_size_a << " bytes = "
            << std::setprecision(2) << std::fixed
            << static_cast<double>(buffer_size_a) / 1024 / 1024 << " MB"
            << std::endl;

  char * in_buffer  = (char *)oam_vsmem__symmetric_malloc(buffer_size_a);
  char * out_buffer = (char *)oam_vsmem__symmetric_malloc(buffer_size_a);

  /* ---------------------------------------------------------------------- *
   * Run kernels on DSPs and record time to completion at host:             *
   * ---------------------------------------------------------------------- */

  std::cout << "Initializing symmetric heap" << std::endl;
  symmetric_heap_a = (char *)(oam_vsmem__symmetric_malloc(sym_heap_a_size));
  oam_vsmem__symmetric_heap_init(symmetric_heap_a, sym_heap_a_size);

  /* Kernel execution time in single repeats for median and stddev: */
  std::vector<double> oam_target_map_durations;
  std::vector<double> oam_sym_alloc_durations;
  for (int i = 0; i < num_repeat; i++) {
    ts_t target_start;

    for (int i = 0; i < buffer_size_a; i++) {
      in_buffer[i]  = i % buffer_size_a;
      out_buffer[i] = i % 3;
    }

    std::cout << ">>> data_map_target() ..." << std::endl;
    target_start = oam_timestamp();
    {
      data_map_target(symmetric_heap_a, symmetric_heap_a + buffer_size_a,
                      buffer_size_a);
    }
    oam_target_map_durations.push_back(oam_elapsed_since(target_start));
    std::cout << "<<< data_map_target()" << std::endl;

#if 0
    target_start = oam_timestamp();
    if (use_sym_heap) {
      sym_alloc_target(sym_heap_a_size, sym_heap_b_size);
    }
    oam_sym_alloc_durations.push_back(oam_elapsed_since(target_start));

    if (i > 0) continue;

    std::cout << "First 10 values in output:\n";
    for (int v = 0; v < 10; v++) {
      std::cout << static_cast<int>(
                     *(symmetric_heap_a + buffer_size_a + v) )
                << " ";
    }
#endif

  }

  std::cout << "Finalizing symmetric heap" << std::endl;
  oam_vsmem__symmetric_free(symmetric_heap_a);

  /* ---------------------------------------------------------------------- *
   * Finalize:                                                              *
   * ---------------------------------------------------------------------- */

  oam_vsmem__symmetric_free(in_buffer);
  oam_vsmem__symmetric_free(out_buffer);

  /* ---------------------------------------------------------------------- *
   * Print time measurements summary:                                       *
   * ---------------------------------------------------------------------- */
  print_kernel_durations("target map latency",      oam_target_map_durations);
  print_kernel_durations("symmetric alloc latency", oam_sym_alloc_durations);

  return EXIT_SUCCESS;
}

static void print_usage()
{
  std::cout << "Options:"
            << std::endl
            << "  -k <num KB>   KB to allocate per repetition"
            << std::endl
            << "  -r <num rep>  Number of repetitions"
            << std::endl
            << "  -cp           Copy offload data (disable sym. heap)"
            << std::endl
            << std::endl;
}

static void print_kernel_durations(
  const std::string   & name,
  std::vector<double> & durations)
{
  using std::cout;
  using std::endl;
  using std::setw;
  using std::fixed;
  using std::setprecision;

  std::sort(durations.begin(), durations.end(), std::less<double>());

  double median  = durations[durations.size() / 2];
  /* Equivalent to std::minmax available since C++11: */
  double min     = durations.front();
  double pre_max = (durations.size() >= 2)
                      ? durations[durations.size() - 2]
                      : durations.back();
  double max     = durations.back();

  double min_us     = min     * 1.0e6;
  double median_us  = median  * 1.0e6;
  double pre_max_us = pre_max * 1.0e6;
  double max_us     = max     * 1.0e6;

  cout << name << ":"
       << endl
       << "  min:    "
       << fixed << setw(12) << setprecision(2) << min_us     << " us"
       << endl
       << "  median: "
       << fixed << setw(12) << setprecision(2) << median_us  << " us"
       << endl
       << "  < max:  "
       << fixed << setw(12) << setprecision(2) << pre_max_us << " us"
       << endl
       << "  max:    "
       << fixed << setw(12) << setprecision(2) << max_us     << " us"
       << endl;
}

