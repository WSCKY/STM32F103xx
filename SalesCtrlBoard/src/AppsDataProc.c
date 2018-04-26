#include "AppsDataProc.h"

static uint8_t _init_flag = 0;
static CommPackageDef *pComData;

static uint8_t BinSigStatus = 0;
static uint8_t VersionRequestFlag = 0;

void AppDataProcTask(uint8_t millis)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		pComData = GetRxPacket();
	}

	if(GotNewData()) {
		switch(pComData->Packet.type) {
			case TYPE_COM_HEARTBEAT:
				__nop();
			break;
			case TYPE_LED_CTRL_Request:
				BinSigStatus = pComData->Packet.PacketData.pData[0];
				if(BinSigStatus & 0x08) {
					DC12V_ENABLE();
				} else {
					DC12V_DISABLE();
				}
			break;
			case TYPE_BAR_MODE_Request:
				if(pComData->Packet.PacketData.pData[0] < 8)
					SetLED_BarMode(pComData->Packet.PacketData.pData[0]);
			break;
			case TYPE_DOOR_CTRL_Request:
				SetDoorState_A((DOOR_CTRL_CMD)(pComData->Packet.PacketData.pData[0] & 0x0F));
				SetDoorState_B((DOOR_CTRL_CMD)((pComData->Packet.PacketData.pData[0] >> 4) & 0x0F));
			break;
			case TYPE_VERSION_REQUEST:
				if(pComData->Packet.PacketData.pData[0] == 0x0F) {
					VersionRequestFlag = 1;
				}
			break;
			default: break;
		}
	}
}

static uint16_t _rsp_tick = 0;
static CommPackageDef TxPacket = {Header1, Header2};
void CtrlBoardRespTask(uint8_t millis)
{
	if(_rsp_tick % 10 == 0) {
		TxPacket.Packet.type = TYPE_COM_HEARTBEAT;
		TxPacket.Packet.len = 3;
		TxPacket.Packet.PacketData.Heartbeat._Cnt = 22;
		SendTxPacket(&TxPacket);
	} else if(_rsp_tick % 10 == 3) {
		TxPacket.Packet.type = TYPE_DevSta_Response;
		TxPacket.Packet.len = 10;
		TxPacket.Packet.PacketData.DevSta.CtrlReady = 0x01;
		TxPacket.Packet.PacketData.DevSta.Voltage = GetSystemVoltage();
		TxPacket.Packet.PacketData.DevSta.DoorsState = (GetDoorState_B() << 4) | GetDoorState_A(); /* Door Control Status */
		TxPacket.Packet.PacketData.DevSta.BinSigState = BinSigStatus;
		TxPacket.Packet.PacketData.DevSta.LEDBarState = GetLED_BarMode();
		SendTxPacket(&TxPacket);
	} else if(_rsp_tick % 10 == 8) {
		if(VersionRequestFlag == 1) {
			VersionRequestFlag = 0;
			TxPacket.Packet.type = TYPE_VERSION_Response;
			TxPacket.Packet.len = 4;
			TxPacket.Packet.PacketData.Version.v = 0x1000;
			SendTxPacket(&TxPacket);
		}
	}

	_rsp_tick ++;
	if(_rsp_tick >= 60000) _rsp_tick = 0;
}

static void SendTxPacket(CommPackageDef* pPacket)
{
	pPacket->Packet.crc = Get_CRC8_Check_Sum(&(pPacket->RawData[2]), pPacket->Packet.len, CRC_INIT);
	pPacket->Packet.PacketData.pData[pPacket->Packet.len - 2] = pPacket->Packet.crc;
	DebugPortSendBytesDMA(pPacket->RawData, pPacket->Packet.len + 3);
}
