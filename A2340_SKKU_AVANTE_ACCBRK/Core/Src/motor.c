/*
 ****************************************************************************************
 * File Name          : motor.c                                                         *
 * Writer             : Choi Hanmin                                                     *
 * Maintainer         : Choi Hanmin                                                     *
 * Description        : This file provides DrSIM control code for the Treeze-Eng's VCU. *
 *                                                                                      *
 ****************************************************************************************
 * @attention                                                                           *
 *                                                                                      *
 * <h2><center>&copy; © COPYRIGHT 2022 TREEZE ENGINEERING CO.,LTD.                      *
 * ALL RIGHTS RESERVED.</center></h2>                                                   *
 *                                                                                      *
 ****************************************************************************************
 */
#include "motor.h"

uint8_t nmt_pre_operation[2] =	{0x80, MOTOR_NODE_ID};
uint8_t nmt_reset_operation[2] ={0x81, MOTOR_NODE_ID};
uint8_t nmt_operation[2] =		{0x01, MOTOR_NODE_ID};

uint8_t servo_on_status = MOTOR_OFF;

//MOTOR_NODE_ID

uint8_t encoder_data_save[8] = {CANOPEN_DATA_SIZE_4, 0x10, 0x10, 0x01, 0x73, 0x61, 0x76, 0x65};

uint8_t disable_watchdog[8] = {0x2B, 0x04, 0x50, 0x06, 0x00, 0x00, 0x00, 0x00};

uint8_t prepare_command[6] = {CANOPEN_DATA_SIZE_2, 0x40, 0x60, 0x00, 0x2F, 0x00};
uint8_t off_command[6] =	 {CANOPEN_DATA_SIZE_2, 0x40, 0x60, 0x00, 0x00, 0x00};

uint8_t ctlword_prepare[3] = { 0x2F, 0x00, 0x01};
uint8_t T_ctlword_prepare[3] = { 0x0F, 0x00, 0x04};
uint8_t ctlword_begin[3] = { 0x1F, 0x00, 0x01 };
uint8_t T_ctlword_begin[3] = { 0x1F, 0x00, 0x04 };
/*
 * Position mode
 */
uint8_t ctlword_servo_on[3] = { 0x0F, 0x00, 0x01};
uint8_t ctlword_servo_RDY[3] = { 0x07, 0x00, 0x01};
uint8_t ctlword_servo_off[3] = { 0x06, 0x00, 0x01};
uint8_t ctlword_alarm_reset[3] = { 0x80, 0x00, 0x01};
/*
 * Toruqe mode
 */
uint8_t T_ctlword_servo_on[3] = { 0x0F, 0x00, 0x04};
uint8_t T_ctlword_servo_RDY[3] = { 0x07, 0x00, 0x04};
uint8_t T_ctlword_servo_off[3] = { 0x06, 0x00, 0x04};
uint8_t T_ctlword_alarm_reset[3] = { 0x80, 0x00, 0x04};

uint8_t Torquevalue[8] = 		{CANOPEN_DATA_SIZE_2, 0x71, 0x60, 0x00, 0x64, 0x00, 0x00, 0x00};
uint8_t TorquespeedLimit[8] = 	{CANOPEN_DATA_SIZE_4, 0x80, 0x60, 0x00, 0xE8, 0x03, 0x00, 0x00};
uint8_t Torquerate[8] = 		{CANOPEN_DATA_SIZE_4, 0x87, 0x60, 0x00, 0x64, 0x00, 0x00, 0x00};

//Type 1
uint8_t ctlword_encoder_init_prepare[3] = { 0x0F, 0x00, 0x06};
uint8_t ctlword_encoder_init_command[5] = { 0x23, 0x00, 0x00, 0x00, 0x00};
//uint8_t ctlword_encoder_init_command[5] = { 0x23, 0x88, 0x13, 0x00, 0x00}; // offset 180deg
uint8_t ctlword_encoder_init_begin[3] = { 0x1F, 0x00, 0x06};
uint8_t ctlword_encoder_init_MotorOFF[3] = { 0x07, 0x00, 0x01};

uint8_t user_init_cmd[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

//Type 2
uint8_t ctlword_encoder_init_prepare_Type2[3]  = { 0x0F, 0x00, 0x06 };
uint8_t ctlword_encoder_init_command_Type2[5]  = { 0x23, 0x00, 0x00, 0x00, 0x00 };
uint8_t ctlword_encoder_init_prepare_1_Type2[3]  = { 0x07, 0x00, 0x06 };
uint8_t ctlword_encoder_init_prepare_2_Type2[3]  = { 0x06, 0x00, 0x06 };
uint8_t ctlword_encoder_init_begin_Type2[3] 	 = { 0x1F, 0x00, 0x06 };
uint8_t ctlword_encoder_init_MotorOFF_Type2[3] = { 0x07, 0x00, 0x01 };
uint8_t eeprom_data_save[8]	= { CANOPEN_DATA_SIZE_4, 0x10, 0x10, 0x01, 0x73, 0x61, 0x76, 0x65 };

/*
 *  mode change
 */

uint8_t ctlword_mode_change_position[3] = { 0x07, 0x00, 0x01};
uint8_t ctlword_mode_change_torque[3] = { 0x07, 0x00, 0x04};

/*
 * Set Motor Position Mode
 */
uint8_t set_mode_position[5] =		{CANOPEN_DATA_SIZE_1, 0x60, 0x60, 0x00, 0x01};
uint8_t set_profile_speed[8] =		{CANOPEN_DATA_SIZE_4, 0x81, 0x60, 0x00, 0x10, 0x27, 0x00, 0x00};
uint8_t set_acceleration[8] =		{CANOPEN_DATA_SIZE_4, 0x83, 0x60, 0x00, 0x10, 0x27, 0x00, 0x00};
uint8_t set_deceleration[8] =		{CANOPEN_DATA_SIZE_4, 0x84, 0x60, 0x00, 0x10, 0x27, 0x00, 0x00};
uint8_t set_target_position[8] =	{CANOPEN_DATA_SIZE_4, 0x7A, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00};


/*
 * RECEIVE PDO SETTING
 */


/*
 * Set Motor Position/Velocity PDO Mapping
 */
uint8_t rx_cobid_pos_vel[8] =		{CANOPEN_DATA_SIZE_4, 0x00, 0x14, 0x01, MOTOR_NODE_ID, 0x02, 0x00, 0xC0};
uint8_t rx_mapping_pos_vel[5] =		{CANOPEN_DATA_SIZE_1, 0x00, 0x16, 0x00, 0x00 };
uint8_t rx_pos_cmd[8] =				{CANOPEN_DATA_SIZE_4, 0x00, 0x16, 0x01, 0x20, 0x00, 0x7A, 0x60};
uint8_t rx_vel_cmd[8] =				{CANOPEN_DATA_SIZE_4, 0x00, 0x16, 0x02, 0x20, 0x00, 0x81, 0x60};
uint8_t rx_mapping_pos_vel_set[5] =	{CANOPEN_DATA_SIZE_1, 0x00, 0x16, 0x00, 0x02 };
uint8_t rx_com_pos_vel[8] =			{CANOPEN_DATA_SIZE_4, 0x00, 0x14, 0x01, MOTOR_NODE_ID, 0x02, 0x00, 0x00};
uint8_t rx_tim_pos_vel[5] =			{CANOPEN_DATA_SIZE_1, 0x00, 0x14, 0x02, 0xFF};
uint8_t rx_tim_pd_pos_vel[6] =		{CANOPEN_DATA_SIZE_2, 0x00, 0x14, 0x05, 0x00, 0x00 };

/*
 * Set Motor Accel/Decel PDO Mapping
 */
uint8_t rx_cobid_acc_dec[8] =		{ CANOPEN_DATA_SIZE_4, 0x01, 0x14, 0x01, MOTOR_NODE_ID, 0x03, 0x00, 0xC0 };
uint8_t rx_mapping_acc_dec[5] =		{ CANOPEN_DATA_SIZE_1, 0x01, 0x16, 0x00, 0x00 };
uint8_t rx_acc_cmd[8] =				{ CANOPEN_DATA_SIZE_4, 0x01, 0x16, 0x01, 0x20, 0x00, 0x83, 0x60 };
uint8_t rx_dec_cmd[8] =				{ CANOPEN_DATA_SIZE_4, 0x01, 0x16, 0x02, 0x20, 0x00, 0x84, 0x60 };
uint8_t rx_mapping_acc_dec_set[5] =	{ CANOPEN_DATA_SIZE_1, 0x01, 0x16, 0x00, 0x02 };
uint8_t rx_com_acc_dec[8] =			{ CANOPEN_DATA_SIZE_4, 0x01, 0x14, 0x01, MOTOR_NODE_ID, 0x03, 0x00, 0x00 };
uint8_t rx_tim_acc_dec[5] =			{ CANOPEN_DATA_SIZE_1, 0x01, 0x14, 0x02, 0xFF };
uint8_t rx_tim_pd_acc_dec[6] =		{ CANOPEN_DATA_SIZE_2, 0x01, 0x14, 0x05, 0x00, 0x00 };

/*
 * Set Motor ControlWord PDO Mapping
 */
uint8_t rx_cobid_ctlword[8] =		{CANOPEN_DATA_SIZE_4, 0x02, 0x14, 0x01, MOTOR_NODE_ID, 0x04, 0x00, 0xC0};
uint8_t rx_mapping_ctlword[5] =		{CANOPEN_DATA_SIZE_1, 0x02, 0x16, 0x00, 0x00};
uint8_t rx_ctlword_cmd[8] =			{CANOPEN_DATA_SIZE_4, 0x02, 0x16, 0x01, 0x10, 0x00, 0x40, 0x60};
uint8_t rx_opermode_cmd[8] =		{CANOPEN_DATA_SIZE_4, 0x02, 0x16, 0x02, 0x08, 0x00, 0x60, 0x60};
uint8_t rx_mapping_ctlword_set[5] =	{CANOPEN_DATA_SIZE_1, 0x02, 0x16, 0x00, 0x02};
uint8_t rx_com_ctlword[8] =			{CANOPEN_DATA_SIZE_4, 0x02, 0x14, 0x01, MOTOR_NODE_ID, 0x04, 0x00, 0x00};
uint8_t rx_tim_ctlword[5] =			{CANOPEN_DATA_SIZE_1, 0x02, 0x14, 0x02, 0xFF};
uint8_t rx_tim_pd_ctlword[6] =		{CANOPEN_DATA_SIZE_2, 0x02, 0x14, 0x05, 0x00, 0x00};

/*
 * Set Motor HomeOffset PDO Mapping
 */
uint8_t rx_cobid_homeoffset[8] =		{CANOPEN_DATA_SIZE_4, 0x03, 0x14, 0x01, MOTOR_NODE_ID, 0x05, 0x00, 0xC0};
uint8_t rx_mapping_homeoffset[5] =		{CANOPEN_DATA_SIZE_1, 0x03, 0x16, 0x00, 0x00};
uint8_t rx_homemode_cmd[8] =			{CANOPEN_DATA_SIZE_4, 0x03, 0x16, 0x01, 0x08, 0x00, 0x98, 0x60};
uint8_t rx_homeoffset_cmd[8] =			{CANOPEN_DATA_SIZE_4, 0x03, 0x16, 0x02, 0x20, 0x00, 0x7C, 0x60};
uint8_t rx_mapping_homeoffset_set[5] =	{CANOPEN_DATA_SIZE_1, 0x03, 0x16, 0x00, 0x02};
uint8_t rx_com_homeoffset[8] =			{CANOPEN_DATA_SIZE_4, 0x03, 0x14, 0x01, MOTOR_NODE_ID, 0x05, 0x00, 0x00};
uint8_t rx_tim_homeoffset[5] =			{CANOPEN_DATA_SIZE_1, 0x03, 0x14, 0x02, 0xFF};
uint8_t rx_tim_pd_homeoffset[6] =		{CANOPEN_DATA_SIZE_2, 0x03, 0x14, 0x05, 0x00, 0x00};


/*
 * TRANSMIT PDO SETTING
 */

/*
 * Set Motor Info Position/Velocity PDO Mapping
 */
uint8_t tx_cobid_pos_vel[8] =		{CANOPEN_DATA_SIZE_4, 0x00, 0x18, 0x01, 0x80+MOTOR_NODE_ID, 0x01, 0x00, 0xC0};
uint8_t tx_mapping_pos_vel[5] =		{CANOPEN_DATA_SIZE_1, 0x00, 0x1A, 0x00, 0x00};
uint8_t tx_pos_info[8] =			{CANOPEN_DATA_SIZE_4, 0x00, 0x1A, 0x01, 0x20, 0x00, 0x64, 0x60};
uint8_t tx_vel_info[8] = 			{CANOPEN_DATA_SIZE_4, 0x00, 0x1A, 0x02, 0x20, 0x00, 0x6C, 0x60};
uint8_t tx_mapping_pos_vel_set[5] = {CANOPEN_DATA_SIZE_1, 0x00, 0x1A, 0x00, 0x02};
uint8_t tx_com_pos_vel[8] =			{CANOPEN_DATA_SIZE_4, 0x00, 0x18, 0x01, 0x80+MOTOR_NODE_ID, 0x01, 0x00, 0x00};
uint8_t tx_tim_pos_vel[5] =			{CANOPEN_DATA_SIZE_1, 0x00, 0x18, 0x02, 0xFF};
uint8_t tx_tim_pd_pos_vel[6]= 		{CANOPEN_DATA_SIZE_2, 0x00, 0x18, 0x05, 0x0A, 0x00};

/*
 * Set Motor Info Torque/Current PDO Mapping
 */
uint8_t tx_cobid_tor_cur[8] =		{CANOPEN_DATA_SIZE_4, 0x01, 0x18, 0x01, 0x80+MOTOR_NODE_ID, 0x02, 0x00, 0xC0};
uint8_t tx_mapping_tor_cur[5] =		{CANOPEN_DATA_SIZE_1, 0x01, 0x1A, 0x00, 0x00};
uint8_t tx_tor_info[8] =			{CANOPEN_DATA_SIZE_4, 0x01, 0x1A, 0x01, 0x10, 0x00, 0x77, 0x60};
uint8_t tx_cur_info[8] =	 		{CANOPEN_DATA_SIZE_4, 0x01, 0x1A, 0x02, 0x10, 0x00, 0x78, 0x60};
uint8_t tx_mapping_tor_cur_set[5] =	{CANOPEN_DATA_SIZE_1, 0x01, 0x1A, 0x00, 0x02};
uint8_t tx_com_tor_cur[8]=			{CANOPEN_DATA_SIZE_4, 0x01, 0x18, 0x01, 0x80+MOTOR_NODE_ID, 0x02, 0x00, 0x00};
uint8_t tx_tim_tor_cur[5] =			{CANOPEN_DATA_SIZE_1, 0x01, 0x18, 0x02, 0xFF};
uint8_t tx_tim_pd_tor_cur[6] =		{CANOPEN_DATA_SIZE_2, 0x01, 0x18, 0x05, 0x0A, 0x00};

/*
 * Set Motor Info Status/Mode PDO Mapping
 */
uint8_t tx_cobid_status[8] =		{CANOPEN_DATA_SIZE_4, 0x02, 0x18, 0x01, 0x80+MOTOR_NODE_ID, 0x03, 0x00, 0xC0};
uint8_t tx_mapping_status[5] =		{CANOPEN_DATA_SIZE_1, 0x02, 0x1A, 0x00, 0x00};
uint8_t tx_status_info[8] =			{CANOPEN_DATA_SIZE_4, 0x02, 0x1A, 0x01, 0x10, 0x00, 0x41, 0x60};
uint8_t tx_modestatus_info[8] =		{CANOPEN_DATA_SIZE_4, 0x02, 0x1A, 0x02, 0x08, 0x00, 0x61, 0x60};
uint8_t tx_errorstatus_info[8] =	{CANOPEN_DATA_SIZE_4, 0x02, 0x1A, 0x03, 0x08, 0x00, 0x01, 0x10};
uint8_t tx_mapping_status_set[5] =	{CANOPEN_DATA_SIZE_1, 0x02, 0x1A, 0x00, 0x03};
uint8_t tx_com_status[8]=			{CANOPEN_DATA_SIZE_4, 0x02, 0x18, 0x01, 0x80+MOTOR_NODE_ID, 0x03, 0x00, 0x00};
uint8_t tx_tim_status[5] =			{CANOPEN_DATA_SIZE_1, 0x02, 0x18, 0x02, 0xFF};
uint8_t tx_tim_pd_status[6] =		{CANOPEN_DATA_SIZE_2, 0x02, 0x18, 0x05, 0x0A, 0x00};

void MotionControlControl(struct ControlCommand *cmd, struct StatusInformation *info)
{
	if( cmd->timer == 2) {

		CanErrorCheck(&hfdcan1, &CAN_ERROR_cnt);
		CanErrorCheck(&hfdcan2, &CAN_ERROR_cnt);


	} else if( cmd->timer == 3 ) {

		//Motor Driver Type 1
#if 1
		if(cmd->encoder_init_flag) {
			if (cmd->init_index == MOTOR_INIT_PREPARE) {
				MotorEncoderInitPrepare();
				cmd->init_index = MOTOR_INIT_COMMAND;
			} else if (cmd->init_index == MOTOR_INIT_COMMAND) {
				MotorEncoderInitCommand();
				cmd->init_index = MOTOR_INIT_START;
			} else if (cmd->init_index == MOTOR_INIT_START) {
				MotorEncoderInitBegin();
				cmd->init_index = MOTOR_INIT_MOTOR_OFF;
			} else if (cmd->init_index == MOTOR_INIT_MOTOR_OFF) {
				MotorEncoderInitMOTOROFF();
				cmd->init_index = MOTOR_INIT_SAVE;
			}else if (cmd->init_index == MOTOR_INIT_SAVE) {
				cmd->save_flag = 1;
				MotorSetCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, encoder_data_save, &cmd->save_flag);
				cmd->init_index = MOTOR_INIT_PREPARE;
				cmd->encoder_init_flag = FALSE;
			}
		}
#endif

		//Motor Driver Type 2
#if 0
		if(cmd -> encoder_init_flag) {
			if(cmd -> init_index == 0) {
				MotorEncoderInitPrepare_Type2();
				MotorEncoderInitCommand_Type2();
				cmd -> init_index = 1;
			}
			else if(cmd -> init_index == 1) {
				MotorEncoderInitPrepare_2_Type2();
				cmd -> init_index = 2;
			}
			else if(cmd -> init_index == 2) {
				MotorEncoderInitPrepare_1_Type2();
				cmd -> init_index = 3;
			}
			else if(cmd -> init_index == 3) {
				MotorEncoderInitPrepare_Type2();
				cmd -> init_index = 4;
			}
			else if(cmd -> init_index == 4) {
				MotorEncoderInitBegin_Type2();
				cmd -> init_index = 5;
			}
			else if(cmd -> init_index == 5) {
				MotorEncoderInitMOTOROFF_Type2();
				cmd -> init_index = 6;

			}
			else if(cmd -> init_index == 6) {
				cmd -> save_flag = 1;
				MotorSetCommand(0x600 + 3, FDCAN_DLC_BYTES_8, eeprom_data_save, &cmd -> save_flag);

				cmd -> init_index = 0;
				cmd -> encoder_init_flag = FALSE;
			}

		}
#endif

if(cmd->alarm_reset_flag){

	MotorAlarmReset();
	cmd->alarm_reset_flag = FALSE;
}

	} else if( cmd->timer == 6) {
		MotorCommandFlagUpdate(cmd, info);
		TorqueCommandConverter(cmd);

		MotorServeOnOff(cmd);

	} else if((cmd->timer == 7) || (cmd->timer == 8) || (cmd->timer == 9) ) {
		/*
		 * position mode
		 */
		if( cmd->user_flag && cmd->auto_mode_flag && (cmd->cotrol_mode == 1)) {
			if(cmd->cmd_index == MOTOR_PREPARE) {
				MotorPrepare();
				cmd->cmd_index = MOTOR_NEW_POS_SET;

			} else if ( cmd->cmd_index == MOTOR_NEW_POS_SET ) {
				MotorCommandConverter(cmd);
				MotorControlCommand(POS_VEL_CONTROL_ID, FDCAN_DLC_BYTES_8, cmd->pos_vel, &cmd->pos_vel_flag);
				MotorSetCommand(ACC_DEC_CONTROL_ID, FDCAN_DLC_BYTES_8, cmd->acc_dec, &cmd->acc_dec_flag);
				cmd->cmd_index = MOTOR_START;

			} else if ( cmd->cmd_index == MOTOR_START ) {
				MotorStart();
				cmd->cmd_index = MOTOR_PREPARE;
				cmd->user_flag = FALSE;
			}
		}
		/*
		 * Torque mode
		 */
		if( cmd->user_flag && cmd->auto_mode_flag && (cmd->cotrol_mode == 4)) {
			if(cmd->cmd_index == MOTOR_PREPARE) {
				T_MotorPrepare();
				MotorSetCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, Torquevalue, &cmd->torquevalue_flag);
				cmd->cmd_index = MOTOR_NEW_POS_SET;

			} else if ( cmd->cmd_index == MOTOR_NEW_POS_SET ) {
				MotorCommandConverter(cmd);
				MotorSetCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, TorquespeedLimit, &cmd->torquespeedlimit_flag);
				cmd->cmd_index = MOTOR_START;

			} else if ( cmd->cmd_index == MOTOR_START ) {
				MotorSetCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, Torquerate, &cmd->torquerate_flag);
				T_MotorStart();
				cmd->cmd_index = MOTOR_PREPARE;
				cmd->user_flag = FALSE;
			}
		}
	} else if( cmd->timer == 0) {
		MotorInfoConverter(info);
		if(feq_20ms){
			//TEST_DISABLE
			//MotorInofrmation(0x400, FDCAN_DLC_BYTES_8, info->pos_vel_curr_tor);
		}
	} else if( cmd->timer == 1) {
		if(feq_20ms){
			//TEST_DISABLE
			//MotorInofrmation(0x410, FDCAN_DLC_BYTES_6, info->status);
			feq_20ms = 0;
		}else{
			feq_20ms = 1;
		}
	}

	MotorSequnceTimer(&cmd->timer, MOTOR_TIMER_INIT, MOTOR_TIMER_MAX);

}

void MotorCommandFlagUpdate(struct ControlCommand *cmd, struct StatusInformation *info )
{
	if (cmd->auto_mode_flag) {

		if(!cmd->encoder_init_flag)cmd->user_flag = TRUE;

		if ((cmd->user[0] == cmd->user_prev[0])
				&& (cmd->user[1] == cmd->user_prev[1])
				&& (cmd->user[2] == cmd->user_prev[2])
				&& (cmd->user[3] == cmd->user_prev[3])
				&& (cmd->user[4] == cmd->user_prev[4])
				&& (cmd->user[5] == cmd->user_prev[5])
				&& (cmd->user[6] == cmd->user_prev[6])
				&& (cmd->user[7] == cmd->user_prev[7])) {
		} else {

			cmd->user_flag = TRUE;

			if ((cmd->user[0] == cmd->user_prev[0])
					&& (cmd->user[1] == cmd->user_prev[1])
					&& (cmd->user[2] == cmd->user_prev[2])
					&& (cmd->user[3] == cmd->user_prev[3])) {
			} else {
				cmd->pos_vel_flag = TRUE;
			}
			if ((cmd->user[4] == cmd->user_prev[4])
					&& (cmd->user[5] == cmd->user_prev[5])
					&& (cmd->user[6] == cmd->user_prev[6])
					&& (cmd->user[7] == cmd->user_prev[7])) {
			} else {
				cmd->acc_dec_flag = TRUE;
			}

		}
		memcpy(cmd->user_prev, cmd->user, 8);
	} else {
		memcpy(cmd->user, info->pos_vel_raw, 8);
		memcpy(cmd->user_prev, cmd->user, 8);

	}


	if( ((cmd->status[0] == cmd->status_prev[0]) &&
			(cmd->status[1] == cmd->status_prev[1]) &&
			(cmd->status[2] == cmd->status_prev[2]) &&
			(cmd->status[3] == cmd->status_prev[3]) &&
			(cmd->status[4] == cmd->status_prev[4]) &&
			(cmd->status[5] == cmd->status_prev[5]) &&
			(cmd->status[6] == cmd->status_prev[6]) &&
			(cmd->status[7] == cmd->status_prev[7])) && (!info->clutch_flag) ) {
		if(info->clutch) {
			info->clutch_flag = TRUE;
		}
	} else {
		if( cmd->status[0] != cmd->status_prev[0] ) {
			/* auto mode command */
			cmd->user_auto_mode = cmd->status[0];
			info->auto_mode = cmd->user_auto_mode;
		}

		if( (cmd->status[1] != cmd->status_prev[1]) ) {
			/* servo on off command */
			cmd->servo_on_off = cmd->status[1];
			cmd->servo_on_off_flag = TRUE;
			info->servo_on_off = cmd->servo_on_off;
			info->clutch = FALSE;

			//cmd->encoder_init_flag = TRUE;
		}

		if( cmd->status[2] != cmd->status_prev[2]) {
			/* alarm reset command */
			if( cmd->status[2] == TRUE ) {
				cmd->alarm_reset_flag = TRUE;
			}
		}
		if( cmd->status[3] != cmd->status_prev[3]) {
			/* encoder init command */
			if( cmd->status[3] == TRUE ) {
				cmd->encoder_init_flag = TRUE;
			}
		}
	}
	memcpy(cmd->status_prev, cmd->status, 8);


	cmd->cotrol_mode = cmd->status[4];
	if(cmd->cotrol_mode != cmd->cotrol_mode_prev){
		switch(cmd->cotrol_mode){
		case 1:
			MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_mode_change_position);
			break;

		case 4:
			MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_mode_change_torque);
			break;
		}
	}
	cmd->cotrol_mode_prev = cmd->cotrol_mode;
}

/* motor config setting */
void MotorInit()
{
	DisableMotorWatchdog();
	MotorOperation();
	MotorSetPositionMode();

	/*
	 * Receive PDO Setting
	 */
	MotorPDOMappingPosVel(); // 20xh
	MotorPDOMappingAccDec(); // 30xh
	MotorPDOMappingCtrlWord();// 40xh
	MotorPDOMappingHomeOffest();// 50xh

	/*
	 * Tranmit PDO Setting
	 */
	MotorPDOMappingPosVelInfo(); //18xh
	MotorPDOMappingTorCurrInfo(); //28xh
	MotorPDOMappingStatusInfo(); //38xh


	MotorAccDecInit(); // Init acc,dec if no command
	motor_cmd.encoder_init_flag = MOTOR_INIT_PREPARE;
}

void DisableMotorWatchdog()
{
	MotorInitCommand(NMT_CMD_ID, FDCAN_DLC_BYTES_2, nmt_pre_operation);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, disable_watchdog);
	HAL_Delay(1);
}

void MotorOperation()
{
	MotorInitCommand(NMT_CMD_ID, FDCAN_DLC_BYTES_2, nmt_operation);
	HAL_Delay(1);
}

void MotorResetOperation()
{
	MotorInitCommand(NMT_CMD_ID, FDCAN_DLC_BYTES_2, nmt_reset_operation);
	HAL_Delay(1);
}

void MotorSetPositionMode()
{
	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, set_mode_position);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, set_profile_speed);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, set_acceleration);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, set_deceleration);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, prepare_command);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, set_target_position);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, off_command);
	HAL_Delay(1);
}

void MotorPDOMappingPosVel()
{
	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_cobid_pos_vel);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_mapping_pos_vel);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_pos_cmd);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_vel_cmd);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_mapping_pos_vel_set);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_com_pos_vel);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_tim_pos_vel);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_6, rx_tim_pd_pos_vel);
	HAL_Delay(10);
}

void MotorPDOMappingAccDec()
{
	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_cobid_acc_dec);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_mapping_acc_dec);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_acc_cmd);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_dec_cmd);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_mapping_acc_dec_set);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_com_acc_dec);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_tim_acc_dec);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_6, rx_tim_pd_acc_dec);
	HAL_Delay(10);
}

void MotorPDOMappingCtrlWord()
{
	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_cobid_ctlword);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_mapping_ctlword);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_ctlword_cmd);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_opermode_cmd);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_mapping_ctlword_set);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_com_ctlword);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_tim_ctlword);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_tim_pd_ctlword);
	HAL_Delay(10);
}

void MotorPDOMappingHomeOffest()
{
	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_cobid_homeoffset);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_mapping_homeoffset);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_homemode_cmd);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_homeoffset_cmd);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_mapping_homeoffset_set);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, rx_com_homeoffset);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_tim_homeoffset);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, rx_tim_pd_homeoffset);
	HAL_Delay(10);
}

void MotorPDOMappingPosVelInfo()
{
	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_cobid_pos_vel);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, tx_mapping_pos_vel);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_pos_info);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_vel_info);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, tx_mapping_pos_vel_set);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_com_pos_vel);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, tx_tim_pos_vel);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_6, tx_tim_pd_pos_vel);
	HAL_Delay(1);
}

void MotorPDOMappingTorCurrInfo()
{
	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_cobid_tor_cur);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, tx_mapping_tor_cur);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_tor_info);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_cur_info);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, tx_mapping_tor_cur_set);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_com_tor_cur);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, tx_tim_tor_cur);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_6, tx_tim_pd_tor_cur);
	HAL_Delay(1);
}

void MotorPDOMappingStatusInfo()
{
	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_cobid_status);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, tx_mapping_status);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_status_info);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_modestatus_info);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_errorstatus_info);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, tx_mapping_status_set);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_8, tx_com_status);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_5, tx_tim_status);
	HAL_Delay(1);

	MotorInitCommand(SDO_CMD_ID, FDCAN_DLC_BYTES_6, tx_tim_pd_status);
	HAL_Delay(1);
}

void MotorAccDecInit()
{
	motor_cmd.acc_dec[0] = 0xFF;
	motor_cmd.acc_dec[1] = 0xFF;
	motor_cmd.acc_dec[2] = 0x01;
	motor_cmd.acc_dec[3] = 0;
	motor_cmd.acc_dec[4] = 0xFF;
	motor_cmd.acc_dec[5] = 0xFF;
	motor_cmd.acc_dec[6] = 0x01;
	motor_cmd.acc_dec[7] = 0;

}
//Motor Driver Type2
void MotorEncoderInitPrepare_Type2()
{
	MotorInitCommand(0x400 + 3, FDCAN_DLC_BYTES_3, ctlword_encoder_init_prepare_Type2);
}

void MotorEncoderInitCommand_Type2()
{
	MotorInitCommand(0x500 + 3, FDCAN_DLC_BYTES_5, ctlword_encoder_init_command_Type2);
}

void MotorEncoderInitPrepare_1_Type2()
{
	MotorInitCommand(0x400 + 3, FDCAN_DLC_BYTES_3, ctlword_encoder_init_prepare_1_Type2);
}

void MotorEncoderInitPrepare_2_Type2()
{
	MotorInitCommand(0x400 + 3, FDCAN_DLC_BYTES_3, ctlword_encoder_init_prepare_2_Type2);
}

void MotorEncoderInitBegin_Type2()
{
	MotorInitCommand(0x400 + 3, FDCAN_DLC_BYTES_3, ctlword_encoder_init_begin_Type2);
}

void MotorEncoderInitMOTOROFF_Type2()
{
	MotorInitCommand(0x400 + 3, FDCAN_DLC_BYTES_3, ctlword_encoder_init_MotorOFF_Type2);
}

//Motor Driver Type1
void MotorEncoderInitPrepare()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_encoder_init_prepare);
}

void MotorEncoderInitCommand()
{
	MotorInitCommand(HOMING_CONTROL_ID, FDCAN_DLC_BYTES_5, ctlword_encoder_init_command);
}

void MotorEncoderInitBegin()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_encoder_init_begin);
}


void MotorEncoderInitMOTOROFF()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_encoder_init_MotorOFF);
}

void MotorEncoderInitMOTOROFF2()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_servo_off);
}

void MotorAlarmReset()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_alarm_reset);
}

void MotorServeOnOff(struct ControlCommand *cmd)
{
	if( cmd->servo_on_off_flag ) {
		if (cmd->servo_on_off && cmd->user_auto_mode) {

			if(cmd->cotrol_mode == 4){
				T_mode_MotorServoOn(cmd);
			}else{
				P_mode_MotorServoOn(cmd);
			}
		} else {
			if(cmd->cotrol_mode == 4){
				T_mode_MotorServoOff();
			}else{
				P_mode_MotorServoOff();
			}
			cmd->auto_mode_flag = FALSE;
		}

		cmd->servo_on_off_flag = FALSE;
	} else if( (!cmd->status[0]) && (cmd->status[1]) ) {
		if(cmd->cotrol_mode == 4){
			T_mode_MotorServoOff();
		}else{
			P_mode_MotorServoOff();
		}
		cmd->auto_mode_flag = FALSE;
	}
}

void P_mode_MotorServoOn()
{
	if(servo_on_status == MOTOR_OFF){
		MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_servo_off);
		servo_on_status = MOTOR_RDY;
	}
	if(servo_on_status == MOTOR_RDY){
		MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_servo_RDY);
		servo_on_status = MOTOR_ON;
	}

	if(servo_on_status == MOTOR_ON){
		MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_servo_on);
		motor_cmd.auto_mode_flag = TRUE;
	}


}

void P_mode_MotorServoOff()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_servo_off);
	servo_on_status = MOTOR_OFF;
}

void T_mode_MotorServoOn()
{
	if(servo_on_status == MOTOR_OFF){
		MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, T_ctlword_servo_off);
		servo_on_status = MOTOR_RDY;
	}
	if(servo_on_status == MOTOR_RDY){
		MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, T_ctlword_servo_RDY);
		servo_on_status = MOTOR_ON;
	}

	if(servo_on_status == MOTOR_ON){
		MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, T_ctlword_servo_on);
		motor_cmd.auto_mode_flag = TRUE;
	}


}

void T_mode_MotorServoOff()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, T_ctlword_servo_off);
	servo_on_status = MOTOR_OFF;
}

void TorqueCommandConverter(struct ControlCommand *cmd)
{
	cmd->torque_cmd = (int16_t)((cmd->torque[1] << 8) | cmd->torque[0]);

	if(cmd->torque_cmd != cmd->torque_cmd_prev){


		Torquevalue[4] = cmd->torque[0];
		Torquevalue[5] = cmd->torque[1];

		TorquespeedLimit[4] = cmd->torque[2];
		TorquespeedLimit[5] = cmd->torque[3];
		TorquespeedLimit[6] = cmd->torque[4];
		TorquespeedLimit[7] = cmd->torque[5];

		Torquerate[4] = cmd->torque[6];
		Torquerate[5] = cmd->torque[7];


		cmd->user_flag = TRUE;
		cmd->torquevalue_flag = TRUE;
		cmd->torquespeedlimit_flag = TRUE;
		cmd->torquerate_flag = TRUE;

	}

	cmd->torque_cmd_prev = cmd->torque_cmd;
}

void MotorPrepare()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_prepare);

}

void T_MotorPrepare()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, T_ctlword_prepare);

}

void MotorStart()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, ctlword_begin);
}

void T_MotorStart()
{
	MotorInitCommand(CONTROL_WORD_ID, FDCAN_DLC_BYTES_3, T_ctlword_begin);
}

void MotorCommandConverter(struct ControlCommand *cmd)
{
	//float _gear_ratio = 5;
	float _gear_ratio = 10; // SKKU AVANTE Gear Ratio 1 : 10
	float _motor_pulse = 10000;
	float _motor_gear_ratio = _motor_pulse * _gear_ratio / 360;
	float _deg_to_rpm = 0.166667;
	float _rpm_to_pulse = 166.66666667;

	cmd->pos_int16 = (int16_t)(cmd->user[1] << 8) | cmd->user[0];
	cmd->pos_int32 = cmd->pos_int16;

	cmd->pos_float = (float)cmd->pos_int32 * _motor_gear_ratio * 0.1;
	cmd->pos_int32 = (int32_t)cmd->pos_float;

	cmd->pos_vel[3] = cmd->pos_int32 >> 24;
	cmd->pos_vel[2] = cmd->pos_int32 >> 16;
	cmd->pos_vel[1] = cmd->pos_int32 >> 8;
	cmd->pos_vel[0] = cmd->pos_int32;

	cmd->vel_int32 = (int16_t)(cmd->user[3] << 8) | cmd->user[2];
	cmd->vel_float = (float)cmd->vel_int32 * _deg_to_rpm;
	cmd->vel_float = cmd->vel_float * _gear_ratio;
	cmd->vel_float = cmd->vel_float * _rpm_to_pulse;
	cmd->vel_int32 = (int32_t)cmd->vel_float;

	cmd->pos_vel[7] = cmd->vel_int32 >> 24;
	cmd->pos_vel[6] = cmd->vel_int32 >> 16;
	cmd->pos_vel[5] = cmd->vel_int32 >> 8;
	cmd->pos_vel[4] = cmd->vel_int32;

	//cmd->acc_int32 = (cmd->user[5] << 8) | cmd->user[4]; //NOT USED
	cmd->acc_int32 = 3000;
	cmd->acc_float = (float)cmd->acc_int32 * _deg_to_rpm;
	cmd->acc_float = cmd->acc_float * _gear_ratio;
	cmd->acc_float = cmd->acc_float * _rpm_to_pulse;
	cmd->acc_int32 = (int32_t)cmd->acc_float;

	cmd->acc_dec[3] = cmd->acc_int32 >> 24;
	cmd->acc_dec[2] = cmd->acc_int32>> 16;
	cmd->acc_dec[1] = cmd->acc_int32 >> 8;
	cmd->acc_dec[0] = cmd->acc_int32;

	//cmd->dec_int32 = (cmd->user[7] << 8) | cmd->user[6]; //NOT USED
	cmd->dec_int32 = 3000;
	cmd->dec_float = (float)cmd->dec_int32 * _deg_to_rpm;
	cmd->dec_float = cmd->dec_float * _gear_ratio;
	cmd->dec_float = cmd->dec_float * _rpm_to_pulse;
	cmd->dec_int32 = (int32_t)cmd->dec_float;

	cmd->acc_dec[7] = cmd->dec_int32 >> 24;
	cmd->acc_dec[6] = cmd->dec_int32>> 16;
	cmd->acc_dec[5] = cmd->dec_int32 >> 8;
	cmd->acc_dec[4] = cmd->dec_int32;

	cmd->pos_vel_flag = TRUE;
	cmd->acc_dec_flag = TRUE;
}

void MotorInfoConverter(struct StatusInformation *info)
{
	//float _gear_ratio = 1;
	float _gear_ratio = 10; // SKKU AVANTE Gear Ratio 1 : 10
	float _motor_pulse = 10000;
	float _motor_gear_ratio = _motor_pulse * _gear_ratio / 360;
	float _deg_to_rpm = 0.166667;
	float _rpm_to_pulse = 166.66666667;

	info->pos_int32 = (info->pos_vel_raw[3] << 24) | (info->pos_vel_raw[2] << 16) | (info->pos_vel_raw[1] << 8) | (info->pos_vel_raw[0]);
	info->pos_float = (float)info->pos_int32;
	info->pos_float = info->pos_float / _motor_gear_ratio * 10;
	info->pos_int32 = (int32_t)info->pos_float;
	info->pos_vel_curr_tor[0] = info->pos_int32;
	info->pos_vel_curr_tor[1] = info->pos_int32 >> 8;

	info->vel_int32 = (info->pos_vel_raw[7] << 24) | (info->pos_vel_raw[6] << 16) | (info->pos_vel_raw[5] << 8) | (info->pos_vel_raw[4]);
	info->vel_float = (float)info->vel_int32;
	info->vel_float = info->vel_float /_rpm_to_pulse / _deg_to_rpm / _gear_ratio;
	info->vel_int32 = (int32_t)info->vel_float;
	info->pos_vel_curr_tor[2] = info->vel_int32;
	info->pos_vel_curr_tor[3] = info->vel_int32 >> 8;

	/* torque */
	info->pos_vel_curr_tor[4] = info->tor_curr_raw[0];
	info->pos_vel_curr_tor[5] = info->tor_curr_raw[1];
	/* current */
	info->pos_vel_curr_tor[6] = info->tor_curr_raw[2];
	info->pos_vel_curr_tor[7] = info->tor_curr_raw[3];

	info->status[0] = info->auto_mode;
	info->status[1] = info->servo_on_off;
	info->status[2] = info->status_raw[0];
	info->status[3] = info->status_raw[1];
	info->status[4] = info->status_raw[2];
	info->status[5] = info->status_raw[3];

}

void MotorInitCommand(uint32_t id, uint32_t length, uint8_t *data)
{
	Can_Tx_Header_can2.Identifier = id;
	Can_Tx_Header_can2.IdType = FDCAN_STANDARD_ID;
	Can_Tx_Header_can2.TxFrameType = FDCAN_DATA_FRAME;
	Can_Tx_Header_can2.FDFormat = FDCAN_FRAME_CLASSIC;
	Can_Tx_Header_can2.DataLength = length;

	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &Can_Tx_Header_can2, data);
}

void MotorSetCommand(uint32_t id, uint32_t length, uint8_t *data, uint8_t *flag)
{
	if(*flag) {
		Can_Tx_Header_can2.Identifier = id;
		Can_Tx_Header_can2.IdType = FDCAN_STANDARD_ID;
		Can_Tx_Header_can2.TxFrameType = FDCAN_DATA_FRAME;
		Can_Tx_Header_can2.FDFormat = FDCAN_FRAME_CLASSIC;
		Can_Tx_Header_can2.DataLength = length;
		Can_Tx_Header_can2.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;

		HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &Can_Tx_Header_can2, data);

		(*flag) = FALSE;
	}
}

void MotorControlCommand(uint32_t id, uint32_t length, uint8_t *data, uint8_t *flag)
{
	if(*flag) {
		Can_Tx_Header_can2.Identifier = id;
		Can_Tx_Header_can2.IdType = FDCAN_STANDARD_ID;
		Can_Tx_Header_can2.TxFrameType = FDCAN_DATA_FRAME;
		Can_Tx_Header_can2.FDFormat = FDCAN_FRAME_CLASSIC;
		Can_Tx_Header_can2.DataLength = length;
		Can_Tx_Header_can2.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;

		HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan2, &Can_Tx_Header_can2, data);

		(*flag) = FALSE;
	}
}

void MotorInofrmation(uint32_t id, uint32_t length, uint8_t *data)
{
	Can_Tx_Header_can1.Identifier = id;
	Can_Tx_Header_can1.IdType = FDCAN_STANDARD_ID;
	Can_Tx_Header_can1.TxFrameType = FDCAN_DATA_FRAME;
	Can_Tx_Header_can1.FDFormat = FDCAN_FRAME_CLASSIC;
	Can_Tx_Header_can1.DataLength = length;
	Can_Tx_Header_can1.TxEventFifoControl = FDCAN_STORE_TX_EVENTS;

	HAL_FDCAN_AddMessageToTxFifoQ(&hfdcan1, &Can_Tx_Header_can1, data);
}

void CanErrorCheck(FDCAN_HandleTypeDef *hfdcan, uint8_t *error_count)
{
	uint8_t _error_state = (hfdcan->ErrorCode & 0x200) >> 9;

	if(_error_state) {
		if((*error_count) == 10) {
			if((HAL_FDCAN_Stop(hfdcan)) != HAL_OK) {
			}
		} else if( (*error_count) > 10) {
			if( HAL_FDCAN_Start(hfdcan) != HAL_OK) {
			}
		}

		if((*error_count) < 15) {
			(*error_count)++;
		} else {
			(*error_count) = 0;
		}

	} else {
		(*error_count) = 0;
	}
}

void MotorSequnceTimer(uint8_t *count, uint8_t init, uint8_t max)
{
	if( ++(*count) == max) {
		(*count) = init;
	}
}

