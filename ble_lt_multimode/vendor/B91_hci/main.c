/********************************************************************************************************
 * @file     main.c 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2015
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
#include <tl_common.h>
#include "drivers.h"
#include "app_config.h"
#include "stack/ble/ble.h"
#include "vendor/common/blt_common.h"
#include "vendor/common/user_config.h"


extern my_fifo_t hci_rx_fifo;
extern void user_init_normal(void);
extern void main_loop (void);

#if 0
_attribute_ram_code_ void irq_handler(void)
{
	irq_blt_sdk_handler ();

#if (HCI_ACCESS==HCI_USE_UART)
	unsigned char irqS = dma_chn_irq_status_get();
    if(irqS & FLD_DMA_CHN_UART_RX)	//rx
    {
    	dma_chn_irq_status_clr(FLD_DMA_CHN_UART_RX);

    	u8* w = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
    	if(w[0]!=0)
    	{
    		my_fifo_next(&hci_rx_fifo);
    		u8* p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
    		reg_dma_uart_rx_addr = (u16)((u32)p);  //switch uart RX dma address
    	}
    }

    if(irqS & FLD_DMA_CHN_UART_TX)	//tx
    {
    	dma_chn_irq_status_clr(FLD_DMA_CHN_UART_TX);
    }
#endif
}


extern my_fifo_t spp_rx_fifo;
_attribute_ram_code_
void dma_irq_handler(void)
{
	volatile static u32 aa0;
	volatile static u32 aa1;
	volatile static u32 aa2;
	volatile static u32 aa3;
	unsigned int uart_dma_irq_src = reg_dma_isr;
	if((uart_dma_irq_src & FLD_DMA_CHANNEL0_IRQ))
	{
		aa0++;
		reg_dma_isr |= FLD_DMA_CHANNEL0_IRQ;
	}
	if((uart_dma_irq_src & FLD_DMA_CHANNEL1_IRQ))
	{
		aa1++;
		reg_dma_isr |= FLD_DMA_CHANNEL1_IRQ;
	}
	if((uart_dma_irq_src & FLD_DMA_CHANNEL3_IRQ))//dma 3 RX
	{
		aa2++;
		reg_dma_isr |= FLD_DMA_CHANNEL3_IRQ;
    	u8* w = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
    	if(w[0]!=0)
    	{
    		my_fifo_next(&hci_rx_fifo);
    	}
    	u8* p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
    	uart_receive_dma(UART0, (unsigned char *)(u32)p);
	}

	if( ( uart_dma_irq_src &FLD_DMA_CHANNEL4_IRQ))////dma 4
	{
		aa3++;
		reg_dma_isr |= FLD_DMA_CHANNEL4_IRQ;
	}
	plic_interrupt_complete(IRQ5_DMA);
}
#endif

_attribute_ram_code_
void rf_irq_handler(void)
{

	//DBG_CHN10_HIGH;

	log_event_irq(BLE_IRQ_DBG_EN, SLEV_irq_rf);

	irq_blt_sdk_handler ();
	//DBG_CHN10_LOW;

}


_attribute_ram_code_
void stimer_irq_handler(void)
{

	//DBG_CHN9_HIGH;
	log_event_irq(BLE_IRQ_DBG_EN, SLEV_irq_sysTimer);

	irq_blt_sdk_handler ();

	//DBG_CHN9_LOW;

}

void uart0_irq_handler(void)
{


	if(uart_get_irq_status(UART0, UART_RXBUF_IRQ_STATUS))
	{
		extern void uart0_recieve_irq(void);

		uart0_recieve_irq();
	}


}


_attribute_ram_code_ int main (void)    //must run in ramcode
{
	blc_pm_select_internal_32k_crystal();

	sys_init(LDO_MODE);

#if (CLOCK_SYS_CLOCK_HZ == 16000000)
	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV12_TO_CCLK, CCLK_DIV1_TO_HCLK,  HCLK_DIV1_TO_PCLK, PLL_DIV8_TO_MSPI_CLK);
#elif (CLOCK_SYS_CLOCK_HZ == 24000000)
	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV8_TO_CCLK, CCLK_DIV1_TO_HCLK,  HCLK_DIV1_TO_PCLK, CCLK_TO_MSPI_CLK);
#elif (CLOCK_SYS_CLOCK_HZ == 32000000)
	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV6_TO_CCLK, CCLK_DIV1_TO_HCLK,  HCLK_DIV1_TO_PCLK, PLL_DIV8_TO_MSPI_CLK);
#elif (CLOCK_SYS_CLOCK_HZ == 48000000)
	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV4_TO_CCLK, CCLK_DIV1_TO_HCLK,  HCLK_DIV2_TO_PCLK, CCLK_TO_MSPI_CLK);
#elif (CLOCK_SYS_CLOCK_HZ == 64000000)
	clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV3_TO_CCLK, CCLK_DIV2_TO_HCLK,  HCLK_DIV1_TO_PCLK, PLL_DIV8_TO_MSPI_CLK);
#endif

	rf_drv_init(RF_MODE_BLE_1M);

	gpio_init(1);

	user_init_normal();

	irq_enable();

	while (1) {
		main_loop ();
	}

	return 0;
}
