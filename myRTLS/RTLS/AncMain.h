#ifndef __ANCMAIN_H
#define __ANCMAIN_H

#include "FrameStruct.h"
#include "RTLS_Conf.h"

#if !(INSTANCE_MODE_TAG)

#include "deca_device_api.h"
#include "deca_regs.h"
#include "port_platform.h"

#include "FreeRTOS.h"
#include "task.h"

/* Delay between frames, in UWB microseconds. See NOTE 4 below. */
/* This is the delay from Frame RX timestamp to TX reply timestamp used for calculating/setting the DW1000's delayed TX function. This includes the
 * frame length of approximately 2.46 ms with above configuration. */
#define POLL_RX_TO_RESP_TX_DLY_UUS 2600
/* This is the delay from the end of the frame transmission to the enable of the receiver, as programmed for the DW1000's wait for response feature. */
#define RESP_TX_TO_FINAL_RX_DLY_UUS 500
/* Receive final timeout. See NOTE 5 below. */
#define FINAL_RX_TIMEOUT_UUS 3300

void anc_rtls_task_function(void * pvParameter);

#endif /* !(INSTANCE_MODE_TAG) */

#endif /* __ANCMAIN_H */
