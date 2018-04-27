#ifndef __REMOTECONTROLLER_H
#define __REMOTECONTROLLER_H

#include "stm32f10x_usart.h"

#define RC_USART                    USART3
#define RC_USART_GPIO               GPIOB
#define RC_USART_GPIO_CLK           RCC_APB2Periph_GPIOB
#define RC_USART_CLK                RCC_APB1Periph_USART3
#define RC_USART_RxPin              GPIO_Pin_11
#define RC_USART_TxPin              GPIO_Pin_10
#define RC_USART_IRQn               USART3_IRQn
#define RC_USART_IRQHandler         USART3_IRQHandler

void RemoteControlInit(void);
uint8_t GetRCUpdateFlag(void);

#endif /* __REMOTECONTROLLER_H */
