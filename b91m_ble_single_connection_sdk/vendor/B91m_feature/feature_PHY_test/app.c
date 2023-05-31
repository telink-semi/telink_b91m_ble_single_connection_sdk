/********************************************************************************************************
 * @file     app.c
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

#include "app_config.h"
#include "app.h"
#include "app_buffer.h"
#include "../default_att.h"

#if (FEATURE_TEST_MODE == TEST_BLE_PHY)


#define		MY_RF_POWER_INDEX					RF_POWER_INDEX_P3dBm

/**
 * @brief	Adv Packet data
 */
const u8	tbl_advData[] = {
	 0x05, 0x09, 'f', 't', 'r', 'e',
	 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
	 0x03, 0x19, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
	 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
};

/**
 * @brief	Scan Response Packet data
 */
const u8	tbl_scanRsp [] = {
		 0x08, 0x09, 'f', 'e', 'a', 't', 'u', 'r', 'e',
	};

volatile u8	phyTest_rxfifo[UART_RX_BUFFER_SIZE * UART_RX_BUFFER_NUM] __attribute__((aligned(4))) = {0};
volatile u8	phyTest_txfifo[UART_TX_BUFFER_SIZE * UART_TX_BUFFER_NUM] __attribute__((aligned(4))) = {0};

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_ENTER"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void  ble_remote_set_sleep_wakeup (u8 e, u8 *p, int n)
{
	if( blc_ll_getCurrentState() == BLS_LINK_STATE_CONN && ((u32)(bls_pm_getSystemWakeupTick() - clock_time())) > 80 * SYSTEM_TIMER_TICK_1MS){  //suspend time > 30ms.add gpio wakeup
		bls_pm_setWakeupSource(PM_WAKEUP_PAD);  //gpio pad wakeup suspend/deepsleep
	}
}







/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_CONNECT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void	task_connect (u8 e, u8 *p, int n)
{
	bls_l2cap_requestConnParamUpdate (8, 8, 99, 400);  // 1 S

#if (UI_LED_ENABLE)
	gpio_write(GPIO_LED_RED, LED_ON_LEVEL);  //red light on
#endif
}



/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_TERMINATE"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void 	task_terminate(u8 e,u8 *p, int n) //*p is terminate reason
{

	if(*p == HCI_ERR_CONN_TIMEOUT){

	}
	else if(*p == HCI_ERR_REMOTE_USER_TERM_CONN){  //0x13

	}
	else if(*p == HCI_ERR_CONN_TERM_MIC_FAILURE){

	}
	else{

	}


#if (UI_LED_ENABLE)
	gpio_write(GPIO_LED_RED, !LED_ON_LEVEL);  //red light off
#endif

}



/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_EXIT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void	task_suspend_exit (u8 e, u8 *p, int n)
{
	rf_set_power_level_index (MY_RF_POWER_INDEX);
}



/**
 * @brief      function to initialization parameter for phy test.
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
#if (MCU_CORE_TYPE == MCU_CORE_B91)
	void phy_test_uart_init(uart_tx_pin_e tx_pin, uart_rx_pin_e rx_pin, unsigned int baudrate )
	{
		unsigned short div;
		unsigned char bwpc;
		g_chip_version = 0x01;
		blc_phyTest_Uart_num_Set (UART_CHN);
		uart_reset(UART_CHN);
		uart_set_pin(tx_pin,rx_pin);
		uart_cal_div_and_bwpc(baudrate, sys_clk.pclk*1000*1000, &div, &bwpc);
		uart_set_rx_timeout(UART_CHN, bwpc, 12, UART_BW_MUL1);
		uart_init(UART_CHN, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

		core_interrupt_enable();
		uart_set_tx_dma_config(UART_CHN, DMA3);
		uart_set_rx_dma_config(UART_CHN, DMA2);
		uart_clr_tx_done(UART_CHN);
		dma_clr_irq_mask(DMA2,TC_MASK|ABT_MASK|ERR_MASK);
		dma_clr_irq_mask(DMA3,TC_MASK|ABT_MASK|ERR_MASK);
		uart_set_irq_mask(UART_CHN, UART_RXDONE_MASK);
		uart_set_irq_mask(UART_CHN, UART_TXDONE_MASK);
	#if(UART_CHN==UART0)
			plic_interrupt_enable(IRQ19_UART0);
	#else
			plic_interrupt_enable(IRQ18_UART1);
	#endif
		uart_receive_dma(UART_CHN, (unsigned char*)(phyTest_rxfifo+4),UART_RX_BUFFER_SIZE-4);

	}
#elif (MCU_CORE_TYPE == MCU_CORE_B92)
void phy_test_uart_init(uart_num_e uart_num,gpio_func_pin_e tx_pin, gpio_func_pin_e rx_pin, unsigned int baudrate )
{
	unsigned short div;
	unsigned char bwpc;

	uart_reset(uart_num);
	uart_set_pin(uart_num,tx_pin,rx_pin);
	uart_cal_div_and_bwpc(baudrate, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_set_rx_timeout(uart_num, bwpc, 12, UART_BW_MUL1);
	uart_init(uart_num, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	core_interrupt_enable();
	uart_set_tx_dma_config(uart_num, DMA3);
	uart_set_rx_dma_config(uart_num, DMA2);
	dma_clr_irq_mask(DMA2,TC_MASK|ABT_MASK|ERR_MASK);
	dma_clr_irq_mask(DMA3,TC_MASK|ABT_MASK|ERR_MASK);
	uart_set_irq_mask(uart_num, UART_RXDONE_MASK);
	uart_set_irq_mask(uart_num, UART_TXDONE_MASK);
	plic_interrupt_enable(uart_num?IRQ18_UART1:IRQ19_UART0);
	uart_receive_dma(uart_num, (unsigned char*)(phyTest_rxfifo+4),UART_RX_BUFFER_SIZE-4);

}


#endif


/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
_attribute_no_inline_ void user_init_normal(void)
{
	/* random number generator must be initiated here( in the beginning of user_init_normal).
	 * When deepSleep retention wakeUp, no need initialize again */
	random_generator_init();  //this is must

	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_init();
		blc_debug_enableStackLog(STK_LOG_ALL);
	#endif

	#if APP_FAST_SETTLE
		blc_ll_initFastSettle(1,1);
	#endif


//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////
	//////////// Controller Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();                      //mandatory

	u8 error_code = blc_contr_checkControllerInitialization();
	if(error_code != INIT_SUCCESS){
		/* It's recommended that user set some UI alarm to know the exact error, e.g. LED shine, print log */
		write_log32(0x88880000 | error_code);
		#if (TLKAPI_DEBUG_ENABLE)
			tlkapi_send_string_data(APP_LOG_EN, "[APP][INI] Controller INIT ERROR", &error_code, 1);
			while(1){
				tlkapi_debug_handler();
			}
		#else
			while(1);
		#endif
	}
	//////////// Controller Initialization  End /////////////////////////


	//////////// PhyTest Initialization  Begin /////////////////////////

	blc_phy_initPhyTest_module();
	blc_phy_setPhyTestEnable( BLC_PHYTEST_ENABLE );
	blc_ll_initHciRxFifo((u8*)(phyTest_rxfifo),UART_RX_BUFFER_SIZE,UART_RX_BUFFER_NUM);
	blc_ll_initHciTxFifo((u8*)(phyTest_txfifo),UART_TX_BUFFER_SIZE,UART_TX_BUFFER_NUM);
#if( BLE_PHYTEST_MODE == PHYTEST_MODE_THROUGH_2_WIRE_UART )
	blc_register_hci_handler (blc_phyTest_2wire_rxUartCb, blc_phyTest_2wire_txUartCb);
#elif( BLE_PHYTEST_MODE == PHYTEST_MODE_OVER_HCI_WITH_UART )
	blc_register_hci_handler (blc_phyTest_hci_rxUartCb, blc_phyTest_2wire_txUartCb);
#endif
#if (MCU_CORE_TYPE == MCU_CORE_B91)
	phy_test_uart_init(UART_TX_PIN, UART_RX_PIN, BAUDRATE);
#elif (MCU_CORE_TYPE == MCU_CORE_B92)
	blc_phyTest_Uart_num_Set(UART_NUM);
	phy_test_uart_init(UART_NUM,UART_TX_PIN, UART_RX_PIN, BAUDRATE);
#endif

	//set rf power index, user must set it after every suspend wakeup, cause relative setting will be reset in suspend
	rf_set_power_level_index (MY_RF_POWER_INDEX);
	//////////// PhyTest Initialization  End ///////////////////////////


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
_attribute_no_inline_ void main_loop (void)
{
	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();

	////////////////////////////////////// Debug entry /////////////////////////////////
	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_handler();
	#endif

	////////////////////////////////////// UI entry /////////////////////////////////

	////////////////////////////////////// PM Process /////////////////////////////////

}





#endif  //end of (FEATURE_TEST_MODE == ...)
