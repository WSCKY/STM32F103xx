#include "DataMonitor.h"
#include "TOF.h"
static uint8_t _init_flag = 0;
static COM_DATA_PACK_DEF ComDataBuf = {0};

//static AccDataDef *pAcc;
//static GyrDataDef *pGyr;
//static EulerAngle *pEulerAngle;
static SENSOR_GROUP_DATA *pSensor;

//extern int16_t speedL, speedR;
//extern float SpeedFilted_L, SpeedFilted_R;

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
//float dist = 0;
void SendDataToMonitor(void)
{
	if(_init_flag == 0) {
		_init_flag = 1;

//		pAcc = GetAccDataPointer();
//		pGyr = GetGyrDataPointer();
//		pEulerAngle = GetAttitudeAngle();
		pSensor = GetSensorGroupDataPointer();

		InitDataPackage(&ComDataBuf.ComData);
		return;
	}

//	GetNewTOFData(&dist);

	ComDataBuf.ComData.data[0].f_data = pSensor[0]._ult_data.fData;//pAcc->accX;//SpeedFilted_L;//
	ComDataBuf.ComData.data[1].f_data = pSensor[1]._ult_data.fData;//pAcc->accY;//SpeedFilted_R;//
	ComDataBuf.ComData.data[2].f_data = pSensor[2]._ult_data.fData;//pAcc->accZ;//
	ComDataBuf.ComData.data[3].f_data = pSensor[3]._ult_data.fData;//pGyr->gyrX;//speedL;//
	ComDataBuf.ComData.data[4].f_data = pSensor[4]._ult_data.fData;//pGyr->gyrY;//speedR;//
	ComDataBuf.ComData.data[5].f_data = pSensor[5]._ult_data.fData;//pGyr->gyrZ;//
	ComDataBuf.ComData.data[6].f_data = pSensor[6]._ult_data.fData;//pEulerAngle->pitch;//
	ComDataBuf.ComData.data[7].f_data = pSensor[7]._ult_data.fData;//pEulerAngle->roll;//
	ComDataBuf.ComData.data[8].f_data = 0;//dist;//pEulerAngle->yaw;//
	DataPackageChkSum(&ComDataBuf.ComData);

	DebugPortSendBytesDMA(ComDataBuf.RawData, 40);
}
