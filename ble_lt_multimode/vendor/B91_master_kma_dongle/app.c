/********************************************************************************************************
 * @file     app.c 
 *
 * @brief    This is the source file for TLSR8258
 *
 * @author	 junwei.lu@telink-semi.com;
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *         
 *******************************************************************************************************/

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"



#include "vendor/common/blt_led.h"
#include "vendor/common/blt_common.h"
#include "app_config.h"
#include "app_buffer.h"

#include "blm_att.h"
#include "blm_pair.h"
#include "blm_host.h"
#include "blm_ota.h"


#define		MY_RF_POWER_INDEX					RF_POWER_P4p35dBm


#define		BLE_DEVICE_ADDRESS_TYPE 			BLE_DEVICE_ADDRESS_PUBLIC

_attribute_data_retention_	own_addr_type_t 	app_own_address_type = OWN_ADDRESS_PUBLIC;

int main_idle_loop (void);



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


	////// Controller Initialization  //////////
	blc_ll_initBasicMCU();                      //mandatory
	blc_ll_initStandby_module(mac_public);				//mandatory
	blc_ll_initScanning_module(mac_public); 			//scan module: 		 mandatory for BLE master,
	blc_ll_initInitiating_module();						//initiate module: 	 mandatory for BLE master,
	blc_ll_initConnection_module();				//connection module  mandatory for BLE slave/master
	blc_ll_initMasterRoleSingleConn_module();			//master module: 	 mandatory for BLE master,


	rf_set_power_level_index (MY_RF_POWER_INDEX);

	blc_ll_initTxFifo(app_ll_txfifo, LL_TX_FIFO_SIZE, LL_TX_FIFO_NUM);
	blc_ll_initRxFifo(app_ll_rxfifo, LL_RX_FIFO_SIZE, LL_RX_FIFO_NUM);



	////// Host Initialization  //////////
//	blc_gap_central_init();										//gap initialization
//	blc_l2cap_register_handler (app_l2cap_handler);  	//l2cap initialization
	blc_hci_registerControllerEventHandler(controller_event_callback); //controller hci event to host all processed in this func



	//bluetooth event
	blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE | HCI_EVT_MASK_ENCRYPTION_CHANGE);

	//bluetooth low energy(LE) event
	blc_hci_le_setEventMask_cmd(		HCI_LE_EVT_MASK_CONNECTION_COMPLETE 		\
									|	HCI_LE_EVT_MASK_ADVERTISING_REPORT 			\
									|   HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE  \
									|	HCI_LE_EVT_MASK_PHY_UPDATE_COMPLETE			\
									|   HCI_LE_EVT_MASK_CONNECTION_ESTABLISH );         //connection establish: telink private event


#if 0
	#if (BLE_HOST_SMP_ENABLE)
		blm_smp_configParingSecurityInfoStorageAddr(FLASH_ADR_PARING);
		blm_smp_registerSmpFinishCb(app_host_smp_finish);

		blc_smp_central_init();

		//SMP trigger by master
		blm_host_smp_setSecurityTrigger(MASTER_TRIGGER_SMP_FIRST_PAIRING | MASTER_TRIGGER_SMP_AUTO_CONNECT);
	#else  //TeLink referenced paring&bonding without standard paring in BLE spec
		blc_smp_setSecurityLevel(No_Security);

		user_master_host_pairing_management_init();
	#endif

#endif

	extern int host_att_register_idle_func (void *p);
	host_att_register_idle_func (main_idle_loop);





	//set scan parameter and scan enable

	blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,	\
							OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
//	blc_ll_setScanParameter(SCAN_TYPE_ACTIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,	\
//							OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
	blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);



	#if (UI_UPPER_COMPUTER_ENABLE)
		extern void app_upper_com_init(void);
		app_upper_com_init();
	#endif


#if (UDB_DEBUG_EN)
	myudb_usb_init (0x120,&uart_txfifo);		//0x120: usb sub-id
	usb_set_pin_en ();
#endif

}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
int main_idle_loop (void)
{


	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();


	///////////////////////////////////// proc usb cmd from host /////////////////////
//	usb_handle_irq();



	/////////////////////////////////////// HCI ///////////////////////////////////////
//	blc_hci_proc ();



	////////////////////////////////////// UI entry /////////////////////////////////
#if (UI_BUTTON_ENABLE)
	static u8 button_detect_en = 0;
	if(!button_detect_en && clock_time_exceed(0, 1000000)){// proc button 1 second later after power on
		button_detect_en = 1;
	}
	static u32 button_detect_tick = 0;
	if(button_detect_en && clock_time_exceed(button_detect_tick, 5000))
	{
		button_detect_tick = clock_time();
		proc_button();  //button triggers pair & unpair  and OTA
	}
#endif


#if (UI_UPPER_COMPUTER_ENABLE)
	extern void app_upper_com_proc(void);
	app_upper_com_proc();
#endif


	////////////////////////////////////// proc audio ////////////////////////////////
#if (UI_AUDIO_ENABLE)
	proc_audio();

	static u32 tick_bo;
	if (REG_ADDR8(0x125) & BIT(0))
	{
		tick_bo = clock_time ();
	}
	else if (clock_time_exceed (tick_bo, 200000))
	{
		REG_ADDR8(0x125) = BIT(0);
	}
#endif


//	host_pair_unpair_proc();


#if(BLE_MASTER_OTA_ENABLE)
	proc_ota();
#endif

#if 1
	//proc master update
//	if(host_update_conn_param_req){
//		host_update_conn_proc();
//	}
#endif




	return 0;
}




void main_loop (void)
{

	main_idle_loop ();

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

	if (main_service)
	{
		main_service ();
		main_service = 0;
	}
}








