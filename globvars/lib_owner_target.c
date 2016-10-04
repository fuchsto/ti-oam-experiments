
#include "lib_owner_target.h"

#include <oam/oam_config.h>
#include <oam/oam_types.h>
#include <oam/oam_task.h>
#include <oam/oam_task_target.h>
#include <oam/logging.h>
#include <oam/macro.h>

/* ======================================================================== *
 * Begin of Target Function Declarations and Includes                       *
 * ------------------------------------------------------------------------ */
#pragma omp declare target
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#include "ti_omp_device.h"

#pragma omp end declare target

char global_target_string[128] = "global var from lib_owner_target.t.a";

/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */

void sentinel_target_region(int a)
{
  int ret = 0;
  #pragma omp target
  {
    ret += a;
  }
  return ret;
}
