#include "FreeControl.h"

static RF_DATA_t RF_Data = {0};
static uint8_t RFDataUpdate = 0;

static RF_CHANNLE_t RF_ChannelData = {0};

static void usart_config(void);
static void _rf_data_decode(uint8_t data);

void FreeControlInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RF_USART_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

	/* Configure RF_USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = RF_USART_RxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(RF_USART_GPIO, &GPIO_InitStructure);

	usart_config();
}

static void usart_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable RF_USART Clock */
  RCC_APB1PeriphClockCmd(RF_USART_CLK, ENABLE);

	/* RF USART configuration -------------------------------------------*/
  /* RF USART configured as follow:
        - BaudRate = 100000
        - Word Length = 8 Bits
        - Two Stop Bit
        - Even parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */

  USART_InitStructure.USART_BaudRate = 100000;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_2;
  USART_InitStructure.USART_Parity = USART_Parity_Even;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx;
  /* Configure RF_USART */
  USART_Init(RF_USART, &USART_InitStructure);

	/* Enable the RF_USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RF_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the RF_USART Receive Interrupt */
  USART_ITConfig(RF_USART, USART_IT_RXNE, ENABLE);

  /* Enable RF_USART */
  USART_Cmd(RF_USART, ENABLE);
}

uint8_t GetRFUpdateFlag(void)
{
	if(RFDataUpdate) {
		RFDataUpdate = 0;
		return 1;
	}
	return 0;
}

RF_CHANNLE_t *GetRF_ChannelData(void)
{
	return &RF_ChannelData;
}

void RF_ParseData(void)
{
	RF_ChannelData.Channel[0]  = ((RF_Data.rf_data.data8[0] | RF_Data.rf_data.data8[1] << 8) & 0x07FF);
	RF_ChannelData.Channel[1]  = ((RF_Data.rf_data.data8[1] >> 3 | RF_Data.rf_data.data8[2] << 5) & 0x07FF);
	RF_ChannelData.Channel[2]  = ((RF_Data.rf_data.data8[2] >> 6 | RF_Data.rf_data.data8[3] << 2 | RF_Data.rf_data.data8[4] << 10) & 0x07FF);
	RF_ChannelData.Channel[3]  = ((RF_Data.rf_data.data8[4] >> 1 | RF_Data.rf_data.data8[5] << 7) & 0x07FF);
	RF_ChannelData.Channel[4]  = ((RF_Data.rf_data.data8[5] >> 4 | RF_Data.rf_data.data8[6] << 4) & 0x07FF);
	RF_ChannelData.Channel[5]  = ((RF_Data.rf_data.data8[6] >> 7 | RF_Data.rf_data.data8[7] << 1 | RF_Data.rf_data.data8[8] << 9) & 0x07FF);
	RF_ChannelData.Channel[6]  = ((RF_Data.rf_data.data8[8] >> 2 | RF_Data.rf_data.data8[9] << 6) & 0x07FF);
	RF_ChannelData.Channel[7]  = ((RF_Data.rf_data.data8[9] >> 5 | RF_Data.rf_data.data8[10]<<3) & 0x07FF); // & the other 8 + 2 channels if you need them
	RF_ChannelData.Channel[8]  = ((RF_Data.rf_data.data8[11] | RF_Data.rf_data.data8[12] << 8) & 0x07FF);
	RF_ChannelData.Channel[9]  = ((RF_Data.rf_data.data8[12] >> 3 | RF_Data.rf_data.data8[13] << 5) & 0x07FF);
	RF_ChannelData.Channel[10] = ((RF_Data.rf_data.data8[13] >> 6 | RF_Data.rf_data.data8[14] << 2 | RF_Data.rf_data.data8[15] << 10) & 0x07FF);
	RF_ChannelData.Channel[11] = ((RF_Data.rf_data.data8[15] >> 1 | RF_Data.rf_data.data8[16] << 7) & 0x07FF);
	RF_ChannelData.Channel[12] = ((RF_Data.rf_data.data8[16] >> 4 | RF_Data.rf_data.data8[17] << 4) & 0x07FF);
	RF_ChannelData.Channel[13] = ((RF_Data.rf_data.data8[17] >> 7 | RF_Data.rf_data.data8[18] << 1 | RF_Data.rf_data.data8[19] << 9) & 0x07FF);
	RF_ChannelData.Channel[14] = ((RF_Data.rf_data.data8[19] >> 2 | RF_Data.rf_data.data8[20] << 6) & 0x07FF);
	RF_ChannelData.Channel[15] = ((RF_Data.rf_data.data8[20] >> 5 | RF_Data.rf_data.data8[21] << 3) & 0x07FF);
}

static uint8_t _data_step = 0;
static uint8_t _rx_data_cnt = 0;
static RF_DATA_t _rf_data = {0};
static void _rf_data_decode(uint8_t data)
{
	switch(_data_step) {
		case 0: {
			if(data == 0x0F) {
				_rf_data.startByte = data;
				_rx_data_cnt = 0;
				_data_step = 1;
			}
		} break;
		case 1: {
			_rf_data.rf_data.data8[_rx_data_cnt] = data;
			_rx_data_cnt ++;
			if(_rx_data_cnt >= 22)
				_data_step = 2;
		} break;
		case 2: {
			_rf_data.flags = data;
			_data_step = 3;
		} break;
		case 3: {
			_data_step = 0;
			_rf_data.endByte = data;
			if(data == 0x04 || data == 0x14 || data == 0x24 || data == 0x34) {
				if((_rf_data.flags & 0x0C) != 0x04 && (_rf_data.flags & 0x0C) != 0x0C) { //useless data.
					RF_Data = _rf_data;
					RFDataUpdate = 1;
				}
			}
		} break;
		default: _data_step = 0; break;
	}
}

void RF_USART_IRQHandler(void)
{
	if(SET == USART_GetITStatus(RF_USART, USART_IT_RXNE)) {
		_rf_data_decode(USART_ReceiveData(RF_USART));
		USART_ClearFlag(RF_USART, USART_FLAG_RXNE);
		USART_ClearITPendingBit(RF_USART, USART_IT_RXNE);
	}
}
