/********************************************************************************************************
 * @file     uart.c
 *
 * @brief    This is the source file for TLSR8258
 *
 * @author	 Driver Group
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
#include "nds_intrinsic.h"
#include "core.h"
#include "sys.h"
#include "compiler.h"
#include "analog.h"
#include "gpio.h"
#include "mspi.h"
#include "../../common/assert.h"
/**********************************************************************************************************************
 *                                			  local constants                                                       *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                           	local macro                                                        *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                             local data type                                                     *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              global variable                                                       *
 *********************************************************************************************************************/
sys_clk_s sys_clk = {
	.pll_clk = 192,
	.cclk = 24,
	.hclk = 24,
	.pclk = 24,
	.mspi_clk = 24,
};
/**********************************************************************************************************************
 *                                              local variable                                                     *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                          local function prototype                                               *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/

extern _attribute_data_retention_ unsigned char tl_24mrc_cal;

/**
 * @brief   This function serves to set 32k clock source.
 * @param[in]   variable of 32k type.
 * @return  none.
 */
void clock_32k_init (CLK_32K_TypeDef src)
{
	unsigned char sel_32k   = analog_read_reg8(0x4e)&0x7f;
	unsigned char power_32k = analog_read_reg8(0x05)&0xfc;
	analog_write_reg8(0x4e, sel_32k|(src<<7));
	if(src)
	{
		analog_write_reg8(0x05, power_32k|0x1);//32k xtal
		//2.set PD0 as pwm output
		unsigned char pwm_clk = read_reg8(0x1401d8);//condition: PCLK is 24MHZ,PCLK = HCLK
		write_reg8(0x1401d8,((pwm_clk & 0xfc) | 0x01));//PCLK = 12M
		unsigned char reg_31e = read_reg8(0x14031e);	//PD0
		write_reg8(0x14031e,reg_31e & 0xfe);
		unsigned short reg_418 = read_reg16(0x140418);	//pwm1 cmp
		write_reg16(0x140418,0x01);
		unsigned short reg_41a = read_reg16(0x14041a);  //pwm1 max
		write_reg16(0x14041a,0x02);
		unsigned char reg_400 = read_reg8(0x140400);	//pwm en
		write_reg8(0x140400,0x02);
		write_reg8(0x140402,0xb6);						//12M/(0xb6 + 1)/2 = 32k

		//3.wait for PWM wake up Xtal
		delay_ms(5);

		//4.Xtal 32k output
		analog_write_reg8(0x03,0x4f); //<7:6>current select

		//5.Recover PD0 as Xtal pin
		write_reg8(0x1401d8,pwm_clk);
		write_reg8(0x14031e,reg_31e);
		write_reg16(0x140418,reg_418);
		write_reg16(0x14041a,reg_41a);
		write_reg8(0x140400,reg_400);
	}
	else
	{
		analog_write_reg8(0x05, power_32k|0x2);//32k rc
	}
}


/**
 * @brief     This function performs to select 24M as the system clock source.
 * @param[in] none.
 * @return    none.
 */
void rc_24m_cal (void)
{
	analog_write_reg8(0xc8, 0x80);


	analog_write_reg8(0x4f, analog_read_reg8(0x4f) | BIT(7) );

	analog_write_reg8(0xc7, 0x0e);
	analog_write_reg8(0xc7, 0x0f);
    while((analog_read_reg8(0xcf) & 0x80) == 0);
    unsigned char cap = analog_read_reg8(0xcb);
    analog_write_reg8(0x52, cap);		//write 24m cap into manual register

    analog_write_reg8(0x4f, analog_read_reg8(0x4f) & (~BIT(7)) );

    analog_write_reg8(0xc7, 0x0e);
	tl_24mrc_cal = analog_read_reg8(0x52);
}

/**
 * @brief     This function performs to select 32K as the system clock source.
 * @param[in] none.
 * @return    none.
 */

void rc_32k_cal (void)
{
	analog_write_reg8(0x4f, ((analog_read_reg8(0x4f) & 0x3f) | 0x40));
	analog_write_reg8(0xc6, 0xf6);
	analog_write_reg8(0xc6, 0xf7);
    while((analog_read_reg8(0xcf) & BIT(6)) == 0);
	unsigned char res1 = analog_read_reg8(0xc9);	//read 32k res[13:6]
	analog_write_reg8(0x51, res1);		//write 32k res[13:6] into manual register
	unsigned char res2 = analog_read_reg8(0xca);	//read 32k res[5:0]
	analog_write_reg8(0x4f, (res2 | (analog_read_reg8(0x4f) & 0xc0)));		//write 32k res[5:0] into manual register
	analog_write_reg8(0xc6, 0xf6);
	analog_write_reg8(0x4f, ((analog_read_reg8(0x4f) & 0x3f) | 0x00));//manual on
}

/**
 * @brief   	This function serves to initialize system.
 * @param[in]	power_mode - power mode(LDO/DCDC/LDO_DCDC)
 * @return  	none
 */
void sys_init(power_mode_e power_mode)
{
	reg_rst 	= 0xffffffff;   //reset function will be cleared by set "1",which is different from the previous configuration
	reg_clk_en 	= 0xffffffff;
	analog_write_reg8(0x8c,0x02);//reg_xo_en_clk_ana_ana=1
	analog_write_reg8(0x7d,0x80);////power on baseband

	analog_write_reg8(0x0a, power_mode);  //set power mode

	analog_write_reg8(0x06,analog_read_reg8(0x06) & (~BIT(0)));////ldo_bbpll_pd = 0

	//stimer
	analog_write_reg8(0x05,analog_read_reg8(0x05) & (~BIT(3)));        //Power up 24MHz XTL oscillator
	reg_system_ctrl	|= FLD_SYSTEM_32K_CAL_EN | FLD_SYSTEM_TIMER_EN ;    //enable 32k cal and stimer

	write_csr(NDS_MILMB,0x01);
	write_csr(NDS_MDLMB,0x80001);


	//add by BLE
	reg_rf_bb_auto_ctrl = 0; //default 0x04

}

/**
 * @brief       This function use to select the system clock source.
 * @param[in]   pll - pll clock.
 * @param[in]	src - cclk source.
 * @param[in]	cclk_div - the cclk divide from pll.it is useless if src is not PAD_PLL_DIV. cclk max is 96M
 * @param[in]	hclk_div - the hclk divide from cclk.hclk max is 48M.
 * @param[in]	pclk_div - the pclk divide from hclk.pclk max is 24M.if hclk = 1/2 * cclk, the pclk can not be 1/4 of hclk.
 * @param[in]	mspi_clk_div - mspi_clk has two source. pll div and hclk.mspi max is 64M.
 * @return      none
 */
_attribute_ram_code_ void clock_init(sys_pll_clk_e pll,
		sys_clock_src_e src,
		sys_pll_div_to_cclk_e cclk_div,
		sys_cclk_div_to_hclk_e hclk_div,
		sys_hclk_div_to_pclk_e pclk_div,
		sys_pll_div_to_mspi_clk_e mspi_clk_div)
{
	//if hclk = 1/2 cclk, pclk can not be 1/4 of hclk.
	if(hclk_div == CCLK_DIV2_TO_HCLK)
	{
		assert(pclk_div != HCLK_DIV4_TO_PCLK);	
	}
	
	//pll clk
	analog_write_reg8(0x80, (analog_read_reg8(0x80) & 0xe0) | ((pll >> 2) & 0x1f));
	analog_write_reg8(0x09, (analog_read_reg8(0x09) & 0xf3) | ((pll&0x03) << 2));
	sys_clk.pll_clk = (pll >> 8);

	//usb clock (192M/4 =48M) pll clock should be the multiple of 48, because USB clock is 48M.
	write_reg8(0x1401fb, sys_clk.pll_clk/48);

	//wait for PLL stable
	analog_write_reg8(0x81, (analog_read_reg8(0x81) | BIT(6)));
	while(BIT(5) != (analog_read_reg8(0x88) & BIT(5)));
	analog_write_reg8(0x81, (analog_read_reg8(0x81) & ~BIT(6)));

	//ensure mspi is not busy status before change mspi clock
	mspi_high();
	while(reg_mspi_status & FLD_MSPI_BUSY);
	while((read_reg8(0x140328) & BIT(0)) == 0);

	//mspi. chagne mspi clock should be ram code.
	if(CCLK_TO_MSPI_CLK == mspi_clk_div)
	{
		write_reg8(0x1401e8, read_reg8(0x1401e8) & 0x7f);  //bit7 0
	}
	else
	{
		write_reg8(0x1401e9, (read_reg8(0x1401e9) & 0x0f) | (mspi_clk_div<<4) );
		write_reg8(0x1401e8, read_reg8(0x1401e8) | BIT(7));  //bit7 1   //if the div is odd, should set two times to ensure the correct sequence.
		write_reg8(0x1401e8, read_reg8(0x1401e8) | BIT(7));  //bit7 1
		sys_clk.mspi_clk = sys_clk.pll_clk / mspi_clk_div;
	}

	//hclk and pclk should be set ahead of cclk, ensure the hclk and pclk not exceed the max clk(cclk max 96M, hclk max 48M, pclk max 24M)
	if(CCLK_DIV1_TO_HCLK == hclk_div)
	{
		write_reg8(0x1401d8, read_reg8(0x1401d8) & ~BIT(2));
	}
	else
	{
		write_reg8(0x1401d8, read_reg8(0x1401d8) | BIT(2));
	}

	//pclk can div1/div2/div4 from hclk.
	if(HCLK_DIV1_TO_PCLK == pclk_div)
	{
		write_reg8(0x1401d8, read_reg8(0x1401d8) & 0xfc);
	}
	else
	{
		write_reg8(0x1401d8, (read_reg8(0x1401d8) & 0xfc) | (pclk_div/2));
	}

	//cclk source
	if(PAD_PLL_DIV == src)
	{
		write_reg8(0x1401e8, (read_reg8(0x1401e8) & 0xf0) | cclk_div);
		sys_clk.cclk = sys_clk.pll_clk / cclk_div;
	}
	else if(PAD_PLL == src)
	{
		sys_clk.cclk = sys_clk.pll_clk;
	}
	else
	{
		sys_clk.cclk = 24;
	}
	write_reg8(0x1401e8, (read_reg8(0x1401e8) & 0x8f) | (src << 4));

	//clk record.
	sys_clk.hclk = sys_clk.cclk/hclk_div;
	sys_clk.pclk = sys_clk.hclk / pclk_div;
	if(CCLK_TO_MSPI_CLK == mspi_clk_div){
		sys_clk.mspi_clk = sys_clk.cclk;
	}
}

/**
 * @brief     This function performs to set delay time by us.
 * @param[in] microsec - need to delay.
 * @return    none
 */
_attribute_ram_code_ void delay_us(u32 microsec)
{
	unsigned long t = sys_get_stimer_tick();
	while(!sys_timeout(t, microsec)){
	}
}

/**
 * @brief     This function performs to set delay time by ms.
 * @param[in] millisec - need to delay.
 * @return    none
 */
_attribute_ram_code_ void delay_ms(u32 millisec)
{

	unsigned long t = sys_get_stimer_tick();
	while(!sys_timeout(t, millisec*1000)){
	}
}
/**********************************************************************************************************************
 *                    						local function implementation                                             *
 *********************************************************************************************************************/
