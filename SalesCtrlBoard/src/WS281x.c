#include "WS281x.h"

#define SEQUENCE_0                     9
#define SEQUENCE_1                     36

#define DELAY_TICKS                    45

//static uint8_t _trans_comp = 0;
static uint8_t SRC_L_Buffer[LED_COUNT * WS2811_COLOR_ZONE + DELAY_TICKS] = {0};
static uint8_t SRC_R_Buffer[LED_COUNT * WS2811_COLOR_ZONE + DELAY_TICKS] = {0};

static void RCC_Configuration(void);
static void DMA_Configuration(void);
static void GPIO_Configuration(void);

void WS281xDriverInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	/* System Clocks Configuration */
	RCC_Configuration();
	/* GPIO Configuration */
	GPIO_Configuration();
	/* DMA Configuration */
	DMA_Configuration();

	/* WS281x_TIMER DMA Transfer -------------------------------------------------
  TIM4CLK = SystemCoreClock (Hz), Prescaler = 1, TIM4 counter clock = SystemCoreClock / 2
  SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
  and Connectivity line devices and to 24 MHz for Low-Density Value line and
  Medium-Density Value line devices.

  The objective is to configure TIM4 channel 1 to generate complementary PWM
  signal with a frequency equal to 800 KHz:
     - TIM4_Period = ((SystemCoreClock / 2) / 800000) - 1 = 44
  and a variable duty cycle that is changed by the DMA after a specific number of
  Update DMA request.

  The number of this repetitive requests is defined by the TIM4 Repetition counter,
  each 3 Update Requests, the TIM4 Channel 1 Duty Cycle changes to the next new 
  value defined by the SRC_Buffer . 
  -----------------------------------------------------------------------------*/

  /* WS281x_TIMER Peripheral Configuration --------------------------------------------*/
  /* Time Base configuration */
  TIM_TimeBaseStructure.TIM_Prescaler = 1;
  TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseStructure.TIM_Period = 44;
  TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseStructure.TIM_RepetitionCounter = 1;
  TIM_TimeBaseInit(WS281x_L_TIMER, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(WS281x_R_TIMER, &TIM_TimeBaseStructure);

  /* Channel x Configuration in PWM mode */
  TIM_OCInitStructure.TIM_Pulse = 0;
  TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
  TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Enable;
  TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_Low;
  TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Set;
	TIM_OCInitStructure.TIM_OCNIdleState = TIM_OCIdleState_Reset;
  TIM_OC3Init(WS281x_L_TIMER, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(WS281x_L_TIMER, TIM_OCPreload_Enable);

	TIM_OC1Init(WS281x_R_TIMER, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(WS281x_R_TIMER, TIM_OCPreload_Enable);

  /* WS281x_TIMER CCR Update DMA Request enable */
  TIM_DMACmd(WS281x_L_TIMER, TIM_DMA_CC3, ENABLE);
	TIM_DMACmd(WS281x_R_TIMER, TIM_DMA_CC1, ENABLE);

//	TIM_ARRPreloadConfig(WS281x_L_TIMER, ENABLE);
//	TIM_ARRPreloadConfig(WS281x_R_TIMER, ENABLE);

  /* WS281x_TIMER counter enable */
  TIM_Cmd(WS281x_L_TIMER, ENABLE);
	TIM_Cmd(WS281x_R_TIMER, ENABLE);

	/* Main Output Enable */
//  TIM_CtrlPWMOutputs(WS281x_L_TIMER, ENABLE);
	TIM_CtrlPWMOutputs(WS281x_R_TIMER, ENABLE);
}

/**
  * @brief  Update SRC_Buffer.
  * @param  *src: color source data.
  * @retval None
  */
uint8_t UpdateColorBarData(uint32_t *src, LED_BAR_POS_DEF id)
{
	uint8_t *p = SRC_L_Buffer;
	if(id == LED_BAR_RIGHT) p = SRC_R_Buffer;

//	if(!_trans_comp) return 0;
//	_trans_comp = 0;

	uint8_t i, j, color, mask;
	for(i = 0; i < LED_COUNT; i ++) {
		mask = 0x80;
		color = (src[i] & WS2811_G_MASK) >> 8;
		for(j = 0; j < 8; j ++) {
			if(color & mask)
				p[i * WS2811_COLOR_ZONE + j] = SEQUENCE_1;
			else
				p[i * WS2811_COLOR_ZONE + j] = SEQUENCE_0;
			mask >>= 1;
		}
		mask = 0x80;
		color = (src[i] & WS2811_R_MASK) >> 16;
		for(j = 8; j < 16; j ++) {
			if(color & mask)
				p[i * WS2811_COLOR_ZONE + j] = SEQUENCE_1;
			else
				p[i * WS2811_COLOR_ZONE + j] = SEQUENCE_0;
			mask >>= 1;
		}
		mask = 0x80;
		color = (src[i] & WS2811_B_MASK) >> 0;
		for(j = 16; j < 24; j ++) {
			if(color & mask)
				p[i * WS2811_COLOR_ZONE + j] = SEQUENCE_1;
			else
				p[i * WS2811_COLOR_ZONE + j] = SEQUENCE_0;
			mask >>= 1;
		}
	}

//	/* WS281x_DMA_CHANNEL enable */
//  DMA_Cmd(WS281x_DMA_CHANNEL, ENABLE);

	return 1;
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{
  /* WS281x_TIMER, WS281x_GPIO_PORT clock enable */
	RCC_APB1PeriphClockCmd(WS281x_L_TIMER_CLK, ENABLE);
  RCC_APB2PeriphClockCmd(WS281x_L_GPIO_PORT_CLK | WS281x_R_GPIO_PORT_CLK | WS281x_R_TIMER_CLK | RCC_APB2Periph_AFIO, ENABLE);
  /* DMA clock enable */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

/**
  * @brief  Configure the WS281x_TIMER PWM Pins.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* WS281x_GPIO Configuration: TIM Channel as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin =  WS281x_L_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(WS281x_L_GPIO_PORT, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin =  WS281x_R_GPIO_PIN;
	GPIO_Init(WS281x_R_GPIO_PORT, &GPIO_InitStructure);
}

/**
  * @brief  Configures the DMA.
  * @param  None
  * @retval None
  */
static void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

  /* WS281x_DMA_CHANNEL Config */
  DMA_DeInit(WS281x_L_DMA_CHANNEL);
	DMA_DeInit(WS281x_R_DMA_CHANNEL);

  DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)WS281x_L_TIMER_CCR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SRC_L_Buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = LED_COUNT * WS2811_COLOR_ZONE + DELAY_TICKS;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(WS281x_L_DMA_CHANNEL, &DMA_InitStructure);

	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)WS281x_R_TIMER_CCR_Address;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)SRC_R_Buffer;
	DMA_Init(WS281x_R_DMA_CHANNEL, &DMA_InitStructure);

//	/* Enable Transfer complete interrupt. */
//	DMA_ITConfig(WS281x_L_DMA_CHANNEL, DMA_IT_TC, ENABLE);
//	DMA_ITConfig(WS281x_R_DMA_CHANNEL, DMA_IT_TC, ENABLE);

//  /* Enable and set priority of WS281x_DMA_CHANNEL_IRQn Interrupt */
//  NVIC_InitStructure.NVIC_IRQChannel = WS281x_L_DMA_CHANNEL_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

//  NVIC_InitStructure.NVIC_IRQChannel = WS281x_R_DMA_CHANNEL_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x1;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

  /* WS281x_DMA_CHANNEL enable */
  DMA_Cmd(WS281x_L_DMA_CHANNEL, ENABLE);
	DMA_Cmd(WS281x_R_DMA_CHANNEL, ENABLE);
}

void WS281x_L_DMA_CHANNEL_IRQHandler(void)
{
	if(DMA_GetFlagStatus(WS281x_L_DMA_TC_FLAG)) {
		DMA_ClearFlag(WS281x_L_DMA_TC_FLAG);
//		/* WS281x_DMA_CHANNEL disable */
//		DMA_Cmd(WS281x_DMA_CHANNEL, DISABLE);
//		WS281x_TIMER->CCR1 = 0;
//		_trans_comp = 1;
	}
}

void WS281x_R_DMA_CHANNEL_IRQHandler(void)
{
	if(DMA_GetFlagStatus(WS281x_R_DMA_TC_FLAG)) {
		DMA_ClearFlag(WS281x_R_DMA_TC_FLAG);
//		/* WS281x_DMA_CHANNEL disable */
//		DMA_Cmd(WS281x_DMA_CHANNEL, DISABLE);
//		WS281x_TIMER->CCR1 = 0;
//		_trans_comp = 1;
	}
}
