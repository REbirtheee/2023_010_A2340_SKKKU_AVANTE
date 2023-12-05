/*
 ****************************************************************************************
 * File Name          : trz_util.h                                                          *
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

#ifndef INC_TRZ_UTIL_H_
#define INC_TRZ_UTIL_H_

/***********************************  Define **********************************/
#define TIM_SEQUENCE_1		0
#define TIM_SEQUENCE_2		1
#define TIM_SEQUENCE_3		2
#define TIM_SEQUENCE_4		3
#define TIM_SEQUENCE_5		4
#define TIM_SEQUENCE_6		5
#define TIM_SEQUENCE_7		6
#define TIM_SEQUENCE_8		7
#define TIM_SEQUENCE_9		8
#define TIM_SEQUENCE_10		9
#define TIM_SEQUENCE_11		10
#define TIM_SEQUENCE_12		11
#define TIM_SEQUENCE_13		12
#define TIM_SEQUENCE_14		13
#define TIM_SEQUENCE_15		14
#define TIM_SEQUENCE_16		15
#define TIM_SEQUENCE_17		16
#define TIM_SEQUENCE_18		17
#define TIM_SEQUENCE_19		18
#define TIM_SEQUENCE_20		19

#define TIM_SEQUENCE_INIT_10	10
#define TIM_SEQUENCE_INIT_20	20

#define DATA_SIZE_1 		1
#define DATA_SIZE_2 		2
#define DATA_SIZE_3 		3
#define DATA_SIZE_4 		4
#define DATA_SIZE_5 		5
#define DATA_SIZE_6 		6
#define DATA_SIZE_7 		7
#define DATA_SIZE_8 		8

#define TEST_ID 			0x313

#define TRUE 				1
#define FALSE 				0

/*********************************** Common ***********************************/
uint8_t timer_10ms;
uint8_t timer_20ms;

/********************************** Function **********************************/
void SequenceTimer(uint8_t *timer, uint8_t init, uint8_t max);
void FlagMakeFalse(uint8_t *flag);
void FlagMakeTrue(uint8_t *flag);



#endif /* INC_TRZ_UTIL_H_ */
