/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par     Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "common/config/user_config.h"
#include "app_config.h"
#include "app.h"
#include "app_buffer.h"
#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"
#include "../default_att.h"

#if (FEATURE_TEST_MODE == TEST_BLE_PHY)

MYFIFO_INIT(hci_rx_fifo, 72, 2);
MYFIFO_INIT(hci_tx_fifo, 72, 8);

_attribute_data_retention_	int device_in_connection_state;

_attribute_data_retention_	u32 advertise_begin_tick;

_attribute_data_retention_	u32	interval_update_tick;

_attribute_data_retention_	u8	sendTerminate_before_enterDeep = 0;

_attribute_data_retention_	u32	latest_user_event_tick;

extern blc_main_loop_phyTest_callback_t	blc_main_loop_phyTest_cb;

volatile u32 test1,test2,test3,test4;
volatile u8 test_addr[16];
extern unsigned int g_chip_version;

u8 uart_dma_send_flag = 0;
volatile unsigned char tx_byte_buff[16] __attribute__((aligned(4))) ={0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
volatile unsigned char rec_buff[32] __attribute__((aligned(4))) = {0};
volatile unsigned char rev_data_len=0;

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_EXIT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void	user_set_rf_power (u8 e, u8 *p, int n)
{
	rf_set_power_level_index (MY_RF_POWER_INDEX);
}

#if 1

/**
 * @brief      UART0 irq function
 * @param[in]  none
 * @return     none
 */
_attribute_ram_code_sec_ void uart0_irq_handler(void)
{
    if(uart_get_irq_status(UART0,UART_TXDONE))
	{
		gpio_toggle(GPIO_LED_GREEN);
	    uart_clr_tx_done(UART0);
	}

    if(uart_get_irq_status(UART0,UART_RXDONE)) //A0-SOC can't use RX-DONE status,so this interrupt can noly used in A1-SOC.
    {
    	gpio_toggle(GPIO_LED_WHITE);
    	/************************get the length of receive data****************************/
    	if(((reg_uart_status1(UART0)&FLD_UART_RBCNT)%4)==0)
    	{
			rev_data_len=4*(0xffffff-reg_dma_size(DMA2));
    	}
    	else
    	{
    		rev_data_len=4*(0xffffff-reg_dma_size(DMA2)-1)+(reg_uart_status1(UART0)&FLD_UART_RBCNT)%4;
    	}
    	/************************cll rx_irq****************************/
    	uart_clr_irq_status(UART0,UART_CLR_RX);
		if(rev_data_len!=0)
		{
			u8* p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
			p[0] = rev_data_len;
			my_fifo_next(&hci_rx_fifo);
			p = hci_rx_fifo.p + (hci_rx_fifo.wptr & (hci_rx_fifo.num-1)) * hci_rx_fifo.size;
			uart_receive_dma(UART0, (unsigned char*)(p+4),hci_rx_fifo.size);
		}

    	if((uart_get_irq_status(UART0,UART_RX_ERR)))
    	{
    		uart_clr_irq_status(UART0,UART_CLR_RX);
    	}
    }

}
#else

#define UART_DMA_CHANNEL_RX  DMA2
#define UART_DMA_CHANNEL_TX  DMA3


_attribute_ram_code_sec_ void uart0_irq_handler(void)
{
    if(uart_get_irq_status(UART0,UART_TXDONE))
	{
    	uart_dma_send_flag=1;
	    uart_clr_tx_done(UART0);
	}

    if(uart_get_irq_status(UART0,UART_RXDONE)) //A0-SOC can't use RX-DONE status,so this interrupt can noly used in A1-SOC.
    {
    	test2 ++;
    	/************************get the length of receive data****************************/
    	if(((reg_uart_status1(UART0)&FLD_UART_RBCNT)%4)==0)
    	{
    		rev_data_len=4*(0xffffff-reg_dma_size(UART_DMA_CHANNEL_RX));
    	}
    	else
    	{
    		rev_data_len=4*(0xffffff-reg_dma_size(UART_DMA_CHANNEL_RX)-1)+(reg_uart_status1(UART0)&FLD_UART_RBCNT)%4;
    	}
    	/************************cll rx_irq****************************/
    	uart_clr_irq_status(UART0,UART_CLR_RX);
    	uart_send_dma(UART0, (unsigned char*)rec_buff, rev_data_len);
    	uart_receive_dma(UART0, (unsigned char*)rec_buff,32);
    	if((uart_get_irq_status(UART0,UART_RX_ERR)))
    	{
    		uart_clr_irq_status(UART0,UART_CLR_RX);
    	}
    }

}

#endif

#if 1

void phy_test_uart_init(void)
{
	unsigned short div;
	unsigned char bwpc;

	uart_reset(UART0);
	uart_set_pin(UART0_TX_PB2,UART0_RX_PB3);
	uart_cal_div_and_bwpc(115200, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_set_dma_rx_timeout(UART0, bwpc, 12, UART_BW_MUL1);
	uart_init(UART0, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	core_interrupt_enable();
	uart_set_tx_dma_config(UART0, DMA3);
	uart_set_rx_dma_config(UART0, DMA2);
	uart_clr_tx_done(UART0);
	dma_clr_irq_mask(DMA3,TC_MASK|ABT_MASK|ERR_MASK);
	dma_clr_irq_mask(DMA2,TC_MASK|ABT_MASK|ERR_MASK);

	uart_receive_dma(UART0, (unsigned char*)(hci_rx_fifo.p + 4),hci_rx_fifo.size);
	uart_set_irq_mask(UART0, UART_RXDONE_MASK);
	uart_set_irq_mask(UART0, UART_TXDONE_MASK);
	plic_interrupt_enable(IRQ19_UART0);
	reg_uart_status2(UART0) &= FLD_UART_TX_DONE;

}
#else

void phy_test_uart_init(void)
{
	unsigned short div;
	unsigned char bwpc;


	uart_reset(UART0);
	uart_set_pin(UART0_TX_PB2,UART0_RX_PB3);
	uart_cal_div_and_bwpc(115200, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_set_dma_rx_timeout(UART0, bwpc, 12, UART_BW_MUL1);
	uart_init(UART0, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);
	core_interrupt_enable();
	uart_set_tx_dma_config(UART0, UART_DMA_CHANNEL_TX);
	uart_set_rx_dma_config(UART0, UART_DMA_CHANNEL_RX);
	uart_clr_tx_done(UART0);
	dma_clr_irq_mask(UART_DMA_CHANNEL_TX,TC_MASK|ABT_MASK|ERR_MASK);
	dma_clr_irq_mask(UART_DMA_CHANNEL_RX,TC_MASK|ABT_MASK|ERR_MASK);
	uart_set_irq_mask(UART0, UART_RXDONE_MASK);
	uart_set_irq_mask(UART0, UART_TXDONE_MASK);
	plic_interrupt_enable(IRQ19_UART0);
	uart_receive_dma(UART0, (unsigned char*)rec_buff,16);
	while(1);
}

#endif

/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void user_init_normal(void)
{
	/* random number generator must be initiated here( in the beginning of user_init_nromal).
	 * When deepSleep retention wakeUp, no need initialize again */
	random_generator_init();  //this is must

//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////
	/* for 1M   Flash, flash_sector_mac_address equals to 0xFF000
	 * for 2M   Flash, flash_sector_mac_address equals to 0x1FF000*/
	u8  mac_public[6];
	u8  mac_random_static[6];
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);

	//////////// Controller Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();                      //mandatory
	blc_ll_initStandby_module(mac_public);		//mandatory

	//////////// Controller Initialization  End /////////////////////////

	//////////// Phy Test Initialization  Begin /////////////////////////

	rf_set_power_level_index (MY_RF_POWER_INDEX);
//	write_reg8(0x402, 0x2b);   //set rf packet preamble for BQB
	blc_phy_initPhyTest_module();
	blc_phy_setPhyTestEnable( BLC_PHYTEST_ENABLE );

//	blc_phy_preamble_length_set(11);	to do

	blc_register_hci_handler (blc_phyTest_2wire_rxUartCb, blc_phyTest_2wire_txUartCb);
//	blc_register_hci_handler (blc_phyTest_hci_rxUartCb, blc_phyTest_2wire_txUartCb);
	phy_test_uart_init();

	//////////// Phy Test Initialization  End   /////////////////////////


//////////////////////////// BLE stack Initialization  End //////////////////////////////////


	///////////////////// Power Management initialization///////////////////
	bls_pm_setSuspendMask (SUSPEND_DISABLE);

}



/////////////////////////////////////////////////////////////////////s
// main loop flow
/////////////////////////////////////////////////////////////////////




/**
 * @brief		This is main_loop function
 * @param[in]	none
 * @return      none
 */
void main_loop (void)
{
	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();
}





#endif  //end of (FEATURE_TEST_MODE == ...)
