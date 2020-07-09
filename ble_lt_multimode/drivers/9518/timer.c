/********************************************************************************************************
 * @file     timer.c
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

#include "timer.h"
#include "compiler.h"
#include "plic.h"


/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/

/**
 * @brief     the specifed timer start working.
 * @param[in] type - select the timer to start.
 * @return    none
 */
void timer_start(timer_type_e type)
{
 	switch(type)
 	{
 		case TIMER0:
 			reg_tmr_ctrl0 |= FLD_TMR0_EN;
 			break;
 		case TIMER1:
 			reg_tmr_ctrl0 |= FLD_TMR1_EN;
 			break;
 		default:
 			break;
 	}
}

/**
 * @brief     set mode, initial tick and capture of timer.
 * @param[in] type - select the timer to start.
 * @param[in] mode - select mode for timer.
 * @param[in] init_tick - initial tick.
 * @param[in] cap_tick  - tick of capture.
 * @return    none
 */
void timer_set_mode(timer_type_e type, timer_mode_e mode,unsigned int init_tick, unsigned int cap_tick)
{
	reg_tmr_tick(type) = init_tick;
	reg_tmr_capt(type) = cap_tick;

	switch(type)
 	{
 		case TIMER0:
 			reg_tmr_sta |= FLD_TMR_STA_TMR0; //clear irq status
 		 	reg_tmr_ctrl0 &= (~FLD_TMR0_MODE);
 		 	reg_tmr_ctrl0 |= mode;
 			break;
 		case TIMER1:
 			reg_tmr_sta |= FLD_TMR_STA_TMR1; //clear irq status
 			reg_tmr_ctrl0 &= (~FLD_TMR1_MODE);
 			reg_tmr_ctrl0 |= (mode<<4);
 			break;
 		default:
 			break;
 	}

}

/**
 * @brief     initiate GPIO for gpio trigger and gpio width mode of timer.
 * @param[in] type - select the timer to start.
 * @param[in] pin - select pin for timer.
 * @param[in] pol - select polarity for gpio trigger and gpio width
 * @param[in] lev_en
 * @return    none
 */
void timer_gpio_init(timer_type_e type, gpio_pin_e pin, gpio_pol_e pol )
{
	gpio_set_gpio_en(pin);
 	gpio_set_output_dis(pin); 	//disable output
 	gpio_set_input_en(pin);		//enable input

 	switch(type)
 	{
 		case TIMER0:
 		 	if(pol==POL_FALLING)
 		 	{
 		 		gpio_set_up_down_res(pin,PM_PIN_PULLUP_10K);
 		 		gpio_gpio2risc0_irq_en(pin);
 		 		gpio_set_gpio2risc0_irq_trigger_type(pin,INTR_LOW_LEVEL);
 		 	}
 		 	else if(pol==POL_RISING)
 		 	{
 		 		gpio_set_up_down_res(pin,PM_PIN_PULLDOWN_100K);
 		 		gpio_gpio2risc0_irq_en(pin);
 		 		gpio_set_gpio2risc0_irq_trigger_type(pin,INTR_HIGH_LEVEL);
 		 	}
 			break;

 		case TIMER1:
 		 	if(pol==POL_FALLING)
 		 	{
 		 		gpio_set_up_down_res(pin,PM_PIN_PULLUP_10K);
 		 		gpio_gpio2risc1_irq_en(pin);
 		 		gpio_set_gpio2risc1_irq_trigger_type(pin,INTR_LOW_LEVEL);
 		 	}
 		 	else if(pol==POL_RISING)
 		 	{
 		 		gpio_set_up_down_res(pin,PM_PIN_PULLDOWN_100K);
 		 		gpio_gpio2risc1_irq_en(pin);
 		 		gpio_set_gpio2risc1_irq_trigger_type(pin,INTR_HIGH_LEVEL);

 		 	}
 			break;

 		default:
 			break;
 	}

}


