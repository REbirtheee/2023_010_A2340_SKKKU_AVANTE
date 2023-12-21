/*
 ****************************************************************************************
 * File Name          : acc_brk_ctrl.c                                                  *
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
#include "acc_brk_ctrl.h"

/***************************************/
/*             Accel Control           */
/***************************************/
void Accel_Control_Task()
{
	/* 01 : Accel Command Data / Status Data Parsing */
	Accel_Command_Info_Data_Parsing();

	/* 02 : Accel Command Data Tx  */
	Accel_Control_Sequnce();

	/* 03 : Task Sequnce Timer [ 10ms ]  */
	SequenceTimer(&acc.timer_10ms, TIM_SEQUENCE_1, TIM_SEQUENCE_INIT_10);
}

void Accel_Command_Info_Data_Parsing()
{
	/* Command Data Parsing */
	acc.AD_ACC_AliveCnt = acc.raw_data_0x310[0];
	acc.AD_ACC_ChkSum = acc.raw_data_0x310[1];
	acc.AD_ACC_CtrlMode = acc.raw_data_0x310[2];
	acc.AD_ACC_Cmd_Pos = acc.raw_data_0x310[3];
	if(acc.raw_data_0x310[4] == TRUE) acc.AD_ACC_TakeOver = TRUE;
	if(acc.AD_ACC_CtrlMode == FALSE) acc.AD_ACC_TakeOver = FALSE;

	/* Accel Command Percentage -> Voltage Convert */
	acc.acc_ctrl_vol_ratio = (float)(acc.AD_ACC_Cmd_Pos * 0.01);
	acc.acc_ctrl_vol_float = (float)(ACC_MASTER_VOLTAGE_MIN + ((ACC_MASTER_VOLTAGE_MAX - ACC_MASTER_VOLTAGE_MIN) * acc.acc_ctrl_vol_ratio));

	acc.acc_ctrl_master_vol = (uint16_t)(acc.acc_ctrl_vol_float * 12500);
	acc.acc_ctrl_slave_vol = (acc.acc_ctrl_master_vol / 2);

	/*  Apu Control Data Parsing */
	acc.raw_data_0x451[0] = acc.acc_ctrl_master_vol >> 8;
	acc.raw_data_0x451[1] = acc.acc_ctrl_master_vol;
	acc.raw_data_0x451[2] = acc.acc_ctrl_slave_vol  >> 8;
	acc.raw_data_0x451[3] = acc.acc_ctrl_slave_vol;

	if(!acc.AD_ACC_TakeOver)acc.raw_data_0x451[4] = acc.AD_ACC_CtrlMode;
	else acc.raw_data_0x451[4] = FALSE;

	acc.raw_data_0x451[5] = acc.acc_ctrl_alivecnt;

	/*  Module Status Data Parsing */
	acc.raw_data_0x610[0] = acc.TRZ_ACC_AliveCnt;
	acc.raw_data_0x610[1] = acc.TRZ_ACC_ChkSum;
	acc.raw_data_0x610[2] = acc.AD_ACC_CtrlMode;
	acc.raw_data_0x610[3] = acc.AD_ACC_Cmd_Pos;
	acc.raw_data_0x610[4] = acc.AD_ACC_TakeOver;

	if((acc.raw_data_0x450[4] == 0x00) && (acc.raw_data_0x450[5] == 0x00)) {

		acc.apu_no_error_cnt = FALSE;
	} else {

		acc.apu_no_error_cnt ++;
	}

	if(acc.apu_no_error_cnt > 200) {

		acc.raw_data_0x610[5] = acc.raw_data_0x450[4];
		acc.raw_data_0x610[6] = acc.raw_data_0x450[5];
	} else {

		acc.raw_data_0x610[5] = 0x00;
		acc.raw_data_0x610[6] = 0x00;
	}



}

void Accel_Control_Sequnce()
{
	if(acc.timer_10ms == TIM_SEQUENCE_1) {
		/* 01 : APC CMD DATA Tx */
		TXCAN2(0x451, FDCAN_DLC_BYTES_6, acc.raw_data_0x451);

	} else if(acc.timer_10ms == TIM_SEQUENCE_2) {

	} else if(acc.timer_10ms == TIM_SEQUENCE_3) {

	} else if(acc.timer_10ms == TIM_SEQUENCE_4) {

	} else if(acc.timer_10ms == TIM_SEQUENCE_9) {

	} else if(acc.timer_10ms == TIM_SEQUENCE_10) {
		/* 10 : Alive Count */
		Accel_Alive_Count();
	}
}

void Accel_Alive_Count()
{
	if(acc.acc_ctrl_alivecnt < 10) {
		acc.acc_ctrl_alivecnt ++;

	} else {
		acc.acc_ctrl_alivecnt = 0;

	}
}




/***************************************/
/*             Brake Control           */
/***************************************/
void Brake_Control_Task()
{
	/* 01 : Brake Command Data / Status Data Parsing */
	Brake_Command_Info_Data_Parsing();

	/* 02 : Motor Encoder Init  */
	Motor_Encoder_Init();

	/* 03 : Brake Command Data  */
	Brake_Control_Sequnce();

	/* 04 : Task Sequnce Timer [ 10ms ]  */
	SequenceTimer(&brk.timer_10ms, TIM_SEQUENCE_1, TIM_SEQUENCE_INIT_10);

}

void Brake_Command_Info_Data_Parsing()
{
	/* Command Data Parsing */
	brk.AD_BRKMOT_AliveCnt = brk.raw_data_0x300[0];
	brk.AD_BRKMOT_ChkSum = brk.raw_data_0x300[1];
	brk.AD_BRKMOT_CtrlMode = brk.raw_data_0x300[2];
	brk.AD_BRKMOT_Cmd_Pos = brk.raw_data_0x300[3];
	brk.AD_BRKMOT_Cmd_Vel = ((brk.raw_data_0x300[5] << 8) | (brk.raw_data_0x300[4]));
	if(brk.raw_data_0x300[6] == TRUE)brk.AD_BRKMOT_TakeOver = TRUE;
	if(brk.AD_BRKMOT_CtrlMode == FALSE)brk.AD_BRKMOT_TakeOver = FALSE;

	/*  Module Status Data Parsing */
	brk.raw_data_0x600[0] = brk.TRZ_BRKMOT_AliveCnt;
	brk.raw_data_0x600[1] = brk.TRZ_BRKMOT_ChkSum;
	brk.raw_data_0x600[2] = brk.AD_BRKMOT_CtrlMode;
	brk.raw_data_0x600[3] = brk.AD_BRKMOT_Cmd_Pos;
	brk.raw_data_0x600[4] = brk.AD_BRKMOT_Cmd_Vel;
	brk.raw_data_0x600[5] = brk.AD_BRKMOT_Cmd_Vel >> 8;
	brk.raw_data_0x600[6] = brk.AD_BRKMOT_TakeOver;
	brk.raw_data_0x600[7] = motor_info.status[5];

}

void Motor_Encoder_Init()
{
	int16_t motor_angle;
	motor_angle = ((motor_info.pos_vel_curr_tor[1] << 8) | motor_info.pos_vel_curr_tor[0]);

	if(!brk.encoder_init_flag) {

		Motor_Auto_On();
		Servo_On();
		motor_cmd.status[3] = TRUE; //ENCODER INIT ON
		brk.encoder_init_timer ++;


		if((brk.encoder_init_timer > 1000) && (motor_angle == 0x00)) {
			Motor_Auto_Off();
			Servo_Off();
			brk.encoder_init_flag = TRUE;
		}
	}

	else {

		motor_cmd.status[3] = FALSE; //ENCODER INIT OFF
	}
}

void Brake_Control_Sequnce()
{
	if(brk.encoder_init_flag) {

		/***        Auto Mode (Servo On)         ***/
		if(brk.AD_BRKMOT_CtrlMode && !brk.AD_BRKMOT_TakeOver) {
			//Motor Posion Control Init
			Motor_Auto_On();
			Servo_On();

			//Motor Position Control
			Position_Control_Mode_On();

		}


		/***       Auto Mode (Servo Off)         ***/
		else {
			//Motor Posion Control Off
			Motor_Auto_Off();
			Servo_Off();

			//Motor Position Control Reset
			Position_Control_Mode_Off();

		}
	}
}

void Position_Control_Mode_On()
{
	int16_t steer_pos_command;
	uint16_t steer_vel_command;

	uint16_t steer_acc_command = 60000;
	uint16_t steer_dec_command = 60000;

	/* Motor Position Mode Command */
	motor_cmd.status[4] = 0x01;

	/* User Position Command Data Parisng */
	/* Brake Command Percentage -> Degree Convert */
	steer_pos_command = (int16_t)(brk.AD_BRKMOT_Cmd_Pos * 4); //100% -> 40.0Deg

	/* User Velocity Command Data Parisng */
	steer_vel_command = brk.AD_BRKMOT_Cmd_Vel;


	/* User Position Command Data SafeLock */
	if(steer_pos_command > 400) steer_pos_command = 400;
	else if(steer_pos_command < 0) steer_pos_command = 0;


	/* Command Position Data Parsing */
	motor_cmd.user[0] = steer_pos_command;
	motor_cmd.user[1] = steer_pos_command >> 8;
	motor_cmd.user[2] = steer_vel_command;
	motor_cmd.user[3] = steer_vel_command >> 8;
	motor_cmd.user[4] = steer_acc_command;
	motor_cmd.user[5] = steer_acc_command >> 8;
	motor_cmd.user[6] = steer_dec_command;
	motor_cmd.user[7] = steer_dec_command >> 8;
}

void Position_Control_Mode_Off()
{
	int16_t steer_pos_command = 0;
	uint16_t steer_vel_command = 0;
	uint16_t steer_acc_command = 0;
	uint16_t steer_dec_command = 0;

	/* Motor Position Mode Command */
	motor_cmd.status[4] = 0x01;

	/* Command Position Data Parsing */
	motor_cmd.user[0] = steer_pos_command;
	motor_cmd.user[1] = steer_pos_command >> 8;
	motor_cmd.user[2] = steer_vel_command;
	motor_cmd.user[3] = steer_vel_command >> 8;
	motor_cmd.user[4] = steer_acc_command;
	motor_cmd.user[5] = steer_acc_command >> 8;
	motor_cmd.user[6] = steer_dec_command;
	motor_cmd.user[7] = steer_dec_command >> 8;
}



void Servo_On()
{
	motor_cmd.status[1] = TRUE;
}

void Servo_Off()
{
	motor_cmd.status[1] = FALSE;
}

void Motor_Auto_On()
{
	motor_cmd.status[0] = TRUE;
}

void Motor_Auto_Off()
{
	motor_cmd.status[0] = FALSE;
}

/***************************************/
/*         Accel / Brake Status        */
/***************************************/
void Module_Status_Task()
{
	/* 01 : Module Status Data Tx  */
	Module_Status_Sequnce();

	/* 02 : Debug Data Parsing  */
	Debug_Data_Parsing();

	/* 03 : Task Sequnce Timer [ 10ms ]  */
	SequenceTimer(&stat.timer_20ms, TIM_SEQUENCE_1, TIM_SEQUENCE_INIT_20);
}

void Module_Status_Sequnce()
{
	if(stat.timer_20ms == TIM_SEQUENCE_1) {
		/* 01 : BRK MOT STAT DATA Tx */
		TXCAN1(0x600, FDCAN_DLC_BYTES_8, brk.raw_data_0x600);

	} else if(stat.timer_20ms == TIM_SEQUENCE_2) {
		/* 02 : ACC STAT DATA Tx */
		TXCAN1(0x610, FDCAN_DLC_BYTES_7, acc.raw_data_0x610);

	} else if(stat.timer_20ms == TIM_SEQUENCE_3) {
		/* 02 : Debug DATA Tx */
		if(stat.debug_flag) TXCAN1(0x700, FDCAN_DLC_BYTES_8, stat.raw_data_0x700);

	} else if(stat.timer_20ms == TIM_SEQUENCE_4) {
		/* 04 : Alive Count */
		Status_Alive_Count();
	}
}

void Status_Alive_Count()
{
	if(stat.alive_cnt < 255) {
		stat.alive_cnt ++;

	} else {
		stat.alive_cnt = 0;

	}

	acc.TRZ_ACC_AliveCnt = stat.alive_cnt;
	brk.TRZ_BRKMOT_AliveCnt = stat.alive_cnt;
}

void Debug_Data_Parsing()
{
	memcpy(stat.raw_data_0x700, motor_info.pos_vel_curr_tor, 8);
}

