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

/**********************************************************************************************************************
 *                                              local variable                                                     *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                          local function prototype                                               *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/


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
 * @brief     This function performs to set delay time by us.
 * @param[in] microsec - need to delay.
 * @return    none
 */
_attribute_ram_code_ void delay_us(u32 microsec)
{
	unsigned long t = clock_time();
	while(!clock_time_exceed(t, microsec)){
	}
}

/**
 * @brief     This function performs to set delay time by ms.
 * @param[in] millisec - need to delay.
 * @return    none
 */
_attribute_ram_code_ void delay_ms(u32 millisec)
{

	unsigned long t = clock_time();
	while(!clock_time_exceed(t, millisec*1000)){
	}
}
/**********************************************************************************************************************
 *                    						local function implementation                                             *
 *********************************************************************************************************************/
