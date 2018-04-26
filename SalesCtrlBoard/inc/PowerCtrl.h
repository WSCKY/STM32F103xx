#ifndef __POWERCTRL_H
#define __POWERCTRL_H

#include "stm32f10x_gpio.h"

#define PWR_GPIO_PORT        GPIOB
#define PWR_GPIO_PORT_CLK    RCC_APB2Periph_GPIOB

#define PWR_GPIO_PIN         GPIO_Pin_5

#define PWR_ON()             PWR_GPIO_PORT->BSRR = PWR_GPIO_PIN;
#define PWR_OFF()            PWR_GPIO_PORT->BRR = PWR_GPIO_PIN;
#define PWR_TOG()            PWR_GPIO_PORT->ODR ^= PWR_GPIO_PIN;

void PowerCtrlInit(void);

#endif /* __POWERCTRL_H */
