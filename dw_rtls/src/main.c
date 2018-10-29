/**
  *****************************************************************************
  * @file    Project/src/main.c
  * @author  kyChu
  * @version V0.0.1
  * @date    20-March-2018
  * @brief   Main program body.
  *****************************************************************************
  */

/* Includes -----------------------------------------------------------------*/
#include "main.h"

/* Private typedef ----------------------------------------------------------*/
/* Private define -----------------------------------------------------------*/
#define LED_STATE_TIMER_RATE           10
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
static osTimerId LEDTimerHandle;
//static osTimerId DebugTimerHandle;

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

/* Private function prototypes ----------------------------------------------*/
static void SystemStartThread(void const *p);
static void MainControlSubThread(void const *p);
static void LEDStateTimerCallback(void const *p);
//static void DebugSendTimerCallback(void const *p);
//static void MainControlTimerCallback(void const *p);
/* Private functions --------------------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock / 1000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	osThreadDef(0, SystemStartThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	osThreadCreate(osThread(0), NULL);

	/* Start scheduler */
  osKernelStart();

	/* Infinite loop */
	for(;;);
}

static void SystemStartThread(void const *p)
{
	LED_Init();
	LOGPortInit();
	_TimeTicksInit();
	DW1000_If_Init();

	USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc,
            &USBD_CDC_cb,
            &USR_cb);

	osTimerDef(0, LEDStateTimerCallback);
	LEDTimerHandle = osTimerCreate(osTimer(0), osTimerPeriodic, NULL);
	osTimerStart(LEDTimerHandle, configTICK_RATE_HZ / LED_STATE_TIMER_RATE);

//	osTimerDef(1, DebugSendTimerCallback);
//	DebugTimerHandle = osTimerCreate(osTimer(1), osTimerPeriodic, NULL);
//	osTimerStart(DebugTimerHandle, configTICK_RATE_HZ / DEBUG_DATA_FRAME_RATE);

//	osTimerDef(2, MainControlTimerCallback);
//	DebugTimerHandle = osTimerCreate(osTimer(2), osTimerPeriodic, NULL);
//	osTimerStart(DebugTimerHandle, configTICK_RATE_HZ / MAIN_CONTROLLER_LOOP_RATE);

	osThreadDef(1, MainControlSubThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 8);
	osThreadCreate(osThread(1), NULL);

	vTaskDelete(NULL);
	for(;;);
}
uint8_t break_flag = 0;
static void LEDStateTimerCallback(void const *p)
{
	LED1_TOG();
	if(break_flag)
		LED2_TOG();
}

//static void DebugSendTimerCallback(void const *p)
//{
//#if (DEBUG_MODE)
////	SendDataToMonitor();
//#else

//#endif
//}

//static void MainControlTimerCallback(void const *p)
//{
////	SystemControlTask();
//}
uint32_t second = 0;
static void MainControlSubThread(void const *p)
{
//	instance_main();
	break_flag = 1;
	uint32_t PreviousWakeTime = osKernelSysTick();
	uint32_t DelayTime = 500;//configTICK_RATE_HZ / MAIN_CONTROLLER_LOOP_RATE;
	for(;;) {
		osDelayUntil(&PreviousWakeTime, DelayTime);
		second = _Get_Secnds();
//		USB_CDC_SendBuffer((uint8_t *)"Hello.\n", 8);
	}
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************************** END OF FILE ********************************/
