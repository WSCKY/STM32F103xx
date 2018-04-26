#ifndef __MOTORDRIVER_H
#define __MOTORDRIVER_H

#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

#define DRV_AIN_GPIO_PORT              GPIOA
#define DRV_AIN_GPIO_PORT_CLK          RCC_APB2Periph_GPIOA
#define DRV_BIN_GPIO_PORT              GPIOA
#define DRV_BIN_GPIO_PORT_CLK          RCC_APB2Periph_GPIOA

#define DRV_AIN_GPIO_PIN               GPIO_Pin_7
#define DRV_BIN_GPIO_PIN               GPIO_Pin_2

#define DRV_EN_GPIO_PORT               GPIOA
#define DRV_EN_GPIO_PORT_CLK           RCC_APB2Periph_GPIOA

#define DRV_EN_GPIO_PIN                GPIO_Pin_0

#define DRV_PWMA_GPIO_PORT             GPIOA
#define DRV_PWMA_GPIO_PORT_CLK         RCC_APB2Periph_GPIOA
#define DRV_PWMB_GPIO_PORT             GPIOA
#define DRV_PWMB_GPIO_PORT_CLK         RCC_APB2Periph_GPIOA

#define DRV_PWM_A_GPIO_PIN             GPIO_Pin_6
#define DRV_PWM_B_GPIO_PIN             GPIO_Pin_1

#define DRV_A_TIMER                    TIM3
#define DRV_A_TIMER_CLK                RCC_APB1Periph_TIM3

#define DRV_B_TIMER                    TIM2
#define DRV_B_TIMER_CLK                RCC_APB1Periph_TIM2

typedef enum {
	STOP = 0,
	R_FWD = 1,
	R_REV = 2
} MOTOR_DIR;

#define MOTOR_A_FWD()                  do {DRV_AIN_GPIO_PORT->BRR = DRV_AIN_GPIO_PIN;} while(0)
#define MOTOR_A_REV()                  do {DRV_AIN_GPIO_PORT->BSRR = DRV_AIN_GPIO_PIN;} while(0)

#define MOTOR_B_FWD()                  do {DRV_BIN_GPIO_PORT->BRR = DRV_BIN_GPIO_PIN;} while(0)
#define MOTOR_B_REV()                  do {DRV_BIN_GPIO_PORT->BSRR = DRV_BIN_GPIO_PIN;} while(0)

void MotorDriverInit(void);
void SetMotorARunParam(uint8_t id, MOTOR_DIR dir, uint16_t kFreq);
void SetMotorBRunParam(uint8_t id, MOTOR_DIR dir, uint16_t kFreq);

#endif /* __MOTORDRIVER_H */
