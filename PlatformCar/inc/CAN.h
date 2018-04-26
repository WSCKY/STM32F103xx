/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __CAN_H
#define __CAN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_can.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define CANx                       CAN1
#define CAN_CLK                    RCC_APB1Periph_CAN1

#define CAN_GPIO_PORT              GPIOB
#define CAN_RX_PIN                 GPIO_Pin_8
#define CAN_TX_PIN                 GPIO_Pin_9
#define CAN_GPIO_CLK               RCC_APB2Periph_GPIOB

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void CAN_If_Init(void);
void CAN_TransmitData(CanTxMsg* pTxMessage);
void CAN_RxMessageCallback(CanRxMsg* pRxMessage);

#endif /* __CAN_H */
