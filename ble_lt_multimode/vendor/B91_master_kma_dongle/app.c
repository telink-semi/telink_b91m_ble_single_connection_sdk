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
#include "app_buffer.h"

#include "blm_att.h"
#include "blm_pair.h"
#include "blm_host.h"
#include "blm_ota.h"


#define		MY_RF_POWER_INDEX					RF_POWER_INDEX_P2p79dBm


#define		BLE_DEVICE_ADDRESS_TYPE 			BLE_DEVICE_ADDRESS_PUBLIC

_attribute_data_retention_	own_addr_type_t 	app_own_address_type = OWN_ADDRESS_PUBLIC;

int main_idle_loop (void);

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

#if (APPLICATION_DONGLE)
	//set USB ID
	REG_ADDR8(0x1401f4) = 0x65;
	REG_ADDR16(0x1401fe) = 0x08d0;
	REG_ADDR8(0x1401f4) = 0x00;

	//////////////// config USB ISO IN/OUT interrupt /////////////////
	reg_usb_mask = BIT(7);			//audio in interrupt enable
	plic_interrupt_enable(IRQ11_USB_ENDPOINT);
	plic_set_priority(IRQ11_USB_ENDPOINT,1);
	reg_usb_ep6_buf_addr = 0x80;
	reg_usb_ep7_buf_addr = 0x60;
	reg_usb_ep_max_size = (256 >> 3);

	usb_dp_pullup_en (1);  //open USB enum
#endif


	//////////////////////////// BLE stack Initialization  Begin //////////////////////////////////
	u8  mac_public[6];
	u8  mac_random_static[6];

	//for 1M   Flash, flash_sector_mac_address equals to 0xFF000
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);


	//////////// Controller Initialization  Begin /////////////////////////
	blc_ll_initBasicMCU();                      //mandatory
	blc_ll_initStandby_module(mac_public);				//mandatory
	blc_ll_initScanning_module(); 			//scan module: 		 mandatory for BLE master,
	blc_ll_initInitiating_module();						//initiate module: 	 mandatory for BLE master,
	blc_ll_initConnection_module();				//connection module  mandatory for BLE slave/master
	blc_ll_initMasterRoleSingleConn_module();			//master module: 	 mandatory for BLE master,

	rf_set_power_level_index (MY_RF_POWER_INDEX);

	blc_ll_initTxFifo(app_ll_txfifo, LL_TX_FIFO_SIZE, LL_TX_FIFO_NUM);
	blc_ll_initRxFifo(app_ll_rxfifo, LL_RX_FIFO_SIZE, LL_RX_FIFO_NUM);
	//////////// Controller Initialization  End /////////////////////////

	//////////// Host Initialization  Begin /////////////////////////
	/* Host Initialization */

	/* GAP initialization must be done before any other host feature initialization !!! */
	blc_gap_central_init();										//gap initialization

	/* L2CAP Initialization */
	blc_l2cap_register_handler (app_l2cap_handler);  	//l2cap initialization
	blc_hci_registerControllerEventHandler(controller_event_callback); //controller hci event to host all processed in this func

	/* bluetooth event */
	blc_hci_setEventMask_cmd (HCI_EVT_MASK_DISCONNECTION_COMPLETE | HCI_EVT_MASK_ENCRYPTION_CHANGE);

	/* bluetooth low energy(LE) event */
	blc_hci_le_setEventMask_cmd(		HCI_LE_EVT_MASK_CONNECTION_COMPLETE 		\
									|	HCI_LE_EVT_MASK_ADVERTISING_REPORT 			\
									|   HCI_LE_EVT_MASK_CONNECTION_UPDATE_COMPLETE  \
									|	HCI_LE_EVT_MASK_PHY_UPDATE_COMPLETE			\
									|   HCI_LE_EVT_MASK_CONNECTION_ESTABLISH );         //connection establish: telink private event

	/* SMP Initialization may involve flash write/erase(when one sector stores too much information,
	 *   is about to exceed the sector threshold, this sector must be erased, and all useful information
	 *   should re_stored) , so it must be done after battery check */
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


	extern int host_att_register_idle_func (void *p);
	host_att_register_idle_func (main_idle_loop);
	//////////// Host Initialization  End /////////////////////////

	//////////////////////////// BLE stack Initialization  End //////////////////////////////////

	////////////////// config scan parameter /////////////////////
#if 1
	blc_ll_setScanParameter(SCAN_TYPE_PASSIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,	\
							OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
#else
	blc_ll_setScanParameter(SCAN_TYPE_ACTIVE, SCAN_INTERVAL_100MS, SCAN_INTERVAL_100MS,	\
							OWN_ADDRESS_PUBLIC, SCAN_FP_ALLOW_ADV_ANY);
#endif

	////////////////// scan enable /////////////////////
	blc_ll_setScanEnable (BLC_SCAN_ENABLE, DUP_FILTER_DISABLE);

	#if (UI_UPPER_COMPUTER_ENABLE)
		extern void app_upper_com_init(void);
		app_upper_com_init();
	#endif


}

/**
 * @brief		This is main_idle_loop function
 * @param[in]	none
 * @return      none
 */
int main_idle_loop (void)
{


	////////////////////////////////////// BLE entry /////////////////////////////////
	blt_sdk_main_loop();


	///////////////////////////////////// proc usb cmd from host /////////////////////
#if (APPLICATION_DONGLE)
	usb_handle_irq();
#endif



	/////////////////////////////////////// HCI ///////////////////////////////////////
	blc_hci_proc ();



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


	host_pair_unpair_proc();


#if(BLE_MASTER_OTA_ENABLE)
	proc_ota();
#endif

#if 1
	//proc master update
	if(host_update_conn_param_req){
		host_update_conn_proc();
	}
#endif

	return 0;
}




void main_loop (void)
{

	main_idle_loop ();

	if (main_service)
	{
		main_service ();
		main_service = 0;
	}
}








