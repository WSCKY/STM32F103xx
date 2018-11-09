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

/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.66 ms with above configuration. */
#define RESP_RX_TO_FINAL_TX_DLY_UUS 3100

/* Delay between frames, in UWB microseconds. See NOTE 1 below. */
#define POLL_TX_TO_RESP_RX_DLY_UUS 100

void tag_rtls_task_function(void * pvParameter);

#endif /* (INSTANCE_MODE_TAG) */

#endif /* __TAGMAIN_H */
