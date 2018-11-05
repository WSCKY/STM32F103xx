#include "HostRespTask.h"

static CommPackageDef TxPacket = {Header1, Header2};

extern SemaphoreHandle_t xSemaphoreUpdate;

static uint8_t _heart_beat = 0;

void HostRespTask(void)
{
  if(xSemaphoreTake(xSemaphoreUpdate, ( TickType_t )portTICK_PERIOD_MS * 100) != pdTRUE) {
		TxPacket.Packet.type = TYPE_COM_HEARTBEAT;
		TxPacket.Packet.len = 3;
		TxPacket.Packet.PacketData.Heartbeat._Cnt = _heart_beat ++;
		SendTxPacket(&TxPacket);
  } else {
    TxPacket.Packet.type = TYPE_DIST_Response;
    TxPacket.Packet.len = sizeof(DistDataRespDef) + 2;
    TxPacket.Packet.PacketData.DistData.InstType = TAG;
    TxPacket.Packet.PacketData.DistData.InstId = 0;
    TxPacket.Packet.PacketData.DistData.ListSize = MAX_ANCHOR_LIST_SIZE;
    for(int i = 0; i < MAX_ANCHOR_LIST_SIZE; i ++) {
      TxPacket.Packet.PacketData.DistData.Distance[i] = instancegetidist_mm(i);
    }
    SendTxPacket(&TxPacket);
    instancecleardisttableall();
  }
//	} else if(_rsp_tick % 10 == 8) {
//		if(VersionRequestFlag == 1) {
//			VersionRequestFlag = 0;
//			TxPacket.Packet.type = TYPE_VERSION_Response;
//			TxPacket.Packet.len = 4;
//			TxPacket.Packet.PacketData.Version.v = 0x1000;
//			SendTxPacket(&TxPacket);
//		}
//	}
}
