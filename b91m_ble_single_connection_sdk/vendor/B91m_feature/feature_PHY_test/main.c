/********************************************************************************************************
 * @file     main.c
 *
 * @brief    This is the source file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "app.h"

#if (FEATURE_TEST_MODE == TEST_BLE_PHY)

/**
 * @brief		BLE SDK RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void rf_irq_handler(void)
{
	DBG_CHN14_HIGH;

	irq_blt_sdk_handler ();

	DBG_CHN14_LOW;
}



/**
 * @brief		BLE SDK System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void stimer_irq_handler(void)
{
	DBG_CHN15_HIGH;

	irq_blt_sdk_handler ();

	DBG_CHN15_LOW;
}

extern hci_fifo_t bltHci_rxfifo;
#if (MCU_CORE_TYPE == MCU_CORE_B91)
	/**
	 * @brief      UART irq function
	 * @param[in]  none
	 * @return     none
	 */
	_attribute_ram_code_
	void uart_irq_handler(void)
	{
		u32 rev_data_len = 0;
			if(uart_get_irq_status(UART_CHN,UART_TXDONE))
			{
			    uart_clr_tx_done(UART_CHN);
			}
		    if(uart_get_irq_status(UART_CHN,UART_RXDONE)) //A0-SOC can't use RX-DONE status,so this interrupt can noly used in A1-SOC.
		    {
		    	/************************get the length of receive data****************************/
				rev_data_len = uart_get_dma_rev_data_len(UART_CHN,DMA2);
		    	/************************clr rx_irq****************************/
		    	uart_clr_irq_status(UART_CHN,UART_CLR_RX);
				if(rev_data_len!=0)
				{
					u8* p = bltHci_rxfifo.p + (bltHci_rxfifo.wptr & (bltHci_rxfifo.num-1)) * bltHci_rxfifo.size;
					p[0] = rev_data_len;
					bltHci_rxfifo.wptr++;
					p = bltHci_rxfifo.p + (bltHci_rxfifo.wptr & (bltHci_rxfifo.num-1)) * bltHci_rxfifo.size;
					uart_receive_dma_set(UART_CHN,DMA2, (unsigned char*)(p+4),UART_RX_BUFFER_SIZE-4);
				}

		    	if((uart_get_irq_status(UART_CHN,UART_RX_ERR)))
		    	{
		    		uart_clr_irq_status(UART_CHN,UART_RX_ERR);
		    	}
		    }
	}
	#if(!UART_CHN)
	_attribute_ram_code_
	void uart0_irq_handler(void)
	{
		uart_irq_handler();
	}
	#else
	_attribute_ram_code_
	void uart1_irq_handler(void)
	{
		uart_irq_handler();
	}
	#endif
#elif (MCU_CORE_TYPE == MCU_CORE_B92)
	/**
	 * @brief      UART irq function
	 * @param[in]  none
	 * @return     none
	 */
	_attribute_ram_code_
	void uart_irq_handler(void)
	{
		u32 rev_data_len = 0;
		if(uart_get_irq_status(UART_NUM,UART_TXDONE_IRQ_STATUS))
		{
			uart_clr_irq_status(UART_NUM,UART_TXDONE_IRQ_STATUS);
		}
	    if(uart_get_irq_status(UART_NUM,UART_RXDONE_IRQ_STATUS)) //A0-SOC can't use RX-DONE status,so this interrupt can noly used in A1-SOC.
	    {
	    	/************************get the length of receive data****************************/
			rev_data_len = uart_get_dma_rev_data_len(UART_NUM,DMA2);
	    	/************************clr rx_irq****************************/
	    	uart_clr_irq_status(UART_NUM,UART_RXDONE_IRQ_STATUS);
	    	u8* p = bltHci_rxfifo.p + (bltHci_rxfifo.wptr & bltHci_rxfifo.mask) * bltHci_rxfifo.size;

			if(bltHci_rxfifo.wptr - bltHci_rxfifo.rptr >= bltHci_rxfifo.num){
				uart_receive_dma(UART_NUM, (p+4), bltHci_rxfifo.size - 4);//[!!important]
				return; //have no memory.
			}

			if(rev_data_len)
			{
				p[0]=rev_data_len;

				bltHci_rxfifo.wptr++;
				p = bltHci_rxfifo.p + (bltHci_rxfifo.wptr & bltHci_rxfifo.mask) * bltHci_rxfifo.size;
			}
			uart_receive_dma(UART_NUM, (p+4), bltHci_rxfifo.size - 4);//[!!important]
		}

	    if((uart_get_irq_status(UART_NUM,UART_RX_ERR)))
	    {
	    	uart_clr_irq_status(UART_NUM,UART_RXBUF_IRQ_STATUS);
	    }
	}
#endif
/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main (void)   //must on ramcode
{
	blc_pm_select_internal_32k_crystal();
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		sys_init(DCDC_1P4_LDO_1P8,VBAT_MAX_VALUE_GREATER_THAN_3V6);
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		sys_init(LDO_1P2_LDO_2P0,VBAT_MAX_VALUE_GREATER_THAN_3V6);
	#endif

	CCLK_24M_HCLK_24M_PCLK_24M;

	rf_drv_ble_init();

//	gpio_init(!deepRetWakeUp);
	#if (MCU_CORE_TYPE == MCU_CORE_B92)
		wd_32k_stop();
	#endif
	
	blc_readFlashSize_autoConfigCustomFlashSector();

	blc_app_loadCustomizedParameters();  //load customized freq_offset cap value

	user_init_normal();

	while (1) {
		main_loop ();
	}
	return 0;
}


#endif  //end of (FEATURE_TEST_MODE == ...)
