#ifndef __DW1000_H
#define __DW1000_H

#include "SysConfig.h"
#include "dw1000_device_api.h"
#include "instance_main.h"

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

#define _DW_RSTn_GPIO                  GPIOA
#define _DW_RSTn_PortSource            GPIO_PortSourceGPIOA
#define _DW_RSTn_GPIO_CLK              RCC_APB2Periph_GPIOA
#define _DW_RSTn_GPIO_Pin              GPIO_Pin_0
#define _DW_RSTn_GPIO_PinSource        GPIO_PinSource0
#define _DW_RSTn_GPIO_EXTI_LINE        EXTI_Line0
#define _DW_RSTn_GPIO_EXTI_IRQn        EXTI0_IRQn
#define _DW_RSTn_GPIO_EXTI_IRQHandler  EXTI0_IRQHandler

#define _DW_IRQn_GPIO                  GPIOB
#define _DW_IRQn_PortSource            GPIO_PortSourceGPIOB
#define _DW_IRQn_GPIO_CLK              RCC_APB2Periph_GPIOB
#define _DW_IRQn_GPIO_Pin              GPIO_Pin_5
#define _DW_IRQn_GPIO_PinSource        GPIO_PinSource5
#define _DW_IRQn_GPIO_EXTI_LINE        EXTI_Line5
#define _DW_IRQn_GPIO_EXTI_IRQn        EXTI9_5_IRQn
#define _DW_IRQn_GPIO_EXTI_IRQHandler  EXTI9_5_IRQHandler

#define _DW_IRQn_GET_LEVEL()           GPIO_ReadInputDataBit(_DW_IRQn_GPIO, _DW_IRQn_GPIO_Pin)
#define _DW_ENABLE_EXT_IRQ()           NVIC_EnableIRQ(_DW_IRQn_GPIO_EXTI_IRQn)
#define _DW_DISABLE_EXT_IRQ()          NVIC_DisableIRQ(_DW_IRQn_GPIO_EXTI_IRQn)
#define _DW_GET_EXT_IRQ_STATUS()       NVIC_GetPendingIRQ(_DW_IRQn_GPIO_EXTI_IRQn)
#define _DW_CLEAR_PENDING_EXT_IRQ()    NVIC_ClearPendingIRQ(_DW_IRQn_GPIO_EXTI_IRQn)

void DW1000_If_Init(void);

int DW1000_SPI_Read(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t readlength, uint8_t *readBuffer);
int DW1000_SPI_Write(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t bodylength, const uint8_t *bodyBuffer);

void DW1000_SystickCallback(void);

#endif /* __DW1000_H */
