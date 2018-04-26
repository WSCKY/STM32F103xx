#include "Button.h"

/*
 * Configure Button GPIO periph.
 */
void ButtonInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* PWR_CHK_GPIO, FWD_x_BTN_GPIO, BWD_x_BTN_GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(PWR_CHK_GPIO_PORT_CLK | \
													FWD_A_BTN_GPIO_PORT_CLK | BWD_A_BTN_GPIO_PORT_CLK | \
													FWD_B_BTN_GPIO_PORT_CLK | BWD_B_BTN_GPIO_PORT_CLK, ENABLE);

	/* Configure PWR_CHK_GPIO_PIN as input pull up */
	GPIO_InitStructure.GPIO_Pin = PWR_CHK_GPIO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(PWR_CHK_GPIO_PORT, &GPIO_InitStructure);

	/* Configure FWD_BTN_PIN, BWD_BTN_PIN as Input pull-up */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = FWD_A_BTN_GPIO_PIN;
	GPIO_Init(FWD_A_BTN_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BWD_A_BTN_GPIO_PIN;
	GPIO_Init(BWD_A_BTN_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = FWD_B_BTN_GPIO_PIN;
	GPIO_Init(FWD_B_BTN_GPIO_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = BWD_B_BTN_GPIO_PIN;
	GPIO_Init(BWD_B_BTN_GPIO_PORT, &GPIO_InitStructure);
}
