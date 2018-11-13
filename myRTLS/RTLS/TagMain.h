#ifndef __TAGMAIN_H
#define __TAGMAIN_H

#include "FrameStruct.h"
#include "RTLS_Conf.h"

#if (INSTANCE_MODE_TAG)

#include "deca_device_api.h"
#include "deca_regs.h"
#include "port_platform.h"

#include "DataMonitor.h"

#include "FreeRTOS.h"
#include "task.h"

void tag_rtls_task_function(void * pvParameter);

#endif /* (INSTANCE_MODE_TAG) */

#endif /* __TAGMAIN_H */
