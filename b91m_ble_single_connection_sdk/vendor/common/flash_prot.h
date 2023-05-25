/********************************************************************************************************
 * @file     flash_prot.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#ifndef VENDOR_COMMON_FLASH_PROT_H_
#define VENDOR_COMMON_FLASH_PROT_H_


#ifndef APP_FLASH_PROTECTION_ENABLE
#define APP_FLASH_PROTECTION_ENABLE					0   //enable or disable
#endif

#ifndef APP_FLASH_PROT_LOG_EN
#define APP_FLASH_PROT_LOG_EN						0
#endif


/**
 * @brief    application lock block definition
 *   Each kind of IC have multiple flash supported, e.g. now (2023.05) B92 have 5 kind of flash supported.
 *   Different flash, especially form different vendor(such as PUYA and GD), do not use all same lock block size.
 *   If use want protect certain range of flash, they must consider different kind of flash supported lock block size.
 *   Here, we just put a few of block size to show how to use flash protection, for typically multiple boot device with firmware size smaller
 *   than 448K(for 1M flash, leave 64K for system data or user data. if user need more data area, should change flash lock size)
 *   If you want use more specific flash protection area, please add by yourself.
 */
typedef enum{
	FLASH_LOCK_LOW_256K			=	2,		//000000h-03FFFFh
	FLASH_LOCK_LOW_512K			=	3,		//000000h-07FFFFh
	FLASH_LOCK_LOW_1M			=   4,      //attention: for 1M flash, can not lock all, we choose most part
}flash_app_lock_e;



typedef struct{
	/* 	attention that, we use: [op_addr_begin, op_addr_end)
	 *  e.g. if now locking LOW_256K, actual locking flash address is 0x00000 ~ 0x3FFFF
	 *  but we use [0x00000, 0x40000):  op_addr_begin = 0x00000, op_addr_end = 0x40000 */
	u32   lock_addr_begin;
	u32   lock_addr_end;

	u8	  locked;  			//1: locked, 0: unlocked
}flash_prot_t;
extern flash_prot_t	blc_flashProt;



typedef void  (*flash_lock_t)(unsigned int);
//extern	flash_lock_t 	flash_lock_mid;

typedef void  (*flash_unlock_t)(void);
//extern	flash_unlock_t 	flash_unlock_mid;

typedef unsigned short  (*flash_read_status_t)(void);
//extern	flash_read_status_t 	flash_read_status_mid;

typedef unsigned short  (*flash_get_lock_status_t)(unsigned short);


typedef void  (*flash_prot_op_callback_t)(u8, u32, u32);
extern	flash_prot_op_callback_t 		flash_prot_op_cb;








/*
 * @brief flash operation event
 *    Here just list all type appeared in SDK sample, including APP initialization and OTA/SMP operating flash
 *    If you have more flash write or erase action for your application, should add more type and process them
 *
 */

/* application layer event, initialization, lock flash */
#define FLASH_OP_EVT_APP_INITIALIZATION					 	1



/* stack layer event, OTA initialization, clear old firmware begin, may need unlock flash */
#define FLASH_OP_EVT_STACK_OTA_CLEAR_OLD_FW_BEGIN			10
/* stack layer event, OTA initialization, clear old firmware end , may need restore locking flash */
#define FLASH_OP_EVT_STACK_OTA_CLEAR_OLD_FW_END				11



/* stack layer event, OTA initialization, write new firmware begin, may need unlock flash */
#define FLASH_OP_EVT_STACK_OTA_WRITE_NEW_FW_BEGIN			12
/* stack layer event, OTA initialization, write new firmware begin, may need restore locking flash */
#define FLASH_OP_EVT_STACK_OTA_WRITE_NEW_FW_END				13





/**
 * @brief      this function is used to initialize flash protection.
 * @param[in]  none
 * @return     none
 */
void flash_protection_init(void);


/**
 * @brief      this function is used to register callback for stack, when OTA write or erase flash, send event to application layer
 * @param[in]  cb - flash protection operation callback function
 * @return     none
 */
void blc_appRegisterStackFlashOperationCallback(flash_prot_op_callback_t cb);

/**
 * @brief 		This function serves to set the protection of the flash.
 * @param[in]   flash_lock_cmd - flash lock block command for different flash type
 * 				e.g. for P25Q16SU, command is selected from "mid156085_lock_block_e"
 * @return 		none.
 */
void flash_lock(unsigned int flash_lock_cmd);

/**
 * @brief 		This function serves to release flash protection.
 * @param[in]   none.
 * @return 		none.
 */
void flash_unlock(void);

/**
 * @brief      this function is used to change application lock block definition to flash lock block definition according to flash type used
 * 			   attention: only processed a few lock block size for SDK demo, if you have more lock block size, please
 * 			              add more code by yourself
 * @param[in]  app_lock_block - application lock block definition
 * @return     flash_lock_block_size - The size of the lock block size of flash.
 */
u16 flash_change_app_lock_block_to_flash_lock_block(flash_app_lock_e app_lock_block);

#endif /* VENDOR_COMMON_FLASH_PROT_H_ */
