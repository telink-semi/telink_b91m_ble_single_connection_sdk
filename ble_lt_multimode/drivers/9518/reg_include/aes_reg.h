/********************************************************************************************************
 * @file     aes_reg.h
 *
 * @brief    This is the source file for TLSR9518
 *
 * @author   BLE Group
 * @date     Jun 16, 2020
 *
 * @par      Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 *           1.initial release(Mar. 16, 2020)
 *
 * @version  A001
 *
 *******************************************************************************************************/
#ifndef _AES_REG_H_
#define _AES_REG_H_

#include "../sys.h"
#include "../../../common/bit.h"

#define reg_aes_mode     			REG_ADDR32(0x1600b0)
typedef enum{
	FLD_AES_START	=	BIT(0),
	FLD_AES_MODE	=	BIT(1),   //0-ciher  1-deciher
}aes_mode_e;

#define reg_embase_addr     		REG_ADDR32(0x140b04)

#define reg_aes_key(v)     		REG_ADDR32(0x1600b4+(v*4))

#define reg_aes_ptr     		REG_ADDR32(0x1600c4)
/*******************************      BLE Stack Use     ******************************/
#define reg_aes_ctrl            REG_ADDR8(0x1600b0)
#define reg_aes_data            reg_aes_ctrl//REG_ADDR32(0x548)
#endif /* _AES_REG_H_ */
