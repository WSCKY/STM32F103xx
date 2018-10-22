#ifndef __FREECONTROL_H
#define __FREECONTROL_H

#include "stm32f10x_usart.h"

#define RF_USART                    USART3
#define RF_USART_GPIO               GPIOB
#define RF_USART_GPIO_CLK           RCC_APB2Periph_GPIOB
#define RF_USART_CLK                RCC_APB1Periph_USART3
#define RF_USART_RxPin              GPIO_Pin_11
#define RF_USART_TxPin              GPIO_Pin_10
#define RF_USART_IRQn               USART3_IRQn
#define RF_USART_IRQHandler         USART3_IRQHandler

#define RF_MIN_VALUE                352
#define RF_MID_VALUE                1024
#define RF_MAX_VALUE                1696

__packed typedef struct
{
	uint8_t startByte;
	__packed union
	{
		uint16_t data8[22];
		__packed struct
		{
			uint16_t data1 :11;
			uint16_t data2 :11;
			uint16_t data3 :11;
			uint16_t data4 :11;
			uint16_t data5 :11;
			uint16_t data6 :11;
			uint16_t data7 :11;
			uint16_t data8 :11;
			uint16_t data9 :11;
			uint16_t data10 :11;
			uint16_t data11 :11;
			uint16_t data12 :11;
			uint16_t data13 :11;
			uint16_t data14 :11;
			uint16_t data15 :11;
			uint16_t data16 :11;
		} data11;
	} rf_data;
	uint8_t flags;
	uint8_t endByte;
} RF_DATA_t;

typedef struct {
	uint16_t Channel[16];
} RF_CHANNLE_t;

void FreeControlInit(void);
uint8_t GetRFUpdateFlag(void);
RF_CHANNLE_t *GetRF_ChannelData(void);
void RF_ParseData(void);

#endif /* __FREECONTROL_H */
