#include "HostRespTask.h"

//static uint16_t _rsp_tick = 0;
static CommPackageDef TxPacket = {Header1, Header2};

extern xQueueHandle xQueueUpdate;

static uint8_t _heart_beat = 0;
static DistDataRespDef *pDist;
//static uint8_t _dist_update = 0;
//static uint32_t _dist_value = 0;

void HostRespTask(void)
{
  if(xQueueReceive(xQueueUpdate, &(pDist), ( TickType_t )portTICK_PERIOD_MS * 100) != pdTRUE) {
//	if(_rsp_tick % 10 == 0) {
		TxPacket.Packet.type = TYPE_COM_HEARTBEAT;
		TxPacket.Packet.len = 3;
		TxPacket.Packet.PacketData.Heartbeat._Cnt = _heart_beat ++;
		SendTxPacket(&TxPacket);
  } else {
    
  }
//	} else if(_rsp_tick % 10 == 3) {
//    if(_dist_update == 1) {
//      TxPacket.Packet.type = TYPE_DIST_Response;
//      TxPacket.Packet.len = 8;
//      TxPacket.Packet.PacketData.DistData.InstType = TAG;
//      TxPacket.Packet.PacketData.DistData.InstId = 0;
//      TxPacket.Packet.PacketData.DistData.Distance = _dist_value;
//      SendTxPacket(&TxPacket);
//    }
//    _dist_update = 0;
//	} else if(_rsp_tick % 10 == 8) {
//		if(VersionRequestFlag == 1) {
//			VersionRequestFlag = 0;
//			TxPacket.Packet.type = TYPE_VERSION_Response;
//			TxPacket.Packet.len = 4;
//			TxPacket.Packet.PacketData.Version.v = 0x1000;
//			SendTxPacket(&TxPacket);
//		}
//	}
//	_rsp_tick ++;
//	if(_rsp_tick >= 60000) _rsp_tick = 0;
}

//void update_dist(int dist)
//{
//  _dist_update = 1;
//  _dist_value = dist;
//}
