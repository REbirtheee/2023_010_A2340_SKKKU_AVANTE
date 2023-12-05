/*
 ****************************************************************************************
 * File Name          : can.c                                                          *
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
#include "can.h"
/******************************************************************************/
/*                               CAN_Callback                                 */
/******************************************************************************/
void HAL_FDCAN_RxFifo0Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo0ITs)
{
	/*************************************/
	/* CAN1 Rx Interrupt     [ AD-CAN ]  */
	/*************************************/
	if(hfdcan->Instance == FDCAN1)
	{
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &Can_Rx_Header_CAN1, rxdata1);
		RXCAN1(&Can_Rx_Header_CAN1, rxdata1);

	}

	/*************************************/
	/* CAN2 Rx Interrupt  [ Module-CAN ] */
	/*************************************/
	if(hfdcan->Instance == FDCAN2)
	{
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &Can_Rx_Header_CAN2, rxdata2);
		RXCAN2(&Can_Rx_Header_CAN2, rxdata2);

	}
}

void HAL_FDCAN_RxFifo1Callback(FDCAN_HandleTypeDef *hfdcan, uint32_t RxFifo1ITs)
{

	/*************************************/
	/* CAN3 Rx Interrupt [ Chassis-CAN ] */
	/*************************************/
	if(hfdcan->Instance == FDCAN3)
	{
		HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO1, &Can_Rx_Header_CAN3, rxdata3);
		RXCAN3(&Can_Rx_Header_CAN3, rxdata3);

	}
}

/******************************************************************************/
/*                             CAN_Tx_Function                                */
/******************************************************************************/
void TXCAN1(uint32_t ID, uint32_t size, uint8_t *send)
{	/* Function Description : Tx CAN Data (CAN1) */
	Can_Tx_Header.Identifier = ID;
	Can_Tx_Header.IdType = FDCAN_STANDARD_ID;
	Can_Tx_Header.TxFrameType = FDCAN_DATA_FRAME;
	Can_Tx_Header.FDFormat =FDCAN_CLASSIC_CAN;
	Can_Tx_Header.DataLength = size;

	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &Can_Tx_Header, send);
}
void TXCAN2(uint32_t ID, uint32_t size, uint8_t *send)
{	/* Function Description : Tx CAN Data (CAN2) */
	Can_Tx_Header.Identifier = ID;
	Can_Tx_Header.IdType = FDCAN_STANDARD_ID;
	Can_Tx_Header.TxFrameType = FDCAN_DATA_FRAME;
	Can_Tx_Header.FDFormat =FDCAN_CLASSIC_CAN;
	Can_Tx_Header.DataLength = size;

	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &Can_Tx_Header, send);
}
void TXCAN3(uint32_t ID, uint32_t size, uint8_t *send)
{	/* Function Description : Tx CAN Data (CAN3) */
	Can_Tx_Header.Identifier = ID;
	Can_Tx_Header.IdType = FDCAN_STANDARD_ID;
	Can_Tx_Header.TxFrameType = FDCAN_DATA_FRAME;
	Can_Tx_Header.FDFormat =FDCAN_CLASSIC_CAN;
	Can_Tx_Header.DataLength = size;

	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan3, &Can_Tx_Header, send);
}

/******************************************************************************/
/*                             CAN_Rx_Function                                */
/******************************************************************************/
void RXCAN1(FDCAN_RxHeaderTypeDef *head, uint8_t *receive)
{	/* Function Description : Rx CAN Data (CAN1) */
	switch (head->Identifier)
	{
#if 0 //MOTOR TEST
	case 0x300:
		memcpy(motor_cmd.user, receive, 8);
		break;

	case 0x310:
		memcpy(motor_cmd.status, receive, 8);
		break;

	case 0x320:
		memcpy(motor_cmd.torque, receive, 8);
		break;
#else
		/* 01 */
	case 0x300:
		memcpy(brk.raw_data_0x300, receive, 7);
		break;

		/* 02 */
	case 0x310:
		memcpy(acc.raw_data_0x310, receive, 5);
		break;

		/* 03 */
	case 0x777:
		stat.debug_flag = TRUE;
		break;

#endif
	}


}
void RXCAN2(FDCAN_RxHeaderTypeDef *head, uint8_t *receive)
{	/* Function Description : Rx CAN Data (CAN2) */
	switch (head->Identifier)
	{
	/* 01 Motor */
	case 0x180 + MOTOR_NODE_ID:
	memcpy(motor_info.pos_vel_raw, receive, 8);
	motor_pw_check.no_can_cnt = 0;
	break;

	/* 02 Motor */
	case 0x280 + MOTOR_NODE_ID:
	memcpy(motor_info.tor_curr_raw, receive, 4);
	break;

	/* 03 Motor */
	case 0x380 + MOTOR_NODE_ID:
	memcpy(motor_info.status_raw, receive, 4);
	break;

	/* 04 APU */
	case 0x450:
		memcpy(acc.raw_data_0x450, receive, 8);
		break;

		/* 05 APU */
	case 0x452:
		memcpy(acc.raw_data_0x452, receive, 8);
		break;

		/* 06 Motor */
	case 0x700 + MOTOR_NODE_ID:
	motor_info.ready = TRUE;
	break;
	}
}
void RXCAN3(FDCAN_RxHeaderTypeDef *head, uint8_t *receive)
{	/* Function Description : Rx CAN Data (CAN3) */
	switch (head->Identifier)
	{
	/* 01 */
	case TEST_ID:
		memcpy(rxdata3, receive, DATA_SIZE_8);
		break;
	}
}


