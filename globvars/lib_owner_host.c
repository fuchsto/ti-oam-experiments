
#include "lib_owner_host.h"

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

char global_host_string[32] = "global host string";

#pragma omp end declare target

/* ------------------------------------------------------------------------ *
 * End of Target Function Declarations and Includes                         *
 * ======================================================================== */

