#include "PowerManage.h"

static uint16_t _time_cnt = 0;
static uint8_t PWR_ReleasedFlag = 0;
static POWER_STATUS _power_status = POWER_ON;

void PowerControlLoop(uint8_t Millis)
{
	if(PWR_ReleasedFlag == 0) {
		if(!PWR_CHK_PRESSED()) PWR_ReleasedFlag = 1;
		return;
	}
	if(PWR_CHK_PRESSED()) {
		_time_cnt ++;
		if(_time_cnt * Millis >= 2000) {
			PWR_OFF();
			_power_status = POWER_OFF;
		}
	} else {
		_time_cnt = 0;
	}
}

POWER_STATUS GetPowerStatus(void) { return _power_status; }
