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
#include "core.h"


/**
 * @brief    This function serves to config plic when enter some function process for example flash,analog.
 * @param[in]   preempt_en -1 can disturb by interrupt, 0 can disturb by interrupt.
 * @param[in]   threshold- interrupt source.when the interrupt priority> function process will be disturb by interrupt.
 * @return  none
*/
_attribute_ram_code_sec_noinline_ unsigned int plic_enter_critical_sec(unsigned char preempt_en ,unsigned char threshold)
{
	unsigned int r;
	if(g_plic_preempt_en&&preempt_en)
	{
		plic_set_threshold(threshold);
		r=0;
	}
	else
	{
	   r = core_interrupt_disable();
	}
	return r ;
}

/**
 * @brief    This function serves to config plic when exit some function process such as flash,analog.
 * @param[in]   preempt_en -1 can disturb by interrupt, 0 can disturb by interrupt.
 * @param[in]   r- the value of mie register to restore.
 * @return  none
*/
_attribute_ram_code_sec_noinline_ void  plic_exit_critical_sec(unsigned char preempt_en ,unsigned int r)
{
	if (g_plic_preempt_en&&preempt_en)
	{
		plic_set_threshold(0);
	}
	else
	{
		core_restore_interrupt(r);
	}
}

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

 __attribute__((section(".ram_code"))) void default_irq_handler(void)
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
_attribute_ram_code_sec_ __attribute__((weak)) void except_handler()
{
	unsigned int mcause = read_csr(NDS_MCAUSE);
	unsigned int mepc = read_csr(NDS_MEPC);

	while(1){
		/* Unhandled Trap */
		printf("Unhandled Trap : mcause = 0x%x, mepc = 0x%x\n", (unsigned int)mcause, (unsigned int)mepc);
		for(volatile unsigned int i = 0; i < 0xffff; i++)
		{
			asm("nop");
		}
	}
}
_attribute_ram_code_sec_noinline_  void trap_entry(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void trap_entry(void)
{
	except_handler();
}

/**
 * @brief system timer interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq1(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq1(void)
{
	core_save_nested_context();
	stimer_irq_handler();
	/* Must ensure the order of execution of complete and
	 * subsequent mret instructions(insert fence instruction)*/
	core_restore_nested_context();
	plic_interrupt_complete(IRQ1_SYSTIMER);
	NDS_FENCE_IORW;
}

/**
 * @brief analog interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq2(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq2(void)
{
	core_save_nested_context();
	analog_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ2_ALG);
	NDS_FENCE_IORW;
}


/**
 * @brief timer1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq3(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq3(void)
{
	core_save_nested_context();
	timer1_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ3_TIMER1);
	NDS_FENCE_IORW;
}

/**
 * @brief timer0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq4(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq4(void)
{
	core_save_nested_context();
	timer0_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ4_TIMER0);
	NDS_FENCE_IORW;
}


/**
 * @brief dma interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq5(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq5(void)
{
	core_save_nested_context();
	dma_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ5_DMA);
	NDS_FENCE_IORW;
}

/**
 * @brief bmc interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq6(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq6(void)
{
	core_save_nested_context();
	bmc_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ6_BMC);
	NDS_FENCE_IORW;
}


/**
 * @brief usb control endpoint setup interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq7(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq7(void)
{
	core_save_nested_context();
	usb_ctrl_ep_setup_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ7_USB_CTRL_EP_SETUP);
	NDS_FENCE_IORW;
}

/**
 * @brief usb control endpoint data interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq8(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq8(void)
{
	core_save_nested_context();
	usb_ctrl_ep_data_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ8_USB_CTRL_EP_DATA);
	NDS_FENCE_IORW;
}

/**
 * @brief usb control endpoint status interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq9(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq9(void)
{
	core_save_nested_context();
	usb_ctrl_ep_status_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ9_USB_CTRL_EP_STATUS);
	NDS_FENCE_IORW;
}


/**
 * @brief usb control endpoint setinf interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq10(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq10(void)
{
	core_save_nested_context();
	usb_ctrl_ep_setinf_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ10_USB_CTRL_EP_SETINF);
	NDS_FENCE_IORW;
}

/**
 * @brief usb endpoint interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq11(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq11(void)
{
	core_save_nested_context();
	usb_endpoint_irq_handler ();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ11_USB_ENDPOINT);
	NDS_FENCE_IORW;
}

/**
 * @brief rf dual mode interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq12(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq12(void)
{
	core_save_nested_context();
	rf_dm_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ12_ZB_DM);
	NDS_FENCE_IORW;
}

/**
 * @brief rf ble interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq13(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq13(void)
{
	core_save_nested_context();
	rf_ble_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ13_ZB_BLE);
	NDS_FENCE_IORW;
}


/**
 * @brief rf BT  interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq14(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq14(void)
{
	core_save_nested_context();
	rf_bt_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ14_ZB_BT);
	NDS_FENCE_IORW;
}

/**
 * @brief telink rf interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq15(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq15(void)
{
	core_save_nested_context();
	rf_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ15_ZB_RT);
	NDS_FENCE_IORW;
}



/**
 * @brief pwm interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq16(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq16(void)
{
	core_save_nested_context();
	pwm_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ16_PWM);
	NDS_FENCE_IORW;
}

/**
 * @brief pke interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq17(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq17(void)
{
	core_save_nested_context();
	pke_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ17_PKE);
	NDS_FENCE_IORW;
}



/**
 * @brief uart1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq18(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq18(void)
{
	core_save_nested_context();
	uart1_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ18_UART1);
	NDS_FENCE_IORW;
}



/**
 * @brief uart0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq19(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq19(void)
{
	core_save_nested_context();
	uart0_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ19_UART0);
	NDS_FENCE_IORW;
}


/**
 * @brief audio interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq20(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq20(void)
{
	core_save_nested_context();
	audio_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ20_DFIFO);
	NDS_FENCE_IORW;
}

/**
 * @brief i2c interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq21(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq21(void)
{
	core_save_nested_context();
	i2c_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ21_I2C);
	NDS_FENCE_IORW;
}


/**
 * @brief hspi interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq22(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq22(void)
{
	core_save_nested_context();
	hspi_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ22_SPI_AHB);
	NDS_FENCE_IORW;
}


/**
 * @brief pspi interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq23(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq23(void)
{
	core_save_nested_context();
	pspi_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ23_SPI_APB);
	NDS_FENCE_IORW;
}


/**
 * @brief usb power down interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq24(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq24(void)
{
	core_save_nested_context();
	usb_pwdn_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ24_USB_PWDN);
	NDS_FENCE_IORW;
}

/**
 * @brief gpio interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq25(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq25(void)
{
	core_save_nested_context();
	gpio_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ25_GPIO);
	NDS_FENCE_IORW;
}

/**
 * @brief gpio_risc0 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq26(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq26(void)
{
	core_save_nested_context();
	gpio_risc0_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ26_GPIO2RISC0);
	NDS_FENCE_IORW;
}


/**
 * @brief gpio_risc1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq27(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq27(void)
{
	core_save_nested_context();
	gpio_risc1_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ27_GPIO2RISC1);
	NDS_FENCE_IORW;
}

/**
 * @brief soft interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq28(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq28(void)
{
	core_save_nested_context();
	soft_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ28_SOFT);
	NDS_FENCE_IORW;
}

/**
 * @brief npe bus0 interrupt handler.
 * @return none
 */

_attribute_ram_code_sec_noinline_ void  entry_irq29(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq29(void)
{
	core_save_nested_context();
	npe_bus0_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ29_NPE_BUS0);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus1 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq30(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq30(void)
{
	core_save_nested_context();
	npe_bus1_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ30_NPE_BUS1);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus2 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq31(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq31(void)
{
	core_save_nested_context();
	npe_bus2_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ31_NPE_BUS2);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus3 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq32(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq32(void)
{
	core_save_nested_context();
	npe_bus3_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ32_NPE_BUS3);
	NDS_FENCE_IORW;
}

/**
 * @brief npe bus4 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq33(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq33(void)
{
	core_save_nested_context();
	npe_bus4_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ33_NPE_BUS4);
	NDS_FENCE_IORW;
}
/**
 * @brief usb 250us interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq34(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq34(void)
{
	core_save_nested_context();
	usb_250us_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ34_USB_250US);
	NDS_FENCE_IORW;
}
/**
 * @brief usb reset interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq35(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq35(void)
{
	core_save_nested_context();
	usb_reset_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ35_USB_RESET);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus7 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq36(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq36(void)
{
	core_save_nested_context();
	npe_bus7_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ36_NPE_BUS7);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus8 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq37(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq37(void)
{
	core_save_nested_context();
	npe_bus8_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ37_NPE_BUS8);
	NDS_FENCE_IORW;
}


_attribute_ram_code_sec_noinline_ void  entry_irq38(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq38(void)
{

}
_attribute_ram_code_sec_noinline_ void  entry_irq39(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq39(void)
{

}
_attribute_ram_code_sec_noinline_ void  entry_irq40(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq40(void)
{

}
_attribute_ram_code_sec_noinline_ void  entry_irq41(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq41(void)
{

}
/**
 * @brief npe bus13 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq42(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq42(void)
{
	core_save_nested_context();
	npe_bus13_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ42_NPE_BUS13);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus14 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq43(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq43(void)
{
	core_save_nested_context();
	npe_bus14_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ43_NPE_BUS14);
	NDS_FENCE_IORW;
}

/**
 * @brief npe bus15 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq44(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq44(void)
{
	core_save_nested_context();
	npe_bus15_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ44_NPE_BUS15);
	NDS_FENCE_IORW;
}
_attribute_ram_code_sec_noinline_ void  entry_irq45(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq45(void)
{

}
/**
 * @brief npe bus17 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq46(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq46(void)
{
	core_save_nested_context();
	npe_bus17_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ46_NPE_BUS17);
	NDS_FENCE_IORW;
}


_attribute_ram_code_sec_noinline_ void  entry_irq47(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq47(void)
{

}

_attribute_ram_code_sec_noinline_ void  entry_irq48(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq48(void)
{

}

_attribute_ram_code_sec_noinline_ void  entry_irq49(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq49(void)
{

}
/**
 * @brief npe bus21 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq50(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq50(void)
{
	core_save_nested_context();
	npe_bus21_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ50_NPE_BUS21);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus22 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq51(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq51(void)
{
	core_save_nested_context();
	npe_bus22_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ51_NPE_BUS22);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus23 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq52(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq52(void)
{
	core_save_nested_context();
	npe_bus23_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ52_NPE_BUS23);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus24 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq53(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq53(void)
{
	core_save_nested_context();
	npe_bus24_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ53_NPE_BUS24);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus25 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq54(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq54(void)
{
	core_save_nested_context();
	npe_bus25_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ54_NPE_BUS25);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus26 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq55(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq55(void)
{
	core_save_nested_context();
	npe_bus26_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ55_NPE_BUS26);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus27 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq56(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq56(void)
{
	core_save_nested_context();
	npe_bus27_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ56_NPE_BUS27);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus28 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq57(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq57(void)
{
	core_save_nested_context();
	npe_bus28_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ57_NPE_BUS28);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus29 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq58(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq58(void)
{
	core_save_nested_context();
	npe_bus29_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ58_NPE_BUS29);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus30 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq59(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq59(void)
{
	core_save_nested_context();
	npe_bus30_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ59_NPE_BUS30);
	NDS_FENCE_IORW;
}
/**
 * @brief npe bus31 interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq60(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq60(void)
{
	core_save_nested_context();
	npe_bus31_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ60_NPE_BUS31);
	NDS_FENCE_IORW;
}
/**
 * @brief npe comb interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq61(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq61(void)
{
	core_save_nested_context();
	npe_comb_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ61_NPE_COMB);
	NDS_FENCE_IORW;
}
/**
 * @brief pm interrupt handler.An interrupt will be generated after wake-up
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq62(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq62(void)
{
	core_save_nested_context();
	pm_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ62_PM_TM);
	NDS_FENCE_IORW;
}
/**
 * @brief eoc interrupt handler.
 * @return none
 */
_attribute_ram_code_sec_noinline_ void  entry_irq63(void) __attribute__ ((interrupt ("machine") , aligned(4)));
void  entry_irq63(void)
{
	core_save_nested_context();
	eoc_irq_handler();
	core_restore_nested_context();
	plic_interrupt_complete(IRQ63_EOC);
	NDS_FENCE_IORW;
}



/// @} DRIVERS
