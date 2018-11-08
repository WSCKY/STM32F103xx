#ifndef __MAINCTRL_H
#define __MAINCTRL_H

#include "stm32f10x.h"
#include "SysConfig.h"

#define ABS(x) ((x) >= 0 ? (x) : -(x))

void SystemControlTask(void);

#endif /* __MAINCTRL_H */
