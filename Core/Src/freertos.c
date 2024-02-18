/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
osThreadId usb_task_handle;
osThreadId decode_task_handle;
/* USER CODE END Variables */
osThreadId LED_REDHandle;
osThreadId LED_GREENHandle;
osThreadId LED_BLUEHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void red_led_task(void const * argument);
extern void green_led_task(void const * argument);
extern void blue_led_task(void const * argument);

extern void MX_USB_DEVICE_Init(void);
void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
// USB数据队列
QueueHandle_t CDC_send_queue;
_Noreturn extern void usb_task(void const * argument);
extern void decode_task(void const * argument);
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of LED_RED */
  osThreadDef(LED_RED, red_led_task, osPriorityNormal, 0, 128);
  LED_REDHandle = osThreadCreate(osThread(LED_RED), NULL);

  /* definition and creation of LED_GREEN */
  osThreadDef(LED_GREEN, green_led_task, osPriorityNormal, 0, 128);
  LED_GREENHandle = osThreadCreate(osThread(LED_GREEN), NULL);

  /* definition and creation of LED_BLUE */
  osThreadDef(LED_BLUE, blue_led_task, osPriorityNormal, 0, 128);
  LED_BLUEHandle = osThreadCreate(osThread(LED_BLUE), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
    // USB发送数据任务
    osThreadDef(USB_task,usb_task,osPriorityHigh,0,128);
    usb_task_handle= osThreadCreate(osThread(USB_task),NULL);
    // USB接收解码任务
    osThreadDef(DecodeTask, decode_task, osPriorityHigh, 0, 128);
    decode_task_handle = osThreadCreate(osThread(DecodeTask), NULL);
    // 创建队列
    CDC_send_queue = xQueueCreate(1, 128);
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_red_led_task */
/**
  * @brief  Function implementing the LED_RED thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_red_led_task */
void red_led_task(void const * argument)
{
  /* init code for USB_DEVICE */
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN red_led_task */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END red_led_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void blue_led_task(void const * argument){
    vTaskDelete(NULL);
}

/* USER CODE END Application */
