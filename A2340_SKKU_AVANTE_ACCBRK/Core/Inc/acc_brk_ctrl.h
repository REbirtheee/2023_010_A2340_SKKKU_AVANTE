/*
 ****************************************************************************************
 * File Name          : acc_brk_ctrl.h                                                  *
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

#ifndef INC_ACC_BRK_CTRL_H_
#define INC_ACC_BRK_CTRL_H_

#define ACC_MASTER_VOLTAGE_MIN 0.76
#define ACC_MASTER_VOLTAGE_MAX 4.13

struct Accel_Control {

	uint8_t raw_data_0x310[5];
	uint8_t raw_data_0x450[8];
	uint8_t raw_data_0x451[6];
	uint8_t raw_data_0x452[8];
	uint8_t raw_data_0x610[7];

	uint8_t AD_ACC_CtrlMode;
	uint8_t AD_ACC_Cmd_Pos;
	uint8_t AD_ACC_TakeOver;
	uint8_t AD_ACC_AliveCnt;
	uint8_t AD_ACC_ChkSum;

	uint8_t TRZ_ACC_AliveCnt;
	uint8_t TRZ_ACC_ChkSum;

	uint16_t acc_ctrl_master_vol;
	uint16_t acc_ctrl_slave_vol;
	float acc_ctrl_vol_ratio;
	float acc_ctrl_vol_float;
	uint8_t acc_ctrl_alivecnt;

	uint8_t apu_no_error_cnt;

	uint8_t timer_10ms;
	uint8_t timer_20ms;
	uint8_t timer_30ms;

}acc;

struct Brake_Control {

	uint8_t raw_data_0x300[7];
	uint8_t raw_data_0x600[8];

	uint8_t AD_BRKMOT_AliveCnt;
	uint8_t AD_BRKMOT_ChkSum;
	uint8_t AD_BRKMOT_CtrlMode;
	uint8_t AD_BRKMOT_Cmd_Pos;
	uint16_t AD_BRKMOT_Cmd_Vel;
	uint8_t AD_BRKMOT_TakeOver;

	uint8_t TRZ_BRKMOT_AliveCnt;
	uint8_t TRZ_BRKMOT_ChkSum;

	uint8_t encoder_init_flag;
	uint32_t encoder_init_timer;

	uint8_t timer_10ms;
	uint8_t timer_20ms;
	uint8_t timer_30ms;

}brk;

struct Module_Status {

	uint8_t raw_data_0x700[8];

	uint8_t alive_cnt;

	uint8_t debug_flag;

	uint8_t timer_10ms;
	uint8_t timer_20ms;

}stat;





void Accel_Control_Task();
void Accel_Command_Info_Data_Parsing();
void Accel_Control_Sequnce();
void Accel_Alive_Count();

void Brake_Control_Task();
void Brake_Command_Info_Data_Parsing();
void Brake_Control_Sequnce();
void Position_Control_Mode_On();
void Position_Control_Mode_Off();
void Servo_On();
void Servo_Off();
void Motor_Auto_On();
void Motor_Auto_Off();

void Module_Status_Task();
void Module_Status_Sequnce();
void Status_Alive_Count();
void Debug_Data_Parsing();
void Motor_Encoder_Init();



#endif /* INC_ACC_BRK_CTRL_H_ */
