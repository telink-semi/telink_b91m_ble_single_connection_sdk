/********************************************************************************************************
 * @file     ble.c
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
 * @date     Sep. 17, 2019
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

#include "ble_common.h"
#include "ble.h"
#include "tl_common.h"
#include "drivers.h"

__attribute__((noinline)) volatile void  stmemset(register char * dest,register int val,register unsigned int len)
{
	while(len--)
	   *(dest++) = val;
}


_attribute_session_(".ram_code")
__attribute__((noinline)) volatile void * stmemcpy(register char * out, register char * in, register unsigned int len)
{
	while(len--)
		*(out++)=*(in++);
}
