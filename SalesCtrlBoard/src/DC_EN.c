#include "DC_EN.h"

void DC_EN_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* DC_EN_GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(EN_12V_GPIO_PORT_CLK | EN_20V1_GPIO_PORT_CLK | EN_20V2_GPIO_PORT_CLK, ENABLE);

	/* Configure DC_EN_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = EN_12V_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(EN_12V_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = EN_20V1_GPIO_PIN;
	GPIO_Init(EN_20V1_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = EN_20V2_GPIO_PIN;
	GPIO_Init(EN_20V2_GPIO_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(EN_20V1_GPIO_PORT, EN_20V1_GPIO_PIN); /* Default: DC Disable */
	GPIO_ResetBits(EN_20V2_GPIO_PORT, EN_20V2_GPIO_PIN); /* Default: DC Disable */
	GPIO_ResetBits(EN_12V_GPIO_PORT, EN_12V_GPIO_PIN); /* Default: DC Disable */
}
