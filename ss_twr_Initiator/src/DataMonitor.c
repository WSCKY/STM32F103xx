#include "DataMonitor.h"

static uint8_t _init_flag = 0;
static COM_DATA_PACK_DEF ComDataBuf = {0};

void InitDataPackage(COMM_DATA *pComData)
{
	pComData->head = 0x55;
	pComData->cmd = 0xDD;
	pComData->length = 36;
	pComData->checksum = 0;
}

void DataPackageChkSum(COMM_DATA *pComData)
{
	uint8_t chk = 0, i = 0;
	for(; i < 36; i ++)
		chk += pComData->data[i / 4].c_data[i % 4];
	pComData->checksum = chk;
}

void SendDataToMonitor(void)
{
	if(_init_flag == 0) {
		_init_flag = 1;

		InitDataPackage(&ComDataBuf.ComData);
		return;
	}

	ComDataBuf.ComData.data[0].f_data = 0;
	ComDataBuf.ComData.data[1].f_data = 1;
	ComDataBuf.ComData.data[2].f_data = 2;
	ComDataBuf.ComData.data[3].f_data = 3;
	ComDataBuf.ComData.data[4].f_data = 4;
	ComDataBuf.ComData.data[5].f_data = 5;
	ComDataBuf.ComData.data[6].f_data = 6;
	ComDataBuf.ComData.data[7].f_data = 7;
	ComDataBuf.ComData.data[8].f_data = 8;
	DataPackageChkSum(&ComDataBuf.ComData);

	DebugPortSendBytesDMA(ComDataBuf.RawData, 40);
}
