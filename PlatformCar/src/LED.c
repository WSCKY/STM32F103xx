#include "LED.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* LED_GPIO Periph clock enable */
	RCC_APB2PeriphClockCmd(LED_GPIO_PORT_CLK, ENABLE);

	/* Configure LED_PIN in output pushpull mode */
	GPIO_InitStructure.GPIO_Pin = LED_GPIO_PIN_1 | LED_GPIO_PIN_2 | LED_GPIO_PIN_3 | LED_GPIO_PIN_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LED_GPIO_PORT, &GPIO_InitStructure);

	GPIO_ResetBits(LED_GPIO_PORT, LED_GPIO_PIN_1 | LED_GPIO_PIN_2 | LED_GPIO_PIN_3 | LED_GPIO_PIN_4);
}
