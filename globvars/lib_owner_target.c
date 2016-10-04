
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

char global_string[32] = "global string";

/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */
