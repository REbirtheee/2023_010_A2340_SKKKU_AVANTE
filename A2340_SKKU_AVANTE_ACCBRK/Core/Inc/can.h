/*
 ****************************************************************************************
 * File Name          : can.h                                                          *
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
#include "core.h"

#ifndef INC_CAN_H_
#define INC_CAN_H_

/************************************* CAN ************************************/
FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;
FDCAN_HandleTypeDef hfdcan3;

//Can_Tx_Header_Set
FDCAN_TxHeaderTypeDef Can_Tx_Header;

FDCAN_TxHeaderTypeDef Can_Tx_Header_can1;
FDCAN_TxHeaderTypeDef Can_Tx_Header_can2;
FDCAN_TxHeaderTypeDef Can_Tx_Header_can3;

//Can_Rx_Header_Set
FDCAN_RxHeaderTypeDef  Can_Rx_Header;
FDCAN_RxHeaderTypeDef  Can_Rx_Header_CAN1;
FDCAN_RxHeaderTypeDef  Can_Rx_Header_CAN2;
FDCAN_RxHeaderTypeDef  Can_Rx_Header_CAN3;

/*********************************** Common ***********************************/
uint8_t rxdata1[8];
uint8_t rxdata2[8];
uint8_t rxdata3[8];

uint8_t txdata1[8];
uint8_t txdata2[8];
uint8_t txdata3[8];

uint8_t test_data1[8];
uint8_t test_data2[8];
uint8_t test_data3[8];

/********************************** Function **********************************/
void TXCAN1(uint32_t ID, uint32_t size, uint8_t *send);
void TXCAN2(uint32_t ID, uint32_t size, uint8_t *send);
void TXCAN3(uint32_t ID, uint32_t size, uint8_t *send);
void RXCAN1(FDCAN_RxHeaderTypeDef *head, uint8_t *receive);
void RXCAN2(FDCAN_RxHeaderTypeDef *head, uint8_t *receive);
void RXCAN3(FDCAN_RxHeaderTypeDef *head, uint8_t *receive);


#endif /* INC_CAN_H_ */
