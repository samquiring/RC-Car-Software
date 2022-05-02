/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include <stdbool.h>
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
osThreadId ledToggle_1_TaskHandle;
osThreadId ledToggle_2_TaskHandle;
osThreadId ledToggle_3_TaskHandle;
osThreadId irSensor_test_1_TaskHandle;
bool ir_triggered;
bool led_1_on;

/* USER CODE END Variables */
osThreadId defaultTaskHandle;
osMessageQId myExampleQueue01Handle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
void ledToggle1(void const * argument);
void ledToggle2(void const * argument);
void ledToggle3(void const * argument);
void irSensor1(void const * argument);
unsigned int bit_toggle(unsigned int, int);
/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);

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
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  ir_triggered = false;
  led_1_on = false;
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

  /* Create the queue(s) */
  /* definition and creation of myExampleQueue01 */
  osMessageQDef(myExampleQueue01, 4, uint16_t);
  myExampleQueue01Handle = osMessageCreate(osMessageQ(myExampleQueue01), NULL);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  osThreadDef(IR_SENSOR_1, irSensor1, osPriorityNormal, 0, 128);
  irSensor_test_1_TaskHandle = osThreadCreate(osThread(IR_SENSOR_1),NULL);
  
  osThreadDef(LED_Toggle1, ledToggle1, osPriorityNormal, 0, 128);
  ledToggle_1_TaskHandle = osThreadCreate(osThread(LED_Toggle1),NULL);
  
  osThreadDef(LED_Toggle2, ledToggle2, osPriorityNormal, 0, 128);
  ledToggle_2_TaskHandle = osThreadCreate(osThread(LED_Toggle2),NULL);
  
  osThreadDef(LED_Toggle3, ledToggle3, osPriorityNormal, 0, 128);
  ledToggle_3_TaskHandle = osThreadCreate(osThread(LED_Toggle3),NULL); 
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  /* Acts as the IDLE task. DONT TOUCH */ 
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

void irSensor1(void const * argument)
{
  /* IR sensor Initialization
  See 'gpio.c' for GPIO initialization */

  while(1){
    ir_triggered = HAL_GPIO_ReadPin(IR_SENSOR_1_GPIO_Port, IR_SENSOR_1_Pin);
    osDelay(200);
  }
  
}

void ledToggle1(void const * argument)
{
  /* LED Initialization */
  /* See 'gpio.c' for GPIO initialization */
  
  
  while(1)
  {
    if(ir_triggered && led_1_on == false)
    {
      HAL_GPIO_TogglePin(LED_1_GPIO_Port,LED_1_Pin);
      led_1_on = true;
    } else if(ir_triggered == false && led_1_on)
    {
      HAL_GPIO_TogglePin(LED_1_GPIO_Port,LED_1_Pin);
      led_1_on = false;
    } 
    osDelay(200);
  }
}

void ledToggle2(void const * argument)
{
  
  while(1)
  {
    HAL_GPIO_TogglePin(LED_2_GPIO_Port,LED_2_Pin);
    osDelay(100);
  }
}

void ledToggle3(void const * argument)
{

  while(1)
  {
    HAL_GPIO_TogglePin(LED_3_GPIO_Port,LED_3_Pin);
    osDelay(1200);
  }
}


/* Utilities */
// bit toggle
unsigned int bit_toggle(unsigned int reg, int bitnum){
  return reg = reg ^ (1<<bitnum);
}


/* USER CODE END Application */
