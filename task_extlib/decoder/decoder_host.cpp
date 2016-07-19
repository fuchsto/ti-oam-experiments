
#include "decoder.h"

unsigned int decode(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_signals)
{
  unsigned int acc = 0;
  int          i;
  int          r;
  for (i = 0; i < size; i++) {
    for (r = 0; r < repeats; r++) {
      acc           += in_buffer[i];
      out_buffer[i] += in_buffer[i];
    }
  }
  return acc;
}

