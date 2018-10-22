#include "DW1000.h"

static DMA_InitTypeDef DMA_InitStructure;

void DW1000_If_Init(void)
{

}

/**
  * @brief  Configure SPI peripheral.
	* @param  None
	* @retval None
	*/
static void SPI_Configuration(void)
{
	SPI_InitTypeDef SPI_InitStructure;
	/* SPI_MASTER configuration ------------------------------------------------------*/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(_DW_SPI, &SPI_InitStructure);

	/* Enable IMU_SPI Rx&Tx request */
  SPI_I2S_DMACmd(_DW_SPI, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, ENABLE);

	/* Enable IMU_SPI */
	SPI_Cmd(_DW_SPI, ENABLE);
}

/**
  * @brief  Configures SPI DMA Peripheral.
  * @param  None
  * @retval None
  */
static void DMA_Configuration(void)
{
	

	/* SPI_SLAVE_Rx_DMA_Channel and SPI_SLAVE_Tx_DMA_Channel configuration -------------*/
  DMA_DeInit(_DW_SPI_Rx_DMA_Channel);
	DMA_DeInit(_DW_SPI_Tx_DMA_Channel);
  DMA_InitStructure.DMA_PeripheralBaseAddr = _DW_SPI_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = 0;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 15;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(_DW_SPI_Rx_DMA_Channel, &DMA_InitStructure);

	DMA_InitStructure.DMA_MemoryBaseAddr = 0;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_Init(_DW_SPI_Tx_DMA_Channel, &DMA_InitStructure);

	DMA_ITConfig(_DW_SPI_Rx_DMA_Channel, DMA_IT_TC, ENABLE);
	DMA_ITConfig(_DW_SPI_Tx_DMA_Channel, DMA_IT_TC, ENABLE);

	/* Enable DMA1 SPI Rx & Tx Channel */
//	DMA_Cmd(_DW_SPI_Rx_DMA_Channel, ENABLE);
//	DMA_Cmd(_DW_SPI_Tx_DMA_Channel, ENABLE);
}

/**
  * @brief  Configures the NVIC.
  * @param  None
  * @retval None
  */
static void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the IMU_SPI_Rx_DMA_IRQn and IMU_SPI_Tx_DMA_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = _DW_SPI_Rx_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = _DW_SPI_Tx_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

/**
  * @brief  Configures the system peripheral clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{
  /* Enable SPI_MASTER clock and GPIO clock */
  RCC_APB2PeriphClockCmd(_DW_SPI_CLK | _DW_SPI_GPIO_CLK | _DW_IRQn_GPIO_CLK, ENABLE);
	/* DMA1 clock enable */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
static void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Configure SPI_MASTER pins: SCK, MISO, MOSI and NSS */
  GPIO_InitStructure.GPIO_Pin = _DW_SPI_PIN_SCK | _DW_SPI_PIN_MISO | _DW_SPI_PIN_MOSI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(_DW_SPI_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = _DW_SPI_PIN_NSS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(_DW_SPI_GPIO, &GPIO_InitStructure);
	_DW_SPI_CS_DISABLE();

	/* Configure GPIO used as DW1000 IRQ for interrupt */
	GPIO_InitStructure.GPIO_Pin = _DW_IRQn_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(_DW_IRQn_GPIO, &GPIO_InitStructure);

	/* Connect EXTI Line to GPIO Pin */
	GPIO_EXTILineConfig(_DW_IRQn_PortSource, _DW_IRQn_GPIO_PinSource);
}
