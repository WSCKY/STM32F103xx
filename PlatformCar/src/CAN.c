#include "CAN.h"

static CanRxMsg _rx_msg = {0};

void CAN_If_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  CAN_InitTypeDef  CAN_InitStructure;
  CAN_FilterInitTypeDef CAN_FilterInitStructure;

  /* CAN GPIOs configuration **************************************************/
	/* CANx Periph clock enable */
	RCC_APB1PeriphClockCmd(CAN_CLK, ENABLE);
	RCC_APB2PeriphClockCmd(CAN_GPIO_CLK | RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_Remap1_CAN1 , ENABLE);

	/* Configure CAN pin: RX */
  GPIO_InitStructure.GPIO_Pin = CAN_RX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

  /* Configure CAN pin: TX */
  GPIO_InitStructure.GPIO_Pin = CAN_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(CAN_GPIO_PORT, &GPIO_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

	/* CAN configuration ********************************************************/  
	/* CAN register init */
	CAN_DeInit(CANx);
	CAN_StructInit(&CAN_InitStructure);

	/* CAN cell init */
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	/* CAN Baudrate = 500KBps (CAN clocked at 36 MHz) */
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1_2tq;
	CAN_InitStructure.CAN_BS2 = CAN_BS2_3tq;
	CAN_InitStructure.CAN_Prescaler = 12;
	CAN_Init(CANx, &CAN_InitStructure);

	/* CAN filter init "FIFO0" */
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	CAN_FilterInit(&CAN_FilterInitStructure);

	/* Enable FIFO 0 message pending Interrupt */
	CAN_ITConfig(CANx, CAN_IT_FMP0, ENABLE);
}

void CAN_TransmitData(CanTxMsg* pTxMessage)
{
	CAN_Transmit(CANx, pTxMessage);
}

/**
	* @brief  This function handles CAN1 Handler.
	* @param  None
	* @retval None
	*/
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	if (CAN_MessagePending(CANx, CAN_FIFO0) != 0) 
  {
    CAN_Receive(CANx, CAN_FIFO0, &_rx_msg);
		CAN_RxMessageCallback(&_rx_msg);
  }
}
