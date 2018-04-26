#ifndef __MPU6500_H
#define __MPU6500_H

#include "stm32f10x.h"

#include "cmsis_os.h"

typedef struct {
	int16_t accX;
	int16_t accY;
	int16_t accZ;
	int16_t temp;
	int16_t gyrX;
	int16_t gyrY;
	int16_t gyrZ;
} MPU_RAW;

typedef struct {
	int16_t accX;
	int16_t accY;
	int16_t accZ;
} AccRawDef;

typedef struct {
	int16_t gyrX;
	int16_t gyrY;
	int16_t gyrZ;
} GyrRawDef;

typedef struct {
	float accX;
	float accY;
	float accZ;
} AccDataDef;

typedef struct {
	float gyrX;
	float gyrY;
	float gyrZ;
} GyrDataDef;

#define IMU_SPI                        SPI1
#define IMU_SPI_CLK                    RCC_APB2Periph_SPI1
#define IMU_SPI_GPIO                   GPIOA
#define IMU_SPI_GPIO_CLK               RCC_APB2Periph_GPIOA  
#define IMU_SPI_PIN_NSS                GPIO_Pin_4
#define IMU_SPI_PIN_SCK                GPIO_Pin_5
#define IMU_SPI_PIN_MISO               GPIO_Pin_6
#define IMU_SPI_PIN_MOSI               GPIO_Pin_7

#define IMU_SPI_DMA_CLK                RCC_AHBPeriph_DMA1
#define IMU_SPI_Rx_DMA_Channel         DMA1_Channel2
#define IMU_SPI_Tx_DMA_Channel         DMA1_Channel3
#define IMU_SPI_DR_Base                0x4001300C
#define IMU_SPI_Rx_DMA_IRQn            DMA1_Channel2_IRQn
#define IMU_SPI_Tx_DMA_IRQn            DMA1_Channel3_IRQn
#define IMU_SPI_Rx_DMA_IRQHandler      DMA1_Channel2_IRQHandler
#define IMU_SPI_Tx_DMA_IRQHandler      DMA1_Channel3_IRQHandler

#define IMU_SPI_CS_ENABLE()            (IMU_SPI_GPIO->BRR = IMU_SPI_PIN_NSS)
#define IMU_SPI_CS_DISABLE()           (IMU_SPI_GPIO->BSRR = IMU_SPI_PIN_NSS)

void MPU6500_Init(void);
void MPU6500_Read(GyrRawDef *Offset, uint8_t IsOffset);
float GetIMUTemperature(void);
AccDataDef *GetAccDataPointer(void);
GyrDataDef *GetGyrDataPointer(void);
MPU_RAW *GetMPU_RawDataPointer(void);

#endif /* __MPU6500_H */
