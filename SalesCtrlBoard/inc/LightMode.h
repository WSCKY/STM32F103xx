#ifndef __LIGHTMODE_H
#define __LIGHTMODE_H

#include "Maths.h"
#include "WS281x.h"

typedef enum {
	CloseMode = 0,
	FlowMode = 1,
} LightModeDef;

typedef struct {
	LED_BAR_POS_DEF id;
	uint32_t *p;
	uint8_t div;
	uint8_t pos;
	uint32_t dt;
} FlowModeParamDef;

typedef struct {
	LED_BAR_POS_DEF id;
	uint32_t *p;
	uint8_t div;
	uint32_t dt;
	uint8_t sta;
} ToggleModeParamDef;

#define CIRCLE_LIMIT(x, min, max)                (((x) < (min)) ? ((max) - (min) + (x)) : (((x) > (max)) ? ((min) + (x) - (max)) : (x)))

void NormalColorLoop(uint8_t millis, LED_BAR_POS_DEF id, uint32_t ColorCode);
void FlowModeLoop(uint8_t millis, LED_BAR_POS_DEF id, uint32_t ColorCode);
void ToggleModeLoop(uint8_t millis, LED_BAR_POS_DEF id, uint32_t ColorCode);

#endif /* __LIGHTMODE_H */
