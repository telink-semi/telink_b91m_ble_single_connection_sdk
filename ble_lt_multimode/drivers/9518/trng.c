/********************************************************************************************************
 * @file     trng.c
 *
 * @brief    This is the source file for TLSR9518
 *
 * @author	 Driver Group
 * @date     September 16, 2019
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
 * 			 1.initial release(September 16, 2019)
 *
 * @version  A001
 *
 *******************************************************************************************************/

#include "trng.h"
#include "compiler.h"

_attribute_data_retention_ unsigned int g_rnd_m_w = 0;
_attribute_data_retention_ unsigned int g_rnd_m_z = 0;

/**
 * @brief     This function performs to get one random number.If chip in suspend TRNG module should be close.
 *            else its current will be larger.
 * @param[in] none.
 * @return    the value of one random number.
 */
void trng_init(void)
{
	//TRNG module Reset clear
	reg_rst2 |= FLD_RST2_TRNG;
	//turn on TRNG clock
	reg_clk_en2 |= FLD_CLK2_TRNG_EN;

	reg_trng_cr0 &= ~(FLD_TRNG_CR0_RBGEN); //disable
	reg_trng_rtcr = 0x00;				//TCR_MSEL
	reg_trng_cr0 |= (FLD_TRNG_CR0_RBGEN); //enable

	while(!(reg_rbg_sr & FLD_RBG_SR_DRDY));
	g_rnd_m_w = reg_rbg_dr;   //get the random number
	while(!(reg_rbg_sr & FLD_RBG_SR_DRDY));
	g_rnd_m_z = reg_rbg_dr;

	//Reset TRNG module
	reg_rst2 &= (~FLD_RST2_TRNG);
	//turn off TRNG module clock
	reg_clk_en2 &= ~(FLD_CLK2_TRNG_EN);

	reg_trng_cr0 &= ~(FLD_TRNG_CR0_RBGEN | FLD_TRNG_CR0_ROSEN0 | FLD_TRNG_CR0_ROSEN1 \
											| FLD_TRNG_CR0_ROSEN2 | FLD_TRNG_CR0_ROSEN3);
}

/**
 * @brief     This function performs to get one random number
 * @param[in] none.
 * @return    the value of one random number.
 */
_attribute_ram_code_  unsigned int trng_rand(void)  //16M clock, code in flash 23us, code in sram 4us
{

	g_rnd_m_w = 18000 * (g_rnd_m_w & 0xffff) + (g_rnd_m_w >> 16);
	g_rnd_m_z = 36969 * (g_rnd_m_z & 0xffff) + (g_rnd_m_z >> 16);
	unsigned int result = (g_rnd_m_z << 16) + g_rnd_m_w;

	return (unsigned int)( result  ^ sys_get_stimer_tick() );
}

/*******************************      BLE Stack Use     ******************************/
void generateRandomNum(int len, unsigned char *data)
{
	int i;
	unsigned int randNums = 0;
    /* if len is odd */
	for (i=0; i<len; i++ ) {
		if( (i & 3) == 0 ){
			randNums = rand();
		}

		data[i] = randNums & 0xff;
		randNums >>=8;
	}
}
