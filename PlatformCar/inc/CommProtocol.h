#ifndef __COMMPROTOCOL_H
#define __COMMPROTOCOL_H

#include "RemoteController.h"

#define STX1                 (0x55)
#define STX2                 (0xAA)

#define FILE_DATA_CACHE      (32) /* 16 * n */
#define PACKET_CACHE         (FILE_DATA_CACHE + 10)
#define PACKET_MAX_LENGTH    (PACKET_CACHE - 3)

__packed typedef enum {
	DECODE_STATE_UNSYNCED = 0,
	DECODE_STATE_GOT_STX1 = 1,
	DECODE_STATE_GOT_STX2 = 2,
	DECODE_STATE_GOT_LEN  = 3,
	DECODE_STATE_GOT_TYPE = 4,
	DECODE_STATE_GOT_DATA = 5
} DECODE_STATE;

__packed typedef enum {
	/* Heartbeat, see HeartBeat.txt */
	TYPE_COM_HEARTBEAT = 0x01,
	/* Device Status, see DeviceState.txt */
	TYPE_DevSta_Response = 0x11,
	/* Board Firmware Version, see VersionRequest.txt */
	TYPE_VERSION_REQUEST = 0x66,
	TYPE_VERSION_Response = 0x67,
} PACKET_TYPE;

__packed typedef struct {
	uint8_t _Cnt;
} HeartBeatDef;

__packed typedef struct {
	uint8_t CtrlReady;
	float Voltage;
} DevStateResponseDef;

__packed typedef struct {
	uint16_t v;
} VersionResponseDef;

__packed typedef union {
	uint8_t pData[PACKET_CACHE - 5];
	HeartBeatDef Heartbeat;
	DevStateResponseDef DevSta;
	VersionResponseDef Version;
} PacketDataUnion;

__packed typedef struct {
	uint8_t stx1;
	uint8_t stx2;
	uint8_t len;
	uint8_t type;
	PacketDataUnion PacketData;
	uint8_t crc;
} PackageStructDef;

__packed typedef union {
	PackageStructDef Packet;
	uint8_t RawData[PACKET_CACHE];
} CommPackageDef;

#define FC_ACK_PACKET_LEN    (sizeof(FCResponseDef) + 2)

uint8_t GotNewData(void);
CommPackageDef* GetRxPacket(void);
void SendTxPacket(CommPackageDef* pPacket);

#endif /* __COMMPROTOCOL */
