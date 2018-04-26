#include "LightMode.h"

static uint32_t LeftBarColor[LED_COUNT] = {0};
static uint32_t RightBarColor[LED_COUNT] = {0};

void NormalColorLoop(uint8_t millis, LED_BAR_POS_DEF id, uint32_t ColorCode)
{
	uint32_t *p = LeftBarColor;
	if(id == LED_BAR_RIGHT) p = RightBarColor;

	for(uint8_t i = 0; i < LED_COUNT; i ++)
		p[i] = ColorCode;

	UpdateColorBarData(p, id);
}

ToggleModeParamDef ToggleLeft = {.id = LED_BAR_LEFT, .p = LeftBarColor, .div = 0, .dt = 200, .sta = 0};
ToggleModeParamDef ToggleRight = {.id = LED_BAR_RIGHT, .p = RightBarColor, .div = 0, .dt = 200, .sta = 0};

void ToggleModeLoop(uint8_t millis, LED_BAR_POS_DEF id, uint32_t ColorCode)
{
	ToggleModeParamDef *pToggle = &ToggleLeft;
	if(id == LED_BAR_RIGHT) pToggle = &ToggleRight;

	if(pToggle->div * millis < pToggle->dt) { pToggle->div ++; return; }
	pToggle->div = 0;

	if(pToggle->sta % 2 == 0) {
		for(uint8_t i = 0; i < LED_COUNT; i ++)
			pToggle->p[i] = ColorCode;
	} else {
		for(uint8_t i = 0; i < LED_COUNT; i ++)
			pToggle->p[i] = 0x0;
	}

	pToggle->sta ++;

	UpdateColorBarData(pToggle->p, id);
}

void BreathLoop(void)
{
	
}

FlowModeParamDef FlowLeft = {.id = LED_BAR_LEFT, .p = LeftBarColor, .div = 0, .pos = 0, .dt = 20};
FlowModeParamDef FlowRight = {.id = LED_BAR_RIGHT, .p = RightBarColor, .div = 0, .pos = 0, .dt = 20};

void FlowModeLoop(uint8_t millis, LED_BAR_POS_DEF id, uint32_t ColorCode)
{
	uint8_t R = ((ColorCode & 0x00FF0000) >> 16), G = ((ColorCode & 0x0000FF00) >> 8), B = ((ColorCode & 0x000000FF) >> 0);
	FlowModeParamDef *pFlow = &FlowLeft;
	if(id == LED_BAR_RIGHT) pFlow = &FlowRight;

	if(pFlow->div * millis < pFlow->dt) { pFlow->div ++; return; }
	pFlow->div = 0;

	uint32_t* pf[13] = {0};

	for(uint8_t i = 0; i < 13; i ++) {
		pf[i] = (uint32_t *)CIRCLE_LIMIT((uint32_t)(pFlow->p + pFlow->pos + i), (uint32_t)pFlow->p, (uint32_t)(pFlow->p + LED_COUNT));
	}

//	pf[0] = (uint32_t *)CIRCLE_LIMIT((uint32_t)(pFlow->p + pFlow->pos - 4), (uint32_t)pFlow->p, (uint32_t)(pFlow->p + LED_COUNT));
//	pf[1] = (uint32_t *)CIRCLE_LIMIT((uint32_t)(pFlow->p + pFlow->pos - 3), (uint32_t)pFlow->p, (uint32_t)(pFlow->p + LED_COUNT));
//	pf[2] = (uint32_t *)CIRCLE_LIMIT((uint32_t)(pFlow->p + pFlow->pos - 2), (uint32_t)pFlow->p, (uint32_t)(pFlow->p + LED_COUNT));
//	pf[3] = (uint32_t *)CIRCLE_LIMIT((uint32_t)(pFlow->p + pFlow->pos - 1), (uint32_t)pFlow->p, (uint32_t)(pFlow->p + LED_COUNT));
//	pf[4] = (uint32_t *)CIRCLE_LIMIT((uint32_t)(pFlow->p + pFlow->pos + 0), (uint32_t)pFlow->p, (uint32_t)(pFlow->p + LED_COUNT));
//	pf[5] = (uint32_t *)CIRCLE_LIMIT((uint32_t)(pFlow->p + pFlow->pos + 1), (uint32_t)pFlow->p, (uint32_t)(pFlow->p + LED_COUNT));
//	pf[6] = (uint32_t *)CIRCLE_LIMIT((uint32_t)(pFlow->p + pFlow->pos + 2), (uint32_t)pFlow->p, (uint32_t)(pFlow->p + LED_COUNT));
//	pf[7] = (uint32_t *)CIRCLE_LIMIT((uint32_t)(pFlow->p + pFlow->pos + 3), (uint32_t)pFlow->p, (uint32_t)(pFlow->p + LED_COUNT));
//	pf[8] = (uint32_t *)CIRCLE_LIMIT((uint32_t)(pFlow->p + pFlow->pos + 4), (uint32_t)pFlow->p, (uint32_t)(pFlow->p + LED_COUNT));

	*pf[0] = 0x00;
	*pf[1] = ((R / 10) << 16) | ((G / 10) << 8) | ((B / 10) << 0);
	*pf[2] = ((R * 2 / 5) << 16) | ((G * 2 / 5) << 8) | ((B * 2 / 5) << 0);
	*pf[3] = ((R * 7 / 10) << 16) | ((G * 7 / 10) << 8) | ((B * 7 / 10) << 0);
	*pf[4] = ((R * 9 / 10) << 16) | ((G * 9 / 10) << 8) | ((B * 9 / 10) << 0);
	*pf[5] = ColorCode;
	*pf[6] = ColorCode;
	*pf[7] = ColorCode;
	*pf[8] = ((R * 9 / 10) << 16) | ((G * 9 / 10) << 8) | ((B * 9 / 10) << 0);
	*pf[9] = ((R * 7 / 10) << 16) | ((G * 7 / 10) << 8) | ((B * 7 / 10) << 0);
	*pf[10] = ((R * 2 / 5) << 16) | ((G * 2 / 5) << 8) | ((B * 2 / 5) << 0);
	*pf[11] = ((R / 10) << 16) | ((G / 10) << 8) | ((B / 10) << 0);
	*pf[12] = 0x00;

//	*pf[0] = 0x00;
//	*pf[1] = ((R / 5) << 16) | ((G / 5) << 8) | ((B / 5) << 0);
//	*pf[2] = ((R * 3 / 5) << 16) | ((G * 3 / 5) << 8) | ((B * 3 / 5) << 0);
//	*pf[3] = ((R * 9 / 10) << 16) | ((G * 9 / 10) << 8) | ((B * 9 / 10) << 0);
//	*pf[4] = ColorCode;
//	*pf[5] = ((R * 9 / 10) << 16) | ((G * 9 / 10) << 8) | ((B * 9 / 10) << 0);
//	*pf[6] = ((R * 3 / 5) << 16) | ((G * 3 / 5) << 8) | ((B * 3 / 5) << 0);
//	*pf[7] = ((R / 5) << 16) | ((G / 5) << 8) | ((B / 5) << 0);
//	*pf[8] = 0x00;

	pFlow->pos --; if(pFlow->pos >= LED_COUNT) pFlow->pos = LED_COUNT - 1;
//	pFlow->pos ++; if(pFlow->pos >= LED_COUNT) pFlow->pos = 0;

	UpdateColorBarData(pFlow->p, id);
}
