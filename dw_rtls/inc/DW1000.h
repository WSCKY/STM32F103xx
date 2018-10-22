#ifndef __DW1000_H
#define __DW1000_H

#include "SysConfig.h"

#define _DW_SPI                        SPI1
#define _DW_SPI_CLK                    RCC_APB2Periph_SPI1
#define _DW_SPI_GPIO                   GPIOA
#define _DW_SPI_GPIO_CLK               RCC_APB2Periph_GPIOA
#define _DW_SPI_PIN_NSS                GPIO_Pin_4
#define _DW_SPI_PIN_SCK                GPIO_Pin_5
#define _DW_SPI_PIN_MISO               GPIO_Pin_6
#define _DW_SPI_PIN_MOSI               GPIO_Pin_7

#define _DW_SPI_DMA_CLK                RCC_AHBPeriph_DMA1
#define _DW_SPI_Rx_DMA_Channel         DMA1_Channel2
#define _DW_SPI_Tx_DMA_Channel         DMA1_Channel3
#define _DW_SPI_DR_Base                0x4001300C
#define _DW_SPI_Rx_DMA_IRQn            DMA1_Channel2_IRQn
#define _DW_SPI_Tx_DMA_IRQn            DMA1_Channel3_IRQn
#define _DW_SPI_Rx_DMA_IRQHandler      DMA1_Channel2_IRQHandler
#define _DW_SPI_Tx_DMA_IRQHandler      DMA1_Channel3_IRQHandler

#define _DW_SPI_CS_ENABLE()            (_DW_SPI_GPIO->BRR = _DW_SPI_PIN_NSS)
#define _DW_SPI_CS_DISABLE()           (_DW_SPI_GPIO->BSRR = _DW_SPI_PIN_NSS)

#define _DW_IRQn_GPIO                  GPIOB
#define _DW_IRQn_PortSource            GPIO_PortSourceGPIOB
#define _DW_IRQn_GPIO_CLK              RCC_APB2Periph_GPIOB
#define _DW_IRQn_GPIO_Pin              GPIO_Pin_5
#define _DW_IRQn_GPIO_PinSource        GPIO_PinSource5

#endif /* __DW1000_H */
