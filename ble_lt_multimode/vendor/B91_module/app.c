/********************************************************************************************************
 * @file	app.c
 *
 * @brief	for TLSR chips
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

#include "spp.h"
#include "vendor/common/blt_common.h"


#define SPP_RXFIFO_SIZE		72
#define SPP_RXFIFO_NUM		2

#define SPP_TXFIFO_SIZE		72
#define SPP_TXFIFO_NUM		8

_attribute_data_retention_  u8 		 	spp_rx_fifo_b[SPP_RXFIFO_SIZE * SPP_RXFIFO_NUM] = {0};
_attribute_data_retention_	my_fifo_t	spp_rx_fifo = {
												SPP_RXFIFO_SIZE,
												SPP_RXFIFO_NUM,
												0,
												0,
												spp_rx_fifo_b,};

_attribute_data_retention_  u8 		 	spp_tx_fifo_b[SPP_TXFIFO_SIZE * SPP_TXFIFO_NUM] = {0};
_attribute_data_retention_	my_fifo_t	spp_tx_fifo = {
												SPP_TXFIFO_SIZE,
												SPP_TXFIFO_NUM,
												0,
												0,
												spp_tx_fifo_b,};


#define LL_RX_FIFO_SIZE		64
#define LL_RX_FIFO_NUM		8

#define LL_TX_FIFO_SIZE		48
#define LL_TX_FIFO_NUM		17  //only 9 and 17  can be used

_attribute_data_retention_	u8	app_ll_rxfifo[LL_RX_FIFO_SIZE * LL_RX_FIFO_NUM] = {0};
_attribute_data_retention_  u8	app_ll_txfifo[LL_TX_FIFO_SIZE * LL_TX_FIFO_NUM] = {0};




#define     MY_APP_ADV_CHANNEL					BLT_ENABLE_ADV_ALL
#define 	MY_ADV_INTERVAL_MIN					ADV_INTERVAL_30MS
#define 	MY_ADV_INTERVAL_MAX					ADV_INTERVAL_40MS

#define 	MY_DIRECT_ADV_TMIE					2000000  //us

#define		MY_RF_POWER_INDEX					RF_POWER_INDEX_P2p79dBm

#define		BLE_DEVICE_ADDRESS_TYPE 			BLE_DEVICE_ADDRESS_PUBLIC

_attribute_data_retention_	own_addr_type_t 	app_own_address_type = OWN_ADDRESS_PUBLIC;




/**
 * @brief	Adv Packet data
 */
const u8	tbl_advData[] = {
	 0x05, 0x09, 'k', 'M', 'o', 'd',
	 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
};

/**
 * @brief	Scan Response Packet data
 */
const u8	tbl_scanRsp [] = {
		 0x07, 0x09, 'k', 'M', 'o', 'd', 'u', 'l',	//scan name " tmodul"
	};


_attribute_data_retention_	u8 	ui_ota_is_working = 0;

_attribute_data_retention_	u32	lowBattDet_tick   = 0;

_attribute_data_retention_ u8 conn_update_cnt;


/**
 * @brief		response for the conn_request
 * @param[in]
 * @param[in]	none
 * @return      none
 */
int app_conn_param_update_response(u8 id, u16  result)
{
#if 0
	if(result == CONN_PARAM_UPDATE_ACCEPT){
		/*SIG: the LE master Host has accepted the connection parameters.*/
		conn_update_cnt = 0;
	}
	else if(result == CONN_PARAM_UPDATE_REJECT)
	{
		/*SIG: the LE master Host has rejected the connection parameters.*/

		conn_update_cnt++;
		if(conn_update_cnt < 4){
			/*Slave sent update connPara req!*/
		}
		if(conn_update_cnt == 1){
			bls_l2cap_requestConnParamUpdate (8, 16, 0, 400);//18.75ms iOS
		}
		else if(conn_update_cnt == 2){
			bls_l2cap_requestConnParamUpdate (16,32, 0, 400);
		}
		else if(conn_update_cnt == 3){
			bls_l2cap_requestConnParamUpdate (32,60, 0, 400);
		}
		else{
			conn_update_cnt = 0;
			/*Slave Connection Parameters Update table all tested and failed!*/
		}
	}
#endif

	return 0;
}


/**
 * @brief		enter in ota mode
 * @param[in]	none
 * @return      none
 */
#if (BLE_OTA_ENABLE)
void entry_ota_mode(void)
{
	bls_ota_setTimeout(15 * 1000 * 1000); //set OTA timeout  15 seconds

	#if(BLT_APP_LED_ENABLE)
		gpio_set_output_en(GPIO_LED, 1);  //output enable
		gpio_write(GPIO_LED, 1);  //LED on for indicate OTA mode
	#endif
}


/**
 * @brief		show the ota result with led according to the input param
 * @param[in]	0: ota success ; else ota fail
 * @return      none
 */
void show_ota_result(int result)
{
	#if(1 && BLT_APP_LED_ENABLE)
		gpio_set_output_en(GPIO_LED, 1);

		if(result == OTA_SUCCESS){  //OTA success
			gpio_write(GPIO_LED, 1);
			sleep_us(500000);
			gpio_write(GPIO_LED, 0);
			sleep_us(500000);
			gpio_write(GPIO_LED, 1);
			sleep_us(500000);
			gpio_write(GPIO_LED, 0);
			sleep_us(500000);
		}
		else{  //OTA fail

			#if 0 //this is only for debug,  can not use this in application code
				irq_disable();
				WATCHDOG_DISABLE;

				while(1){
					gpio_write(GPIO_LED, 1);
					sleep_us(200000);
					gpio_write(GPIO_LED, 0);
					sleep_us(200000);
				}
			#endif

		}

		gpio_set_output_en(GPIO_LED, 0);
	#endif
}
#endif


#define MAX_INTERVAL_VAL		16

_attribute_data_retention_	u32 tick_wakeup;
_attribute_data_retention_	int	mcu_uart_working;
_attribute_data_retention_	int	module_uart_working;
_attribute_data_retention_	int module_task_busy;


_attribute_data_retention_	int	module_uart_data_flg;
_attribute_data_retention_	u32 module_wakeup_module_tick;

#define UART_TX_BUSY			((spp_tx_fifo.rptr != spp_tx_fifo.wptr) || uart_tx_is_busy(UART0) )
#define UART_RX_BUSY			(spp_rx_fifo.rptr != spp_rx_fifo.wptr)


/**
 * @brief		obtain uart working status
 * @param[in]	none
 * @return      0 for idle  else for busy
 */
int app_module_busy ()
{
	mcu_uart_working = gpio_read(GPIO_WAKEUP_MODULE);  //mcu use GPIO_WAKEUP_MODULE to indicate the UART data transmission or receiving state
	module_uart_working = UART_TX_BUSY || UART_RX_BUSY; //module checks to see if UART rx and tX are all processed
	module_task_busy = mcu_uart_working || module_uart_working;
	return module_task_busy;
}

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_ADV_DURATION_TIMEOUT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void 	app_switch_to_indirect_adv(u8 e, u8 *p, int n)
{

	bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
						ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
						0,  NULL,
						BLT_ENABLE_ADV_ALL,
						ADV_FP_NONE);

	bls_ll_setAdvEnable(1);  //must: set adv enable
}


/**
 * @brief		stop sleep and enter in working mode
 * @param[in]	none
 * @return      none
 */
void app_suspend_exit ()
{
	GPIO_WAKEUP_MODULE_HIGH;  //module enter working state
	bls_pm_setSuspendMask(SUSPEND_DISABLE);
	tick_wakeup = clock_time () | 1;
}


int app_suspend_enter ()
{
	if (app_module_busy ())
	{
		app_suspend_exit ();
		return 0;
	}
	return 1;
}

/**
 * @brief      power management code for application
 * @param	   none
 * @return     none
 */
void app_power_management ()
{
#if (BLE_MODULE_INDICATE_DATA_TO_MCU)
	module_uart_working = UART_TX_BUSY || UART_RX_BUSY;

	//When module's UART data is sent, the GPIO_WAKEUP_MCU is lowered or suspended (depending on how the user is designed)
	if(module_uart_data_flg && !module_uart_working){
		module_uart_data_flg = 0;
		module_wakeup_module_tick = 0;
		GPIO_WAKEUP_MCU_LOW;
	}
#endif
// pullup GPIO_WAKEUP_MODULE: exit from suspend
// pulldown GPIO_WAKEUP_MODULE: enter suspend
#if (BLE_MODULE_PM_ENABLE)

	if (!app_module_busy() && !tick_wakeup)
	{
		#if (PM_NO_SUSPEND_ENABLE)
			bls_pm_setSuspendMask ( DEEPSLEEP_RETENTION_ADV | DEEPSLEEP_RETENTION_CONN);
		#elif (PM_DEEPSLEEP_RETENTION_ENABLE)
			bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
		#else
			bls_pm_setSuspendMask(SUSPEND_ADV | SUSPEND_CONN);
		#endif

		bls_pm_setWakeupSource(PM_WAKEUP_PAD);  // GPIO_WAKEUP_MODULE needs to be wakened
	}

	if (tick_wakeup && clock_time_exceed (tick_wakeup, 500))
	{
		GPIO_WAKEUP_MODULE_LOW;
		tick_wakeup = 0;
	}
#endif
}

#define UATRT_TIMNEOUT_US					   100 //100uS for 115200

_attribute_data_retention_ volatile unsigned int   uart0_ndmairq_cnt=0;//Pre-4B for len
_attribute_data_retention_ volatile unsigned int   uart0_ndma_tick = 0;
_attribute_data_retention_ volatile unsigned char  uart0_flag = 0;
enum
{
	UART0_RECIEVE_IDLE=0,
	UART0_RECIEVE_START=1,
};
volatile unsigned int uart0_rx_buff_byte[16] __attribute__((aligned(4))) ={0x00};


/**
 * @brief      uart0 irq code for application
 * @param	   none
 * @return     none
 */
void uart0_recieve_irq(void)
{
	if(uart0_flag == UART0_RECIEVE_IDLE)
	{
		uart0_ndmairq_cnt = 4;//recieve packet start
	}

	u8* p = my_fifo_wptr(&spp_rx_fifo);
	p[uart0_ndmairq_cnt++] = uart_read_byte(UART0);

	if(uart0_flag == UART0_RECIEVE_IDLE)
	{
		uart0_flag = UART0_RECIEVE_START;
	}
	uart0_ndma_tick = clock_time();
}


/**
 * @brief      this function for count data numbers
 * @param	   none
 * @return     none
 */
void uart0_recieve_process(void)
{
	if(uart0_flag == UART0_RECIEVE_START)
	{
		if(clock_time_exceed(uart0_ndma_tick,UATRT_TIMNEOUT_US))//recieve timeout && 1 packet end
		{
			//add len
			uart0_ndmairq_cnt -= 4;
			u8* p = my_fifo_wptr(&spp_rx_fifo);
			tmemcpy(p,(u8 *)&uart0_ndmairq_cnt,4);
			my_fifo_next(&spp_rx_fifo);
			uart0_flag = UART0_RECIEVE_IDLE;
		}
	}
}


/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void user_init_normal(void)
{
	//random number generator must be initiated here( in the beginning of user_init_nromal)
	//when deepSleep retention wakeUp, no need initialize again
	random_generator_init();  //this is must


	/*****************************************************************************************
	 Note: battery check must do before any flash write/erase operation, cause flash write/erase
		   under a low or unstable power supply will lead to error flash operation

		   Some module initialization may involve flash write/erase, include: OTA initialization,
				SMP initialization, ..
				So these initialization must be done after  battery check
	*****************************************************************************************/
	#if (BATT_CHECK_ENABLE)  //battery check must do before OTA relative operation
		if(analog_read_reg8(USED_DEEP_ANA_REG) & LOW_BATT_FLG){
			app_battery_power_check(VBAT_ALRAM_THRES_MV + 200);  //2.2 V
		}
		else{
			app_battery_power_check(VBAT_ALRAM_THRES_MV);  //2.0 V
		}
	#endif

////////////////// BLE stack initialization ////////////////////////////////////
	u8  mac_public[6];
	u8  mac_random_static[6];
	//for 1M   Flash, flash_sector_mac_address equals to 0xFF000
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);

	#if(BLE_DEVICE_ADDRESS_TYPE == BLE_DEVICE_ADDRESS_PUBLIC)
		app_own_address_type = OWN_ADDRESS_PUBLIC;
	#elif(BLE_DEVICE_ADDRESS_TYPE == BLE_DEVICE_ADDRESS_RANDOM_STATIC)
		app_own_address_type = OWN_ADDRESS_RANDOM;
		blc_ll_setRandomAddr(mac_random_static);
	#endif
	////// Controller Initialization  //////////
	blc_ll_initBasicMCU();                      //mandatory
	blc_ll_initStandby_module(mac_public);				//mandatory
	blc_ll_initAdvertising_module(); 	//adv module: 		 mandatory for BLE slave,
	blc_ll_initConnection_module();				//connection module  mandatory for BLE slave/master
	blc_ll_initSlaveRole_module();				//slave module: 	 mandatory for BLE slave,

	blc_ll_initTxFifo(app_ll_txfifo, LL_TX_FIFO_SIZE, LL_TX_FIFO_NUM);
	blc_ll_initRxFifo(app_ll_rxfifo, LL_RX_FIFO_SIZE, LL_RX_FIFO_NUM);

	////// Host Initialization  //////////
	blc_gap_peripheral_init();    //gap initialization
	extern void my_att_init ();
	my_att_init (); //gatt initialization
	blc_l2cap_register_handler (blc_l2cap_packet_receive);  	//l2cap initialization

	blc_l2cap_registerConnUpdateRspCb(app_conn_param_update_response);
	////////////////// config adv packet /////////////////////
#if (BLE_SECURITY_ENABLE)
	u8 bond_number = blc_smp_param_getCurrentBondingDeviceNumber();  //get bonded device number
	smp_param_save_t  bondInfo;
	if(bond_number)   //at least 1 bonding device exist
	{
		bls_smp_param_loadByIndex( bond_number - 1, &bondInfo);  //get the latest bonding device (index: bond_number-1 )

	}

	if(bond_number)   //set direct adv
	{
		//set direct adv
		u8 status = bls_ll_setAdvParam( MY_ADV_INTERVAL_MIN, MY_ADV_INTERVAL_MAX,
										ADV_TYPE_CONNECTABLE_DIRECTED_LOW_DUTY, app_own_address_type,
										bondInfo.peer_addr_type,  bondInfo.peer_addr,
										MY_APP_ADV_CHANNEL,
										ADV_FP_NONE);
		if(status != BLE_SUCCESS) { while(1); }  //debug: adv setting err

		//it is recommended that direct adv only last for several seconds, then switch to indirect adv
		bls_ll_setAdvDuration(MY_DIRECT_ADV_TMIE, 1);
		bls_app_registerEventCallback (BLT_EV_FLAG_ADV_DURATION_TIMEOUT, &app_switch_to_indirect_adv);

	}
	else   //set indirect adv
#endif
	{
		u8 status = bls_ll_setAdvParam(  ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
										 ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
										 0,  NULL,
										 BLT_ENABLE_ADV_ALL,
										 ADV_FP_NONE);
		if(status != BLE_SUCCESS) {  }  //debug: adv setting err
	}


	///////////////////// USER application initialization ///////////////////
	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));

	bls_ll_setAdvEnable(1);  //adv enable

	rf_set_power_level_index (MY_RF_POWER_INDEX);

	//uart config
	uart_reset(UART0);
	uart_set_pin(UART0_TX_PB2, UART0_RX_PB3 );// uart tx/rx pin set
	unsigned short div;
	unsigned char bwpc;
	uart_cal_div_and_bwpc(115200, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_init(UART0, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	//uart irq set
	plic_interrupt_enable(IRQ19_UART0);
	plic_set_priority(IRQ19_UART0,1);

	uart_tx_irq_trig_level(UART0, 0);
	uart_rx_irq_trig_level(UART0, 1);

	uart_set_irq_mask(UART0, UART_RX_IRQ_MASK);

	extern int rx_from_uart_cb (void);
	extern int tx_to_uart_cb (void);
	blc_register_hci_handler(rx_from_uart_cb, tx_to_uart_cb);				//customized uart handler

	extern int controller_event_handler(u32 h, u8 *para, int n);
	blc_hci_registerControllerEventHandler(controller_event_handler);		//register event callback
	bls_hci_mod_setEventMask_cmd(0xfffff);			//enable all 18 events,event list see ll.h

#if (BLE_MODULE_PM_ENABLE)
	blc_ll_initPowerManagement_module();        //pm module:      	 optional

	#if PM_NO_SUSPEND_ENABLE
		bls_pm_setSuspendMask ( DEEPSLEEP_RETENTION_ADV | DEEPSLEEP_RETENTION_CONN);
		blc_pm_setDeepsleepRetentionThreshold(3, 3);
		blc_pm_setDeepsleepRetentionEarlyWakeupTiming(TEST_CONN_CURRENT_ENABLE ? 420 : 440);
	#elif (PM_DEEPSLEEP_RETENTION_ENABLE)
		bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
		blc_pm_setDeepsleepRetentionThreshold(95, 95);
		blc_pm_setDeepsleepRetentionEarlyWakeupTiming(250);
	#else
		bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	#endif

	//mcu can wake up module from suspend or deepsleep by pulling up GPIO_WAKEUP_MODULE
	cpu_set_gpio_wakeup (GPIO_WAKEUP_MODULE, Level_High, 1);  // pad high wakeup deepsleep

	GPIO_WAKEUP_MODULE_LOW;

	bls_pm_registerFuncBeforeSuspend( &app_suspend_enter );
#else
	bls_pm_setSuspendMask (SUSPEND_DISABLE);
#endif

#if (BLE_OTA_ENABLE)
	// OTA init
	bls_ota_clearNewFwDataArea(); //must
	bls_ota_set_VersionFlashAddr_and_VersionNumber(OTA_VERSION_FLASH_ADDR, OTA_VERSION_NUMBER); //must
	bls_ota_registerStartCmdCb(entry_ota_mode);
	bls_ota_registerResultIndicateCb(show_ota_result);
#endif
}


/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
#if (PM_DEEPSLEEP_RETENTION_ENABLE)
_attribute_ram_code_ void user_init_deepRetn(void)
{
	blc_ll_initBasicMCU();   //mandatory
	rf_set_power_level_index (MY_RF_POWER_INDEX);
	blc_ll_recoverDeepRetention();

	DBG_CHN0_HIGH;    //debug
	irq_enable();
	//uart config
	uart_reset(UART0);
	uart_set_pin(UART0_TX_PB2, UART0_RX_PB3 );// uart tx/rx pin set
	unsigned short div;
	unsigned char bwpc;
	uart_cal_div_and_bwpc(115200, sys_clk.pclk*1000*1000, &div, &bwpc);
	uart_init(UART0, div, bwpc, UART_PARITY_NONE, UART_STOP_BIT_ONE);

	//uart irq set
	plic_interrupt_enable(IRQ19_UART0);
	plic_set_priority(IRQ19_UART0,1);

	uart_tx_irq_trig_level(UART0, 0);
	uart_rx_irq_trig_level(UART0, 1);

	uart_set_irq_mask(UART0, UART_RX_IRQ_MASK);

	//mcu can wake up module from suspend or deepsleep by pulling up GPIO_WAKEUP_MODULE
	cpu_set_gpio_wakeup (GPIO_WAKEUP_MODULE, Level_High, 1);  // pad high wakeup deepsleep

	GPIO_WAKEUP_MODULE_LOW;

	DBG_CHN0_LOW;   //debug
}
#endif


/**
 * @brief		This is main_loop function
 * @param[in]	none
 * @return      none
 */
void main_loop (void)
{
	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();

	////////////////////////////////////// UI entry /////////////////////////////////
#if (BATT_CHECK_ENABLE)
	if(battery_get_detect_enable() && clock_time_exceed(lowBattDet_tick, 500000) ){
		lowBattDet_tick = clock_time();
		if(analog_read_reg8(USED_DEEP_ANA_REG) & LOW_BATT_FLG){
			app_battery_power_check(VBAT_ALRAM_THRES_MV + 200);  //2.2 V
		}
		else{
			app_battery_power_check(VBAT_ALRAM_THRES_MV);  //2.0 V
		}
	}
#endif

	//  add spp UI task
	app_power_management ();

	uart0_recieve_process();

	spp_restart_proc();
}




