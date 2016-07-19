
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

#include <base/types.h>
#include <base/symmetric_memory.h>
#include <base/timestamp.h>

#include "offload_target.h"

#define HI_BYTE(num) (((num) & 0x0000FF00) >> 8)
#define LO_BYTE(num)  ((num) & 0x000000FF)

static void print_usage();

static void print_kernel_durations(
  const std::string   & name,
  std::vector<double> & durations);


int main(int argc, char *argv[])
{
  int  kb_alloc     = 1024 * 8;
  int  num_repeat   = 1;
  bool use_sym_heap = true;

  if (argc < 3) {
    print_usage();
    return EXIT_FAILURE;
  }

  for (int argi = 1; argi < argc; argi += 2) {
    if (strcmp(argv[argi], "-k") == 0) {
      kb_alloc = std::max<int>(1, strtoll(argv[argi + 1], NULL, 10));
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
  int    buffer_size    = 1024 * kb_alloc;
  int    sym_heap_size  = 2 * buffer_size; /* size of in + out buffers */
  char * symmetric_heap;

  if (use_sym_heap) {
    std::cout << "Using symmetric heap ";
    symmetric_heap = (char *)(symmetric_malloc(sym_heap_size));
  } else {
    std::cout << "Allocating buffer at host ";
    symmetric_heap = (char *)(malloc(sym_heap_size));
  }

  /* ---------------------------------------------------------------------- *
   * Initialize the pre-allocated buffers as new DDR and MSMC heaps         *
   * accesible to DSP cores.                                                *
   * ---------------------------------------------------------------------- */
  std::cout << "(" << sym_heap_size << " bytes = "
            << std::setprecision(2) << std::fixed
            << static_cast<double>(sym_heap_size) / 1024 / 1024 << " MB)"
            << std::endl;

  if (use_sym_heap) {
    symmetric_heap_init(symmetric_heap, sym_heap_size);
  }

  for (int i = 0; i < sym_heap_size; i++) {
    symmetric_heap[i] = (char)(i % 256);
  }

  /* ---------------------------------------------------------------------- *
   * Run kernels on DSPs and record time to completion at host:             *
   * ---------------------------------------------------------------------- */

  /* Kernel execution time in single repeats for median and stddev: */
  std::vector<double> target_durations;
  for (int i = 0; i < num_repeat; i++) {
    ts_t target_start = timestamp();

    offload_target(symmetric_heap, symmetric_heap + buffer_size, buffer_size);

    target_durations.push_back(time_elapsed_since(target_start));
  }

  /* ---------------------------------------------------------------------- *
   * Print time measurements summary:                                       *
   * ---------------------------------------------------------------------- */
  print_kernel_durations("target latency", target_durations);

  /* ---------------------------------------------------------------------- *
   * Finalize:                                                              *
   * ---------------------------------------------------------------------- */

  if (use_sym_heap) {
    symmetric_free(symmetric_heap);
  } else {
    free(symmetric_heap);
  }

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
  typedef const std::vector<double> vec_t;

  using std::cout;
  using std::endl;
  using std::setw;
  using std::fixed;
  using std::setprecision;

  std::sort(durations.begin(), durations.end(), std::less<double>());

  double median  = durations[durations.size() / 2];
  /* Equivalent to std::minmax available since C++11: */
  double min     = durations.front();
  double pre_max = durations[durations.size() - 2];
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

