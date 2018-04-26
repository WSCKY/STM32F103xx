#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"

#include "LED.h"
#include "Debug.h"
#include "DC_EN.h"
#include "Button.h"
#include "WS281x.h"
#include "Battery.h"
#include "PowerCtrl.h"
#include "MotorDriver.h"

#include "MainCtrl.h"
#if (DEBUG_MODE)
#include "DataMonitor.h"
#else
#include "AppsDataProc.h"
#include "CommProtocol.h"
#endif
#include "PowerManage.h"

#include "cmsis_os.h"

#endif /* __MAIN_H */
