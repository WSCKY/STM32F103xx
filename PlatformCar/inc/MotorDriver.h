#ifndef __MOTORDRIVER_H
#define __MOTORDRIVER_H

#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"

#define DRV_EN_GPIO_PORT               GPIOA
#define DRV_EN_GPIO_PORT_CLK           RCC_APB2Periph_GPIOA
#define DRV_EN_GPIO_PIN                GPIO_Pin_12

#define DRV_AIN_GPIO_PORT              GPIOC
#define DRV_AIN_GPIO_PORT_CLK          RCC_APB2Periph_GPIOC
#define DRV_BIN_GPIO_PORT              GPIOC
#define DRV_BIN_GPIO_PORT_CLK          RCC_APB2Periph_GPIOC

#define DRV_AIN_GPIO_PIN               GPIO_Pin_7
#define DRV_BIN_GPIO_PIN               GPIO_Pin_9

#define DRV_PWMA_GPIO_PORT             GPIOC
#define DRV_PWMA_GPIO_PORT_CLK         RCC_APB2Periph_GPIOC
#define DRV_PWMB_GPIO_PORT             GPIOC
#define DRV_PWMB_GPIO_PORT_CLK         RCC_APB2Periph_GPIOC

#define DRV_PWM_A_GPIO_PIN             GPIO_Pin_6
#define DRV_PWM_B_GPIO_PIN             GPIO_Pin_8

#define DRV_TIMER                      TIM8
#define DRV_TIMER_CLK                  RCC_APB2Periph_TIM8

#define MOTOR_A_FWD()                  do {DRV_AIN_GPIO_PORT->BSRR = DRV_AIN_GPIO_PIN;} while(0)
#define MOTOR_A_REV()                  do {DRV_AIN_GPIO_PORT->BRR = DRV_AIN_GPIO_PIN;} while(0)

#define MOTOR_B_FWD()                  do {DRV_BIN_GPIO_PORT->BSRR = DRV_BIN_GPIO_PIN;} while(0)
#define MOTOR_B_REV()                  do {DRV_BIN_GPIO_PORT->BRR = DRV_BIN_GPIO_PIN;} while(0)

void MotorDriverInit(void);
void SetMotorEnable(FunctionalState NewState);
void SetRunningSpeed(int16_t spd_L, int16_t spd_R);

#endif /* __MOTORDRIVER_H */
