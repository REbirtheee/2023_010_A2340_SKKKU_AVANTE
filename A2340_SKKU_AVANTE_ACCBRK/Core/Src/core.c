/*
 ****************************************************************************************
 * File Name          : core.c                                                          *
 * Writer             : *********name**********                                         *
 * Maintainer         : *********name**********                                         *
 * Description        : This file provides ****** control code for the Treeze-Eng's VCU.*
 *                                                                                      *
 ****************************************************************************************
 * @attention                                                                           *
 *                                                                                      *
 * <h2><center>&copy; © COPYRIGHT 2022 TREEZE ENGINEERING CO.,LTD.                      *
 * ALL RIGHTS RESERVED.</center></h2>                                                   *
 *                                                                                      *
 ****************************************************************************************
 */
#include "core.h"

/******************************************************************************/
/*                                    RTOS                                    */
/******************************************************************************/

void StartTreezeTask_1(void const * argument)
{
	/* USER CODE BEGIN 5 */
	/**********************/
	/* Brake Control Task */
	/**********************/
	/* Infinite loop */
	for(;;)
	{
		Brake_Control_Task();
		Module_Status_Task();
		osDelay(1);
	}
	/* USER CODE END 5 */
}

void StartTreezeTask_2(void const * argument)
{
	/* USER CODE BEGIN 5 */
	/**********************/
	/* Accel Control Task */
	/**********************/
	/* Infinite loop */
	for(;;)
	{
		Accel_Control_Task();
		osDelay(1);
	}
	/* USER CODE END 5 */
}

void StartTreezeTask_3(void const * argument)
{
	/* USER CODE BEGIN 5 */
	/**********************/
	/*LeadShine Motor Task*/
	/**********************/
	/* Infinite loop */
	for(;;)
	{
		MotionControlControl(&motor_cmd, &motor_info);
		osDelay(1);
	}
	/* USER CODE END 5 */
}

/******************************************************************************/
/*                                Function                                    */
/******************************************************************************/
void TEST_TASK()
{
	if(timer_20ms == TIM_SEQUENCE_1)
	{/* SEQUNCE_01 */
		TXCAN1(0x001, FDCAN_DLC_BYTES_8, txdata1);
		TXCAN2(0x002, FDCAN_DLC_BYTES_8, rxdata2);
		TXCAN3(0x003, FDCAN_DLC_BYTES_8, rxdata3);
	}
	else if(timer_20ms == TIM_SEQUENCE_2)
	{/* SEQUNCE_02 */


	}

	SequenceTimer(&timer_20ms, TIM_SEQUENCE_1, TIM_SEQUENCE_INIT_20);
}
