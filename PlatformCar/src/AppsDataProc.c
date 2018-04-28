#include "AppsDataProc.h"

static uint8_t _init_flag = 0;
static CommPackageDef *pComData;

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
static CommPackageDef TxPacket = {STX1, STX2};
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
