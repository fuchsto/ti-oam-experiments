
#include "decoder_host.h"
#include "decoder_target.h"

#include <base/oam_comm.h>

unsigned int decode_host(
  char        * in_buffer,
  char        * out_buffer,
  int           size,
  int           repeats,
  HostMessage * host_signals)
{
  unsigned int result = decode_target(in_buffer, out_buffer, size, repeats,
                                      host_signals);
  return result;
}

