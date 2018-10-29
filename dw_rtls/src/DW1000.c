#include "DW1000.h"

static uint8_t _rx_comp_flag = 1;
static uint8_t _tx_comp_flag = 1;
static DMA_InitTypeDef DMA_InitStructure;

static void SPI_Configuration(void);
static void DMA_Configuration(void);
static void RCC_Configuration(void);
static void GPIO_Configuration(void);
static void NVIC_Configuration(void);

void DW1000_If_Init(void)
{
	RCC_Configuration();
	SPI_Configuration();
	DMA_Configuration();
	GPIO_Configuration();
	NVIC_Configuration();
}

int DW1000_SPI_Read(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t readlength, uint8_t *readBuffer)
{
	uint8_t dummy = 0;
	decaIrqStatus_t stat = decamutexon();
	_tx_comp_flag = 0; _rx_comp_flag = 0;
	_DW_SPI_CS_ENABLE();
	DMA_InitStructure.DMA_BufferSize     = headerLength;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)headerBuffer;
	DMA_InitStructure.DMA_MemoryInc      = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR            = DMA_DIR_PeripheralDST;
	DMA_Init(_DW_SPI_Tx_DMA_Channel, &DMA_InitStructure);
	DMA_InitStructure.DMA_BufferSize     = headerLength;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)readBuffer;
	DMA_InitStructure.DMA_MemoryInc      = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_DIR            = DMA_DIR_PeripheralSRC;
	DMA_Init(_DW_SPI_Rx_DMA_Channel, &DMA_InitStructure);
	DMA_Cmd(_DW_SPI_Rx_DMA_Channel, ENABLE);
	DMA_Cmd(_DW_SPI_Tx_DMA_Channel, ENABLE);
	while(_tx_comp_flag == 0);
	while(_rx_comp_flag == 0);
	_tx_comp_flag = 0; _rx_comp_flag = 0;
	DMA_InitStructure.DMA_BufferSize     = readlength;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&dummy);
	DMA_InitStructure.DMA_MemoryInc      = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_DIR            = DMA_DIR_PeripheralDST;
	DMA_Init(_DW_SPI_Tx_DMA_Channel, &DMA_InitStructure);
	DMA_InitStructure.DMA_BufferSize     = readlength;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)readBuffer;
	DMA_InitStructure.DMA_MemoryInc      = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR            = DMA_DIR_PeripheralSRC;
	DMA_Init(_DW_SPI_Rx_DMA_Channel, &DMA_InitStructure);
	DMA_Cmd(_DW_SPI_Rx_DMA_Channel, ENABLE);
	DMA_Cmd(_DW_SPI_Tx_DMA_Channel, ENABLE);
	while(_tx_comp_flag == 0);
	while(_rx_comp_flag == 0);
	_DW_SPI_CS_DISABLE();
	decamutexoff(stat);
	return 0;
}

int DW1000_SPI_Write(uint16_t headerLength, const uint8_t *headerBuffer, uint32_t bodylength, const uint8_t *bodyBuffer)
{
	uint8_t dummy = 0;
	decaIrqStatus_t stat = decamutexon();
	_tx_comp_flag = 0; _rx_comp_flag = 0;
	_DW_SPI_CS_ENABLE();
	DMA_InitStructure.DMA_BufferSize     = headerLength;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)headerBuffer;
	DMA_InitStructure.DMA_MemoryInc      = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR            = DMA_DIR_PeripheralDST;
	DMA_Init(_DW_SPI_Tx_DMA_Channel, &DMA_InitStructure);
	DMA_InitStructure.DMA_BufferSize     = headerLength;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&dummy);
	DMA_InitStructure.DMA_MemoryInc      = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_DIR            = DMA_DIR_PeripheralSRC;
	DMA_Init(_DW_SPI_Rx_DMA_Channel, &DMA_InitStructure);
	DMA_Cmd(_DW_SPI_Rx_DMA_Channel, ENABLE);
	DMA_Cmd(_DW_SPI_Tx_DMA_Channel, ENABLE);
	while(_tx_comp_flag == 0);
	while(_rx_comp_flag == 0);
	_tx_comp_flag = 0; _rx_comp_flag = 0;
	DMA_InitStructure.DMA_BufferSize     = bodylength;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)bodyBuffer;
	DMA_InitStructure.DMA_MemoryInc      = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_DIR            = DMA_DIR_PeripheralDST;
	DMA_Init(_DW_SPI_Tx_DMA_Channel, &DMA_InitStructure);
	DMA_InitStructure.DMA_BufferSize     = bodylength;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)(&dummy);
	DMA_InitStructure.DMA_MemoryInc      = DMA_MemoryInc_Disable;
	DMA_InitStructure.DMA_DIR            = DMA_DIR_PeripheralSRC;
	DMA_Init(_DW_SPI_Rx_DMA_Channel, &DMA_InitStructure);
	DMA_Cmd(_DW_SPI_Rx_DMA_Channel, ENABLE);
	DMA_Cmd(_DW_SPI_Tx_DMA_Channel, ENABLE);
	while(_tx_comp_flag == 0);
	while(_rx_comp_flag == 0);
	_DW_SPI_CS_DISABLE();
	decamutexoff(stat);
	return 0;
}

void setup_DW1000RSTnIRQ(int enable)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	if(enable) {
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = _DW_RSTn_GPIO_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
		GPIO_Init(_DW_RSTn_GPIO, &GPIO_InitStructure);
		/* Configure EXTI line */
		EXTI_InitStructure.EXTI_Line = _DW_RSTn_GPIO_EXTI_LINE;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = ENABLE;
		EXTI_Init(&EXTI_InitStructure);
	} else {
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Pin = _DW_RSTn_GPIO_Pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
		GPIO_Init(_DW_RSTn_GPIO, &GPIO_InitStructure);
		/* Configure EXTI line */
		EXTI_InitStructure.EXTI_Line = _DW_RSTn_GPIO_EXTI_LINE;
		EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
		EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
		EXTI_InitStructure.EXTI_LineCmd = DISABLE;
		EXTI_Init(&EXTI_InitStructure);
	}
}

void SPI_ConfigFastRate(uint32_t scalingfactor)
{
	uint16_t tmpreg = 0;
	if(scalingfactor != DW1000_SPI_HIGH && scalingfactor != DW1000_SPI_LOW) return;
	/* Disable the selected SPI peripheral */
	_DW_SPI->CR1 &= 0xFFBF;
	/* Get the SPIx CR1 value */
	tmpreg = _DW_SPI->CR1;
	/* Clear BR[2:0] bits */
	tmpreg &= 0xFFC7;
	/* Set the scaling bits */
	tmpreg |= scalingfactor;
	/* Write to SPIx CR1 */
	_DW_SPI->CR1 = tmpreg;
	/* Enable the selected SPI peripheral */
	_DW_SPI->CR1 |= 0x0040;
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
	NVIC_InitStructure.NVIC_IRQChannel = _DW_IRQn_GPIO_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = _DW_RSTn_GPIO_EXTI_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
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
  RCC_APB2PeriphClockCmd(_DW_SPI_CLK | _DW_SPI_GPIO_CLK | _DW_IRQn_GPIO_CLK | _DW_RSTn_GPIO_CLK, ENABLE);
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
	EXTI_InitTypeDef EXTI_InitStructure;

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

	/* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line = _DW_IRQn_GPIO_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	/* Configure GPIO used as DW1000 RSTn */
	GPIO_InitStructure.GPIO_Pin = _DW_RSTn_GPIO_Pin;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(_DW_RSTn_GPIO, &GPIO_InitStructure);

	/* Connect EXTI Line to GPIO Pin */
	GPIO_EXTILineConfig(_DW_RSTn_PortSource, _DW_RSTn_GPIO_PinSource);

	/* Configure EXTI line */
	EXTI_InitStructure.EXTI_Line = _DW_RSTn_GPIO_EXTI_LINE;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	EXTI_Init(&EXTI_InitStructure);
}

void _DW_IRQn_GPIO_EXTI_IRQHandler(void)
{
	process_deca_irq();
	EXTI_ClearITPendingBit(_DW_IRQn_GPIO_EXTI_LINE);
}

void _DW_RSTn_GPIO_EXTI_IRQHandler(void)
{
	process_dwRSTn_irq();
	EXTI_ClearITPendingBit(_DW_RSTn_GPIO_EXTI_LINE);
}

/*
 * this function handles _DW_SPI Rx DMA Interrupt.
 */
void _DW_SPI_Rx_DMA_IRQHandler(void)
{
	if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC2)) {
		DMA_ClearFlag(DMA1_FLAG_TC2);
		_rx_comp_flag = 1;
		/* Disable DMA1 SPI Rx Channel */
		DMA_Cmd(_DW_SPI_Rx_DMA_Channel, DISABLE);
	}
}

/*
 * this function handles _DW_SPI Tx DMA Interrupt.
 */
void _DW_SPI_Tx_DMA_IRQHandler(void)
{
	if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC3)) {
		DMA_ClearFlag(DMA1_FLAG_TC3);
		_tx_comp_flag = 1;
		/* Disable DMA1 SPI Tx Channel */
		DMA_Cmd(_DW_SPI_Tx_DMA_Channel, DISABLE);
	}
}
