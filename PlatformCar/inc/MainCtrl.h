#ifndef __MAINCTRL_H
#define __MAINCTRL_H

#include "SysConfig.h"

#include "TOF.h"
#include "CAN.h"
#include "Button.h"
#include "Encoder.h"
#include "MotorDriver.h"
#include "SensorGroup.h"
#include "VoltageCheck.h"
#include "pidController.h"
#include "YawController.h"
#include "SpeedController.h"
#include "AttitudeEstimator.h"
#include "AttitudeController.h"

#define ABS(x) ((x) >= 0 ? (x) : -(x))

#define RUN_ENABLE_CONFIRM             200
#define RUN_DISABLE_CONFIRM            20

void SystemControlTask(void);

uint8_t IMU_GotOffset(void);
uint8_t GetVehicleRunState(void);
void SetUsrCtrlVal(float ExpVel, float ExpYaw);

#endif /* __MAINCTRL_H */
