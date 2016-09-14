#ifndef OAM__OAM_TYPES_H__INCLUDED
#define OAM__OAM_TYPES_H__INCLUDED

#include <oam/oam_config.h>
#include <oam/macro.h>

typedef enum OMPAccTaskRet_t
{
  OMPACC_TASK__OK      = 0,
  OMPACC_TASK__CANCEL,
  OMPACC_TASK__TIMEOUT
} OMPAccTaskRet;

typedef struct HostMessage_t
{
  int           cancel;
  int           time_start_us;
  int           timeout_after_us;
  int           poll_interval_us;
  int           last_poll_time_us[OAM_TASK__MAX_THREADS];
  int           param;
  OMPAccTaskRet ret;
} HostMessage;

#endif // OAM__OAM_TYPES_H__INCLUDED
