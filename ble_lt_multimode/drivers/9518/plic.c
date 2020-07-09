/********************************************************************************************************
 * @file     interrupt.c
 *
 * @brief    This is the source file for TLSR9518
 *
 * @author	 Driver Group
 * @date     September 16, 2019
 *
 * @par      Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
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

#include "../../drivers.h"

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void default_irq_handler(void)
{
	//printf("Default interrupt handler\n");

}
void stimer_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void analog_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void timer1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void timer0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void dma_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void bmc_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_setup_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_data_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_status_irq_handler(void)  __attribute__((weak, alias("default_irq_handler")));
void usb_ctrl_ep_setinf_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_endpoint_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void rf_dm_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void rf_ble_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void rf_bt_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void rf_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void pwm_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void pke_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void uart1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void uart0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void audio_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void i2c_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void hspi_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void pspi_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_pwdn_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_risc0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void gpio_risc1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void soft_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus0_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus1_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus2_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus3_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus4_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_250us_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void usb_reset_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus7_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus8_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus13_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus14_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus15_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus17_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus21_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus22_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus23_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus24_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus25_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus26_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus27_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus28_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));

void npe_bus29_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus30_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_bus31_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void npe_comb_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void pm_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
void eoc_irq_handler(void) __attribute__((weak, alias("default_irq_handler")));
/**
 * @brief  exception handler.
 * 		   this defines an exception handler to handle all the platform pre-defined
 *         exceptions.
 * @return none
 */

void trap_entry(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void trap_entry(void)
{}

/**
 * @brief system timer interrupt handler.
 * @return none
 */
void  entry_irq1(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq1(void)
{
	stimer_irq_handler();
}

/**
 * @brief analog interrupt handler.
 * @return none
 */
void  entry_irq2(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq2(void)
{
	analog_irq_handler();
}


/**
 * @brief timer1 interrupt handler.
 * @return none
 */
void  entry_irq3(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq3(void)
{
	timer1_irq_handler();
}

/**
 * @brief timer0 interrupt handler.
 * @return none
 */
void  entry_irq4(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq4(void)
{
	timer0_irq_handler();
}


/**
 * @brief dma interrupt handler.
 * @return none
 */
void  entry_irq5(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq5(void)
{
	dma_irq_handler();
}

/**
 * @brief bmc interrupt handler.
 * @return none
 */
void  entry_irq6(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq6(void)
{
	bmc_irq_handler();
}


/**
 * @brief usb control endpoint setup interrupt handler.
 * @return none
 */
void  entry_irq7(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq7(void)
{
	usb_ctrl_ep_setup_irq_handler();
}

/**
 * @brief usb control endpoint data interrupt handler.
 * @return none
 */
void  entry_irq8(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq8(void)
{
	usb_ctrl_ep_data_irq_handler();
}

/**
 * @brief usb control endpoint status interrupt handler.
 * @return none
 */
void  entry_irq9(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq9(void)
{
	usb_ctrl_ep_status_irq_handler();
}


/**
 * @brief usb control endpoint setinf interrupt handler.
 * @return none
 */
void  entry_irq10(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq10(void)
{
	usb_ctrl_ep_setinf_irq_handler();
}

/**
 * @brief usb endpoint interrupt handler.
 * @return none
 */
void  entry_irq11(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq11(void)
{
	usb_endpoint_irq_handler ();
}

/**
 * @brief rf dual mode interrupt handler.
 * @return none
 */
void  entry_irq12(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq12(void)
{
	rf_dm_irq_handler();
}

/**
 * @brief rf ble interrupt handler.
 * @return none
 */
void  entry_irq13(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq13(void)
{
	rf_ble_irq_handler();
}


/**
 * @brief rf BT  interrupt handler.
 * @return none
 */
void  entry_irq14(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq14(void)
{
	rf_bt_irq_handler();
}

/**
 * @brief telink rf interrupt handler.
 * @return none
 */
void  entry_irq15(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq15(void)
{
	rf_irq_handler();
}



/**
 * @brief pwm interrupt handler.
 * @return none
 */
void  entry_irq16(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq16(void)
{
	pwm_irq_handler();
}

/**
 * @brief pke interrupt handler.
 * @return none
 */
void  entry_irq17(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq17(void)
{
	pke_irq_handler();
}



/**
 * @brief uart1 interrupt handler.
 * @return none
 */
void  entry_irq18(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq18(void)
{
	uart1_irq_handler();
}



/**
 * @brief uart0 interrupt handler.
 * @return none
 */
void  entry_irq19(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq19(void)
{
	uart0_irq_handler();
}


/**
 * @brief audio interrupt handler.
 * @return none
 */
void  entry_irq20(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq20(void)
{
	audio_irq_handler();
}

/**
 * @brief i2c interrupt handler.
 * @return none
 */
void  entry_irq21(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq21(void)
{
	i2c_irq_handler();
}


/**
 * @brief hspi interrupt handler.
 * @return none
 */
void  entry_irq22(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq22(void)
{
	hspi_irq_handler();
}


/**
 * @brief pspi interrupt handler.
 * @return none
 */
void  entry_irq23(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq23(void)
{
	pspi_irq_handler();
}


/**
 * @brief usb power down interrupt handler.
 * @return none
 */
void  entry_irq24(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq24(void)
{
	usb_pwdn_irq_handler();
}

/**
 * @brief gpio interrupt handler.
 * @return none
 */
void  entry_irq25(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq25(void)
{
	gpio_irq_handler();
}

/**
 * @brief gpio_risc0 interrupt handler.
 * @return none
 */
void  entry_irq26(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq26(void)
{
	gpio_risc0_irq_handler();
}


/**
 * @brief gpio_risc1 interrupt handler.
 * @return none
 */
void  entry_irq27(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq27(void)
{
	gpio_risc1_irq_handler();
}

/**
 * @brief soft interrupt handler.
 * @return none
 */
void  entry_irq28(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq28(void)
{
	soft_irq_handler();
}

/**
 * @brief npe bus0 interrupt handler.
 * @return none
 */

void  entry_irq29(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq29(void)
{
	npe_bus0_irq_handler();
}
/**
 * @brief npe bus1 interrupt handler.
 * @return none
 */
void  entry_irq30(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq30(void)
{
	npe_bus1_irq_handler();
}
/**
 * @brief npe bus2 interrupt handler.
 * @return none
 */
void  entry_irq31(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq31(void)
{
	npe_bus2_irq_handler();
}
/**
 * @brief npe bus3 interrupt handler.
 * @return none
 */
void  entry_irq32(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq32(void)
{
	npe_bus3_irq_handler();
}

/**
 * @brief npe bus4 interrupt handler.
 * @return none
 */
void  entry_irq33(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq33(void)
{
	npe_bus4_irq_handler();
}
/**
 * @brief usb 250us interrupt handler.
 * @return none
 */
void  entry_irq34(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq34(void)
{
	usb_250us_irq_handler();
}
/**
 * @brief usb reset interrupt handler.
 * @return none
 */
void  entry_irq35(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq35(void)
{
	usb_reset_irq_handler();
}
/**
 * @brief npe bus7 interrupt handler.
 * @return none
 */
void  entry_irq36(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq36(void)
{
	npe_bus7_irq_handler();
}
/**
 * @brief npe bus8 interrupt handler.
 * @return none
 */
void  entry_irq37(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq37(void)
{
	npe_bus8_irq_handler();
}


void  entry_irq38(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq38(void)
{

}
void  entry_irq39(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq39(void)
{

}
void  entry_irq40(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq40(void)
{

}
void  entry_irq41(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq41(void)
{

}
/**
 * @brief npe bus13 interrupt handler.
 * @return none
 */
void  entry_irq42(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq42(void)
{
	npe_bus13_irq_handler();
}
/**
 * @brief npe bus14 interrupt handler.
 * @return none
 */
void  entry_irq43(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq43(void)
{
	npe_bus14_irq_handler();
}

/**
 * @brief npe bus15 interrupt handler.
 * @return none
 */
void  entry_irq44(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq44(void)
{
	npe_bus15_irq_handler();
}
void  entry_irq45(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq45(void)
{

}
/**
 * @brief npe bus17 interrupt handler.
 * @return none
 */
void  entry_irq46(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq46(void)
{
	npe_bus17_irq_handler();
}


void  entry_irq47(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq47(void)
{

}

void  entry_irq48(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq48(void)
{

}

void  entry_irq49(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq49(void)
{

}
/**
 * @brief npe bus21 interrupt handler.
 * @return none
 */
void  entry_irq50(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq50(void)
{
	npe_bus21_irq_handler();
}
/**
 * @brief npe bus22 interrupt handler.
 * @return none
 */
void  entry_irq51(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq51(void)
{
	npe_bus22_irq_handler();
}
/**
 * @brief npe bus23 interrupt handler.
 * @return none
 */
void  entry_irq52(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq52(void)
{
	npe_bus23_irq_handler();
}
/**
 * @brief npe bus24 interrupt handler.
 * @return none
 */
void  entry_irq53(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq53(void)
{
	npe_bus24_irq_handler();
}
/**
 * @brief npe bus25 interrupt handler.
 * @return none
 */
void  entry_irq54(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq54(void)
{
	npe_bus25_irq_handler();
}
/**
 * @brief npe bus26 interrupt handler.
 * @return none
 */
void  entry_irq55(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq55(void)
{
	npe_bus26_irq_handler();
}
/**
 * @brief npe bus27 interrupt handler.
 * @return none
 */
void  entry_irq56(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq56(void)
{
	npe_bus27_irq_handler();
}
/**
 * @brief npe bus28 interrupt handler.
 * @return none
 */
void  entry_irq57(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq57(void)
{
	npe_bus28_irq_handler();
}
/**
 * @brief npe bus29 interrupt handler.
 * @return none
 */
void  entry_irq58(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq58(void)
{
	npe_bus29_irq_handler();
}
/**
 * @brief npe bus30 interrupt handler.
 * @return none
 */
void  entry_irq59(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq59(void)
{
	npe_bus30_irq_handler();
}
/**
 * @brief npe bus31 interrupt handler.
 * @return none
 */
void  entry_irq60(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq60(void)
{
	npe_bus31_irq_handler();
}
/**
 * @brief npe comb interrupt handler.
 * @return none
 */
void  entry_irq61(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq61(void)
{
	npe_comb_irq_handler();
}
/**
 * @brief pm interrupt handler.An interrupt will be generated after wake-up
 * @return none
 */
void  entry_irq62(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq62(void)
{
	pm_irq_handler();
}
/**
 * @brief eoc interrupt handler.
 * @return none
 */
void  entry_irq63(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq63(void)
{
	eoc_irq_handler();
}



/// @} DRIVERS
