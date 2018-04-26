#ifndef __WS281x_H
#define __WS281x_H

#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

/* Left LED Bar */
#define WS281x_L_GPIO_PORT                  GPIOB
#define WS281x_L_GPIO_PORT_CLK              RCC_APB2Periph_GPIOB

#define WS281x_L_GPIO_PIN                   GPIO_Pin_8

#define WS281x_L_TIMER                      TIM4
#define WS281x_L_TIMER_CLK                  RCC_APB1Periph_TIM4
#define WS281x_L_TIMER_CCR_Address          (0x4000083C)

#define WS281x_L_DMA_CHANNEL                DMA1_Channel5
#define WS281x_L_DMA_TC_FLAG                DMA1_FLAG_TC5
#define WS281x_L_DMA_CHANNEL_IRQn           DMA1_Channel5_IRQn
#define WS281x_L_DMA_CHANNEL_IRQHandler     DMA1_Channel5_IRQHandler

/* Right LED Bar */
#define WS281x_R_GPIO_PORT                  GPIOA
#define WS281x_R_GPIO_PORT_CLK              RCC_APB2Periph_GPIOA

#define WS281x_R_GPIO_PIN                   GPIO_Pin_8

#define WS281x_R_TIMER                      TIM1
#define WS281x_R_TIMER_CLK                  RCC_APB2Periph_TIM1
#define WS281x_R_TIMER_CCR_Address          (0x40012C34)

#define WS281x_R_DMA_CHANNEL                DMA1_Channel2
#define WS281x_R_DMA_TC_FLAG                DMA1_FLAG_TC2
#define WS281x_R_DMA_CHANNEL_IRQn           DMA1_Channel2_IRQn
#define WS281x_R_DMA_CHANNEL_IRQHandler     DMA1_Channel2_IRQHandler

/* WS2811 Drive Frequency (1.25us / 800KHz) */
#define WS2811_TARGET_FREQ                  800000

/* WS2811 Color Mask */
#define WS2811_R_MASK                       0x00FF0000
#define WS2811_G_MASK                       0x0000FF00
#define WS2811_B_MASK                       0x000000FF
#define WS2811_COLOR_ZONE                   (24)

typedef enum {
	LED_BAR_LEFT = 0,
	LED_BAR_RIGHT = 1,
} LED_BAR_POS_DEF;

#define LED_COUNT                           36

void WS281xDriverInit(void);
uint8_t UpdateColorBarData(uint32_t *src, LED_BAR_POS_DEF id);

#endif /* __WS281x_H */
