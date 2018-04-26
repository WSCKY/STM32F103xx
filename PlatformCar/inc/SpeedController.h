#ifndef __SPEEDCONTROLLER_H
#define __SPEEDCONTROLLER_H

#include "SysConfig.h"

#include "Encoder.h"
#include "pidController.h"

#define SPEED_READ_RATE_DIV            (2)

#define SPEED_CTRL_RATE_DIV            (2)

#if ((SPEED_READ_RATE_DIV == 0) || (SPEED_CTRL_RATE_DIV == 0))
 #error "ERROR CONFIG: SPEED RATE DIV"
#endif

void SpeedComputeTask(void);
void GetSpeedControllerOutput(float **pL, float **pR);
void SpeedControlLoop(float ExpVel_L, float ExpVel_R, uint8_t ControllerEnable);

#endif /* __SPEEDCONTROLLER_H */
