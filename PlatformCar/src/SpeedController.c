#include "SpeedController.h"

static uint8_t _init_flag = 0;
static uint16_t ControllerTicks = 0;

static uint8_t SpeedReadDelay = 0;
static int16_t MotorSpeed_L = 0, MotorSpeed_R = 0;
float SpeedFilted_L = 0.0f, SpeedFilted_R = 0.0f;//static

void SpeedComputeTask(void)
{
	/* Read Per (SPEED_READ_RATE_DIV * 5)ms */
	SpeedReadDelay ++;
	if(SpeedReadDelay >= SPEED_READ_RATE_DIV) {
		SpeedReadDelay = 0;
		MotorSpeed_L = ReadEncoderCounter(Encoder_L);
		MotorSpeed_R = ReadEncoderCounter(Encoder_R);
	}
	SpeedFilted_L = SpeedFilted_L * 0.7f + MotorSpeed_L * 0.3f;
	SpeedFilted_R = SpeedFilted_R * 0.7f + MotorSpeed_R * 0.3f;
}

static float CTRL_DT = 0.01f;
static float OUT_L = 0.0f, OUT_R = 0.0f;
static float PID_L_KP = 2.5f, PID_L_KI = 4.0f, Last_L_Bias = 0.0f;
static float PID_R_KP = 2.5f, PID_R_KI = 4.0f, Last_R_Bias = 0.0f;

static void ControllerInit(void)
{
	OUT_L = 0.0f, OUT_R = 0.0f;
	CTRL_DT = (float)SPEED_CTRL_RATE_DIV / (float)MAIN_CONTROLLER_LOOP_RATE;
}

void SpeedControlLoop(float ExpVel_L, float ExpVel_R, uint8_t ControllerEnable)
{
	float L_Bias = ExpVel_L - SpeedFilted_L;
	float R_Bias = ExpVel_R - SpeedFilted_R;

	if(_init_flag == 0) {
		_init_flag = 1;

		ControllerInit();
	}

	if(ControllerTicks % SPEED_CTRL_RATE_DIV == 0) {
		OUT_L += (PID_L_KP * (L_Bias - Last_L_Bias) + PID_L_KI * L_Bias) * CTRL_DT;
		OUT_R += (PID_R_KP * (R_Bias - Last_R_Bias) + PID_R_KI * R_Bias) * CTRL_DT;

		if(OUT_L > 800) OUT_L = 800; if(OUT_L < -800) OUT_L = -800;
		if(OUT_R > 800) OUT_R = 800; if(OUT_R < -800) OUT_R = -800;

		Last_L_Bias = L_Bias; Last_R_Bias = R_Bias;
	}

	if(ControllerEnable == 0) {
		OUT_L = 0.0f, OUT_R = 0.0f;
		Last_L_Bias = 0.0f; Last_R_Bias = 0.0f;
	}

	ControllerTicks ++;
	if(ControllerTicks >= 60000)
		ControllerTicks = 0;
}

void GetSpeedControllerOutput(float **pL, float **pR)
{
	*pL = &OUT_L;
	*pR = &OUT_R;
}
