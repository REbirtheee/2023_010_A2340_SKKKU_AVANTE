/*
 ****************************************************************************************
 * File Name          : uart.c                                                          *
 * Writer             : *********name**********                                         *
 * Maintainer         : *********name**********                                         *
 * Description        : This file provides ****** control code for the Treeze-Eng's VCU.*
 *                                                                                      *
 ****************************************************************************************
 * @attention                                                                           *
 *                                                                                      *
 * <h2><center>&copy; © COPYRIGHT 2023 TREEZE ENGINEERING CO.,LTD.                      *
 * ALL RIGHTS RESERVED.</center></h2>                                                   *
 *                                                                                      *
 ****************************************************************************************
 */
#include "uart.h"
/******************************************************************************/
/*                              UART_Callback                                 */
/******************************************************************************/
uint8_t test_buffer[8];

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{

	if (huart->Instance == USART1)
	{
		memcpy (test_buffer, uart.rx_buffer, 8);

		//HAL_UART_Transmit_DMA(&huart1, uart.tx_buffer, 5);

		/* start the DMA again */
		HAL_UARTEx_ReceiveToIdle_DMA(&huart1, uart.rx_buffer, 8);
	}

}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	//Flag Clear
	uart.tx_done_flag = FALSE;
}



