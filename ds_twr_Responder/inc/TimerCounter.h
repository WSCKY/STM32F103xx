#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x_tim.h"

void _TimeTicksInit(void);

uint16_t _Get_Ticks(void);
uint32_t _Get_Micros(void);
uint32_t _Get_Millis(void);
uint32_t _Get_Secnds(void);

void _delay_us(uint32_t us);
void _delay_ms(uint32_t ms);

#endif /* __TIMER_H */
