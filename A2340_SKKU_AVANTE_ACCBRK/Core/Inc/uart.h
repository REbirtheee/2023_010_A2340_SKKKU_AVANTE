/*
 ****************************************************************************************
 * File Name          : uart.h                                                          *
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

#ifndef INC_UART_H_
#define INC_UART_H_

#include "core.h"

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

struct UART {

	uint8_t rx_buffer[8];
	uint8_t tx_buffer[8];

	uint8_t tx_done_flag;
}uart;
#endif /* INC_UART_H_ */
