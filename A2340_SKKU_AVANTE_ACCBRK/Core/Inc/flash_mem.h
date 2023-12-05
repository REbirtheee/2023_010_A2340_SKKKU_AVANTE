/*
 ****************************************************************************************
 * File Name          : flash.h                                                          *
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

/***********************************  Define **********************************/
#define page_size 0x800 //2KB

/*BANK_2 100KB User Flash MEM*/
#define ADDR_FLASH_PAGE_78		((uint32_t)0x08067000)
#define ADDR_FLASH_PAGE_79		((uint32_t)0x08067800)
#define ADDR_FLASH_PAGE_80		((uint32_t)0x08068000)
#define ADDR_FLASH_PAGE_81		((uint32_t)0x08068800)
#define ADDR_FLASH_PAGE_82		((uint32_t)0x08069000)
#define ADDR_FLASH_PAGE_83		((uint32_t)0x08069800)
#define ADDR_FLASH_PAGE_84		((uint32_t)0x0806A000)
#define ADDR_FLASH_PAGE_85		((uint32_t)0x0806A800)
#define ADDR_FLASH_PAGE_86		((uint32_t)0x0806B000)
#define ADDR_FLASH_PAGE_87		((uint32_t)0x0806B800)
#define ADDR_FLASH_PAGE_88		((uint32_t)0x0806C000)
#define ADDR_FLASH_PAGE_89		((uint32_t)0x0806C800)
#define ADDR_FLASH_PAGE_90		((uint32_t)0x0806D000)
#define ADDR_FLASH_PAGE_91		((uint32_t)0x0806D800)
#define ADDR_FLASH_PAGE_92		((uint32_t)0x0806E000)
#define ADDR_FLASH_PAGE_93		((uint32_t)0x0806E800)
#define ADDR_FLASH_PAGE_94		((uint32_t)0x0806F000)
#define ADDR_FLASH_PAGE_95		((uint32_t)0x0806F800)
#define ADDR_FLASH_PAGE_96		((uint32_t)0x08070000)
#define ADDR_FLASH_PAGE_97		((uint32_t)0x08070800)
#define ADDR_FLASH_PAGE_98		((uint32_t)0x08071000)
#define ADDR_FLASH_PAGE_99		((uint32_t)0x08071800)
#define ADDR_FLASH_PAGE_100		((uint32_t)0x08072000)
#define ADDR_FLASH_PAGE_101		((uint32_t)0x08072800)
#define ADDR_FLASH_PAGE_102		((uint32_t)0x08073000)
#define ADDR_FLASH_PAGE_103		((uint32_t)0x08073800)
#define ADDR_FLASH_PAGE_104		((uint32_t)0x08074000)
#define ADDR_FLASH_PAGE_105		((uint32_t)0x08074800)
#define ADDR_FLASH_PAGE_106		((uint32_t)0x08075000)
#define ADDR_FLASH_PAGE_107		((uint32_t)0x08075800)
#define ADDR_FLASH_PAGE_108		((uint32_t)0x08076000)
#define ADDR_FLASH_PAGE_109		((uint32_t)0x08076800)
#define ADDR_FLASH_PAGE_110		((uint32_t)0x08077000)
#define ADDR_FLASH_PAGE_111		((uint32_t)0x08077800)
#define ADDR_FLASH_PAGE_112		((uint32_t)0x08078000)
#define ADDR_FLASH_PAGE_113		((uint32_t)0x08078800)
#define ADDR_FLASH_PAGE_114		((uint32_t)0x08079000)
#define ADDR_FLASH_PAGE_115		((uint32_t)0x08079800)
#define ADDR_FLASH_PAGE_116		((uint32_t)0x0807A000)
#define ADDR_FLASH_PAGE_117		((uint32_t)0x0807A800)
#define ADDR_FLASH_PAGE_118		((uint32_t)0x0807B000)
#define ADDR_FLASH_PAGE_119		((uint32_t)0x0807B800)
#define ADDR_FLASH_PAGE_120		((uint32_t)0x0807C000)
#define ADDR_FLASH_PAGE_121		((uint32_t)0x0807C800)
#define ADDR_FLASH_PAGE_122		((uint32_t)0x0807D000)
#define ADDR_FLASH_PAGE_123		((uint32_t)0x0807D800)
#define ADDR_FLASH_PAGE_124		((uint32_t)0x0807E000)
#define ADDR_FLASH_PAGE_125		((uint32_t)0x0807E800)
#define ADDR_FLASH_PAGE_126		((uint32_t)0x0807F000)
#define ADDR_FLASH_PAGE_127		((uint32_t)0x0807F800)


#define FLASH_USER_START_ADDR	ADDR_FLASH_PAGE_91
#define FLASH_USER_END_ADDR		ADDR_FLASH_PAGE_120

/*********************************** Common ***********************************/



/********************************** Function **********************************/
uint64_t  FlashRead(uint32_t page_number, uint32_t data_number);
void Flash_Erase(uint32_t page_number);
void Flash_Write(uint32_t page_number, uint32_t data_number, uint64_t data);
uint8_t GetPageNumber(uint32_t page_address);


#ifndef INC_FLASH_MEM_H_
#define INC_FLASH_MEM_H_



#endif /* INC_FLASH_MEM_H_ */
