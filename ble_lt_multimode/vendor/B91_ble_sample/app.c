/********************************************************************************************************
 * @file     app.c
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

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"



#include "vendor/common/blt_led.h"
#include "vendor/common/blt_common.h"
#include "app_config.h"
#include "app_att.h"
#include "app_buffer.h"

#define 	ADV_IDLE_ENTER_DEEP_TIME			60  //60 s
#define 	CONN_IDLE_ENTER_DEEP_TIME			60  //60 s

#define 	MY_DIRECT_ADV_TMIE					2000000



#define		MY_RF_POWER_INDEX					RF_POWER_P4p35dBm


#define		BLE_DEVICE_ADDRESS_TYPE 			BLE_DEVICE_ADDRESS_PUBLIC

_attribute_data_retention_	own_addr_type_t 	app_own_address_type = OWN_ADDRESS_PUBLIC;






//////////////////////////////////////////////////////////////////////////////
//	 Adv Packet, Response Packet
//////////////////////////////////////////////////////////////////////////////

#if(0)


	const u8	tbl_advData[] = {
		 17,   0x09, 'E','a','g','l','e',' ', 's','a','m', 'p','l','e', 'T','e','s', 't',
		 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
		 0x03, 0x19, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
		 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
	};

	const u8	tbl_scanRsp [] = {
		 17,   0x09, 'E','a','g','l','e',' ', 's','a','m', 'p','l','e', 'T','e','s', 't',
		 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
		 0x03, 0x19, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
		 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
	};

#else

const u8	tbl_advData[] = {
	 0x05, 0x09, 'e', 'H', 'I', 'D',
	 0x02, 0x01, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
	 0x03, 0x19, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
	 0x05, 0x02, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
};

const u8	tbl_scanRsp [] = {
		 0x08, 0x09, 'e', 'S', 'a', 'm', 'p', 'l', 'e',
	};

#endif


_attribute_data_retention_	int device_in_connection_state;

_attribute_data_retention_	u32 advertise_begin_tick;

_attribute_data_retention_	u32	interval_update_tick;

_attribute_data_retention_	u8	sendTerminate_before_enterDeep = 0;

_attribute_data_retention_	u32	latest_user_event_tick;


#if (UI_KEYBOARD_ENABLE)


	void proc_keyboard (u8 e, u8 *p, int n);

#elif (UI_BUTTON_ENABLE)

	extern int button_not_released;
	extern void proc_button (u8 e, u8 *p, int n);

#endif



_attribute_ram_code_ void  ble_remote_set_sleep_wakeup (u8 e, u8 *p, int n)
{
	if( blc_ll_getCurrentState() == BLS_LINK_STATE_CONN && ((u32)(bls_pm_getSystemWakeupTick() - clock_time())) > 80 * CLOCK_16M_SYS_TIMER_CLK_1MS){  //suspend time > 30ms.add gpio wakeup
		bls_pm_setWakeupSource(PM_WAKEUP_PAD);  //gpio pad wakeup suspend/deepsleep
	}
}











void 	app_switch_to_indirect_adv(u8 e, u8 *p, int n)
{

	bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
						ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
						0,  NULL,
						BLT_ENABLE_ADV_ALL,
						ADV_FP_NONE);

	bls_ll_setAdvEnable(1);  //must: set adv enable
}





void	task_connect (u8 e, u8 *p, int n)
{
//	bls_l2cap_requestConnParamUpdate (8, 8, 19, 200);  // 200mS
	bls_l2cap_requestConnParamUpdate (8, 8, 99, 400);  // 1 S
//	bls_l2cap_requestConnParamUpdate (8, 8, 149, 600);  // 1.5 S
//	bls_l2cap_requestConnParamUpdate (8, 8, 199, 800);  // 2 S
//	bls_l2cap_requestConnParamUpdate (8, 8, 249, 800);  // 2.5 S
//	bls_l2cap_requestConnParamUpdate (8, 8, 299, 800);  // 3 S

	latest_user_event_tick = clock_time();

	device_in_connection_state = 1;//

	interval_update_tick = clock_time() | 1; //none zero


#if (UI_LED_ENABLE)
	gpio_write(GPIO_LED_RED, LED_ON_LEVAL);  //yellow light on
#endif
}




void 	task_terminate(u8 e,u8 *p, int n) //*p is terminate reason
{
	device_in_connection_state = 0;


	if(*p == HCI_ERR_CONN_TIMEOUT){

	}
	else if(*p == HCI_ERR_REMOTE_USER_TERM_CONN){  //0x13

	}
	else if(*p == HCI_ERR_CONN_TERM_MIC_FAILURE){

	}
	else{

	}



#if (BLE_APP_PM_ENABLE)
	 //user has push terminate pkt to ble TX buffer before deepsleep
	if(sendTerminate_before_enterDeep == 1){
		sendTerminate_before_enterDeep = 2;
	}
#endif


#if (UI_LED_ENABLE)
	gpio_write(GPIO_LED_RED, !LED_ON_LEVAL);  //yellow light off
#endif

	advertise_begin_tick = clock_time();

}




_attribute_ram_code_ void	user_set_rf_power (u8 e, u8 *p, int n)
{
	rf_set_power_level_index (MY_RF_POWER_INDEX);
}




void	task_conn_update_req (u8 e, u8 *p, int n)
{

}

void	task_conn_update_done (u8 e, u8 *p, int n)
{

}


_attribute_ram_code_
void blt_pm_proc(void)
{

#if(BLE_APP_PM_ENABLE)


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





	#if (!TEST_CONN_CURRENT_ENABLE)   //test connection power, should disable deepSleep
			if(sendTerminate_before_enterDeep == 2){  //Terminate OK
				analog_write(USED_DEEP_ANA_REG, analog_read(USED_DEEP_ANA_REG) | CONN_DEEP_FLG);
				cpu_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD, 0);  //deepSleep
			}


			if(  !blc_ll_isControllerEventPending() ){  //no controller event pending
				//adv 60s, deepsleep
				if( blc_ll_getCurrentState() == BLS_LINK_STATE_ADV && !sendTerminate_before_enterDeep && \
					clock_time_exceed(advertise_begin_tick , ADV_IDLE_ENTER_DEEP_TIME * 1000000))
				{
					cpu_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD, 0);  //deepsleep
				}
				//conn 60s no event(key/voice/led), enter deepsleep
				else if( device_in_connection_state && \
						clock_time_exceed(latest_user_event_tick, CONN_IDLE_ENTER_DEEP_TIME * 1000000) )
				{

					bls_ll_terminateConnection(HCI_ERR_REMOTE_USER_TERM_CONN); //push terminate cmd into ble TX buffer
					bls_ll_setAdvEnable(0);   //disable adv
					sendTerminate_before_enterDeep = 1;
				}
			}
	#endif  //end of !TEST_CONN_CURRENT_ENABLE


#endif  //end of BLE_APP_PM_ENABLE
}







void user_init_normal(void)
{
	//random number generator must be initiated here( in the beginning of user_init_nromal)
	//when deepSleep retention wakeUp, no need initialize again
//	random_generator_init();  //this is must



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
	blc_ll_initAdvertising_module(mac_public); 	//adv module: 		 mandatory for BLE slave,
	blc_ll_initConnection_module();				//connection module  mandatory for BLE slave/master
	blc_ll_initSlaveRole_module();				//slave module: 	 mandatory for BLE slave,
//	blc_ll_initPowerManagement_module();        //pm module:      	 optional


	blc_ll_initTxFifo(app_ll_txfifo, LL_TX_FIFO_SIZE, LL_TX_FIFO_NUM);
	blc_ll_initRxFifo(app_ll_rxfifo, LL_RX_FIFO_SIZE, LL_RX_FIFO_NUM);


#if 1
	////// Host Initialization  //////////
	blc_gap_peripheral_init();    //gap initialization
	extern void my_att_init ();
	my_att_init (); //gatt initialization
	blc_l2cap_register_handler (blc_l2cap_packet_receive);  	//l2cap initialization

	//Smp Initialization may involve flash write/erase(when one sector stores too much information,
	//   is about to exceed the sector threshold, this sector must be erased, and all useful information
	//   should re_stored) , so it must be done after battery check
	#if (APP_SECURITY_ENABLE)
		blc_smp_peripheral_init();
	#else
		blc_smp_setSecurityLevel(No_Security);
	#endif




	////////////////// config adv packet /////////////////////
	#if (APP_SECURITY_ENABLE)
		u8 bond_number = blc_smp_param_getCurrentBondingDeviceNumber();  //get bonded device number
		smp_param_save_t  bondInfo;
		if(bond_number)   //at least 1 bonding device exist
		{
			bls_smp_param_loadByIndex( bond_number - 1, &bondInfo);  //get the latest bonding device (index: bond_number-1 )

		}

		if(bond_number)   //set direct adv
		{
			//set direct adv
			u8 status = bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
											ADV_TYPE_CONNECTABLE_DIRECTED_LOW_DUTY, app_own_address_type,
											bondInfo.peer_addr_type,  bondInfo.peer_addr,
											BLT_ENABLE_ADV_ALL,
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
			if(status != BLE_SUCCESS) { while(1); }  //debug: adv setting err
		}
	#endif



	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));



	bls_ll_setAdvEnable(1);  //adv enable



	//set rf power index, user must set it after every suspend wakeup, cause relative setting will be reset in suspend
	user_set_rf_power(0, 0, 0);


	bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &task_connect);
	bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &task_terminate);
	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_EXIT, &user_set_rf_power);



#if (UI_KEYBOARD_ENABLE)
	/////////// keyboard gpio wakeup init ////////
//	u32 pin[] = KB_DRIVE_PINS;
//	for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
//	{
//		cpu_set_gpio_wakeup (pin[i], Level_High,1);  //drive pin pad high wakeup deepsleep
//	}
//
//	bls_app_registerEventCallback (BLT_EV_FLAG_GPIO_EARLY_WAKEUP, &proc_keyboard);
#elif (UI_BUTTON_ENABLE)

	cpu_set_gpio_wakeup (SW1_GPIO, Level_Low,1);  //button pin pad low wakeUp suspend/deepSleep
	cpu_set_gpio_wakeup (SW2_GPIO, Level_Low,1);  //button pin pad low wakeUp suspend/deepSleep

	bls_app_registerEventCallback (BLT_EV_FLAG_GPIO_EARLY_WAKEUP, &proc_button);
#endif


//	advertise_begin_tick = clock_time();




//	blc_ll_setAdvCustomedChannel( 37, 37, 37);  //debug


#if (UDB_DEBUG_EN)
	myudb_usb_init (0x120,&uart_txfifo);		//0x120: usb sub-id
	usb_set_pin_en ();
#endif

}




_attribute_ram_code_ void user_init_deepRetn(void)
{
#if (PM_DEEPSLEEP_RETENTION_ENABLE)

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

#endif
}


/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////





void main_loop (void)
{

	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();


	////////////////////////////////////// UI entry /////////////////////////////////
	#if (UI_KEYBOARD_ENABLE)
			proc_keyboard (0,0, 0);
	#elif (UI_BUTTON_ENABLE)
			// process button 1 second later after power on, to avoid power unstable
			if(!button_detect_en && clock_time_exceed(0, 1000000)){
				button_detect_en = 1;
			}
			if(button_detect_en && clock_time_exceed(button_detect_tick, 5000))
			{
				button_detect_tick = clock_time();
				proc_button(0, 0, 0);  //button triggers pair & unpair  and OTA
			}
	#endif


	////////////////////////////////////// PM Process /////////////////////////////////
	#if (UI_KEYBOARD_ENABLE)
			blt_pm_proc();
	#elif (UI_BUTTON_ENABLE)
			if(button_not_released){
				bls_pm_setSuspendMask (SUSPEND_DISABLE);
			}
			else{
				bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
			}
	#endif




#if (UDB_DEBUG_EN)
	myudb_usb_handle_irq ();

	#if (0)  //demo
		static u32 tick_str = 0;
		static u32 test_cnt1 = 0;
		static u32 test_cnt2 = 0;

		static u32 tick_logEvt = 0;
		static u32 tick_logTask = 0;
		static u32 tick_data = 0;

		DBG_CHN6_HIGH;
		if(clock_time_exceed(tick_str, 500*1000)){
			tick_str = clock_time();
			test_cnt1 ++;
			my_dump_str_data (1, "test_cnt:", &test_cnt1, 4);
		}
		DBG_CHN6_LOW;

		DBG_CHN7_HIGH;
		if(clock_time_exceed(tick_logEvt, 9*1000)){
			tick_logEvt = clock_time();
			log_tick(1, SLEV_timestamp);
			log_event(1, SLEV_test_event);
		}
		DBG_CHN7_LOW;


		DBG_CHN8_HIGH;
		if(clock_time_exceed(tick_logTask, 13*1000)){
			tick_logTask = clock_time();

			log_task(1, SL01_test_task, 1);
			sleep_us(1000);
			log_task(1, SL01_test_task, 0);
		}
		DBG_CHN8_LOW;


		DBG_CHN9_HIGH;
		if(clock_time_exceed(tick_data, 7*1000)){
			tick_data = clock_time();
			test_cnt2 ++;
			log_tick(1, SLEV_timestamp);
			log_b8 (1, SL08_test_1B, test_cnt2);
			log_b16 (1, SL16_test_2B, test_cnt2);
		}
		DBG_CHN9_LOW;

	#endif

#endif
}





