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
#include "app_ui.h"
#include "app_att.h"
#include "app_buffer.h"

#define 	ADV_IDLE_ENTER_DEEP_TIME			60  //60 s
#define 	CONN_IDLE_ENTER_DEEP_TIME			60  //60 s

#define 	MY_DIRECT_ADV_TIME					2000000

#define		MY_RF_POWER_INDEX					RF_POWER_INDEX_P3dBm

#define		BLE_DEVICE_ADDRESS_TYPE 			BLE_DEVICE_ADDRESS_PUBLIC

_attribute_data_retention_	own_addr_type_t 	app_own_address_type = OWN_ADDRESS_PUBLIC;



/**
 * @brief	BLE Advertising data
 */
const u8	tbl_advData[] = {
	 5,  DT_COMPLETE_LOCAL_NAME, 				't', 'H', 'I', 'D',
	 2,	 DT_FLAGS, 								0x05, 					// BLE limited discoverable mode and BR/EDR not supported
	 3,  DT_APPEARANCE, 						0x80, 0x01, 			// 384, Generic Remote Control, Generic category
	 5,  DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID,	0x12, 0x18, 0x0F, 0x18,	// incomplete list of service class UUIDs (0x1812, 0x180F)
};

/**
 * @brief	BLE Scan Response Packet data
 */
const u8	tbl_scanRsp [] = {
	 8,  DT_COMPLETE_LOCAL_NAME, 				 't', 'S', 'a', 'm', 'p', 'l', 'e',
};


_attribute_data_retention_	int device_in_connection_state;

_attribute_data_retention_	u32 advertise_begin_tick;

_attribute_data_retention_	u8	sendTerminate_before_enterDeep = 0;

_attribute_data_retention_	u32	latest_user_event_tick;

_attribute_data_retention_	u8 button_detect_en;




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
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_ADV_DURATION_TIMEOUT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void 	app_switch_to_undirected_adv(u8 e, u8 *p, int n)
{

	bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
						ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
						0,  NULL,
						BLT_ENABLE_ADV_ALL,
						ADV_FP_NONE);

	bls_ll_setAdvEnable(BLC_ADV_ENABLE);  //must: set adv enable
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
	tlk_evt_connect_t *pConnEvt = (tlk_evt_connect_t *)p;

	tlkapi_send_string_data(APP_CONTR_EVENT_LOG_EN, "[APP][EVT] connect", pConnEvt->initA, 12);

//	bls_l2cap_requestConnParamUpdate (8, 8, 19, 200);  // 200mS
	bls_l2cap_requestConnParamUpdate (8, 8, 99, 400);  // 1 S
//	bls_l2cap_requestConnParamUpdate (8, 8, 149, 600);  // 1.5 S
//	bls_l2cap_requestConnParamUpdate (8, 8, 199, 800);  // 2 S
//	bls_l2cap_requestConnParamUpdate (8, 8, 249, 800);  // 2.5 S
//	bls_l2cap_requestConnParamUpdate (8, 8, 299, 800);  // 3 S

	latest_user_event_tick = clock_time();

	device_in_connection_state = 1;//


#if (UI_LED_ENABLE && !TEST_CONN_CURRENT_ENABLE)
	gpio_write(GPIO_LED_RED, LED_ON_LEVEL);  //red led on
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
	tlkapi_send_string_data(APP_CONTR_EVENT_LOG_EN, "[APP][EVT] disconnect", p, 1);

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
	if(sendTerminate_before_enterDeep == 1 && !TEST_CONN_CURRENT_ENABLE){
		sendTerminate_before_enterDeep = 2;
		bls_ll_setAdvEnable(BLC_ADV_DISABLE);   //disable adv
	}
#endif


#if (UI_LED_ENABLE && !TEST_CONN_CURRENT_ENABLE)
	gpio_write(GPIO_LED_RED, !LED_ON_LEVEL);  //red led off
#endif

	advertise_begin_tick = clock_time();
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
	#if (!TEST_CONN_CURRENT_ENABLE && UI_KEYBOARD_ENABLE)
		if(scan_pin_need || key_not_released || ota_is_working)
		{
			bls_pm_setSuspendMask (SUSPEND_DISABLE);
		}
	#endif





	#if (!TEST_CONN_CURRENT_ENABLE)   //test connection power, should disable deepSleep
			if(sendTerminate_before_enterDeep == 2){  //Terminate OK
				analog_write_reg8(USED_DEEP_ANA_REG, analog_read_reg8(USED_DEEP_ANA_REG) | CONN_DEEP_FLG);
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
					sendTerminate_before_enterDeep = 1;
				}
			}
	#endif  //end of !TEST_CONN_CURRENT_ENABLE


#endif  //end of BLE_APP_PM_ENABLE
}


#if (BATT_CHECK_ENABLE)  //battery check must do before OTA relative operation

_attribute_data_retention_	u32	lowBattDet_tick   = 0;

/**
 * @brief		this function is used to process battery power.
 * 				The low voltage protection threshold 2.0V is an example and reference value. Customers should
 * 				evaluate and modify these thresholds according to the actual situation. If users have unreasonable designs
 * 				in the hardware circuit, which leads to a decrease in the stability of the power supply network, the
 * 				safety thresholds must be increased as appropriate.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void user_battery_power_check(u16 alarm_vol_mv)
{
	/*For battery-powered products, as the battery power will gradually drop, when the voltage is low to a certain
	  value, it will cause many problems.
		a) When the voltage is lower than operating voltage range of chip, chip can no longer guarantee stable operation.
		b) When the battery voltage is low, due to the unstable power supply, the write and erase operations
			of Flash may have the risk of error, causing the program firmware and user data to be modified abnormally,
			and eventually causing the product to fail. */
	u8 battery_check_returnValue=0;
	if(analog_read(USED_DEEP_ANA_REG) & LOW_BATT_FLG){
		battery_check_returnValue=app_battery_power_check(alarm_vol_mv+200);
	}
	else{
		battery_check_returnValue=app_battery_power_check(alarm_vol_mv);
	}
	if(battery_check_returnValue)
	{
		analog_write_reg8(USED_DEEP_ANA_REG,  analog_read_reg8(USED_DEEP_ANA_REG) &(~LOW_BATT_FLG));  //clr
	}
	else
	{
		#if (UI_LED_ENABLE)  //led indicate
			for(int k=0;k<3;k++){
				gpio_write(GPIO_LED_BLUE, LED_ON_LEVEL);
				sleep_us(200000);
				gpio_write(GPIO_LED_BLUE, !LED_ON_LEVEL);
				sleep_us(200000);
			}
		#endif
		analog_write_reg8(USED_DEEP_ANA_REG,  analog_read_reg8(USED_DEEP_ANA_REG) | LOW_BATT_FLG);  //mark

		u32 pin[] = KB_DRIVE_PINS;
		for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
		{
			cpu_set_gpio_wakeup (pin[i], Level_High, 1);  //drive pin pad high wakeup deepsleep
		}

		cpu_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD, 0);  //deepsleep
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
	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_init();
		blc_debug_enableStackLog(STK_LOG_NONE);
	#endif


	#if (BATT_CHECK_ENABLE)
	/*The SDK must do a quick low battery detect during user initialization instead of waiting
	  until the main_loop. The reason for this process is to avoid application errors that the device
	  has already working at low power.
	  Considering the working voltage of MCU and the working voltage of flash, if the Demo is set below 2.0V,
	  the chip will alarm and deep sleep (Due to PM does not work in the current version of B92, it does not go
	  into deepsleep), and once the chip is detected to be lower than 2.0V, it needs to wait until the voltage rises to 2.2V,
	  the chip will resume normal operation. Consider the following points in this design:
		At 2.0V, when other modules are operated, the voltage may be pulled down and the flash will not
		work normally. Therefore, it is necessary to enter deepsleep below 2.0V to ensure that the chip no
		longer runs related modules;
		When there is a low voltage situation, need to restore to 2.2V in order to make other functions normal,
		this is to ensure that the power supply voltage is confirmed in the charge and has a certain amount of
		power, then start to restore the function can be safer.*/
		user_battery_power_check(2000);
	#endif


	blc_readFlashSize_autoConfigCustomFlashSector();

	/* attention that this function must be called after "blc readFlashSize_autoConfigCustomFlashSector" !!!*/
	blc_app_loadCustomizedParameters_normal();

	#if (APP_FLASH_PROTECTION_ENABLE)
		app_flash_protection_operation(FLASH_OP_EVT_APP_INITIALIZATION, 0, 0);
		blc_appRegisterStackFlashOperationCallback(app_flash_protection_operation); //register flash operation callback for stack
	#endif

//////////////////////////// basic hardware Initialization  Begin //////////////////////////////////
	/* random number generator must be initiated here( in the beginning of user_init_normal).
	 * When deepSleep retention wakeUp, no need initialize again */
	random_generator_init();  //this is must

//////////////////////////// basic hardware Initialization  End //////////////////////////////////




//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////
	/* for 1M   Flash, flash_sector_mac_address equals to 0xFF000
	 * for 2M   Flash, flash_sector_mac_address equals to 0x1FF000*/
	u8  mac_public[6];
	u8  mac_random_static[6];
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);
	//tlkapi_printf(APP_LOG_EN, "[APP][INI] Flash address for MAC: %x", flash_sector_mac_address);
	tlkapi_send_string_data(APP_LOG_EN, "[APP][INI] Public Address", mac_public, 6);

	#if(BLE_DEVICE_ADDRESS_TYPE == BLE_DEVICE_ADDRESS_PUBLIC)
		app_own_address_type = OWN_ADDRESS_PUBLIC;
	#elif(BLE_DEVICE_ADDRESS_TYPE == BLE_DEVICE_ADDRESS_RANDOM_STATIC)
		app_own_address_type = OWN_ADDRESS_RANDOM;
		blc_ll_setRandomAddr(mac_random_static);
	#endif


	//////////// Controller Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();                      //mandatory
	blc_ll_initStandby_module(mac_public);		//mandatory
	blc_ll_initLegacyAdvertising_module(); 		//legacy advertising module: mandatory for BLE slave
	blc_ll_initConnection_module();				//connection module  mandatory for BLE slave/master
	blc_ll_initSlaveRole_module();				//slave module: 	 mandatory for BLE slave,

#if FAST_SETTLE
	blc_ll_initFastSettle(0,0);
#endif

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


	/* SMP Initialization may involve flash write/erase(when one sector stores too much information,
	 *   is about to exceed the sector threshold, this sector must be erased, and all useful information
	 *   should re_stored) , so it must be done after battery check */
	//blc_smp_setParingMethods (LE_Secure_Connection);
	#if (APP_SECURITY_ENABLE)
		if(blc_flash_capacity == FLASH_SIZE_1M){
			bls_smp_configParingSecurityInfoStorageAddr(FLASH_ADR_SMP_PAIRING_1M_FLASH);
		}
		else if(blc_flash_capacity == FLASH_SIZE_2M){
			bls_smp_configParingSecurityInfoStorageAddr(FLASH_ADR_SMP_PAIRING_2M_FLASH);
		}
		else if(blc_flash_capacity == FLASH_SIZE_4M){
			bls_smp_configParingSecurityInfoStorageAddr(FLASH_ADR_SMP_PAIRING_4M_FLASH);
		}

		blc_smp_peripheral_init();

		// Hid device on android7.0/7.1 or later version
		// New paring: send security_request immediately after connection complete
		// reConnect:  send security_request 1000mS after connection complete. If master start paring or encryption before 1000mS timeout, slave do not send security_request.
		blc_smp_configSecurityRequestSending(SecReq_IMM_SEND, SecReq_PEND_SEND, 1000); //if not set, default is:  send "security request" immediately after link layer connection established(regardless of new connection or reconnection)
	#else
		blc_smp_setSecurityLevel(No_Security);
	#endif
	//////////// Host Initialization  End /////////////////////////

//////////////////////////// BLE stack Initialization  End //////////////////////////////////


	////////////////// config adv packet /////////////////////
	#if (APP_SECURITY_ENABLE && APP_DIRECT_ADV_ENABLE)
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

			//it is recommended that direct adv only last for several seconds, then switch to undirected adv
			bls_ll_setAdvDuration(MY_DIRECT_ADV_TIME, 1);
			bls_app_registerEventCallback (BLT_EV_FLAG_ADV_DURATION_TIMEOUT, &app_switch_to_undirected_adv);

		}
		else   //set undirected adv
	#endif
		{
			u8 status = bls_ll_setAdvParam(  ADV_INTERVAL_30MS, ADV_INTERVAL_35MS,
											 ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
											 0,  NULL,
											 BLT_ENABLE_ADV_ALL,
											 ADV_FP_NONE);
			if(status != BLE_SUCCESS) { while(1); }  //debug: adv setting err
		}




	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));



	bls_ll_setAdvEnable(BLC_ADV_ENABLE);  //adv enable



	//set rf power index, user must set it after every suspend wakeup, cause relative setting will be reset in suspend
	rf_set_power_level_index (MY_RF_POWER_INDEX);


	bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &task_connect);
	bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &task_terminate);
	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_EXIT, &task_suspend_exit);

	///////////////////// Power Management initialization///////////////////
	#if(BLE_APP_PM_ENABLE)
		blc_ll_initPowerManagement_module();

		#if (PM_DEEPSLEEP_RETENTION_ENABLE)
			/* pay attention: if your RamCode and retention data bigger than 32K, you must choose retention SRAM 64K */
			blc_pm_setDeepsleepRetentionType(DEEPSLEEP_RAM_SIZE_TO_MODE(PM_DEEPSLEEP_RETENTION_SIZE));
			bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
			blc_pm_setDeepsleepRetentionThreshold(95, 95);
			blc_pm_setDeepsleepRetentionEarlyWakeupTiming(TEST_CONN_CURRENT_ENABLE ? 370 : 400);

		#else
			bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
		#endif

		bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_ENTER, &ble_remote_set_sleep_wakeup);
	#else
		#if (PM_DEEPSLEEP_RETENTION_ENABLE)
			#error "can not use deep retention when PM disable !!!"
		#endif

		bls_pm_setSuspendMask (SUSPEND_DISABLE);
	#endif

	///////////////////// OS Management initialization///////////////////
	#if( FREERTOS_ENABLE )
		extern void os_give_sem_from_isr(void);
		extern void os_give_sem(void);
		blc_ll_set_freertos_en(1);
		blc_ll_registerGiveSemCb(os_give_sem_from_isr, os_give_sem);
		bls_pm_setOsSleepMask(SUSPEND_OS_SLEEP | DEEPRET_OS_SLEEP);
		blc_pm_setOsSleepThreshold(200);
		blc_pm_setDeepsleepRetentionType(DEEPSLEEP_MODE_RET_SRAM_LOW64K); //default use 32k deep retention
	#endif


	#if (BLE_OTA_SERVER_ENABLE)
		////////////////// OTA relative ////////////////////////
		#if (TLKAPI_DEBUG_ENABLE)
			blc_debug_addStackLog(STK_LOG_OTA_FLOW);  // STK_LOG_OTA_FLOW | STK_LOG_OTA_DATA
		#endif

		#if (APP_HW_FIRMWARE_ENCRYPTION_ENABLE)
			#if (!HARDWARE_FIRMWARE_ENCRYPTION_SUPPORT_EN)
				#error "MCU do not support HW FIRMWARE ENCRYPTION"
			#endif
			blc_ota_enableFirmwareEncryption();
		#endif

		#if (APP_HW_SECURE_BOOT_ENABLE)
			#if (!HARDWARE_SECURE_BOOT_SUPPORT_EN)
				#error "MCU do not support HW SECURE BOOT"
			#endif
			/* attention that "blc_ota enableSecureBoot" must be called before "blc_ota initOtaServer_module" !!! */
			blc_ota_enableSecureBoot();
		#endif

		blc_ota_initOtaServer_module();

		blc_ota_setOtaProcessTimeout(30);   //OTA process timeout:  30 seconds
		blc_ota_setOtaDataPacketTimeout(4);	//OTA data packet timeout:  4 seconds
		blc_ota_registerOtaStartCmdCb(app_enter_ota_mode);
		blc_ota_registerOtaResultIndicationCb(app_ota_end_result);
	#endif

	#if (UI_KEYBOARD_ENABLE)
		/////////// keyboard gpio wakeup init ////////
		u32 pin[] = KB_DRIVE_PINS;
		for (int i=0; i<(sizeof (pin)/sizeof(*pin)); i++)
		{
			cpu_set_gpio_wakeup (pin[i], Level_High,1);  //drive pin pad high wakeup deepsleep
		}

		bls_app_registerEventCallback (BLT_EV_FLAG_GPIO_EARLY_WAKEUP, &proc_keyboard);
	#elif (UI_BUTTON_ENABLE)

		cpu_set_gpio_wakeup (SW1_GPIO, Level_Low,1);  //button pin pad low wakeUp suspend/deepSleep
		cpu_set_gpio_wakeup (SW2_GPIO, Level_Low,1);  //button pin pad low wakeUp suspend/deepSleep

		bls_app_registerEventCallback (BLT_EV_FLAG_GPIO_EARLY_WAKEUP, &proc_button);

	#endif

	advertise_begin_tick = clock_time();


	tlkapi_send_string_data(APP_LOG_EN, "[APP][INI] BLE sample init", NULL, 0);
}



/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void user_init_deepRetn(void)
{
#if (BLE_APP_PM_ENABLE && PM_DEEPSLEEP_RETENTION_ENABLE)
	blc_app_loadCustomizedParameters_deepRetn();

	blc_ll_initBasicMCU();   //mandatory
	rf_set_power_level_index (MY_RF_POWER_INDEX);
	blc_ll_recoverDeepRetention();

#if	FAST_SETTLE
	if(is_rf_tx_fast_settle_en() | is_rf_rx_fast_settle_en())
	{
		set_ldo_trim_val((u8*)&fast_settle.ldo_trim);
		if(is_rf_tx_fast_settle_en())
		{
			rf_tx_fast_settle();
			rf_tx_fast_settle_en();
		}
		else
		{
			rf_rx_fast_settle();
			rf_rx_fast_settle_en();
		}
	}
#endif

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
	#if (BATT_CHECK_ENABLE)
		adc_hw_initialized=0;
	#endif


	#if (TLKAPI_DEBUG_ENABLE)
		tlkapi_debug_deepRetn_init();
	#endif

#endif
}




#if (APP_FLASH_PROTECTION_ENABLE)

/**
 * @brief      flash protection operation, including all locking & unlocking for application
 * 			   handle all flash write & erase action for this demo code. use should add more more if they have more flash operation.
 * @param[in]  flash_op_evt - flash operation event, including application layer action and stack layer action event(OTA write & erase)
 * 			   attention 1: if you have more flash write or erase action, you should should add more type and process them
 * 			   attention 2: for "end" event, no need to pay attention on op_addr_begin & op_addr_end, we set them to 0 for
 * 			   			    stack event, such as stack OTA write new firmware end event
 * @param[in]  op_addr_begin - operating flash address range begin value
 * @param[in]  op_addr_end - operating flash address range end value
 * 			   attention that, we use: [op_addr_begin, op_addr_end)
 * 			   e.g. if we write flash sector from 0x10000 to 0x20000, actual operating flash address is 0x10000 ~ 0x1FFFF
 * 			   		but we use [0x10000, 0x20000):  op_addr_begin = 0x10000, op_addr_end = 0x20000
 * @return     none
 */
_attribute_data_retention_ u16  flash_lockBlock_cmd;
void app_flash_protection_operation(u8 flash_op_evt, u32 op_addr_begin, u32 op_addr_end)
{
	if(flash_op_evt == FLASH_OP_EVT_APP_INITIALIZATION)
	{
		/* ignore "op addr_begin" and "op addr_end" for initialization event
		 * must call "flash protection_init" first, will choose correct flash protection relative API according to current internal flash type in MCU */
		flash_protection_init();

		/* just sample code here, protect all flash area for old firmware and OTA new firmware.
		 * user can change this design if have other consideration */
		u32  app_lockBlock;
		#if (BLE_OTA_SERVER_ENABLE)
			u32 multiBootAddress = blc_ota_getCurrentUsedMultipleBootAddress();
			if(multiBootAddress == MULTI_BOOT_ADDR_0x20000){
				app_lockBlock = FLASH_LOCK_LOW_256K;
			}
			else if(multiBootAddress == MULTI_BOOT_ADDR_0x40000){
				app_lockBlock = FLASH_LOCK_LOW_512K;
			}
			else if(multiBootAddress == MULTI_BOOT_ADDR_0x80000){
				/* attention that 1M capacity flash can not lock all 1M area, should leave some upper sector for system data and user data
				 * will use a approximate value */
				app_lockBlock = FLASH_LOCK_LOW_1M;
			}
		#else
			app_lockBlock = FLASH_LOCK_LOW_256K; //just demo value, user can change this value according to application
		#endif


		flash_lockBlock_cmd = flash_change_app_lock_block_to_flash_lock_block(app_lockBlock);

		tlkapi_send_string_data(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] initialization, lock flash", 0, 0);
		flash_lock(flash_lockBlock_cmd);
	}
	else if(flash_op_evt == FLASH_OP_EVT_STACK_OTA_CLEAR_OLD_FW_BEGIN)
	{
		/* OTA clear old firmware begin event is triggered by stack, in "blc ota_initOtaServer_module", rebooting from a successful OTA.
		 * Software will erase whole old firmware for potential next new OTA, need unlock flash if any part of flash address from
		 * "op addr_begin" to "op addr_end" is in locking block area.
		 * In this sample code, we protect whole flash area for old and new firmware, so here we do not need judge "op addr_begin" and "op addr_end",
		 * must unlock flash */
		tlkapi_send_string_u32s(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] OTA clear old FW begin, un_lock flash", op_addr_begin, op_addr_end, 0, 0);
		flash_unlock();
	}
	else if(flash_op_evt == FLASH_OP_EVT_STACK_OTA_CLEAR_OLD_FW_END)
	{
		/* ignore "op addr_begin" and "op addr_end" for END event
		 * OTA clear old firmware end event is triggered by stack, in "blc ota_initOtaServer_module", erasing old firmware data finished.
		 * In this sample code, we need lock flash again, because we have unlocked it at the begin event of clear old firmware */
		tlkapi_send_string_data(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] OTA clear old FW end, restore flash locking ", 0, 0);
		flash_lock(flash_lockBlock_cmd);
	}
	else if(flash_op_evt == FLASH_OP_EVT_STACK_OTA_WRITE_NEW_FW_BEGIN)
	{
		/* OTA write new firmware begin event is triggered by stack, when receive first OTA data PDU.
		 * Software will write data to flash on new firmware area,  need unlock flash if any part of flash address from
		 * "op addr_begin" to "op addr_end" is in locking block area.
		 * In this sample code, we protect whole flash area for old and new firmware, so here we do not need judge "op addr_begin" and "op addr_end",
		 * must unlock flash */
		tlkapi_send_string_u32s(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] OTA write new FW begin, un_lock flash", op_addr_begin, op_addr_end, 0, 0);
		flash_unlock();
	}
	else if(flash_op_evt == FLASH_OP_EVT_STACK_OTA_WRITE_NEW_FW_END)
	{
		/* ignore "op addr_begin" and "op addr_end" for END event
		 * OTA write new firmware end event is triggered by stack, after OTA end or an OTA error happens, writing new firmware data finished.
		 * In this sample code, we need lock flash again, because we have unlocked it at the begin event of write new firmware */
		tlkapi_send_string_data(APP_FLASH_PROT_LOG_EN, "[FLASH][PROT] OTA write new FW end, restore flash locking ", 0, 0);
		flash_lock(flash_lockBlock_cmd);
	}
	/* add more flash protection operation for your application if needed */
}


#endif



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
	#if (BATT_CHECK_ENABLE)
		/*The frequency of low battery detect is controlled by the variable lowBattDet_tick, which is executed every
		 500ms in the demo. Users can modify this time according to their needs.*/
		if(battery_get_detect_enable() && clock_time_exceed(lowBattDet_tick, 500000) ){
			lowBattDet_tick = clock_time();
			user_battery_power_check(BAT_DEEP_THRESHOLD_MV);
		}
	#endif


	#if (UI_KEYBOARD_ENABLE)
			proc_keyboard (0, 0, 0);
	#elif (UI_BUTTON_ENABLE)
			// process button 1 second later after power on, to avoid power unstable
			if(!button_detect_en && clock_time_exceed(0, 1000000)){
				button_detect_en = 1;
			}
			if(button_detect_en){
				proc_button(0, 0, 0);
			}
	#endif

	////////////////////////////////////// PM Process /////////////////////////////////

	blt_pm_proc();
	#if (UI_BUTTON_ENABLE)
			if(button_not_released){
				bls_pm_setSuspendMask (SUSPEND_DISABLE);
			}
			else{
				bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
			}
	#endif

}





