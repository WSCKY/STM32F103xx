#include "MotorDriver.h"

static void timer_config(void);

void MotorDriverInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(DRV_AIN_GPIO_PORT_CLK | DRV_BIN_GPIO_PORT_CLK | \
	                       DRV_PWMA_GPIO_PORT_CLK | DRV_PWMB_GPIO_PORT_CLK | \
												 DRV_EN_GPIO_PORT_CLK, ENABLE);

	/* Configure CTRL_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = DRV_AIN_GPIO_PIN;
	GPIO_Init(DRV_AIN_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DRV_BIN_GPIO_PIN;
	GPIO_Init(DRV_BIN_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DRV_EN_GPIO_PIN;
	GPIO_Init(DRV_EN_GPIO_PORT, &GPIO_InitStructure);
	/* Configure PWM_PIN as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = DRV_PWM_A_GPIO_PIN;
	GPIO_Init(DRV_PWMA_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DRV_PWM_B_GPIO_PIN;
	GPIO_Init(DRV_PWMB_GPIO_PORT, &GPIO_InitStructure);

	GPIO_SetBits(DRV_EN_GPIO_PORT, DRV_EN_GPIO_PIN);

	timer_config();
}

static void timer_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;

	/* DRVx_TIMER clock enable */
  RCC_APB1PeriphClockCmd(DRV_A_TIMER_CLK, ENABLE);
	RCC_APB1PeriphClockCmd(DRV_B_TIMER_CLK, ENABLE);

	/* -----------------------------------------------------------------------
    DRVx_TIMER Configuration: generate 1 PWM signals with 0% duty cycles:
    The DRVx_TIMER_CLK frequency is set to SystemCoreClock (Hz), to get DRVx_TIMER counter
    clock at 8 MHz the Prescaler is computed as following:
     - Prescaler = (DRVx_TIMER_CLK / DRV_TIMER counter clock) - 1
    SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
    and Connectivity line devices and to 8 MHz for Low-Density Value line and
    Medium-Density Value line devices

    The DRVx_TIMER is running at 10 KHz: DRVx_TIMER Frequency = DRVx_TIMER counter clock/(ARR + 1)
                                                  = 8 MHz / 800 = 10 KHz
    DRV_TIMER PWM duty cycle = (DRVx_TIMER_CCRy/ DRVx_TIMER_ARR) * 100
  ----------------------------------------------------------------------- */
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 8000000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 799;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(DRV_A_TIMER, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(DRV_B_TIMER, &TIM_TimeBaseStructure);

  /* DRV_A_TIMER PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC1Init(DRV_A_TIMER, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(DRV_A_TIMER, TIM_OCPreload_Enable);

  /* DRV_B_TIMER PWM1 Mode configuration: Channel2 */
  TIM_OC2Init(DRV_B_TIMER, &TIM_OCInitStructure);
  TIM_OC2PreloadConfig(DRV_B_TIMER, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(DRV_A_TIMER, ENABLE);
	TIM_ARRPreloadConfig(DRV_B_TIMER, ENABLE);

  /* DRVx_TIMER enable counter */
  TIM_Cmd(DRV_A_TIMER, ENABLE);
	TIM_Cmd(DRV_B_TIMER, ENABLE);
}

void SetMotorARunParam(uint8_t id, MOTOR_DIR dir, uint16_t kFreq)
{
	if(kFreq < 10) kFreq = 10;
	if(kFreq > 200) kFreq = 200;
	DRV_A_TIMER->ARR = (8000000 / (kFreq * 1000)) - 1;

	if(dir == STOP) {
		DRV_A_TIMER->CCR1 = 0;
	} else {
		DRV_A_TIMER->CCR1 = DRV_A_TIMER->ARR >> 1;
		if(dir == R_FWD) {
			MOTOR_A_FWD();
		} else {
			MOTOR_A_REV();
		}
	}
}

void SetMotorBRunParam(uint8_t id, MOTOR_DIR dir, uint16_t kFreq)
{
	if(kFreq < 10) kFreq = 10;
	if(kFreq > 200) kFreq = 200;
	DRV_B_TIMER->ARR = (8000000 / (kFreq * 1000)) - 1;

	if(dir == STOP) {
		DRV_B_TIMER->CCR2 = 0;
	} else {
		DRV_B_TIMER->CCR2 = DRV_B_TIMER->ARR >> 1;
		if(dir == R_FWD) {
			MOTOR_B_FWD();
		} else {
			MOTOR_B_REV();
		}
	}
}
