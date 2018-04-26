#include "MainCtrl.h"

static uint8_t _init_flag = 0;
static uint16_t _ctrl_ticks = 0;

static DOOR_CTRL_CMD DoorCmd_A = Close;
static DOOR_CTRL_CMD DoorCmd_B = Close;
static DOOR_STATE_DEF CurDoorState_A = Closing;
static DOOR_STATE_DEF CurDoorState_B = Closing;

#define STEP_FREQ                      (20)
float run_step = (200.0f - 10.0f) / 4.0f / (float)STEP_FREQ; /* 20Hz */
uint8_t step_freq_div = 0;

float exp_freq = 10, UsrMaxFreq = 120;
float freq = 10, last_freq = 10;
MOTOR_DIR dir = STOP, last_dir = STOP;

float exp_freq_b = 10;//, UsrMaxFreq = 120;
float freq_b = 10, last_freq_b = 10;
MOTOR_DIR dir_b = STOP, last_dir_b = STOP;

static uint8_t LED_BarMode = 0;

static void MainCtrlLoopInit(void)
{
}

void SystemControlTask(void) /* MAIN_CONTROLLER_LOOP_RATE Hz */
{
	if(_init_flag == 0) {
		_init_flag = 1;

		MainCtrlLoopInit();
	}

	switch(LED_BarMode) {
		case 0:
			NormalColorLoop(5, LED_BAR_LEFT, 0x0);
			NormalColorLoop(5, LED_BAR_RIGHT, 0x0);
		break;
		case 1:
			FlowModeLoop(5, LED_BAR_LEFT, 0x80);
			FlowModeLoop(5, LED_BAR_RIGHT, 0x80);
		break;
		case 2:
			NormalColorLoop(5, LED_BAR_LEFT, 0x80);
			NormalColorLoop(5, LED_BAR_RIGHT, 0x80);
		break;
		case 3:
			ToggleModeLoop(5, LED_BAR_LEFT, 0x808000);
			NormalColorLoop(5, LED_BAR_RIGHT, 0x0);
		break;
		case 4:
			NormalColorLoop(5, LED_BAR_LEFT, 0x0);
			ToggleModeLoop(5, LED_BAR_RIGHT, 0x808000);
		break;
		case 5:
			NormalColorLoop(5, LED_BAR_LEFT, 0x808000);
			NormalColorLoop(5, LED_BAR_RIGHT, 0x808000);
		break;
		case 6:
			FlowModeLoop(5, LED_BAR_LEFT, 0x808000);
			FlowModeLoop(5, LED_BAR_RIGHT, 0x808000);
		break;
		case 7:
			NormalColorLoop(5, LED_BAR_LEFT, 0x800000);
			NormalColorLoop(5, LED_BAR_RIGHT, 0x800000);
		break;
		default:
			NormalColorLoop(5, LED_BAR_LEFT, 0x0);
			NormalColorLoop(5, LED_BAR_RIGHT, 0x0);
		break;
	}

	if(FWD_A_BTN_PRESSED()) { CurDoorState_A = Closed; }
	if(BWD_A_BTN_PRESSED()) { CurDoorState_A = Opened; }
	if(FWD_B_BTN_PRESSED()) { CurDoorState_B = Closed; }
	if(BWD_B_BTN_PRESSED()) { CurDoorState_B = Opened; }

	if(DoorCmd_A == Close && CurDoorState_A != Closed) {
		dir = R_FWD; exp_freq = UsrMaxFreq; CurDoorState_A = Closing;
	} else if(DoorCmd_A == Open && CurDoorState_A != Opened) {
		dir = R_REV; exp_freq = UsrMaxFreq; CurDoorState_A = Opening;
	} else {
		dir = STOP; exp_freq = 10;
	}

	if(DoorCmd_B == Close && CurDoorState_B != Closed) {
		dir_b = R_FWD; exp_freq_b = UsrMaxFreq; CurDoorState_B = Closing;
	} else if(DoorCmd_B == Open && CurDoorState_B != Opened) {
		dir_b = R_REV; exp_freq_b = UsrMaxFreq; CurDoorState_B = Opening;
	} else {
		dir_b = STOP; exp_freq_b = 10;
	}

	step_freq_div ++;
	if(step_freq_div >= MAIN_CONTROLLER_LOOP_RATE / STEP_FREQ) {
		step_freq_div = 0;
		
		if(dir == STOP) freq = 10;
		else {
			if(freq < exp_freq - run_step) freq += run_step;
			else freq = exp_freq;
		}
		if(last_dir != dir) freq = 10;

		if(dir_b == STOP) freq_b = 10;
		else {
			if(freq_b < exp_freq_b - run_step) freq_b += run_step;
			else freq_b = exp_freq_b;
		}
		if(last_dir_b != dir_b) freq_b = 10;
	}

	if(last_freq != freq || last_dir != dir) {
		SetMotorARunParam(0, dir, (uint16_t)freq);
		last_dir = dir;
		last_freq = freq;
	}

	if(last_freq_b != freq_b || last_dir_b != dir_b) {
		SetMotorBRunParam(0, dir_b, (uint16_t)freq_b);
		last_dir_b = dir_b;
		last_freq_b = freq_b;
	}

	_ctrl_ticks ++;
	if(_ctrl_ticks >= 60000)
		_ctrl_ticks = 0;
}

inline void SetLED_BarMode(uint8_t mode) { LED_BarMode = mode; }
inline uint8_t GetLED_BarMode(void) { return LED_BarMode; }

inline void SetDoorState_A(DOOR_CTRL_CMD cmd) { DoorCmd_A = cmd; }
inline void SetDoorState_B(DOOR_CTRL_CMD cmd) { DoorCmd_B = cmd; }
inline DOOR_STATE_DEF GetDoorState_A(void) { return CurDoorState_A; }
inline DOOR_STATE_DEF GetDoorState_B(void) { return CurDoorState_B; }
