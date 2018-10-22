#ifndef __SIGNALPROCESS_H
#define __SIGNALPROCESS_H

#include "FreeControl.h"

#define RF_LOST_CONFIRM_TIME           (1000)

void SignalProcessTask(uint8_t millis);
uint8_t GetRFLostFlag(void);

#endif /* __SIGNALPROCESS_H */
