#ifndef __MAINCTRL_H
#define __MAINCTRL_H

#include "SysConfig.h"

#include "Button.h"
#include "LightMode.h"
#include "MotorDriver.h"
#include "VoltageCheck.h"

#define ABS(x) ((x) >= 0 ? (x) : -(x))

//#define RUN_ENABLE_CONFIRM             200
//#define RUN_DISABLE_CONFIRM            20

typedef enum {
	Closed = 0,
	Opened = 1,
	Closing = 2,
	Opening = 3,
	Exception = 4
} DOOR_STATE_DEF;

typedef enum { Close = 0, Open = 1, Suspend = 5 } DOOR_CTRL_CMD;

void SystemControlTask(void);

void SetLED_BarMode(uint8_t mode);
uint8_t GetLED_BarMode(void);

void SetDoorState_A(DOOR_CTRL_CMD cmd);
void SetDoorState_B(DOOR_CTRL_CMD cmd);
DOOR_STATE_DEF GetDoorState_A(void);
DOOR_STATE_DEF GetDoorState_B(void);

#endif /* __MAINCTRL_H */
