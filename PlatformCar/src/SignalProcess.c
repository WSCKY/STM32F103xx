#include "SignalProcess.h"

static uint32_t _task_cnt = 0;

uint8_t _rf_lost_flag = 1;
uint32_t _rf_lost_cnt = 0;

void SignalProcessTask(uint8_t millis)
{
	if(GetRFUpdateFlag()) {
		_rf_lost_cnt = 0;
		_rf_lost_flag = 0;
		RF_ParseData();
	} else {
		if(_rf_lost_cnt * millis < RF_LOST_CONFIRM_TIME)
			_rf_lost_cnt ++;
		else {
			_rf_lost_flag = 1;
		}
	}
	_task_cnt ++;
}

inline uint8_t GetRFLostFlag(void) { return _rf_lost_flag; }
