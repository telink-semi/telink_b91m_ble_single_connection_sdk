/********************************************************************************************************
 * @file     main.c 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2018
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

	log_event_irq(BLE_IRQ_DBG_EN, SLEV_irq_rf);

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

	log_event_irq(BLE_IRQ_DBG_EN, SLEV_irq_sysTimer);

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
//	blc_pm_select_internal_32k_crystal();

	sys_init(LDO_MODE);

#if (CLOCK_SYS_CLOCK_HZ == 16000000)
	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV12_TO_CCLK, CCLK_DIV1_TO_HCLK,  HCLK_DIV1_TO_PCLK, PLL_DIV4_TO_MSPI_CLK);
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV8_TO_CCLK, CCLK_DIV1_TO_HCLK,  HCLK_DIV1_TO_PCLK, PLL_DIV4_TO_MSPI_CLK);
#elif (CLOCK_SYS_CLOCK_HZ == 32000000)
	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV6_TO_CCLK, CCLK_DIV1_TO_HCLK,  HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK);
#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV4_TO_CCLK, CCLK_DIV1_TO_HCLK,  HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK);
#elif (CLOCK_SYS_CLOCK_HZ == 64000000)
	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV3_TO_CCLK, CCLK_DIV2_TO_HCLK,  HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK);
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


