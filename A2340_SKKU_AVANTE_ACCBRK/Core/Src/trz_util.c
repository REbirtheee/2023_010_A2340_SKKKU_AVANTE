/*
 ****************************************************************************************
 * File Name          : trz_util.c                                                          *
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
#include "trz_util.h"

/******************************************************************************/
/*                              Treeze Function                               */
/******************************************************************************/
void SequenceTimer(uint8_t *timer, uint8_t init, uint8_t max)
{	/* Function Description : SequnceTimer For CAN */
	if (++(*timer) == max) {
		(*timer) = init;
	}
}

void FlagMakeFalse(uint8_t *flag)
{	/* Function Description : Make Flag False */
	(*flag) = FALSE;
}
void FlagMakeTrue(uint8_t *flag)
{	/* Function Description : Make Flag True */
	(*flag) = TRUE;
}
