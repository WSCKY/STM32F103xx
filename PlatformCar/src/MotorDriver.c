#include "MotorDriver.h"

static void timer_config(void);

void MotorDriverInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(DRV_EN_GPIO_PORT_CLK | DRV_AIN_GPIO_PORT_CLK | DRV_BIN_GPIO_PORT_CLK | \
	                       DRV_PWMA_GPIO_PORT_CLK | DRV_PWMB_GPIO_PORT_CLK | RCC_APB2Periph_AFIO, ENABLE);

	/* Configure CTRL_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = DRV_EN_GPIO_PIN;
	GPIO_Init(DRV_EN_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DRV_AIN_GPIO_PIN;
	GPIO_Init(DRV_AIN_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DRV_BIN_GPIO_PIN;
	GPIO_Init(DRV_BIN_GPIO_PORT, &GPIO_InitStructure);
	/* Stop Mode */
	GPIO_ResetBits(DRV_EN_GPIO_PORT, DRV_EN_GPIO_PIN);
	GPIO_ResetBits(DRV_AIN_GPIO_PORT, DRV_AIN_GPIO_PIN);
	GPIO_ResetBits(DRV_BIN_GPIO_PORT, DRV_BIN_GPIO_PIN);
	/* Configure PWM_PIN as alternate function push-pull */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = DRV_PWM_A_GPIO_PIN;
	GPIO_Init(DRV_PWMA_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = DRV_PWM_B_GPIO_PIN;
	GPIO_Init(DRV_PWMB_GPIO_PORT, &GPIO_InitStructure);

	timer_config();

//	RCC->APB2ENR|=1<<3;       //PORTB????   
//	RCC->APB2ENR|=1<<4;       //PORTC????  
//	GPIOB->CRL&=0XFFFFFF00;   //????
//	GPIOB->CRL|=0X00000022;   //????
//	//GPIOB->ODR|=0<<0;  	
//	//GPIOB->ODR|=0<<1;  	
//	
//	GPIOC->CRL&=0XFF00FFFF;   //????
//	GPIOC->CRL|=0X00220000;   //????
//	//GPIOC->ODR|=0<<4;
//	//GPIOC->ODR|=0<<5;  

//	RCC->APB2ENR|=1<<13;       //??TIM8??    
//	RCC->APB2ENR|=1<<4;        //PORTC????     
//	GPIOC->CRH&=0XFFFFFF00;    //PORTC8????
//	GPIOC->CRH|=0X000000BB;    //PORTC8????
//	
//	GPIOC->CRL&=0X00FFFFFF;    //PORTC6 7????
//	GPIOC->CRL|=0XBB000000;    //PORTC6 7????
//	TIM8->ARR=799;             //?????????? 
//	TIM8->PSC=8;             //???????
//	TIM8->CCMR1|=6<<4;         //CH1 PWM1??	
//	TIM8->CCMR1|=6<<12;        //CH2 PWM1??	
//	TIM8->CCMR2|=6<<4;         //CH3 PWM1??	
//	TIM8->CCMR2|=6<<12;        //CH4 PWM1??	
//	
//	TIM8->CCMR1|=1<<3;         //CH1?????	  
//	TIM8->CCMR1|=1<<11;        //CH2?????	 
//	TIM8->CCMR2|=1<<3;         //CH3?????	  
//	TIM8->CCMR2|=1<<11;        //CH4?????	  
//	
//	TIM8->CCER|=1<<0;         //CH1????	
//	TIM8->CCER|=1<<4;         //CH2????	   
//	TIM8->CCER|=1<<8;         //CH3????	 
//	TIM8->CCER|=1<<12;        //CH4????
//	TIM8->BDTR |= 1<<15;       //TIM??????????PWM
//	TIM8->CR1=0x8000;          //ARPE?? 
//	TIM8->CR1|=0x01;          //?????1 			
}

static void timer_config(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	uint16_t PrescalerValue = 0;

	/* DRV_TIMER clock enable */
  RCC_APB2PeriphClockCmd(DRV_TIMER_CLK, ENABLE);

	/* -----------------------------------------------------------------------
    DRV_TIMER Configuration: generate 2 PWM signals with 2 different duty cycles:
    The DRV_TIMER_CLK frequency is set to SystemCoreClock (Hz), to get DRV_TIMER counter
    clock at 8 MHz the Prescaler is computed as following:
     - Prescaler = (DRV_TIMER_CLK / DRV_TIMER counter clock) - 1
    SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
    and Connectivity line devices and to 8 MHz for Low-Density Value line and
    Medium-Density Value line devices

    The DRV_TIMER is running at 10 KHz: DRV_TIMER Frequency = DRV_TIMER counter clock/(ARR + 1)
                                                  = 8 MHz / 800 = 10 KHz
    DRV_TIMER Channel1 duty cycle = (DRV_TIMER_CCR1/ DRV_TIMER_ARR) * 100
    DRV_TIMER Channel3 duty cycle = (DRV_TIMER_CCR3/ DRV_TIMER_ARR) * 100
  ----------------------------------------------------------------------- */
  /* Compute the prescaler value */
  PrescalerValue = (uint16_t) (SystemCoreClock / 8000000) - 1;
  /* Time base configuration */
  TIM_TimeBaseStructure.TIM_Period = 799;
  TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
  TIM_TimeBaseStructure.TIM_ClockDivision = 0;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

  TIM_TimeBaseInit(DRV_TIMER, &TIM_TimeBaseStructure);

  /* PWM1 Mode configuration: Channel1 */
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCNIdleState_Set;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC1Init(DRV_TIMER, &TIM_OCInitStructure);
  TIM_OC1PreloadConfig(DRV_TIMER, TIM_OCPreload_Enable);

  /* PWM1 Mode configuration: Channel3 */
  TIM_OCInitStructure.TIM_Pulse = 0;

  TIM_OC3Init(DRV_TIMER, &TIM_OCInitStructure);
  TIM_OC3PreloadConfig(DRV_TIMER, TIM_OCPreload_Enable);

  TIM_ARRPreloadConfig(DRV_TIMER, ENABLE);

  /* DRV_TIMER enable counter */
  TIM_Cmd(DRV_TIMER, ENABLE);

	/* DRV_TIMER Main Output Enable */
  TIM_CtrlPWMOutputs(DRV_TIMER, ENABLE);
}

void SetMotorEnable(FunctionalState NewState)
{
	if(NewState == ENABLE) {
		GPIO_SetBits(DRV_EN_GPIO_PORT, DRV_EN_GPIO_PIN);
	} else {
		GPIO_ResetBits(DRV_EN_GPIO_PORT, DRV_EN_GPIO_PIN);
	}
}

void SetRunningSpeed(int16_t spd_L, int16_t spd_R)
{
	if(spd_L > 800) spd_L = 800; if(spd_L < -800) spd_L = -800;
	if(spd_R > 800) spd_R = 800; if(spd_R < -800) spd_R = -800;

	if(spd_L < 0) {
		MOTOR_A_REV();
		DRV_TIMER->CCR1 = 800 + spd_L;
	} else {
		MOTOR_A_FWD();
		DRV_TIMER->CCR1 = spd_L;
	}
	if(spd_R < 0) {
		MOTOR_B_REV();
		DRV_TIMER->CCR3 = 800 + spd_R;
	} else {
		MOTOR_B_FWD();
		DRV_TIMER->CCR3 = spd_R;
	}
}
