#ifndef __POWERMANAGE_H
#define __POWERMANAGE_H

#include "Button.h"
#include "PowerCtrl.h"

typedef enum {
	POWER_ON =0,
	POWER_OFF = 1
} POWER_STATUS;

void PowerControlLoop(uint8_t Millis);
POWER_STATUS GetPowerStatus(void);

#endif /* __POWERMANAGE_H */
