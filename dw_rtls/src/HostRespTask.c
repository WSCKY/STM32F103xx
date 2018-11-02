#include "HostRespTask.h"

static uint16_t _rsp_tick = 0;
static CommPackageDef TxPacket = {Header1, Header2};

static uint8_t _heart_beat = 0;

void HostRespTask(uint8_t millis)
{
	if(_rsp_tick % 10 == 0) {
		TxPacket.Packet.type = TYPE_COM_HEARTBEAT;
		TxPacket.Packet.len = 3;
		TxPacket.Packet.PacketData.Heartbeat._Cnt = _heart_beat ++;
		SendTxPacket(&TxPacket);
	} else if(_rsp_tick % 10 == 3) {
//		TxPacket.Packet.type = TYPE_DevSta_Response;
//		TxPacket.Packet.len = 10;
//		TxPacket.Packet.PacketData.DevSta.CtrlReady = 0x01;
//		TxPacket.Packet.PacketData.DevSta.Voltage = GetSystemVoltage();
//		TxPacket.Packet.PacketData.DevSta.DoorsState = (GetDoorState_B() << 4) | GetDoorState_A(); /* Door Control Status */
//		TxPacket.Packet.PacketData.DevSta.BinSigState = BinSigStatus;
//		TxPacket.Packet.PacketData.DevSta.LEDBarState = GetLED_BarMode();
//		SendTxPacket(&TxPacket);
	} else if(_rsp_tick % 10 == 8) {
//		if(VersionRequestFlag == 1) {
//			VersionRequestFlag = 0;
//			TxPacket.Packet.type = TYPE_VERSION_Response;
//			TxPacket.Packet.len = 4;
//			TxPacket.Packet.PacketData.Version.v = 0x1000;
//			SendTxPacket(&TxPacket);
//		}
	}
	_rsp_tick ++;
	if(_rsp_tick >= 60000) _rsp_tick = 0;
}
