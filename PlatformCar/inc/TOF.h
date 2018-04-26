#ifndef __TOF_H
#define __TOF_H

#include "stm32f10x_usart.h"

#include "CRC16.h"

#define TOF_USART                    USART3
#define TOF_USART_GPIO               GPIOC
#define TOF_USART_GPIO_CLK           RCC_APB2Periph_GPIOC
#define TOF_USART_CLK                RCC_APB1Periph_USART3
#define TOF_USART_RxPin              GPIO_Pin_11
#define TOF_USART_TxPin              GPIO_Pin_10
#define TOF_USART_IRQn               USART3_IRQn
#define TOF_USART_IRQHandler         USART3_IRQHandler

__packed typedef struct
{
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
