/********************************************************************************************************
 * @file	mdec_reg.h
 *
 * @brief	This is the header file for B91
 *
 * @author	Ble Group
 * @date	2019
 *
 * @par		Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *			The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *          alter) any information contained herein in whole or in part except as expressly authorized  
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef MDEC_REG_H_
#define MDEC_REG_H_
#include "../sys.h"
#include "../../../common/bit.h"
#define mdec_rst_addr                   0x53

typedef enum
{
	FLD_SELE_PA0		= BIT(0),
	FLD_SELE_PB7		= BIT(1),
	FLD_SELE_PC4		= BIT(2),
	FLD_SELE_PD0		= BIT(3),
	FLD_SELE_PE0		= BIT(4),
}mdec_pin_e;

typedef enum{
	FLD_CLS_MDEC	    = BIT_RNG(0,4),
	FLD_RSVD			= BIT_RNG(5,6),
	FLD_MDEC_RST		= BIT(7),
}mdec_set_e;

#define	mdec_ctrl						0x54


#define REG_PG_STATUS_BASE_ADRESS		0x64

enum
{
	FLD_WKUP_CMP		= BIT(0),
	FLD_WKUP_TIMER		= BIT(1),
	FLD_WKUP_DIG		= BIT(2),
	FLD_WKUP_PAD		= BIT(3),
	FLD_WKUP_MDEC		= BIT(4),
	FLD_MDEC_RSVD		= BIT_RNG(5,7),
}mdec_wake_e;
//#define reg_mdec_rec_data				REG8(REG_PG_STATUS_BASE_ADRESS+0x05)


#endif /* DRIVERS_9518_REG_INCLUDE_MDEC_REG_H_ */
