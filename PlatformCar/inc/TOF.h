#ifndef __TOF_H
#define __TOF_H

#include "stm32f10x_usart.h"

#include "CRC16.h"

#define TOF_USART                    UART5
#define TOF_USART_Rx_GPIO            GPIOD
#define TOF_USART_Tx_GPIO            GPIOC
#define TOF_USART_Rx_GPIO_CLK        RCC_APB2Periph_GPIOD
#define TOF_USART_Tx_GPIO_CLK        RCC_APB2Periph_GPIOC
#define TOF_USART_CLK                RCC_APB1Periph_UART5
#define TOF_USART_RxPin              GPIO_Pin_2
#define TOF_USART_TxPin              GPIO_Pin_12
#define TOF_USART_IRQn               UART5_IRQn
#define TOF_USART_IRQHandler         UART5_IRQHandler

__packed typedef struct {
	uint8_t Header1;
	uint8_t Header2;
	uint8_t HighByte;
	uint8_t LowByte;
	uint8_t HighCRC;
	uint8_t LowCRC;
} TOF_ORIGIN_DATA;

void TOFDriverInit(void);
uint8_t GetNewTOFData(float *d);

#endif /* __TOF_H */
