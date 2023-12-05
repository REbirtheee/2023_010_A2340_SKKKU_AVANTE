/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
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
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include"string.h"
#include "core.h"
#include <stdio.h>
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
FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;
FDCAN_HandleTypeDef hfdcan3;

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

osThreadId TreezeTask_1Handle;
osThreadId TreezeTask_2Handle;
osThreadId TreezeTask_3Handle;
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_FDCAN2_Init(void);
static void MX_FDCAN3_Init(void);
static void MX_USART1_UART_Init(void);
void StartTreezeTask_1(void const * argument);
void StartTreezeTask_2(void const * argument);
void StartTreezeTask_3(void const * argument);

/* USER CODE BEGIN PFP */

//Can_Tx_Header_Set
FDCAN_TxHeaderTypeDef Can_Tx_Header;

//Can_Rx_Header_Set
FDCAN_RxHeaderTypeDef  Can_Rx_Header;

//Can_Filter_Set
FDCAN_FilterTypeDef can1_filter;
FDCAN_FilterTypeDef can2_filter;
FDCAN_FilterTypeDef can3_filter;

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
	MX_DMA_Init();
	MX_FDCAN1_Init();
	MX_FDCAN2_Init();
	MX_FDCAN3_Init();
	MX_USART1_UART_Init();
	/* USER CODE BEGIN 2 */
	//Motor INIT
	motor_pw_check.no_can_cnt = 0;
	while(!motor_info.ready);
	HAL_Delay(1000);
	MotorInit();
	/* USER CODE END 2 */

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
	/* definition and creation of TreezeTask_1 */
	osThreadDef(TreezeTask_1, StartTreezeTask_1, osPriorityHigh, 0, 512);
	TreezeTask_1Handle = osThreadCreate(osThread(TreezeTask_1), NULL);

	/* definition and creation of TreezeTask_2 */
	osThreadDef(TreezeTask_2, StartTreezeTask_2, osPriorityNormal, 0, 512);
	TreezeTask_2Handle = osThreadCreate(osThread(TreezeTask_2), NULL);

	/* definition and creation of TreezeTask_3 */
	osThreadDef(TreezeTask_3, StartTreezeTask_3, osPriorityLow, 0, 512);
	TreezeTask_3Handle = osThreadCreate(osThread(TreezeTask_3), NULL);

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

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
	RCC_OscInitStruct.PLL.PLLN = 34;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
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
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
	{
		Error_Handler();
	}
}

/**
 * @brief FDCAN1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_FDCAN1_Init(void)
{

	/* USER CODE BEGIN FDCAN1_Init 0 */

	/* USER CODE END FDCAN1_Init 0 */

	/* USER CODE BEGIN FDCAN1_Init 1 */

	/* USER CODE END FDCAN1_Init 1 */
	hfdcan1.Instance = FDCAN1;
	hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
	hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan1.Init.AutoRetransmission = ENABLE;
	hfdcan1.Init.TransmitPause = DISABLE;
	hfdcan1.Init.ProtocolException = DISABLE;
	hfdcan1.Init.NominalPrescaler = 20;
	hfdcan1.Init.NominalSyncJumpWidth = 1;
	hfdcan1.Init.NominalTimeSeg1 = 14;
	hfdcan1.Init.NominalTimeSeg2 = 2;
	hfdcan1.Init.DataPrescaler = 1;
	hfdcan1.Init.DataSyncJumpWidth = 1;
	hfdcan1.Init.DataTimeSeg1 = 1;
	hfdcan1.Init.DataTimeSeg2 = 1;
	hfdcan1.Init.StdFiltersNbr = 1;
	hfdcan1.Init.ExtFiltersNbr = 0;
	hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN FDCAN1_Init 2 */
	//Can_filter
	can1_filter.IdType = FDCAN_STANDARD_ID;
	can1_filter.FilterIndex = 0;
	can1_filter.FilterType = FDCAN_FILTER_RANGE;
	can1_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	can1_filter.FilterID1 = 0x001;
	can1_filter.FilterID2 = 0x7FF;

	//Can_start
	if( HAL_FDCAN_ConfigFilter(&hfdcan1, &can1_filter) != HAL_OK ) {
		Error_Handler();
	}

	if(HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_INTERRUPT_LINE0, FDCAN_TX_BUFFER0|FDCAN_TX_BUFFER1|FDCAN_TX_BUFFER2) != HAL_OK )
	{
		Error_Handler();
	}

	if(HAL_FDCAN_Start(&hfdcan1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE END FDCAN1_Init 2 */

}

/**
 * @brief FDCAN2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_FDCAN2_Init(void)
{

	/* USER CODE BEGIN FDCAN2_Init 0 */

	/* USER CODE END FDCAN2_Init 0 */

	/* USER CODE BEGIN FDCAN2_Init 1 */

	/* USER CODE END FDCAN2_Init 1 */
	hfdcan2.Instance = FDCAN2;
	hfdcan2.Init.ClockDivider = FDCAN_CLOCK_DIV1;
	hfdcan2.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan2.Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan2.Init.AutoRetransmission = ENABLE;
	hfdcan2.Init.TransmitPause = DISABLE;
	hfdcan2.Init.ProtocolException = DISABLE;
	hfdcan2.Init.NominalPrescaler = 20;
	hfdcan2.Init.NominalSyncJumpWidth = 1;
	hfdcan2.Init.NominalTimeSeg1 = 14;
	hfdcan2.Init.NominalTimeSeg2 = 2;
	hfdcan2.Init.DataPrescaler = 1;
	hfdcan2.Init.DataSyncJumpWidth = 1;
	hfdcan2.Init.DataTimeSeg1 = 1;
	hfdcan2.Init.DataTimeSeg2 = 1;
	hfdcan2.Init.StdFiltersNbr = 1;
	hfdcan2.Init.ExtFiltersNbr = 0;
	hfdcan2.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	if (HAL_FDCAN_Init(&hfdcan2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN FDCAN2_Init 2 */
	//Can_filter
	can2_filter.IdType = FDCAN_STANDARD_ID;
	can2_filter.FilterIndex = 0;
	can2_filter.FilterType = FDCAN_FILTER_RANGE;
	can2_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	can2_filter.FilterID1 = 0x001;
	can2_filter.FilterID2 = 0x7FF;

	//Can_Start
	if( HAL_FDCAN_ConfigFilter(&hfdcan2, &can2_filter) != HAL_OK ) {
		Error_Handler();
	}

	if(HAL_FDCAN_ActivateNotification(&hfdcan2, FDCAN_INTERRUPT_LINE0, FDCAN_TX_BUFFER0|FDCAN_TX_BUFFER1|FDCAN_TX_BUFFER2) != HAL_OK )
	{
		Error_Handler();
	}

	if(HAL_FDCAN_Start(&hfdcan2) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE END FDCAN2_Init 2 */

}

/**
 * @brief FDCAN3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_FDCAN3_Init(void)
{

	/* USER CODE BEGIN FDCAN3_Init 0 */

	/* USER CODE END FDCAN3_Init 0 */

	/* USER CODE BEGIN FDCAN3_Init 1 */

	/* USER CODE END FDCAN3_Init 1 */
	hfdcan3.Instance = FDCAN3;
	hfdcan3.Init.ClockDivider = FDCAN_CLOCK_DIV1;
	hfdcan3.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
	hfdcan3.Init.Mode = FDCAN_MODE_NORMAL;
	hfdcan3.Init.AutoRetransmission = ENABLE;
	hfdcan3.Init.TransmitPause = DISABLE;
	hfdcan3.Init.ProtocolException = DISABLE;
	hfdcan3.Init.NominalPrescaler = 20;
	hfdcan3.Init.NominalSyncJumpWidth = 1;
	hfdcan3.Init.NominalTimeSeg1 = 14;
	hfdcan3.Init.NominalTimeSeg2 = 2;
	hfdcan3.Init.DataPrescaler = 1;
	hfdcan3.Init.DataSyncJumpWidth = 1;
	hfdcan3.Init.DataTimeSeg1 = 1;
	hfdcan3.Init.DataTimeSeg2 = 1;
	hfdcan3.Init.StdFiltersNbr = 1;
	hfdcan3.Init.ExtFiltersNbr = 0;
	hfdcan3.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
	if (HAL_FDCAN_Init(&hfdcan3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN FDCAN3_Init 2 */
	//Can_filter
	can3_filter.IdType = FDCAN_STANDARD_ID;
	can3_filter.FilterIndex = 0;
	can3_filter.FilterType = FDCAN_FILTER_RANGE;
	can3_filter.FilterConfig = FDCAN_FILTER_TO_RXFIFO1;
	can3_filter.FilterID1 = 0x001;
	can3_filter.FilterID2 = 0x7FF;

	//Can_Start
	if( HAL_FDCAN_ConfigFilter(&hfdcan3, &can3_filter) != HAL_OK ) {
		Error_Handler();
	}

	if(HAL_FDCAN_ConfigGlobalFilter(&hfdcan3, FDCAN_REJECT, FDCAN_REJECT, FDCAN_FILTER_REMOTE, FDCAN_FILTER_REMOTE) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_FDCAN_ActivateNotification(&hfdcan3, FDCAN_IT_RX_FIFO1_NEW_MESSAGE|FDCAN_IT_RX_FIFO1_FULL|FDCAN_IT_RX_FIFO1_MESSAGE_LOST, 0) != HAL_OK)
	{
		Error_Handler();
	}

	if(HAL_FDCAN_Start(&hfdcan3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE END FDCAN3_Init 2 */

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
	huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART1_Init 2 */

	HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart.rx_buffer, 8);
	__HAL_DMA_DISABLE_IT(&hdma_usart1_rx, DMA_IT_HT);
	//Start DMA
	/* USER CODE END USART1_Init 2 */

}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void)
{

	/* DMA controller clock enable */
	__HAL_RCC_DMAMUX1_CLK_ENABLE();
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA interrupt init */
	/* DMA1_Channel1_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
	/* DMA1_Channel2_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel2_IRQn, 5, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel2_IRQn);

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartTreezeTask_1 */
/**
 * @brief  Function implementing the TreezeTask_1 thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTreezeTask_1 */
__weak void StartTreezeTask_1(void const * argument)
{
	/* USER CODE BEGIN 5 */
	//  /* Infinite loop */
	//  for(;;)
	//  {
	//    osDelay(1);
	//  }
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartTreezeTask_2 */
/**
 * @brief Function implementing the TreezeTask_2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTreezeTask_2 */
__weak void StartTreezeTask_2(void const * argument)
{
	/* USER CODE BEGIN StartTreezeTask_2 */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
	/* USER CODE END StartTreezeTask_2 */
}

/* USER CODE BEGIN Header_StartTreezeTask_3 */
/**
 * @brief Function implementing the TreezeTask_3 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_StartTreezeTask_3 */
__weak void StartTreezeTask_3(void const * argument)
{
	/* USER CODE BEGIN StartTreezeTask_3 */
	/* Infinite loop */
	for(;;)
	{
		osDelay(1);
	}
	/* USER CODE END StartTreezeTask_3 */
}

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM1 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM1) {
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
