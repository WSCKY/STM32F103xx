#include "SensorGroup.h"

static uint8_t _init_flag = 0;
static CanTxMsg TxMessage = {0};
static SENSOR_GROUP_DATA SensorData[SENSOR_NUMBER] = {0};

static void CAN_MsgPrepare(void);

void CAN_RxMessageCallback(CanRxMsg* pRxMessage)
{
	uint8_t *p, i;

	if(pRxMessage->StdId >= SensorPair_1 && pRxMessage->StdId <= SensorPair_8) {
		p = &SensorData[pRxMessage->StdId - SensorPair_1]._tof_data.uData[0];
		for(i = 0; i < 8; i ++)
			p[i] = pRxMessage->Data[i];
	}
}

void SensorGroupSendTask(void)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		CAN_MsgPrepare();
	}
	CAN_TransmitData(&TxMessage);
}

SENSOR_GROUP_DATA *GetSensorGroupDataPointer(void) { return SensorData; }

/**
  * @brief  Transmit Structure preparation.
  * @param  pTxMessage: pointer to the Tx Message.
  * @retval None
  */
static void CAN_MsgPrepare(void)
{
	TxMessage.StdId = CAN_MASTER_ID;
	TxMessage.ExtId = 0x00;
	TxMessage.RTR = CAN_RTR_DATA;
	TxMessage.IDE = CAN_ID_STD;
	TxMessage.DLC = 8;

	TxMessage.Data[0] = 1;
	TxMessage.Data[1] = 2;
	TxMessage.Data[2] = 3;
	TxMessage.Data[3] = 4;
	TxMessage.Data[4] = 5;
	TxMessage.Data[5] = 6;
	TxMessage.Data[6] = 7;
	TxMessage.Data[7] = 8;
}
