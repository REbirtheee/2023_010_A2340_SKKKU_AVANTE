/*
 ****************************************************************************************
 * File Name          : motor.h                                                         *
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

#ifndef INC_MOTOR_H_
#define INC_MOTOR_H_

#include "core.h"

#define FALSE 	0
#define TRUE	1

#define MOTOR_NODE_ID			3

#define NMT_CMD_ID				0x00
#define NMT_INFO_ID				0x700 + MOTOR_NODE_ID
#define MOTOR_EMG_ID			0x80 + MOTOR_NODE_ID
#define SDO_CMD_ID				0x600 + MOTOR_NODE_ID
#define SDO_INFO_ID				0x580 + MOTOR_NODE_ID

#define POS_VEL_CONTROL_ID		0x200 + MOTOR_NODE_ID
#define	ACC_DEC_CONTROL_ID		0x300 + MOTOR_NODE_ID
#define CONTROL_WORD_ID			0x400 + MOTOR_NODE_ID
#define HOMING_CONTROL_ID		0x500 + MOTOR_NODE_ID

#define CANOPEN_DATA_SIZE_1		0x2F
#define CANOPEN_DATA_SIZE_2		0x2B
#define CANOPEN_DATA_SIZE_3		0x27
#define CANOPEN_DATA_SIZE_4		0x23

#define MOTOR_PREPARE			0
#define MOTOR_NEW_POS_SET		1
#define MOTOR_START				2

#define MOTOR_INIT_PREPARE		0
#define MOTOR_INIT_PREPARE_2	1
#define MOTOR_INIT_COMMAND		2
#define MOTOR_INIT_START		3
#define MOTOR_INIT_START_1		4
#define MOTOR_INIT_START_2		5
#define MOTOR_INIT_STOP			6
#define MOTOR_INIT_STOP_2		7
#define MOTOR_INIT_STOP_3		8
#define MOTOR_INIT_MOTOR_OFF	9
#define MOTOR_INIT_SAVE			10
#define MOTOR_OFF	0
#define MOTOR_RDY	1
#define MOTOR_ON	2

//#define INSIDE_TIMER_ENABLE
#define MOTOR_TIMER_INIT		0
#define MOTOR_TIMER_MAX			10
#if 0
#define MOTOR_GEAR_RATIO_60				60			// -> motor gear ratio : 1:60
#define MOTOR_GEAR_RATIO				666.6666667 // -> motor 1cycle * gear ratio / steer 1cycle : 10000 * 60 / 360
#define MOTOR_DEG_TO_RPS				0.002777778	// -> rps change : (vel/360)
#define MOTOR_VEL_SAMPLE_PER_EPR		0.5			// -> encoder per rev/smaple Rate : 4000/8000
#define MOTOR_ACCDEC_SAMPLE_PER_EPR		0.0000625	// -> encoder per rev/smaple Rate : 4000/(8000 * 8000)
#define MOTOR_GEAR						65535
#else
#if 1
#define MOTOR_PULSE						10000
#define GEAR_RATIO						1											// -> motor gear ratio : 1:60
#define MOTOR_GEAR_RATIO				27.777777778				// -> motor 1cycle * gear ratio / steer 1cycle : 10000 * 60 / 360
#define MOTOR_DEG_TO_RPM				0.1666667							// -> rpm change : (vel/360)
#define MOTOR_RPM_TO_PULSE				166.6666667							// -> encoder per rev/smaple Rate : 1500/10000
#define MOTOR_RPM_PER_SEC				0.016666666
#define MOTOR_RPM_PER_SEC_TO_PULSE		166.6666667							// -> encoder per rev/smaple Rate : 1500/10000

#else
#define MOTOR_PULSE						10000
#define GEAR_RATIO						70											// -> motor gear ratio : 1:60
#define MOTOR_GEAR_RATIO				194.44444444				// -> motor 1cycle * gear ratio / steer 1cycle : 10000 * 60 / 360
#define MOTOR_DEG_TO_RPM				0.1666667							// -> rpm change : (vel/360)
#define MOTOR_RPM_TO_PULSE				166.6666667							// -> encoder per rev/smaple Rate : 1500/10000
#define MOTOR_RPM_PER_SEC				0.016666666
#define MOTOR_RPM_PER_SEC_TO_PULSE		166.6666667							// -> encoder per rev/smaple Rate : 1500/10000
#endif
#endif
FDCAN_HandleTypeDef hfdcan1;
FDCAN_HandleTypeDef hfdcan2;

FDCAN_TxHeaderTypeDef can1_tx_header;
FDCAN_RxHeaderTypeDef can1_rx_header;
FDCAN_TxHeaderTypeDef can2_tx_header;
FDCAN_RxHeaderTypeDef can2_rx_header;

uint8_t feq_20ms;
uint8_t CAN_ERROR_cnt;
//uint8_t servo_on_status;

struct ControlCommand {
	uint8_t user[8];
	uint8_t user_prev[8];

	int16_t pos_int16;
	int32_t pos_int32;
	float pos_float;
	uint8_t pos[8];

	int32_t vel_int32;
	float vel_float;
	uint8_t vel[8];

	uint8_t pos_vel[8];
	uint8_t pos_vel_prev[8];

	int32_t acc_int32;
	float acc_float;
	int32_t dec_int32;
	float dec_float;

	uint8_t acc_dec[8];
	uint8_t acc_dec_prev[8];
	uint8_t status[8];
	uint8_t status_prev[8];

	uint8_t cmd_index;
	uint8_t timer;
	uint8_t user_flag;
	uint8_t init_index;
	uint8_t pos_vel_flag;
	uint8_t acc_dec_flag;
	uint8_t save_flag;

	uint8_t user_auto_mode;
	uint8_t auto_mode_flag;

	uint8_t servo_on_off;
	uint8_t servo_on_off_flag;
	uint8_t alarm_reset_flag;
	uint8_t encoder_init_flag;

	uint8_t torque[8];
	uint8_t cotrol_mode;
	uint8_t cotrol_mode_prev;

	uint8_t torquevalue_flag;
	uint8_t torquespeedlimit_flag;
	uint8_t torquerate_flag;

	int16_t torque_cmd;
	int16_t torque_cmd_prev;

} motor_cmd;



struct StatusInformation {

	int16_t pos_int16;
	int32_t pos_int32;
	float pos_float;

	int32_t vel_int32;
	float vel_float;
	uint8_t pos_vel_curr_tor[8];
	uint8_t pos_vel_raw[8];
	uint8_t status_raw[8];
	uint8_t tor_curr_raw[8];

	uint8_t auto_mode;
	uint8_t clutch;
	uint8_t clutch_flag;
	uint8_t clutch_enable_time;
	uint8_t servo_on_off;
	uint8_t status[8];

	uint8_t ready;

} motor_info;

struct Motorpowercheck{

	uint8_t no_can;
	uint8_t no_can_cnt;

}motor_pw_check;

void MotionControlControl(struct ControlCommand *cmd, struct StatusInformation *info);
void MotorCommandFlagUpdate(struct ControlCommand *cmd, struct StatusInformation *info);
void MotorInit();
void DisableMotorWatchdog();
void MotorOperation();
void MotorResetOperation();
void MotorSetPositionMode();
void MotorPDOMappingPosVel();
void MotorPDOMappingAccDec();
void MotorPDOMappingCtrlWord();
void MotorPDOMappingHomeOffest();
void MotorPDOMappingPosVelInfo();
void MotorPDOMappingTorCurrInfo();
void MotorPDOMappingStatusInfo();
void MotorAccDecInit();
void MotorEncoderInitPrepare_Type2();
void MotorEncoderInitCommand_Type2();
void MotorEncoderInitPrepare_1_Type2();
void MotorEncoderInitPrepare_2_Type2();
void MotorEncoderInitBegin_Type2();
void MotorEncoderInitMOTOROFF_Type2();
void MotorEncoderInitPrepare();
void MotorEncoderInitPrepare2();
void MotorEncoderInitPrepare_1();
void MotorEncoderInitPrepare_2();
void MotorEncoderInitCommand();
void MotorEncoderInitBegin();
void MotorEncoderInitStop();
void MotorEncoderInitStop2();
void MotorEncoderInitStop3();
void MotorEncoderInitMOTOROFF();
void MotorEncoderInitMOTOROFF2();
void MotorAlarmReset();
void MotorServeOnOff(struct ControlCommand *cmd);
void T_mode_MotorServoOn();
void T_mode_MotorServoOff();
void TorqueCommandConverter(struct ControlCommand *cmd);
void P_mode_MotorServoOn();
void P_mode_MotorServoOff();
void MotorPrepare();
void T_MotorPrepare();
void MotorStart();
void T_MotorStart();
void MotorCommandConverter(struct ControlCommand *cmd);
void MotorControlCommand(uint32_t id, uint32_t length, uint8_t *data, uint8_t *flag);
void MotorInfoConverter(struct StatusInformation *info);


void CanErrorCheck(FDCAN_HandleTypeDef *hfdcan, uint8_t *error_count);
void MotorInitCommand(uint32_t id, uint32_t length, uint8_t *data);
void MotorSetCommand(uint32_t id, uint32_t length, uint8_t *data, uint8_t *flag);
void MotorInofrmation(uint32_t id, uint32_t length, uint8_t *data);
void MotorSequnceTimer(uint8_t *count, uint8_t init, uint8_t max);


#endif /* INC_MOTOR_H_ */
