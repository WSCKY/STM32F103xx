#ifndef __DC_EN_H
#define __DC_EN_H

#include "stm32f10x_gpio.h"

#define EN_12V_GPIO_PORT               GPIOC
#define EN_12V_GPIO_PORT_CLK           RCC_APB2Periph_GPIOC

#define EN_12V_GPIO_PIN                GPIO_Pin_14

#define EN_20V1_GPIO_PORT              GPIOC
#define EN_20V1_GPIO_PORT_CLK          RCC_APB2Periph_GPIOC

#define EN_20V1_GPIO_PIN               GPIO_Pin_15

#define EN_20V2_GPIO_PORT              GPIOA
#define EN_20V2_GPIO_PORT_CLK          RCC_APB2Periph_GPIOA

#define EN_20V2_GPIO_PIN               GPIO_Pin_12

#define DC12V_ENABLE()                 EN_12V_GPIO_PORT->BSRR = EN_12V_GPIO_PIN;
#define DC20V1_ENABLE()                EN_20V1_GPIO_PORT->BSRR = EN_20V1_GPIO_PIN;
#define DC20V2_ENABLE()                EN_20V2_GPIO_PORT->BSRR = EN_20V2_GPIO_PIN;

#define DC12V_DISABLE()                EN_12V_GPIO_PORT->BRR = EN_12V_GPIO_PIN;
#define DC20V1_DISABLE()               EN_20V1_GPIO_PORT->BRR = EN_20V1_GPIO_PIN;
#define DC20V2_DISABLE()               EN_20V2_GPIO_PORT->BRR = EN_20V2_GPIO_PIN;

void DC_EN_Init(void);

#endif /* __DC_EN_H */
