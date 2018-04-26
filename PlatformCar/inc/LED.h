#ifndef __LED_H
#define __LED_H

#include "stm32f10x_gpio.h"

#define LED_GPIO_PORT        GPIOB
#define LED_GPIO_PORT_CLK    RCC_APB2Periph_GPIOB

#define LED_GPIO_PIN_1       GPIO_Pin_12
#define LED_GPIO_PIN_2       GPIO_Pin_13
#define LED_GPIO_PIN_3       GPIO_Pin_14
#define LED_GPIO_PIN_4       GPIO_Pin_15

#define LED1_ON()            LED_GPIO_PORT->BSRR = LED_GPIO_PIN_1;
#define LED1_OFF()           LED_GPIO_PORT->BRR = LED_GPIO_PIN_1;
#define LED1_TOG()           LED_GPIO_PORT->ODR ^= LED_GPIO_PIN_1;

#define LED2_ON()            LED_GPIO_PORT->BSRR = LED_GPIO_PIN_2;
#define LED2_OFF()           LED_GPIO_PORT->BRR = LED_GPIO_PIN_2;
#define LED2_TOG()           LED_GPIO_PORT->ODR ^= LED_GPIO_PIN_2;

#define LED3_ON()            LED_GPIO_PORT->BSRR = LED_GPIO_PIN_3;
#define LED3_OFF()           LED_GPIO_PORT->BRR = LED_GPIO_PIN_3;
#define LED3_TOG()           LED_GPIO_PORT->ODR ^= LED_GPIO_PIN_3;

#define LED4_ON()            LED_GPIO_PORT->BSRR = LED_GPIO_PIN_4;
#define LED4_OFF()           LED_GPIO_PORT->BRR = LED_GPIO_PIN_4;
#define LED4_TOG()           LED_GPIO_PORT->ODR ^= LED_GPIO_PIN_4;

void LED_Init(void);

#endif /* __LED_H */
