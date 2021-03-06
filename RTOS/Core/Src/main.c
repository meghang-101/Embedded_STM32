/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
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
UART_HandleTypeDef huart1;

osThreadId defaultTaskHandle;
osThreadId Thread1Handle;
osThreadId Thread2Handle;
osThreadId Thread3Handle;
osMutexId uart_mutex01Handle;
osMutexId uart_mutex02Handle;
osMutexId uart_mutex03Handle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
void StartDefaultTask(void const * argument);
void Thread1Func(void const * argument);
void Thread2Func(void const * argument);
void Thread3Func(void const * argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
typedef uint32_t TaskProfiler;
TaskProfiler Default_Thread_Profiler, Thread_1_Profiler, Thread_2_Profiler;
uint8_t msgThreadDefault[30] = "Hello Thread Default";
uint8_t msgThread1[30] = "Hello Thread 1";
uint8_t msgThread2[30] = "Hello Thread 2";
uint8_t msgThread3[30] = "Hello Thread 3";
uint8_t msgThread4[30] = "Hello Thread 4";
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */

  /* USER CODE END 2 */

  /* Create the mutex(es) */
  /* definition and creation of uart_mutex01 */
  osMutexDef(uart_mutex01);
  uart_mutex01Handle = osMutexCreate(osMutex(uart_mutex01));

  /* definition and creation of uart_mutex02 */
  osMutexDef(uart_mutex02);
  uart_mutex02Handle = osMutexCreate(osMutex(uart_mutex02));

  /* definition and creation of uart_mutex03 */
  osMutexDef(uart_mutex03);
  uart_mutex03Handle = osMutexCreate(osMutex(uart_mutex03));

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
  /* definition and creation of defaultTask */
  osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);

  /* definition and creation of Thread1 */
  osThreadDef(Thread1, Thread1Func, osPriorityIdle, 0, 128);
  Thread1Handle = osThreadCreate(osThread(Thread1), NULL);

  /* definition and creation of Thread2 */
  osThreadDef(Thread2, Thread2Func, osPriorityIdle, 0, 128);
  Thread2Handle = osThreadCreate(osThread(Thread2), NULL);

  /* definition and creation of Thread3 */
  osThreadDef(Thread3, Thread3Func, osPriorityIdle, 0, 128);
  Thread3Handle = osThreadCreate(osThread(Thread3), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : LED_Pin */
  GPIO_InitStruct.Pin = LED_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : BUTTON_Pin */
  GPIO_InitStruct.Pin = BUTTON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(BUTTON_GPIO_Port, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void const * argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
//	  xTaskNotify(Thread3Handle, 0x01, eSetBits);

	  xSemaphoreTake(uart_mutex01Handle, portMAX_DELAY);
	  xSemaphoreTake(uart_mutex02Handle, portMAX_DELAY);
	  xSemaphoreTake(uart_mutex03Handle, portMAX_DELAY);
	  HAL_UART_Transmit(&huart1, msgThreadDefault, 30, 10);
	  xSemaphoreGive(uart_mutex01Handle);
	  xSemaphoreGive(uart_mutex02Handle);
	  xSemaphoreGive(uart_mutex03Handle);
//	  Default_Thread_Profiler++;
//    osDelay(1);
  }
  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_Thread1Func */
/**
* @brief Function implementing the Thread1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Thread1Func */
void Thread1Func(void const * argument)
{
  /* USER CODE BEGIN Thread1Func */
  /* Infinite loop */
  for(;;)
  {
//	  Thread_1_Profiler++;
	  xSemaphoreTake(uart_mutex01Handle, portMAX_DELAY);
	  HAL_UART_Transmit(&huart1, msgThread1, 20, 10);
	  xSemaphoreGive(uart_mutex01Handle);
//    osDelay(1);
  }
  /* USER CODE END Thread1Func */
}

/* USER CODE BEGIN Header_Thread2Func */
/**
* @brief Function implementing the Thread2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Thread2Func */
void Thread2Func(void const * argument)
{
  /* USER CODE BEGIN Thread2Func */
  /* Infinite loop */
  for(;;)
  {
//	  Thread_2_Profiler++;
	  xSemaphoreTake(uart_mutex02Handle, portMAX_DELAY);
	  HAL_UART_Transmit(&huart1, msgThread2, 20, 10);
	  xSemaphoreGive(uart_mutex02Handle);
//    osDelay(1);
  }
  /* USER CODE END Thread2Func */
}

/* USER CODE BEGIN Header_Thread3Func */
/**
* @brief Function implementing the Thread3 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Thread3Func */
void Thread3Func(void const * argument)
{
  /* USER CODE BEGIN Thread3Func */
//	uint32_t notifyValue=0;
  /* Infinite loop */
  for(;;)
  {
//	  xTaskNotifyWait(pdFalse, 0xFF, &notifyValue, portMAX_DELAY);

//	  if ((notifyValue & 0x01)) != 0x00) {

	  xSemaphoreTake(uart_mutex03Handle, portMAX_DELAY);
	  HAL_UART_Transmit(&huart1, msgThread3, 20, 10);
	  xSemaphoreGive(uart_mutex03Handle);
//	  }
//    osDelay(1);
  }
  /* USER CODE END Thread3Func */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM2 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM2) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
