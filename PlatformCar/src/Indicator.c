#include "Indicator.h"

static uint32_t _task_cnt = 0;

void IndicatorTask(uint8_t millis)
{
	if(!IMU_GotOffset()) {
		if(((_task_cnt * millis) / 200) % 2 == 0) { LED2_TOG(); }
	} else {
		LED2_ON();
	}
	if(GetRFLostFlag()) {
		if(((_task_cnt * millis) / 200) % 2 == 0) { LED3_TOG(); }
	} else {
		LED3_ON();
	}

	_task_cnt ++;
}
