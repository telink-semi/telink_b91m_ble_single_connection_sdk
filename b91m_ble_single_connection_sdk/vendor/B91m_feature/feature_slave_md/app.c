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
#include "app_att.h"

#if (FEATURE_TEST_MODE == TEST_SLAVE_MD)


#define		MY_RF_POWER_INDEX					RF_POWER_INDEX_P3dBm

#define TEST_DATA_LEN		20

int write_data_test_tick = 0;

u8	app_test_data[TEST_DATA_LEN]={0x00,0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,
															  0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x11,0x11,0x11};

/**
 * @brief	Adv Packet data
 */
const u8	tbl_advData[] = {
	 0x08, DT_COMPLETE_LOCAL_NAME, 'f', 'e', 'a', 't', 'u', 'r', 'e',
	 0x02, DT_FLAGS, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
	 0x03, DT_APPEARANCE, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
	 0x05, DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
};

/**
 * @brief	Scan Response Packet data
 */
const u8	tbl_scanRsp [] = {
		 0x08, DT_COMPLETE_LOCAL_NAME, 'f', 'e', 'a', 't', 'u', 'r', 'e',
	};




/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_ENTER"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void  task_suspend_enter (u8 e, u8 *p, int n)
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

	write_data_test_tick = clock_time();

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
 * @brief      power management code for application
 * @param	   none
 * @return     none
 */
_attribute_ram_code_
void blt_pm_proc(void)
{

#if(BLE_APP_PM_ENABLE)

	#if (TLKAPI_DEBUG_ENABLE)
		if(tlkapi_debug_isBusy()){
			bls_pm_setSuspendMask(SUSPEND_DISABLE);
		}
	#endif


	#if (PM_DEEPSLEEP_RETENTION_ENABLE)
		bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
	#else
		bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	#endif


	//do not care about keyScan/button_detect power here, if you care about this, please refer to "8258_ble_remote" demo
	#if (UI_KEYBOARD_ENABLE)
		if(scan_pin_need || key_not_released)
		{
			bls_pm_setSuspendMask (SUSPEND_DISABLE);
		}
	#endif
#endif  //end of BLE_APP_PM_ENABLE
}




#if (UI_KEYBOARD_ENABLE)



_attribute_data_retention_	int 	key_not_released;

#define CONSUMER_KEY   	   		1
#define KEYBOARD_KEY   	   		2
_attribute_data_retention_	u8 		key_type;





/**
 * @brief		this function is used to process keyboard matrix status change.
 * @param[in]	none
 * @return      none
 */
void key_change_proc(void)
{
	u8 key0 = kb_event.keycode[0];
	u8 key_buf[8] = {0,0,0,0,0,0,0,0};

	key_not_released = 1;
	if (kb_event.cnt == 2)   //two key press, do  not process
	{

	}
	else if(kb_event.cnt == 1)
	{
		if(key0 >= CR_VOL_UP )  //volume up/down
		{
			key_type = CONSUMER_KEY;
			u16 consumer_key;
			if(key0 == CR_VOL_UP){  	//volume up
				consumer_key = MKEY_VOL_UP;
				gpio_write(GPIO_LED_WHITE,1);
			}
			else if(key0 == CR_VOL_DN){ //volume down
				consumer_key = MKEY_VOL_DN;
				gpio_write(GPIO_LED_GREEN,1);
			}

			blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
		}
		else
		{
			key_type = KEYBOARD_KEY;
			key_buf[2] = key0;
			if(key0 == VK_1)
			{
				gpio_write(GPIO_LED_BLUE,1);
			}
			else if(key0 == VK_2)
			{
				gpio_write(GPIO_LED_BLUE,1);
			}

			blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_NORMAL_KB_REPORT_INPUT_DP_H, key_buf, 8);
		}

	}
	else   //kb_event.cnt == 0,  key release
	{
		gpio_write(GPIO_LED_WHITE,0);
		gpio_write(GPIO_LED_GREEN,0);
		key_not_released = 0;
		if(key_type == CONSUMER_KEY)
		{
			u16 consumer_key = 0;

			blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
		}
		else if(key_type == KEYBOARD_KEY)
		{
			gpio_write(GPIO_LED_BLUE,0);
			gpio_write(GPIO_LED_BLUE,0);
			key_buf[2] = 0;

			blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_NORMAL_KB_REPORT_INPUT_DP_H, key_buf, 8); //release
		}
	}


}



_attribute_data_retention_		static u32 keyScanTick = 0;



/**
 * @brief      this function is used to detect if key pressed or released.
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_
void proc_keyboard (u8 e, u8 *p, int n)
{
	if(clock_time_exceed(keyScanTick, 8000)){
		keyScanTick = clock_time();
	}
	else{
		return;
	}

	kb_event.keycode[0] = 0;
	int det_key = kb_scan_key (0, 1);

	if (det_key){
		key_change_proc();
	}
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

//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////
	/* for 1M   Flash, flash_sector_mac_address equals to 0xFF000
	 * for 2M   Flash, flash_sector_mac_address equals to 0x1FF000*/
	u8  mac_public[6];
	u8  mac_random_static[6];
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);


	//////////// Controller Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();                      //mandatory
	blc_ll_initStandby_module(mac_public);		//mandatory
	blc_ll_initLegacyAdvertising_module(); 		//legacy advertising module: mandatory for BLE slave
	blc_ll_initConnection_module();				//connection module  mandatory for BLE slave/master
	blc_ll_initSlaveRole_module();				//slave module: 	 mandatory for BLE slave,

	blc_ll_setAclConnMaxOctetsNumber(ACL_CONN_MAX_RX_OCTETS, ACL_CONN_MAX_TX_OCTETS);

	blc_ll_initAclConnTxFifo(app_acl_txfifo, ACL_TX_FIFO_SIZE, ACL_TX_FIFO_NUM);
	blc_ll_initAclConnRxFifo(app_acl_rxfifo, ACL_RX_FIFO_SIZE, ACL_RX_FIFO_NUM);

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


	//////////// Host Initialization  Begin /////////////////////////
	/* Host Initialization */
	/* GAP initialization must be done before any other host feature initialization !!! */
	blc_gap_peripheral_init();    //gap initialization
	extern void my_att_init ();
	my_att_init (); //gatt initialization

	/* L2CAP Initialization */
	blc_l2cap_register_handler (blc_l2cap_packet_receive);

	#if (APP_SECURITY_ENABLE)
		/* SMP Initialization may involve flash write/erase(when one sector stores too much information,
		 *   is about to exceed the sector threshold, this sector must be erased, and all useful information
		 *   should re_stored) , so it must be done after battery check */
		blc_smp_peripheral_init();

		// Hid device on android7.0/7.1 or later version
		// New paring: send security_request immediately after connection complete
		// reConnect:  send security_request 1000mS after connection complete. If master start paring or encryption before 1000mS timeout, slave do not send security_request.
		//blc_smp_configSecurityRequestSending(SecReq_IMM_SEND, SecReq_PEND_SEND, 1000); //if not set, default is:  send "security request" immediately after link layer connection established(regardless of new connection or reconnection)
		blc_smp_configSecurityRequestSending( SecReq_NOT_SEND,  SecReq_NOT_SEND, 0);
	#else
		blc_smp_setSecurityLevel(No_Security);
	#endif
	//////////// Host Initialization  End /////////////////////////

//////////////////////////// BLE stack Initialization  End //////////////////////////////////


	u8 status = bls_ll_setAdvParam(  ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
									 ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC,
									 0,  NULL,
									 BLT_ENABLE_ADV_ALL,
									 ADV_FP_NONE);
	if(status != BLE_SUCCESS) { while(1); }  //debug: adv setting err



	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));



	bls_ll_setAdvEnable(BLC_ADV_ENABLE);  //adv enable



	//set rf power index, user must set it after every suspend wakeup, cause relative setting will be reset in suspend
	rf_set_power_level_index (MY_RF_POWER_INDEX);


	bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &task_connect);
	bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &task_terminate);
	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_EXIT, &task_suspend_exit);
	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_ENTER, &task_suspend_enter);

	///////////////////// Power Management initialization///////////////////
#if(BLE_APP_PM_ENABLE)
	blc_ll_initPowerManagement_module();

	#if (PM_DEEPSLEEP_RETENTION_ENABLE)
		/* pay attention: if your RamCode and retention data bigger than 32K, you must choose retention SRAM 64K */
		blc_pm_setDeepsleepRetentionType(DEEPSLEEP_MODE_RET_SRAM_LOW64K);
		bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
		blc_pm_setDeepsleepRetentionThreshold(95, 95);
		blc_pm_setDeepsleepRetentionEarlyWakeupTiming(400);

	#else
		bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	#endif
#else
	#if (PM_DEEPSLEEP_RETENTION_ENABLE)
		#error "can not use deep retention when PM disable !!!"
	#endif

	bls_pm_setSuspendMask (SUSPEND_DISABLE);
#endif



#if (UI_KEYBOARD_ENABLE)
	/////////// keyboard gpio wakeup init ////////
	u32 pin[] = KB_DRIVE_PINS;
	for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
	{
		cpu_set_gpio_wakeup (pin[i], Level_High,1);  //drive pin pad high wakeup deepsleep
	}

	bls_app_registerEventCallback (BLT_EV_FLAG_GPIO_EARLY_WAKEUP, &proc_keyboard);
#endif

}



/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void user_init_deepRetn(void)
{
#if (BLE_APP_PM_ENABLE && PM_DEEPSLEEP_RETENTION_ENABLE)

	blc_ll_initBasicMCU();   //mandatory
	rf_set_power_level_index (MY_RF_POWER_INDEX);
	blc_ll_recoverDeepRetention();

	DBG_CHN0_HIGH;    //debug
	irq_enable();
	#if (UI_KEYBOARD_ENABLE)
		/////////// keyboard gpio wakeup init ////////
		u32 pin[] = KB_DRIVE_PINS;
		for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
		{
			cpu_set_gpio_wakeup (pin[i], Level_High,1);  //drive pin pad high wakeup deepsleep
		}
	#endif
	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_deepRetn_init();
	#endif

#endif
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
	proc_keyboard (0, 0, 0);

	if(write_data_test_tick && clock_time_exceed(write_data_test_tick, 500)){ // >1s
		write_data_test_tick = clock_time() | 1;

		if(blc_ll_getCurrentState() == BLS_LINK_STATE_CONN)
		{
			if( BLE_SUCCESS == blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE,SPP_SERVER_TO_CLIENT_DP_H, app_test_data, 20)){
				app_test_data[0] ++;
			}
		}
	}

	////////////////////////////////////// PM Process /////////////////////////////////
	blt_pm_proc();



}


u8 seq_num_next = 0;

int myC2SWrite(u16 connHandle, void * p)
{
	rf_packet_att_data_t *req = (rf_packet_att_data_t*)p;

	u8 seq_num = req->dat[0];

	if(seq_num == seq_num_next){

	}
	else{
		write_reg8(0x40000, 0x77);
		irq_disable();

		#if (UI_LED_ENABLE)
			gpio_write(GPIO_LED_WHITE, LED_ON_LEVEL);
		#endif

		while(1);
		write_reg8(0x40000, 0x22);
	}

	seq_num_next = seq_num + 1;

	return 0;
}



#endif  //end of (FEATURE_TEST_MODE == ...)
