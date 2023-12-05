/*
 ****************************************************************************************
 * File Name          : flash.c                                                          *
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
#include "flash_mem.h"

/******************************************************************************/
/*                                   FUNCTION                                 */
/******************************************************************************/
uint64_t  FlashRead(uint32_t page_number, uint32_t data_number)
{
	//We Can Use [1] [Page Number] -> [0 ~ 255] [Data Number]
	uint32_t _address = page_number + (data_number * 8);

	return *((uint64_t*)_address);
}

void Flash_Erase(uint32_t page_number)
{
	HAL_FLASH_Unlock();

	static FLASH_EraseInitTypeDef EraseInitStruct;
	uint32_t PAGEError = 0;

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_ALL_ERRORS);

	EraseInitStruct.TypeErase = FLASH_TYPEERASE_PAGES;
	EraseInitStruct.Page = GetPageNumber(page_number);
	EraseInitStruct.NbPages = 1;
	EraseInitStruct.Banks = FLASH_BANK_2;

	if (HAL_FLASHEx_Erase(&EraseInitStruct, &PAGEError) != HAL_OK)
	{
		/*Error occurred while page erase.*/
		 HAL_FLASH_GetError ();
	}

	HAL_FLASH_Lock();
}

void Flash_Write(uint32_t page_number, uint32_t data_number, uint64_t data)
{
	HAL_FLASH_Unlock();
	//We Can Use [1] [Page Number] -> [0 ~ 255] [Data Number]
	uint32_t address = page_number + (data_number * 8);

	__HAL_FLASH_CLEAR_FLAG(FLASH_FLAG_EOP | FLASH_FLAG_OPERR | FLASH_FLAG_WRPERR | FLASH_FLAG_PGAERR | FLASH_FLAG_PGSERR);

	if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, data) == HAL_OK) {

	} else {
		HAL_FLASH_GetError ();
	}

	HAL_FLASH_Lock();
}

uint8_t GetPageNumber(uint32_t page_address)
{
	uint8_t page_number = 0;

	if(page_address == ADDR_FLASH_PAGE_78) page_number = 78;
	else if(page_address == ADDR_FLASH_PAGE_79) page_number = 79;
	else if(page_address == ADDR_FLASH_PAGE_80) page_number = 80;
	else if(page_address == ADDR_FLASH_PAGE_81) page_number = 81;
	else if(page_address == ADDR_FLASH_PAGE_82) page_number = 82;
	else if(page_address == ADDR_FLASH_PAGE_83) page_number = 83;
	else if(page_address == ADDR_FLASH_PAGE_84) page_number = 84;
	else if(page_address == ADDR_FLASH_PAGE_85) page_number = 85;
	else if(page_address == ADDR_FLASH_PAGE_86) page_number = 86;
	else if(page_address == ADDR_FLASH_PAGE_87) page_number = 87;
	else if(page_address == ADDR_FLASH_PAGE_88) page_number = 88;
	else if(page_address == ADDR_FLASH_PAGE_89) page_number = 89;
	else if(page_address == ADDR_FLASH_PAGE_90) page_number = 90;
	else if(page_address == ADDR_FLASH_PAGE_91) page_number = 91;
	else if(page_address == ADDR_FLASH_PAGE_92) page_number = 92;
	else if(page_address == ADDR_FLASH_PAGE_93) page_number = 93;
	else if(page_address == ADDR_FLASH_PAGE_94) page_number = 94;
	else if(page_address == ADDR_FLASH_PAGE_95) page_number = 95;
	else if(page_address == ADDR_FLASH_PAGE_96) page_number = 96;
	else if(page_address == ADDR_FLASH_PAGE_97) page_number = 97;
	else if(page_address == ADDR_FLASH_PAGE_98) page_number = 98;
	else if(page_address == ADDR_FLASH_PAGE_99) page_number = 99;
	else if(page_address == ADDR_FLASH_PAGE_100) page_number = 100;
	else if(page_address == ADDR_FLASH_PAGE_101) page_number = 101;
	else if(page_address == ADDR_FLASH_PAGE_102) page_number = 102;
	else if(page_address == ADDR_FLASH_PAGE_103) page_number = 103;
	else if(page_address == ADDR_FLASH_PAGE_104) page_number = 104;
	else if(page_address == ADDR_FLASH_PAGE_105) page_number = 105;
	else if(page_address == ADDR_FLASH_PAGE_106) page_number = 106;
	else if(page_address == ADDR_FLASH_PAGE_107) page_number = 107;
	else if(page_address == ADDR_FLASH_PAGE_108) page_number = 108;
	else if(page_address == ADDR_FLASH_PAGE_109) page_number = 109;
	else if(page_address == ADDR_FLASH_PAGE_110) page_number = 110;
	else if(page_address == ADDR_FLASH_PAGE_111) page_number = 111;
	else if(page_address == ADDR_FLASH_PAGE_112) page_number = 112;
	else if(page_address == ADDR_FLASH_PAGE_113) page_number = 113;
	else if(page_address == ADDR_FLASH_PAGE_114) page_number = 114;
	else if(page_address == ADDR_FLASH_PAGE_115) page_number = 115;
	else if(page_address == ADDR_FLASH_PAGE_116) page_number = 116;
	else if(page_address == ADDR_FLASH_PAGE_117) page_number = 117;
	else if(page_address == ADDR_FLASH_PAGE_118) page_number = 118;
	else if(page_address == ADDR_FLASH_PAGE_119) page_number = 119;
	else if(page_address == ADDR_FLASH_PAGE_120) page_number = 120;
	else if(page_address == ADDR_FLASH_PAGE_121) page_number = 121;
	else if(page_address == ADDR_FLASH_PAGE_122) page_number = 122;
	else if(page_address == ADDR_FLASH_PAGE_123) page_number = 123;
	else if(page_address == ADDR_FLASH_PAGE_124) page_number = 124;
	else if(page_address == ADDR_FLASH_PAGE_125) page_number = 125;
	else if(page_address == ADDR_FLASH_PAGE_126) page_number = 126;
	else if(page_address == ADDR_FLASH_PAGE_127) page_number = 127;


	return page_number;
}
