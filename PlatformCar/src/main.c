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
#define LED_STATE_TIMER_RATE           100
/* Private macro ------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
static osTimerId LEDTimerHandle;
static osTimerId DebugTimerHandle;

static uint16_t LEDTicksCnt = 0;
static uint16_t LEDFlashDelay = 0;
/* Private function prototypes ----------------------------------------------*/
static void SystemStartThread(void const *p);
static void MainControlSubThread(void const *p);
static void LEDStateTimerCallback(void const *p);
static void DebugSendTimerCallback(void const *p);
static void MainControlTimerCallback(void const *p);
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
//	ButtonInit();
	MPU6500_Init();
	TOFDriverInit();
	DebugPortInit();
	MotorDriverInit();
	EncoderDriverInit();
	RemoteControlInit();
	VoltageMeasureInit();

	CAN_If_Init();

	EstimatorInit();

	osTimerDef(0, LEDStateTimerCallback);
	LEDTimerHandle = osTimerCreate(osTimer(0), osTimerPeriodic, NULL);
	osTimerStart(LEDTimerHandle, configTICK_RATE_HZ / LED_STATE_TIMER_RATE);

	osTimerDef(1, DebugSendTimerCallback);
	DebugTimerHandle = osTimerCreate(osTimer(1), osTimerPeriodic, NULL);
	osTimerStart(DebugTimerHandle, configTICK_RATE_HZ / DEBUG_DATA_FRAME_RATE);

	osTimerDef(2, MainControlTimerCallback);
	DebugTimerHandle = osTimerCreate(osTimer(2), osTimerPeriodic, NULL);
	osTimerStart(DebugTimerHandle, configTICK_RATE_HZ / MAIN_CONTROLLER_LOOP_RATE);

	osThreadDef(1, MainControlSubThread, osPriorityNormal, 0, configMINIMAL_STACK_SIZE);
	osThreadCreate(osThread(1), NULL);

	vTaskDelete(NULL);
	for(;;);
}

static void LEDStateTimerCallback(void const *p)
{
	static uint8_t div = 0;
	if(!IMU_GotOffset()) {
		LEDFlashDelay = LED_STATE_TIMER_RATE / LED_FLASH_RATE_WAIT_STABLE;
	} else {// if(GetSignalLostFlag())
		LEDFlashDelay = LED_STATE_TIMER_RATE / LED_FLASH_RATE_SIGNAL_LOST;
	}// else {
//		LEDFlashDelay = 0;
//	}

	if(LEDFlashDelay != 0) {
		if(LEDTicksCnt % LEDFlashDelay == 0) {
			if(div == 0) { LED1_TOG(); div ++; }
			else if(div == 1) { LED2_TOG(); div ++; }
			else if(div == 2) { LED3_TOG(); div ++; }
			else if(div == 3) { LED4_TOG(); div = 0; }
		}
	} else {
		if(GetVehicleRunState()) {
			LED1_ON();
		} else {
			LED1_OFF();
		}
	}

	LEDTicksCnt ++;
	if(LEDTicksCnt >= 60000)
		LEDTicksCnt = 0;
}

static void DebugSendTimerCallback(void const *p)
{
	SendDataToMonitor();
	SensorGroupSendTask();
}

static void MainControlTimerCallback(void const *p)
{
	SystemControlTask();
}

static void MainControlSubThread(void const *p)
{
	uint32_t PreviousWakeTime = osKernelSysTick();
	uint32_t DelayTime = configTICK_RATE_HZ / MAIN_CONTROLLER_LOOP_RATE;
	for(;;) {
		osDelayUntil(&PreviousWakeTime, DelayTime);

		VoltageCheckLoop();
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
