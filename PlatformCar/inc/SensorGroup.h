#ifndef __SENSORGROUP_H
#define __SENSORGROUP_H

#include "CAN.h"
#include "Maths.h"

#define SENSOR_NUMBER                  (8)
#define CAN_MASTER_ID                  (0x5A0 | 0x1F)

/*
                SENS ASM Model

             ---- (S1) ----
					  /      /\      \
          (S5)     ||     (S6)
				  /   \   /  \   /   \
          |    \  |  |  /    |
					|     \ |  | /     |
          |      \|  |/      |
				 (S2)     +--+      (S3)
          |      /|  |\      |
          |     / |  | \     |
				  |    /  /  \  \    |
					\   /  /    \  \   /
					(S7)   |    |   (S8)
					  \    |    |    /
						 ---- (S4) ----
*/

typedef enum {
	SensorPair_1 = 0x5A0,
	SensorPair_2 = 0x5A1,
	SensorPair_3 = 0x5A2,
	SensorPair_4 = 0x5A3,
	SensorPair_5 = 0x5A4,
	SensorPair_6 = 0x5A5,
	SensorPair_7 = 0x5A6,
	SensorPair_8 = 0x5A7,
} SensorPosDef;

typedef struct {
	_FLOAT_UNION _tof_data;
	_FLOAT_UNION _ult_data;
} SENSOR_GROUP_DATA;

void SensorGroupSendTask(void);
SENSOR_GROUP_DATA *GetSensorGroupDataPointer(void);

#endif /* __SENSORGROUP_H */
