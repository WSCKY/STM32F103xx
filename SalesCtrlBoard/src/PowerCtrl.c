#include "PowerCtrl.h"

void PowerCtrlInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* PWR_GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(PWR_GPIO_PORT_CLK, ENABLE);

	/* Configure PWR_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = PWR_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(PWR_GPIO_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(PWR_GPIO_PORT, PWR_GPIO_PIN); /* Default: Power Off */
}
