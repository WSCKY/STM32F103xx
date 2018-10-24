#ifndef __LED_H
#define __LED_H

#include "stm32f10x_gpio.h"

#define LED_GPIO_PORT        GPIOC
#define LED_GPIO_PORT_CLK    RCC_APB2Periph_GPIOC

#define LED_GPIO_PIN_1       GPIO_Pin_8
#define LED_GPIO_PIN_2       GPIO_Pin_9

#define LED1_ON()             LED_GPIO_PORT->BRR = LED_GPIO_PIN_1;
#define LED1_OFF()            LED_GPIO_PORT->BSRR = LED_GPIO_PIN_1;
#define LED1_TOG()            LED_GPIO_PORT->ODR ^= LED_GPIO_PIN_1;

#define LED2_ON()             LED_GPIO_PORT->BRR = LED_GPIO_PIN_2;
#define LED2_OFF()            LED_GPIO_PORT->BSRR = LED_GPIO_PIN_2;
#define LED2_TOG()            LED_GPIO_PORT->ODR ^= LED_GPIO_PIN_2;

void LED_Init(void);

#endif /* __LED_H */
