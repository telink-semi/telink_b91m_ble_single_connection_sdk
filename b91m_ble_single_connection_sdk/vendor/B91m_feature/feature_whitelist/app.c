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

#if (FEATURE_TEST_MODE == TEST_WHITELIST)


#define		MY_RF_POWER_INDEX					RF_POWER_INDEX_P3dBm

/**
 * @brief	Adv Packet data
 */
const u8	tbl_advData[] = {
	 0x0A, DT_COMPLETE_LOCAL_NAME, 't', 'e', 's', 't','w', 'h', 'i', 't','e',
	 0x02, DT_FLAGS, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
	 0x03, DT_APPEARANCE, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
	 0x05, DT_INCOMPLETE_LIST_16BIT_SERVICE_UUID, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
};

/**
 * @brief	Scan Response Packet data
 */
const u8	tbl_scanRsp [] = {
		0x0A, DT_COMPLETE_LOCAL_NAME, 't', 'e', 's', 't','w', 'h', 'i', 't','e',
	};




_attribute_data_retention_ int  app_whiteList_enable;











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


#endif  //end of BLE_APP_PM_ENABLE
}








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

	/* SMP Initialization may involve flash write/erase(when one sector stores too much information,
	 *   is about to exceed the sector threshold, this sector must be erased, and all useful information
	 *   should re_stored) , so it must be done after battery check */
	blc_smp_peripheral_init();

	// Hid device on android7.0/7.1 or later version
	// New paring: send security_request immediately after connection complete
	// reConnect:  send security_request 1000mS after connection complete. If master start paring or encryption before 1000mS timeout, slave do not send security_request.
	blc_smp_configSecurityRequestSending(SecReq_IMM_SEND, SecReq_PEND_SEND, 1000); //if not set, default is:  send "security request" immediately after link layer connection established(regardless of new connection or reconnection)
	//////////// Host Initialization  End /////////////////////////

//////////////////////////// BLE stack Initialization  End //////////////////////////////////


	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));


	smp_param_save_t  bondInfo;
	u8 bond_number = blc_smp_param_getCurrentBondingDeviceNumber();  //get bonded device number
	if(bond_number)   //get latest device info
	{
		bls_smp_param_loadByIndex( bond_number - 1, &bondInfo);  //get the latest bonding device (index: bond_number-1 )
	}


	blc_ll_clearWhiteList(); 	  //clear whitelist
	blc_ll_clearResolvingList(); //clear resolving list


	if(bond_number)  //use whitelist to filter master device
	{
		app_whiteList_enable = 1;

		blc_ll_addDeviceToWhiteList(bondInfo.peer_id_adrType, bondInfo.peer_id_addr);
		//if master device use RPA(resolvable private address), must add irk to resolving list
		if( IS_RESOLVABLE_PRIVATE_ADDR(bondInfo.peer_addr_type, bondInfo.peer_addr) ){
			//resolvable private address, should add peer irk to resolving list
			blc_ll_addDeviceToResolvingList(bondInfo.peer_id_adrType, bondInfo.peer_id_addr, bondInfo.peer_irk, NULL); //no local IRK
			blc_ll_setAddressResolutionEnable(1);
		}


		bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_30MS, \
							ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC, \
							0,  NULL, BLT_ENABLE_ADV_37, ADV_FP_ALLOW_SCAN_WL_ALLOW_CONN_WL);

	}
	else{

		bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_30MS,
							ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC,
							0,  NULL, BLT_ENABLE_ADV_ALL, ADV_FP_NONE);
	}





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


	////////////////////////////////////// PM Process /////////////////////////////////
	blt_pm_proc();


}





#endif  //end of (FEATURE_TEST_MODE == ...)
