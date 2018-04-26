#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x_gpio.h"

#define BUTTON_GPIO_PORT               GPIOB
#define BUTTON_GPIO_PORT_CLK           RCC_APB2Periph_GPIOB

#define BUTTON_GPIO_PIN                GPIO_Pin_14

#define BUTTON_STATE()                 (BUTTON_GPIO_PORT->IDR & BUTTON_GPIO_PIN)
#define BUTTON_PRESSED()               (BUTTON_STATE() == RESET)

void ButtonInit(void);

#endif /* __BUTTON_H */
