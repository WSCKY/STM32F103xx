#ifndef __SYSCONFIG_H
#define __SYSCONFIG_H

#include "stm32f10x.h"

/* main controller loop rate (in Hz) */
#define MAIN_CONTROLLER_LOOP_RATE      (200)

/* task timer rate */
#define TASK_TIMER_RATE                (100)

/* indicator timer rate */
#define STATE_TIMER_RATE               (20)

/* LED flash rate define */
#define LED_FLASH_RATE_WAIT_STABLE     (10)
#define LED_FLASH_RATE_SIGNAL_LOST     (2)

#define DEBUG_DATA_FRAME_RATE          (100)

/* Battery Low Power Limit */
#define BATTERY_NO_POWER_LIMIT         (11.0f)
#define BATTERY_LOW_POWER_LIMIT        (11.2f)

#define BATTERY_VOLT_AMPL_FACTOR       (21.0f / 1.0f)

#define DEBUG_MODE                     (0)

#define UNUSED_VARIABLE(X)  ((void)(X))
#define UNUSED_PARAMETER(X) UNUSED_VARIABLE(X)
#define UNUSED_RETURN_VALUE(X) UNUSED_VARIABLE(X)

#endif /* __SYSCONFIG_H */
