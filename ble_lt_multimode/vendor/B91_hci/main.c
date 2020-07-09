/********************************************************************************************************
 * @file     main.c 
 *
 * @brief    This is the source file for TLSR8258
 *
 * @author	 junwei.lu@telink-semi.com;
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
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
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *
 *******************************************************************************************************/
#include "app_config.h"
#include "../../drivers.h"

extern void user_init();
extern void main_loop (void);


/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)   //must on ramcode
{
	sys_init(LDO_MODE);

	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV8_TO_CCLK,CCLK_DIV1_TO_HCLK, HCLK_DIV1_TO_PCLK,CCLK_TO_MSPI_CLK);

	user_init();


	while (1) {
		main_loop ();
	}
	return 0;
}
