#ifndef __MAIN_H
#define __MAIN_H

#include "stm32f10x.h"

#include "LED.h"
#include "Debug.h"

#include "MainCtrl.h"
#if (DEBUG_MODE)
#include "DataMonitor.h"
#else

#endif

#include "cmsis_os.h"

#endif /* __MAIN_H */
