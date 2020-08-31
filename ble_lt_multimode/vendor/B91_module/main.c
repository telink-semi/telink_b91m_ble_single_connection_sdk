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

#include "tl_common.h"
#include "../common/blt_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

extern void user_init_normal();
extern void user_init();
extern void main_loop (void);
extern volatile unsigned char rec_buff[];

_attribute_ram_code_
void rf_irq_handler(void)
{
	NESTED_IRQ_ENTER();
	DBG_CHN10_HIGH;

	log_event_irq(BLE_IRQ_DBG_EN, SLEV_irq_rf);

	irq_blt_sdk_handler ();
	DBG_CHN10_LOW;
	NESTED_IRQ_EXIT();
	plic_interrupt_complete(IRQ15_ZB_RT);
	NDS_FENCE_IORW;
}


_attribute_ram_code_
void stimer_irq_handler(void)
{
	NESTED_IRQ_ENTER();
	DBG_CHN9_HIGH;
	log_event_irq(BLE_IRQ_DBG_EN, SLEV_irq_sysTimer);

	irq_blt_sdk_handler ();

	DBG_CHN9_LOW;
	NESTED_IRQ_EXIT();
	plic_interrupt_complete(IRQ1_SYSTIMER);  	//plic_interrupt_complete
	NDS_FENCE_IORW;
}

void uart0_irq_handler(void)
{
	NESTED_IRQ_ENTER();

	if(uart_get_irq_status(UART0, UART_RXBUF_IRQ_STATUS))
	{
		extern void uart0_recieve_irq(void);

		uart0_recieve_irq();
	}

	NESTED_IRQ_EXIT();
	plic_interrupt_complete(IRQ19_UART0);
	NDS_FENCE_IORW;
}

/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
int main (void)   //must on ramcode
{
	blc_pm_select_internal_32k_crystal();

	cpu_wakeup_init(LDO_MODE);

	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

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

	gpio_init(!deepRetWakeUp);

	user_init_normal();

	irq_enable();

	while (1) {
		main_loop ();
	}

	return 0;
}
