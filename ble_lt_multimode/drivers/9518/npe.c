/********************************************************************************************************
 * @file     app.c
 * @brief    This is the source file for TLSR9518
 *
 * @author	 Driver Group
 * @date     April 1, 2020
 *
 * @par      Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
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
 * 			 1.initial release(Mar. 16, 2020)
 *
 * @version  A001
 *
 *******************************************************************************************************/
#include "npe.h"


/**
We don't know the meaning of the register, so let's them together
 */

void npe_set_reg(void)
{
	reg_npe_0x84=0xffff;
	reg_npe_0x74=0x03000000;
	reg_npe_0x80=0x100;
	reg_npe_0x40=0x100;

}
/**
npe set config
 */
void npe_set_config(u32 desc_pointer )
{
	reg_npe_0x6c=desc_pointer;

	npe_set_reg();
}
