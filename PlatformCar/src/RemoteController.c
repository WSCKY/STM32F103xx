#include "RemoteController.h"

static uint8_t RCDataUpdate = 0;

static void usart_config(void);
static void _rc_data_decode(uint8_t data);

void RemoteControlInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clock */
  RCC_APB2PeriphClockCmd(RC_USART_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

	/* Configure RC_USART Rx as input floating */
  GPIO_InitStructure.GPIO_Pin = RC_USART_RxPin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_Init(RC_USART_GPIO, &GPIO_InitStructure);

	/* Configure RC_USART Tx as alternate function push-pull */
  GPIO_InitStructure.GPIO_Pin = RC_USART_TxPin;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(RC_USART_GPIO, &GPIO_InitStructure);

	usart_config();
}

static void usart_config(void)
{
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable RC_USART Clock */
  RCC_APB1PeriphClockCmd(RC_USART_CLK, ENABLE);

	/* RC USART configuration -------------------------------------------*/
  /* RC USART configured as follow:
        - BaudRate = 100000
        - Word Length = 8 Bits
        - Two Stop Bit
        - Even parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */

  USART_InitStructure.USART_BaudRate = 115200;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  /* Configure RC_USART */
  USART_Init(RC_USART, &USART_InitStructure);

	/* Enable the RC_USART Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = RC_USART_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

  /* Enable the RC_USART Receive Interrupt */
  USART_ITConfig(RC_USART, USART_IT_RXNE, ENABLE);

  /* Enable RC_USART */
  USART_Cmd(RC_USART, ENABLE);
}

uint8_t GetRCUpdateFlag(void)
{
	if(RCDataUpdate) {
		RCDataUpdate = 0;
		return 1;
	}
	return 0;
}

void RC_USART_IRQHandler(void)
{
	if(SET == USART_GetITStatus(RC_USART, USART_IT_RXNE)) {
		_rc_data_decode(USART_ReceiveData(RC_USART));
		USART_ClearFlag(RC_USART, USART_FLAG_RXNE);
		USART_ClearITPendingBit(RC_USART, USART_IT_RXNE);
	}
}
