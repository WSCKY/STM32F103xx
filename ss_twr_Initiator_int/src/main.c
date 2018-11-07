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
__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;

/* Private function prototypes ----------------------------------------------*/
//static void DataSendThread(void const *p);
/* Private functions --------------------------------------------------------*/

/*****************************************************************************/
//-----------------dw1000----------------------------

static dwt_config_t config = {
    5,                /* Channel number. */
    DWT_PRF_64M,      /* Pulse repetition frequency. */
    DWT_PLEN_128,     /* Preamble length. Used in TX only. */
    DWT_PAC8,         /* Preamble acquisition chunk size. Used in RX only. */
    10,               /* TX preamble code. Used in TX only. */
    10,               /* RX preamble code. Used in RX only. */
    0,                /* 0 to use standard SFD, 1 to use non-standard SFD. */
    DWT_BR_6M8,       /* Data rate. */
    DWT_PHRMODE_STD,  /* PHY header mode. */
    (129 + 8 - 8)     /* SFD timeout (preamble length + 1 + SFD length - PAC size). Used in RX only. */
};

/* Preamble timeout, in multiple of PAC size. See NOTE 3 below. */
#define PRE_TIMEOUT 1000

/* Delay between frames, in UWB microseconds. See NOTE 1 below. */
#define POLL_TX_TO_RESP_RX_DLY_UUS 100 

/*Should be accurately calculated during calibration*/
#define TX_ANT_DLY 16300
#define RX_ANT_DLY 16456	

//--------------dw1000---end---------------


#define TASK_DELAY        200           /**< Task delay. Delays a LED0 task for 200 ms */
#define TIMER_PERIOD      2000          /**< Timer period. LED1 timer will expire after 1000 ms */

#ifdef USE_FREERTOS

TaskHandle_t  ss_initiator_task_handle;   /**< Reference to SS TWR Initiator FreeRTOS task. */
extern void ss_initiator_task_function (void * pvParameter);
TaskHandle_t  led_toggle_task_handle;   /**< Reference to LED0 toggling FreeRTOS task. */
TaskHandle_t  send_data_task_handle;    /**< Reference to data sending FreeRTOS task. */
TimerHandle_t led_toggle_timer_handle;  /**< Reference to LED1 toggling FreeRTOS timer. */
#endif

#ifdef USE_FREERTOS

/**@brief LED0 task entry function.
 *
 * @param[in] pvParameter   Pointer that will be used as the parameter for the task.
 */
static void led_toggle_task_function (void * pvParameter)
{
  UNUSED_PARAMETER(pvParameter);
  while (true)
  {
    LED1_TOG();
    /* Delay a task for a given number of ticks */
    vTaskDelay(TASK_DELAY);
    /* Tasks must be implemented to never return... */
  }
}

static void send_data_task_function (void * pvParameter)
{
  UNUSED_PARAMETER(pvParameter);
  while (true)
  {
    /* send data to upper monitor. */
    SendDataToMonitor();
    /* Delay a task for a given number of ticks */
    vTaskDelay(10);
    /* Tasks must be implemented to never return... */
  }
}

/**@brief The function to call when the LED1 FreeRTOS timer expires.
 *
 * @param[in] pvParameter   Pointer that will be used as the parameter for the timer.
 */
static void led_toggle_timer_callback (void * pvParameter)
{
  UNUSED_PARAMETER(pvParameter);
  LED2_TOG();
}
#else

  extern int ss_init_run(void);

#endif   // #ifdef USE_FREERTOS

/*****************************************************************************/

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

	LED_Init();
  /*Initialization UART*/
	LOGPortInit();
	_TimeTicksInit();
	DW1000_If_Init();

	USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID,
            &USR_desc,
            &USBD_CDC_cb,
            &USR_cb);

  #ifdef USE_FREERTOS
    /* Create task for LED0 blinking with priority set to 1 */
    UNUSED_VARIABLE(xTaskCreate(led_toggle_task_function, "LED0", configMINIMAL_STACK_SIZE + 200, NULL, 1, &led_toggle_task_handle));

    /* Create task for send data to monitor with priority set to 2 */
    UNUSED_VARIABLE(xTaskCreate(send_data_task_function, "Monitor", configMINIMAL_STACK_SIZE + 200, NULL, 2, &send_data_task_handle));
    
    /* Start timer for LED1 blinking */
    led_toggle_timer_handle = xTimerCreate( "LED1", TIMER_PERIOD, pdTRUE, NULL, led_toggle_timer_callback);
    UNUSED_VARIABLE(xTimerStart(led_toggle_timer_handle, 0));

    /* Create task for SS TWR Initiator set to 2 */
    UNUSED_VARIABLE(xTaskCreate(ss_initiator_task_function, "SSTWR_INIT", configMINIMAL_STACK_SIZE + 200, NULL, 2, &ss_initiator_task_handle));
  #endif // #ifdef USE_FREERTOS

  //-------------dw1000  ini------------------------------------	

  /* Setup DW1000 IRQ pin */  
  DW1000RSTn_NOP_IN();

  DW1000_SetIRQnHandler(dwt_isr);

  printf("  Singled Sided Two Way Ranging Initiator Example \r\n");
  
  /* Reset DW1000 */
  reset_DW1000(); 

  /* Set SPI clock to 2MHz */
  port_set_dw1000_slowrate();
  
  /* Init the DW1000 */
  if (dwt_initialise(DWT_LOADUCODE) == DWT_ERROR)
  {
    //Init of DW1000 Failed
    while (1) {};
  }

  // Set SPI to 8MHz clock
  port_set_dw1000_fastrate();

  /* Configure DW1000. */
  dwt_configure(&config);

  /* Apply default antenna delay value. See NOTE 2 below. */
  dwt_setrxantennadelay(RX_ANT_DLY);
  dwt_settxantennadelay(TX_ANT_DLY);

  /* Set preamble timeout for expected frames. See NOTE 3 below. */
  //dwt_setpreambledetecttimeout(0); // PRE_TIMEOUT
          
  /* Set expected response's delay and timeout. 
  * As this example only handles one incoming frame with always the same delay and timeout, those values can be set here once for all. */
  dwt_setrxaftertxdelay(POLL_TX_TO_RESP_RX_DLY_UUS);
  dwt_setrxtimeout(65000); // Maximum value timeout with DW1000 is 65ms  

  //-------------dw1000  ini------end---------------------------	
  // IF WE GET HERE THEN THE LEDS WILL BLINK

  #ifdef USE_FREERTOS		
    /* Start FreeRTOS scheduler. */
    vTaskStartScheduler();	

    while(1) 
    {};
  #else

    // No RTOS task here so just call the main loop here.
    // Loop forever responding to ranging requests.
    while (1)
    {
      ss_init_run();
    }

  #endif
}

//static void DataSendThread(void const *p)
//{
//  for(;;) {
//    HostRespTask();
//  }
//}

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

/*****************************************************************************************************************************************************
 * NOTES:
 *
 * 1. The single-sided two-way ranging scheme implemented here has to be considered carefully as the accuracy of the distance measured is highly
 *    sensitive to the clock offset error between the devices and the length of the response delay between frames. To achieve the best possible
 *    accuracy, this response delay must be kept as low as possible. In order to do so, 6.8 Mbps data rate is used in this example and the response
 *    delay between frames is defined as low as possible. The user is referred to User Manual for more details about the single-sided two-way ranging
 *    process.  NB:SEE ALSO NOTE 11.
 * 2. The sum of the values is the TX to RX antenna delay, this should be experimentally determined by a calibration process. Here we use a hard coded
 *    value (expected to be a little low so a positive error will be seen on the resultant distance estimate. For a real production application, each
 *    device should have its own antenna delay properly calibrated to get good precision when performing range measurements.
 * 3. This timeout is for complete reception of a frame, i.e. timeout duration must take into account the length of the expected frame. Here the value
 *    is arbitrary but chosen large enough to make sure that there is enough time to receive the complete response frame sent by the responder at the
 *    6.8M data rate used (around 200 µs).
 * 4. In a real application, for optimum performance within regulatory limits, it may be necessary to set TX pulse bandwidth and TX power, (using
 *    the dwt_configuretxrf API call) to per device calibrated values saved in the target system or the DW1000 OTP memory.
 * 5. The user is referred to DecaRanging ARM application (distributed with EVK1000 product) for additional practical example of usage, and to the
 *     DW1000 API Guide for more details on the DW1000 driver functions.
 *
 ****************************************************************************************************************************************************/
