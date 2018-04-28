#ifndef __REMOTECONTROLLER_H
#define __REMOTECONTROLLER_H

#include "stm32f10x_usart.h"

#define RC_USART                    UART4
#define RC_USART_GPIO               GPIOC
#define RC_USART_GPIO_CLK           RCC_APB2Periph_GPIOC
#define RC_USART_CLK                RCC_APB1Periph_UART4
#define RC_USART_DMA_CLK            RCC_AHBPeriph_DMA2
#define RC_USART_RxPin              GPIO_Pin_11
#define RC_USART_TxPin              GPIO_Pin_10
#define RC_USART_Tx_DMA_Channel     DMA2_Channel5
#define RC_USART_Tx_DMA_FLAG        DMA2_FLAG_TC5
#define RC_USART_DR_Base            0x40004C04
#define RC_USART_IRQn               UART4_IRQn
#define RC_USART_IRQHandler         UART4_IRQHandler
#define RC_USART_Tx_DMA_IRQn        DMA2_Channel4_5_IRQn
#define RC_USART_Tx_DMA_IRQHandler  DMA2_Channel4_5_IRQHandler

void RemoteControlInit(void);
void RCPortReceivedDataCallBack(uint8_t Data);
void RCPortSendBytesDMA(uint8_t *p, uint32_t l);

#endif /* __REMOTECONTROLLER_H */
