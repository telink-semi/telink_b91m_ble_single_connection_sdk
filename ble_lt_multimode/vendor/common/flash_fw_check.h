/********************************************************************************************************
 * @file     flash_fw_check.h
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
 * @date     May. 12, 2018
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#ifndef _FLASH_FW_CHECK_H_
#define _FLASH_FW_CHECK_H_


#include "../../common/types.h"


/**
 * @brief		This function is used to check the firmware is ok or not
 * @param[in]	crc_init_value - the initial value of CRC
 * @return		0 - CRC is check success
 * 				1 - CRC is check fail
 */
bool flash_fw_check( u32 crc_init_value );



#endif
