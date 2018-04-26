#ifndef __ENCODER_H
#define __ENCODER_H

#include "stm32f10x_tim.h"

/* ENCODER A PHY DEFINITION */
#define ENCODER_A_GPIO_PORT_1          GPIOA
#define ENCODER_A_GPIO_PORT_CLK_1      RCC_APB2Periph_GPIOA
#define ENCODER_A_GPIO_PORT_2          GPIOA
#define ENCODER_A_GPIO_PORT_CLK_2      RCC_APB2Periph_GPIOA
#define ENCODER_A_GPIO_PIN_1           GPIO_Pin_0
#define ENCODER_A_GPIO_PIN_2           GPIO_Pin_1

#define ENCODER_A_TIM                  TIM2
#define ENCODER_A_TIM_CLK              RCC_APB1Periph_TIM2
#define ENCODER_A_TIM_IRQn             TIM2_IRQn
#define ENCODER_A_TIM_Handler          TIM2_IRQHandler
/* ENCODER B PHY DEFINITION */
#define ENCODER_B_GPIO_PORT            GPIOB
#define ENCODER_B_GPIO_PORT_CLK        RCC_APB2Periph_GPIOB
#define ENCODER_B_GPIO_PIN_1           GPIO_Pin_4
#define ENCODER_B_GPIO_PIN_2           GPIO_Pin_5

#define ENCODER_B_TIM                  TIM3
#define ENCODER_B_TIM_CLK              RCC_APB1Periph_TIM3
#define ENCODER_B_TIM_IRQn             TIM3_IRQn
#define ENCODER_B_TIM_Handler          TIM3_IRQHandler

typedef enum {
	Encoder_L = 0,
	Encoder_R = 1
} EncoderID_Def;

void EncoderDriverInit(void);
int16_t ReadEncoderCounter(EncoderID_Def id);

#endif /* __ENCODER_H */
