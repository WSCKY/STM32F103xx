#ifndef __BUTTON_H
#define __BUTTON_H

#include "stm32f10x_gpio.h"

#define PWR_CHK_GPIO_PORT               GPIOA
#define PWR_CHK_GPIO_PORT_CLK           RCC_APB2Periph_GPIOA

#define PWR_CHK_GPIO_PIN                GPIO_Pin_5

#define PWR_CHK_STATE()                 (PWR_CHK_GPIO_PORT->IDR & PWR_CHK_GPIO_PIN)
#define PWR_CHK_PRESSED()               (PWR_CHK_STATE() == RESET)

#define FWD_A_BTN_GPIO_PORT              GPIOB
#define FWD_A_BTN_GPIO_PORT_CLK          RCC_APB2Periph_GPIOB
#define BWD_A_BTN_GPIO_PORT              GPIOB
#define BWD_A_BTN_GPIO_PORT_CLK          RCC_APB2Periph_GPIOB
#define FWD_B_BTN_GPIO_PORT              GPIOB
#define FWD_B_BTN_GPIO_PORT_CLK          RCC_APB2Periph_GPIOB
#define BWD_B_BTN_GPIO_PORT              GPIOB
#define BWD_B_BTN_GPIO_PORT_CLK          RCC_APB2Periph_GPIOB

#define FWD_A_BTN_GPIO_PIN               GPIO_Pin_12
#define BWD_A_BTN_GPIO_PIN               GPIO_Pin_13
#define FWD_B_BTN_GPIO_PIN               GPIO_Pin_14
#define BWD_B_BTN_GPIO_PIN               GPIO_Pin_15

#define FWD_A_BTN_STATE()                (FWD_A_BTN_GPIO_PORT->IDR & FWD_A_BTN_GPIO_PIN)
#define BWD_A_BTN_STATE()                (BWD_A_BTN_GPIO_PORT->IDR & BWD_A_BTN_GPIO_PIN)

#define FWD_B_BTN_STATE()                (FWD_B_BTN_GPIO_PORT->IDR & FWD_B_BTN_GPIO_PIN)
#define BWD_B_BTN_STATE()                (BWD_B_BTN_GPIO_PORT->IDR & BWD_B_BTN_GPIO_PIN)

#define FWD_A_BTN_PRESSED()              (FWD_A_BTN_STATE() == RESET)
#define BWD_A_BTN_PRESSED()              (BWD_A_BTN_STATE() == RESET)

#define FWD_B_BTN_PRESSED()              (FWD_B_BTN_STATE() == RESET)
#define BWD_B_BTN_PRESSED()              (BWD_B_BTN_STATE() == RESET)

void ButtonInit(void);

#endif /* __BUTTON_H */
