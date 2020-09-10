/********************************************************************************************************
 * @file	main.c
 *
 * @brief	This is the source file for B91
 *
 * @author	Ble Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *          All rights reserved.
 *          
 *          Redistribution and use in source and binary forms, with or without
 *          modification, are permitted provided that the following conditions are met:
 *          
 *              1. Redistributions of source code must retain the above copyright
 *              notice, this list of conditions and the following disclaimer.
 *          
 *              2. Unless for usage inside a TELINK integrated circuit, redistributions 
 *              in binary form must reproduce the above copyright notice, this list of 
 *              conditions and the following disclaimer in the documentation and/or other
 *              materials provided with the distribution.
 *          
 *              3. Neither the name of TELINK, nor the names of its contributors may be 
 *              used to endorse or promote products derived from this software without 
 *              specific prior written permission.
 *          
 *              4. This software, with or without modification, must only be used with a
 *              TELINK integrated circuit. All other usages are subject to written permission
 *              from TELINK and different commercial license may apply.
 *
 *              5. Licensee shall be solely responsible for any claim to the extent arising out of or 
 *              relating to such deletion(s), modification(s) or alteration(s).
 *         
 *          THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 *          ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 *          WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *          DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDER BE LIABLE FOR ANY
 *          DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 *          (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 *          LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 *          ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *          (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *          SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *         
 *******************************************************************************************************/
#include "app_config.h"
#include "tl_common.h"
#include "../common/blt_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "app_audio.h"

extern void user_init_normal();
extern void main_loop (void);
extern void usb_init(void) ;


/**
 * @brief		BLE SDK RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void rf_irq_handler(void)
{

	DBG_CHN10_HIGH;

	irq_blt_sdk_handler ();

	DBG_CHN10_LOW;

}


/**
 * @brief		BLE SDK System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void stimer_irq_handler(void)
{

	DBG_CHN11_HIGH;

	irq_blt_sdk_handler ();


	DBG_CHN11_LOW;

}


/**
 * @brief		BLE USB endpoint interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void usb_endpoint_irq_handler(void)
{

	usb_endpoints_irq_handler ();




}


/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)   //must on ramcode
{
	blc_pm_select_internal_32k_crystal();

	sys_init(LDO_1P4_LDO_1P8);

#if (CLOCK_SYS_CLOCK_HZ == 16000000)
	CCLK_16M_HCLK_16M_PCLK_16M;
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
	CCLK_24M_HCLK_24M_PCLK_24M;
#elif (CLOCK_SYS_CLOCK_HZ == 32000000)
	CCLK_32M_HCLK_32M_PCLK_16M;
#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
	CCLK_48M_HCLK_48M_PCLK_24M;
#elif (CLOCK_SYS_CLOCK_HZ == 64000000)
	CCLK_64M_HCLK_32M_PCLK_16M
#endif

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(1);

#if (APPLICATION_DONGLE)
	usb_init();
#endif

	user_init_normal();

	irq_enable();

	while (1) {
		main_loop ();
	}

	return 0;
}


