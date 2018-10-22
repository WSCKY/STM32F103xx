#include "CtrlArbitration.h"

static uint8_t _init_flag = 0;

static RF_CHANNLE_t* pRFData;

static float tof_dist = 0.0f, avoid_gain = 1.0f;
static float exp_vel = 0, exp_yaw = 0;

static void avoid_check(uint8_t millis);

void CtrlCmdArbitLoop(uint8_t millis)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		pRFData = GetRF_ChannelData();
	}

	avoid_check(millis);

	if(!GetRFLostFlag()) {
		if(pRFData->Channel[5] > RF_MID_VALUE)
			VehicleRunEnable();
		else
			VehicleRunDisable();
		exp_vel = -(pRFData->Channel[1] - RF_MID_VALUE) * 2;
		exp_yaw = -(pRFData->Channel[0] - RF_MID_VALUE) * 0.13f;
		if(exp_vel > 0) exp_vel *= avoid_gain;
		SetUsrCtrlVal(apply_deadband(exp_vel, 130), apply_deadband(exp_yaw, 9));
	} else {
		VehicleRunDisable();
		SetUsrCtrlVal(0, 0);
	}
}

static void avoid_check(uint8_t millis)
{
	static uint32_t _cnt = 0;
	if(GetNewTOFData(&tof_dist)) {
		_cnt = 0;
		avoid_gain = (tof_dist > 100) ? 1.0f : ((tof_dist > 20) ? (tof_dist * 0.0125f - 0.25f) : 0.0f);
	} else {
		if(_cnt * millis < 500)
			_cnt ++;
		else {
			if(avoid_gain < 1.0f)
				avoid_gain += 0.005f;
			else
				avoid_gain = 1.0f;
		}
	}
}
