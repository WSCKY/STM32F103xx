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
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
static osTimerId TaskTimerHandle;
static osTimerId StateTimerHandle;

xQueueHandle xQueueUpdate;

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

/* Private function prototypes ----------------------------------------------*/
static void SystemStartThread(void const *p);
static void MainThread(void const *p);
static void DataSendThread(void const *p);
static void TaskTimerCallback(void const *p);
static void StateTimerCallback(void const *p);
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

	osTimerDef(0, TaskTimerCallback);
	TaskTimerHandle = osTimerCreate(osTimer(0), osTimerPeriodic, NULL);
	osTimerStart(TaskTimerHandle, configTICK_RATE_HZ / TASK_TIMER_RATE);

  osTimerDef(1, StateTimerCallback);
  StateTimerHandle = osTimerCreate(osTimer(1), osTimerPeriodic, NULL);
  osTimerStart(StateTimerHandle, configTICK_RATE_HZ / STATE_TIMER_RATE);

	osThreadDef(0, MainThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE * 8);
	osThreadCreate(osThread(0), NULL);

  osThreadDef(1, DataSendThread, osPriorityAboveNormal, 0, configMINIMAL_STACK_SIZE * 2);
	osThreadCreate(osThread(1), NULL);

	vTaskDelete(NULL);
	for(;;);
}

static void TaskTimerCallback(void const *p)
{

}

uint8_t break_flag = 0;
uint8_t tim_div = 0;
static void StateTimerCallback(void const *p)
{
  if(tim_div % 2 == 0) {
    LED1_TOG();
    if(break_flag)
      LED2_TOG();
  }
  tim_div ++;
}

static void MainThread(void const *p)
{
	instance_main();
	break_flag = 1;
	uint32_t PreviousWakeTime = osKernelSysTick();
	uint32_t DelayTime = configTICK_RATE_HZ / MAIN_CONTROLLER_LOOP_RATE;
	for(;;) {
		osDelayUntil(&PreviousWakeTime, DelayTime);
	}
}

static void DataSendThread(void const *p)
{
  xQueueUpdate = xQueueCreate(5, sizeof(DistDataRespDef *));
  for(;;) {
    HostRespTask();
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
