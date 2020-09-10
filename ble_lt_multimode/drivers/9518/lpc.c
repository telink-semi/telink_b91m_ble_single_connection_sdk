/********************************************************************************************************
 * @file	lpc.c
 *
 * @brief	This is the source file for B91
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
#include "lpc.h"









/**
 * @brief This function selects input reference voltage for low power comparator .
 * @param[in] mode-lower power comparator working mode includes normal mode and low power mode.
 * @param[in] ref- selected input reference voltage.
 * @return none
 */
void lpc_set_input_ref(lpc_mode_e mode,lpc_reference_e ref)
{
	if(mode == LPC_LOWPOWER)
	{
		analog_write_reg8(0x0b,analog_read_reg8(0x0b)|0x08);
		analog_write_reg8(0x0d,analog_read_reg8(0x0d)&0x7f);
	}
	else if(mode == LPC_NORMAL)
	{
		analog_write_reg8(0x0b,analog_read_reg8(0x0b)&0xf7);
		analog_write_reg8(0x0d,analog_read_reg8(0x0d)|0x80);
	}
	analog_write_reg8(0x0d,(analog_read_reg8(0x0d)&0x8f)|(ref<<4));
}

















