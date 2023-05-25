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
#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"
#include "../default_att.h"

#if (FEATURE_TEST_MODE == TEST_PRIVACY)

#define 	MY_DIRECT_ADV_TMIE					20000000


#define     MY_APP_ADV_CHANNEL					BLT_ENABLE_ADV_ALL
#define 	MY_ADV_INTERVAL_MIN					ADV_INTERVAL_30MS
#define 	MY_ADV_INTERVAL_MAX					ADV_INTERVAL_35MS

#define		MY_RF_POWER_INDEX					RF_POWER_INDEX_P3dBm
#define		BLE_DEVICE_ADDRESS_TYPE 			BLE_DEVICE_ADDRESS_PUBLIC

#define 	GATT_UUID_CENTRAL_ADDR_RES       	0x2AA6

_attribute_data_retention_	own_addr_type_t 	app_own_address_type = OWN_ADDRESS_PUBLIC;

/**
 * @brief	Adv Packet data
 */
const u8	tbl_advData[] = {
	 0x08, DT_COMPLETE_LOCAL_NAME, 'f', 'e', 'a', 't', 'u', 'r', 'e',
	 0x02, DT_FLAGS, 0x05, 							// BLE limited discoverable mode and BR/EDR not supported
	 0x03, DT_APPEARANCE, 0x80, 0x01, 					// 384, Generic Remote Control, Generic category
	 0x05, DT_INCOMPLT_LIST_16BIT_SERVICE_UUID, 0x12, 0x18, 0x0F, 0x18,		// incomplete list of service class UUIDs (0x1812, 0x180F)
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

#if (UI_LED_ENABLE)
	gpio_write(GPIO_LED_RED, LED_ON_LEVAL);  //red light on
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
	gpio_write(GPIO_LED_RED, !LED_ON_LEVAL);  //red light off
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

_attribute_data_retention_ u32 bondingFlashAddr;
_attribute_data_retention_ u16 centralAddrResHdlReq = 0;

u8 * l2cap_matt_handler(u16 connHandle, u8 * p)
{
	rf_packet_l2cap_req_t * req = (rf_packet_l2cap_req_t *)p;

	switch(req->opcode){
		case ATT_OP_READ_BY_TYPE_RSP: {
			if(centralAddrResHdlReq == 1){
				centralAddrResHdlReq = 0;
				rf_pkt_att_readByTypeRsp_t *ptr = (rf_pkt_att_readByTypeRsp_t *)&req->type;
				u16 centralAddrResHdl = ptr->data[0] | (u16)ptr->data[1]<<8;
				printf("central addr res handle: 0x%x\n", centralAddrResHdl);

				//set bonding flg's bit0 field
				extern int	blc_smp_setPeerAddrResSupportFlg(u32 flash_addr, u8 support);
				blc_smp_setPeerAddrResSupportFlg(bondingFlashAddr, ptr->data[2]);
				printf("central addr res %s support\n", ptr->data[2] ? "" : "not");
				DBG_CHN4_TOGGLE;
			}
		}
		break;

		case ATT_OP_ERROR_RSP: {
			if(centralAddrResHdlReq == 1){
				centralAddrResHdlReq = 0;
				rf_packet_att_errRsp_t * errRsp = (rf_packet_att_errRsp_t*)p;
				//printf("errOpcode: 0x%\n", errRsp->errOpcode);
				//printf("errHandle: 0x%\n", errRsp->errHandle);
				printf("errReason: 0x%\n", errRsp->errReason);
				printf("central addr res not support\n");
			}
		}
		break;
	}

	return NULL;
}

void att_req_read_by_type (u8 *p, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len)
{
	p[0] = 2;
	p[1] = 9 + uuid_len;
	p[2] = 5 + uuid_len;
	p[3] = 0;
	p[4] = 4;
	p[5] = 0;
	p[6] = ATT_OP_READ_BY_TYPE_REQ;
	p[7] = start_attHandle;
	p[8] = start_attHandle >> 8;
	p[9] = end_attHandle;
	p[10] = end_attHandle >> 8;
	memcpy (p + 11, uuid, uuid_len);
}


int app_host_event_callback (u32 h, u8 *para, int n)
{
	u8 event = h & 0xFF;

	switch(event)
	{
		case GAP_EVT_SMP_PAIRING_BEGIN:
		{
			my_dump_str_data(APP_DUMP_EN,"Pairing Begin",0,0);
		}
		break;

		case GAP_EVT_SMP_PAIRING_SUCCESS:
		{
			gap_smp_paringSuccessEvt_t* p = (gap_smp_paringSuccessEvt_t*)para;
			my_dump_str_u8s(APP_DUMP_EN,"Pairing success,bond flg", p->bonding ?1:0,0,0,0);

			if(p->bonding_result){
				my_dump_str_data(APP_DUMP_EN,"save smp key succ",0,0);

			    u8 bond_number = blc_smp_param_getCurrentBondingDeviceNumber();  //get bonded device number
			    smp_param_save_t  bondInfo;

			    bondingFlashAddr = bls_smp_param_loadByIndex(bond_number - 1, &bondInfo);  //get the latest bonding device (index: bond_number-1 )
			    my_dump_str_u32s(APP_DUMP_EN,"bondingFlashAddr", bondingFlashAddr,0,0,0);

				u16 my_centralAddrResUUID = GATT_UUID_CENTRAL_ADDR_RES;
				u8 dat[64];
			    att_req_read_by_type (dat, 0x0001, 0xffff, (u8*)&my_centralAddrResUUID, 2);

			    if(bls_ll_pushTxFifo (BLS_CONN_HANDLE, dat)){
			    	DBG_CHN3_TOGGLE;
			    	centralAddrResHdlReq = 1;
			    	my_dump_str_data(APP_DUMP_EN,"read by type req: UUID: 0x%x\n", &my_centralAddrResUUID,2);
			    }
			}
			else{
				my_dump_str_data(APP_DUMP_EN,"save smp key failed",0,0);
			}
		}
		break;

		case GAP_EVT_SMP_PAIRING_FAIL:
		{
			gap_smp_paringFailEvt_t* p = (gap_smp_paringFailEvt_t*)para;
			my_dump_str_u8s(APP_DUMP_EN,"Pairing failed", p->reason,0,0,0);
		}
		break;

		case GAP_EVT_SMP_CONN_ENCRYPTION_DONE:
		{
			gap_smp_connEncDoneEvt_t* p = (gap_smp_connEncDoneEvt_t*)para;
			my_dump_str_data(APP_DUMP_EN,"Pairing failed",0,0);

			if(p->re_connect == SMP_STANDARD_PAIR){  //first paring

			}
			else if(p->re_connect == SMP_FAST_CONNECT){  //auto connect

			}
		}
		break;

		default:
		break;
	}

	return 0;
}

int controller_event_callback (u32 h, u8 *p, int n)
{
	if (h &HCI_FLAG_EVENT_BT_STD)		//ble controller hci event
	{
		u8 evtCode = h & 0xff;

		//------------ disconnect -------------------------------------
		if(evtCode == HCI_EVT_DISCONNECTION_COMPLETE)  //connection terminate
		{
			my_dump_str_data(APP_DUMP_EN,"le connection terminate event",0,0);
		}
		else if(evtCode == HCI_EVT_LE_META)
		{
			u8 subEvt_code = p[0];


			//------hci le event: le connection complete event---------------------------------
			if (subEvt_code == HCI_SUB_EVT_LE_CONNECTION_COMPLETE)	// connection complete
			{
				my_dump_str_data(APP_DUMP_EN,"le conn complete event",0,0);
			}
			else if (subEvt_code == HCI_SUB_EVT_LE_ENHANCED_CONNECTION_COMPLETE)
			{
				hci_le_enhancedConnCompleteEvt_t *pEvt = (hci_le_enhancedConnCompleteEvt_t *)p;
				my_dump_str_data(APP_DUMP_EN,"le enhanced conn complete event",0,0);
				my_dump_str_data(APP_DUMP_EN,"peerAddrType,peerAddr",&pEvt->PeerAddrType,7);
				my_dump_str_data(APP_DUMP_EN,"localRpa",pEvt->localRslvPrivAddr, 6);
				my_dump_str_data(APP_DUMP_EN,"peerRpa",pEvt->Peer_RslvPrivAddr, 6);
			}
		}
	}

	return 0;

}

void 	app_switch_to_undirect_adv(u8 e, u8 *p, int n)
{
	bls_ll_setAdvEnable(0);

	app_own_address_type = OWN_ADDRESS_PUBLIC;
	u8 status = bls_ll_setAdvParam(  MY_ADV_INTERVAL_MIN, MY_ADV_INTERVAL_MAX,
									 ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
									 0,  NULL,
									 MY_APP_ADV_CHANNEL,
									 ADV_FP_NONE);
	if(status != BLE_SUCCESS) { 	while(1);}  //debug: adv setting err
	status = blc_ll_setAddressResolutionEnable(0);
	printf("LL add resolution disable status: 0x%x\n", status);

	bls_ll_setAdvEnable(1);  //must: set adv enable
}


void slave_cfgLegAdvParam(void){
	bls_ll_setAdvEnable(0);  //adv disable
	////////////////// config adv packet /////////////////////
	u8 bond_number = blc_smp_param_getCurrentBondingDeviceNumber();  //get bonded device number
	my_dump_str_u8s(APP_DUMP_EN,"bond_number", bond_number,0,0,0);
	smp_param_save_t  bondInfo;
	if(bond_number)   //at least 1 bonding device exist
	{

		u32 current_addr = bls_smp_param_loadByIndex( bond_number - 1, &bondInfo);  //get the latest bonding device (index: bond_number-1 )
		my_dump_str_u32s(APP_DUMP_EN,"smpAddr", current_addr,0,0,0);
		ble_sts_t status;

		my_dump_str_data(APP_DUMP_EN,"central bondInfo.flag", &bondInfo.flag,1);

		u8 own_use_rpa = 1;
		u8 empty_16_ff[16] = {0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF,  0xFF, 0xFF, 0xFF, 0xFF};
		if(!memcmp(bondInfo.peer_irk, empty_16_ff, 16)){ //all 0xff
			memset(bondInfo.peer_irk, 0, 16);
		}
		if(!memcmp(bondInfo.local_irk, empty_16_ff, 16)){ //all 0xff
			own_use_rpa = 0;
			memset(bondInfo.local_irk, 0, 16);
		}

		status = blc_ll_addDeivceToResolvingList(bondInfo.peer_id_adrType, bondInfo.peer_id_addr, bondInfo.peer_irk, bondInfo.local_irk);
		my_dump_str_data(APP_DUMP_EN,"LL resolving list add status", &status,1);

		status = blc_ll_setAddressResolutionEnable(1);
		my_dump_str_data(APP_DUMP_EN,"LL add resolution enable status", &status,1);

		my_dump_str_data(APP_DUMP_EN,"central bondInfo.flag", &bondInfo.flag,1);

		app_own_address_type = own_use_rpa ? OWN_ADDRESS_RESOLVE_PRIVATE_PUBLIC : OWN_ADDRESS_PUBLIC;
		if(app_own_address_type == OWN_ADDRESS_RESOLVE_PRIVATE_PUBLIC){
			my_dump_str_data(APP_DUMP_EN,"RPA",0,0);
			DBG_CHN3_TOGGLE;;
		}else{
			my_dump_str_data(APP_DUMP_EN,"PUB",0,0);
			DBG_CHN4_TOGGLE;
		}

		u8* peerAddr;
		u8  peerAddrType;
		if(app_own_address_type < OWN_ADDRESS_RESOLVE_PRIVATE_PUBLIC){
			peerAddr = bondInfo.peer_addr;
			peerAddrType = bondInfo.peer_addr_type;
			my_dump_str_data(APP_DUMP_EN,"AdvA: pub",bondInfo.peer_addr,6);
		}
		else{
			peerAddr = bondInfo.peer_id_addr;
			peerAddrType = bondInfo.peer_id_adrType;
			my_dump_str_data(APP_DUMP_EN,"AdvA: rpa",bondInfo.peer_id_addr,6);
		}

		status = bls_ll_setAdvParam( MY_ADV_INTERVAL_MIN, MY_ADV_INTERVAL_MAX,
									ADV_TYPE_CONNECTABLE_DIRECTED_LOW_DUTY, app_own_address_type,
									 peerAddrType,  peerAddr,
									 MY_APP_ADV_CHANNEL,
									 ADV_FP_NONE);//  ADV_FP_NONE  ADV_FP_ALLOW_SCAN_ANY_ALLOW_CONN_WL
		if(status != BLE_SUCCESS) { 	while(1);}  //debug: adv setting err

		bls_ll_setAdvDuration(MY_DIRECT_ADV_TMIE>>1, 1);
		bls_app_registerEventCallback (BLT_EV_FLAG_ADV_DURATION_TIMEOUT, &app_switch_to_undirect_adv);
	}
	else{
#if	0
		app_own_address_type = OWN_ADDRESS_PUBLIC;
		u8 status = bls_ll_setAdvParam(  MY_ADV_INTERVAL_MIN, MY_ADV_INTERVAL_MAX,
										 ADV_TYPE_CONNECTABLE_UNDIRECTED, app_own_address_type,
										 0,  NULL,
										 MY_APP_ADV_CHANNEL,
										 ADV_FP_NONE);
		if(status != BLE_SUCCESS) { 	while(1);}  //debug: adv setting err
		status = blc_ll_setAddressResolutionEnable(0);
		my_dump_str_u8s(APP_DUMP_EN,"LL add resolution disable status",status,0,0,0);
#else

		u8	tmp_peer_irk[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
		u8	tmp_local_irk[16] = {0x00,0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88,0x99,0xaa,0xbb,0xcc,0xdd,0xee,0xff};
		u8	tmp_peer_addr[6] = {0x11,0x22,0x33,0x44,0x55,0x66};
		u8	tmp_peer_addr_type = OWN_ADDRESS_PUBLIC;

		my_dump_str_data(APP_DUMP_EN,"First Connection,made Irk and related message", tmp_local_irk,16);

		u8 status =blc_ll_addDeivceToResolvingList(tmp_peer_addr_type,tmp_peer_addr,tmp_peer_irk,tmp_local_irk);
		my_dump_str_data(APP_DUMP_EN,"LL resolving list add status", &status,1);

		status = blc_ll_setAddressResolutionEnable(1);

		status = bls_ll_setAdvParam( MY_ADV_INTERVAL_MIN, MY_ADV_INTERVAL_MAX,
									 ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_RESOLVE_PRIVATE_PUBLIC,
									 tmp_peer_addr_type,  tmp_peer_addr,
									 MY_APP_ADV_CHANNEL,
									 ADV_FP_NONE);//  ADV_FP_NONE  ADV_FP_ALLOW_SCAN_ANY_ALLOW_CONN_WL
		if(status != BLE_SUCCESS) { 	while(1);}  //debug: adv setting err



#endif
	}

		bls_ll_setAdvEnable(BLC_ADV_ENABLE);  //adv enable
}


/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
_attribute_no_inline_ void user_init_normal(void)
{
	/* random number generator must be initiated here( in the beginning of user_init_nromal).
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

	blc_l2cap_reg_att_cli_hander(l2cap_matt_handler);

	/* SMP Initialization may involve flash write/erase(when one sector stores too much information,
	 *   is about to exceed the sector threshold, this sector must be erased, and all useful information
	 *   should re_stored) , so it must be done after battery check */
	blc_smp_peripheral_init();

	//bluetooth low energy(LE) event
	blc_hci_le_setEventMask_cmd(		HCI_LE_EVT_MASK_CONNECTION_COMPLETE 		\
									|	HCI_LE_EVT_MASK_ENHANCED_CONNECTION_COMPLETE \
									|   HCI_LE_EVT_MASK_CONNECTION_ESTABLISH );         //connection establish: telink private event

	// Hid device on android7.0/7.1 or later version
	// New paring: send security_request immediately after connection complete
	// reConnect:  send security_request 1000mS after connection complete. If master start paring or encryption before 1000mS timeout, slave do not send security_request.
	//blc_smp_configSecurityRequestSending(SecReq_IMM_SEND, SecReq_PEND_SEND, 1000); //if not set, default is:  send "security request" immediately after link layer connection established(regardless of new connection or reconnection)
	blc_smp_configSecurityRequestSending( SecReq_NOT_SEND,  SecReq_NOT_SEND, 0);
	//////////// Host Initialization  End /////////////////////////

//////////////////////////// BLE stack Initialization  End //////////////////////////////////

	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));

#if(BLE_DEVICE_ADDRESS_TYPE == BLE_DEVICE_ADDRESS_PUBLIC)
	app_own_address_type = OWN_ADDRESS_PUBLIC;
#elif(BLE_DEVICE_ADDRESS_TYPE == BLE_DEVICE_ADDRESS_RANDOM_STATIC)
	app_own_address_type = OWN_ADDRESS_RANDOM;
	blc_ll_setRandomAddr(mac_random_static);
#endif

	slave_cfgLegAdvParam(); ///note: this API set resolve list.

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
//	proc_keyboard (0, 0, 0);

	////////////////////////////////////// PM Process /////////////////////////////////
	blt_pm_proc();



}





#endif  //end of (FEATURE_TEST_MODE == ...)
