#ifndef __DATAMONITOR_H
#define __DATAMONITOR_H

#include "Monitor_If_conf.h"

#define MONITOR_DATA_LENGTH            (9)

__packed typedef struct
{
	uint8_t head;
	uint8_t cmd;
	uint8_t length;
	__packed union{
			uint8_t c_data[4];
			float f_data;	
	}data[MONITOR_DATA_LENGTH];
	uint8_t checksum;
} COMM_DATA;

__packed typedef union {
	COMM_DATA ComData;
	uint8_t RawData[MONITOR_DATA_LENGTH * 4 + 4];
} COM_DATA_PACK_DEF;

void SendDataToMonitor(void);
void MonitorUpdateDataPos(float f, uint8_t p);

#endif /* __DATAMONITOR_H */
