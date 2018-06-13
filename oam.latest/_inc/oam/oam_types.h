#ifndef OAM__OAM_TYPES_H__INCLUDED
#define OAM__OAM_TYPES_H__INCLUDED

#include <oam/oam_config.h>
#include <oam/macro.h>

typedef enum OMPAccTaskRet_t
{
  OMPACC_TASK__UNDEFINED = -1,
  OMPACC_TASK__OK        =  0,
  OMPACC_TASK__CANCEL,
  OMPACC_TASK__TIMEOUT
} OMPAccTaskRet;

typedef struct HostMessage_t
{
  int           param;
  int           cancel;
  int           targets_entered;
  long          timeout_after_us;
  long          time_start_us;
  long          poll_interval_us;
  long          last_poll_time_us[OAM_TASK__MAX_THREADS];
  OMPAccTaskRet ret;
} HostMessage;

#endif // OAM__OAM_TYPES_H__INCLUDED
