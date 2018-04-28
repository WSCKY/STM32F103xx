#include "mpu6500.h"

/* private variables */
static MPU_RAW mpu_data;
static AccDataDef accVal;
static GyrDataDef gyrVal;
static float temperature = 0;

static uint8_t _tx_comp_flag = 1;
static uint8_t mpu_tx_buffer[15] = {0};
static uint8_t mpu_rx_buffer[15] = {0};

static DMA_InitTypeDef DMA_InitStructure;

static void IMU_Configuration(void);
static void SPI_Configuration(void);
static void DMA_Configuration(void);
static void RCC_Configuration(void);
static void GPIO_Configuration(void);
static void spi_rx_tx_dma(uint8_t *w, uint8_t *r, uint16_t l);
static void mpu_write_reg_dma(uint8_t reg, uint8_t val);
static void mpu_read_reg_dma(uint8_t reg, uint8_t num, uint8_t *r);
static void mpu_read_reg_dma_util(uint8_t reg, uint8_t num, uint8_t *r);

void MPU6500_Init(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	DMA_Configuration();
	SPI_Configuration();

	IMU_Configuration();
}

uint16_t mpu_id = 0xFFFF; /* (mpu_id >> 8) = 0x70 */
/*
 * configure the MPU6500 registers.
 */
static void IMU_Configuration(void)
{
	mpu_write_reg_dma(0x6B, 0x80);
	osDelay(10);
	mpu_write_reg_dma(0x68, 0x07);
	mpu_write_reg_dma(0x6B, 0x00);
	osDelay(10);
	mpu_write_reg_dma(0x19, 0x00);
	mpu_write_reg_dma(0x1A, 0x03);
	mpu_write_reg_dma(0x1D, 0x06);
	mpu_write_reg_dma(0x1C, 0x10);
	mpu_write_reg_dma(0x1B, 0x18);
	osDelay(1);
	mpu_read_reg_dma_util(0x75, 1, (uint8_t *)&mpu_id);

	for(uint8_t i = 0; i < 15; i ++) mpu_tx_buffer[i] = 0x00;
}

/*
 * start read mpu6500 once.
 */
void MPU6500_Read(GyrRawDef *Offset, uint8_t IsOffset)
{
	if(_tx_comp_flag == 1) {
		/* Reorganize received data */
//		mpu_data = *(MPU_RAW *)&mpu_rx_buffer[1];
		((int8_t *)&mpu_data.accX)[0] = mpu_rx_buffer[2];
		((int8_t *)&mpu_data.accX)[1] = mpu_rx_buffer[1];

		((int8_t *)&mpu_data.accY)[0] = mpu_rx_buffer[4];
		((int8_t *)&mpu_data.accY)[1] = mpu_rx_buffer[3];

		((int8_t *)&mpu_data.accZ)[0] = mpu_rx_buffer[6];
		((int8_t *)&mpu_data.accZ)[1] = mpu_rx_buffer[5];

		((int8_t *)&mpu_data.temp)[0] = mpu_rx_buffer[8];
		((int8_t *)&mpu_data.temp)[1] = mpu_rx_buffer[7];

		((int8_t *)&mpu_data.gyrX)[0] = mpu_rx_buffer[10];
		((int8_t *)&mpu_data.gyrX)[1] = mpu_rx_buffer[9];

		((int8_t *)&mpu_data.gyrY)[0] = mpu_rx_buffer[12];
		((int8_t *)&mpu_data.gyrY)[1] = mpu_rx_buffer[11];

		((int8_t *)&mpu_data.gyrZ)[0] = mpu_rx_buffer[14];
		((int8_t *)&mpu_data.gyrZ)[1] = mpu_rx_buffer[13];

		accVal.accX = mpu_data.accX * 0.002392578125f;
		accVal.accY = mpu_data.accY * 0.002392578125f;
		accVal.accZ = mpu_data.accZ * 0.002392578125f;
		temperature = (mpu_data.temp / 340.0f) + 36.53f;
		if(IsOffset) {
			gyrVal.gyrX = (mpu_data.gyrX - Offset->gyrX) * 0.06103515625f;
			gyrVal.gyrY = (mpu_data.gyrY - Offset->gyrY) * 0.06103515625f;
			gyrVal.gyrZ = (mpu_data.gyrZ - Offset->gyrZ) * 0.06103515625f;
		} else {
			gyrVal.gyrX = mpu_data.gyrX * 0.06103515625f;
			gyrVal.gyrY = mpu_data.gyrY * 0.06103515625f;
			gyrVal.gyrZ = mpu_data.gyrZ * 0.06103515625f;
		}
	}
	mpu_read_reg_dma(0x3B, 14, mpu_rx_buffer);
}

/* return the pointer of data buffer. */
AccDataDef *GetAccDataPointer(void) {return &accVal;}
GyrDataDef *GetGyrDataPointer(void) {return &gyrVal;}
float GetIMUTemperature(void) {return temperature;}
MPU_RAW *GetMPU_RawDataPointer(void) {return &mpu_data;}

/*
 * read & write any bytes through spi with dma.
 */
static void spi_rx_tx_dma(uint8_t *w, uint8_t *r, uint16_t l)
{
	if(_tx_comp_flag == 1) {
		_tx_comp_flag = 0;
		IMU_SPI_CS_ENABLE();
		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)r;
		DMA_InitStructure.DMA_BufferSize     = l;
		DMA_InitStructure.DMA_DIR            = DMA_DIR_PeripheralSRC;
		DMA_Init(IMU_SPI_Rx_DMA_Channel, &DMA_InitStructure);
		DMA_Cmd(IMU_SPI_Rx_DMA_Channel, ENABLE);

		DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)w;
		DMA_InitStructure.DMA_DIR            = DMA_DIR_PeripheralDST;
		DMA_Init(IMU_SPI_Tx_DMA_Channel, &DMA_InitStructure);
		DMA_Cmd(IMU_SPI_Tx_DMA_Channel, ENABLE);
	}
}

/*
 * write data to the specific register.
 */
static void mpu_write_reg_dma(uint8_t reg, uint8_t val)
{
	mpu_tx_buffer[0] = reg;
	mpu_tx_buffer[1] = val;
	spi_rx_tx_dma(mpu_tx_buffer, mpu_rx_buffer, 2);
	while (_tx_comp_flag == 0);
}

/*
 * read number of data from the specific register.
 */
static void mpu_read_reg_dma(uint8_t reg, uint8_t num, uint8_t *r)
{
	mpu_tx_buffer[0] = reg | 0x80;
	spi_rx_tx_dma(mpu_tx_buffer, r, num + 1);//ignore the first byte.
}

/*
 * read number of data from specific register until the operation completed.
 */
static void mpu_read_reg_dma_util(uint8_t reg, uint8_t num, uint8_t *r)
{
	mpu_tx_buffer[0] = reg | 0x80;
	spi_rx_tx_dma(mpu_tx_buffer, r, num + 1);//ignore the first byte.
	while (_tx_comp_flag == 0);
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
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_CRCPolynomial = 7;
  SPI_Init(IMU_SPI, &SPI_InitStructure);

	/* Enable IMU_SPI Rx&Tx request */
  SPI_I2S_DMACmd(IMU_SPI, SPI_I2S_DMAReq_Rx | SPI_I2S_DMAReq_Tx, ENABLE);

	/* Enable IMU_SPI */
	SPI_Cmd(IMU_SPI, ENABLE);
}

/**
  * @brief  Configures SPI DMA Peripheral.
  * @param  None
  * @retval None
  */
static void DMA_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* SPI_SLAVE_Rx_DMA_Channel and SPI_SLAVE_Tx_DMA_Channel configuration -------------*/
  DMA_DeInit(IMU_SPI_Rx_DMA_Channel);
	DMA_DeInit(IMU_SPI_Tx_DMA_Channel);
  DMA_InitStructure.DMA_PeripheralBaseAddr = IMU_SPI_DR_Base;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)mpu_rx_buffer;
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
  DMA_InitStructure.DMA_BufferSize = 15;
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  DMA_Init(IMU_SPI_Rx_DMA_Channel, &DMA_InitStructure);

	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)mpu_tx_buffer;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
	DMA_Init(IMU_SPI_Tx_DMA_Channel, &DMA_InitStructure);

	/* Enable the IMU_SPI_Rx_DMA_IRQn and IMU_SPI_Tx_DMA_IRQn Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = IMU_SPI_Rx_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannel = IMU_SPI_Tx_DMA_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	DMA_ITConfig(IMU_SPI_Rx_DMA_Channel, DMA_IT_TC, ENABLE);
	DMA_ITConfig(IMU_SPI_Tx_DMA_Channel, DMA_IT_TC, ENABLE);

	/* Enable DMA1 SPI Rx & Tx Channel */
//	DMA_Cmd(IMU_SPI_Rx_DMA_Channel, ENABLE);
//	DMA_Cmd(IMU_SPI_Tx_DMA_Channel, ENABLE);
}

/**
  * @brief  Configures the system peripheral clocks.
  * @param  None
  * @retval None
  */
static void RCC_Configuration(void)
{
  /* Enable SPI_MASTER clock and GPIO clock */
  RCC_APB2PeriphClockCmd(IMU_SPI_CLK | IMU_SPI_GPIO_CLK, ENABLE);
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
  GPIO_InitStructure.GPIO_Pin = IMU_SPI_PIN_SCK | IMU_SPI_PIN_MISO | IMU_SPI_PIN_MOSI;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(IMU_SPI_GPIO, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = IMU_SPI_PIN_NSS;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(IMU_SPI_GPIO, &GPIO_InitStructure);
	IMU_SPI_CS_DISABLE();
}

/*
 * this function handles IMU_SPI Rx DMA Interrupt.
 */
void IMU_SPI_Rx_DMA_IRQHandler(void)
{
	if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC2)) {
		DMA_ClearFlag(DMA1_FLAG_TC2);
		_tx_comp_flag = 1;
		IMU_SPI_CS_DISABLE();
		/* Disable DMA1 SPI Rx Channel */
		DMA_Cmd(IMU_SPI_Rx_DMA_Channel, DISABLE);
	}
}

/*
 * this function handles IMU_SPI Tx DMA Interrupt.
 */
void IMU_SPI_Tx_DMA_IRQHandler(void)
{
	if(SET == DMA_GetFlagStatus(DMA1_FLAG_TC3)) {
		DMA_ClearFlag(DMA1_FLAG_TC3);
		/* Disable DMA1 SPI Tx Channel */
		DMA_Cmd(IMU_SPI_Tx_DMA_Channel, DISABLE);
	}
}
