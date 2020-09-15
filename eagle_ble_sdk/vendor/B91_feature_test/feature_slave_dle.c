/********************************************************************************************************
 * @file	feature_slave_dle.c
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
#include "vendor/common/blt_led.h"
#include "vendor/common/blt_common.h"
#include "app_config.h"
#include "app.h"
#include "app_buffer.h"




#if (FEATURE_TEST_MODE == TEST_SDATA_LENGTH_EXTENSION)



#define FEATURE_PM_ENABLE								0
#define FEATURE_PM_NO_SUSPEND_ENABLE					0
#define FEATURE_DEEPSLEEP_RETENTION_ENABLE				0




_attribute_data_retention_	u32 connect_event_occurTick = 0;
_attribute_data_retention_  u32 mtuExchange_check_tick = 0;

_attribute_data_retention_ 	int  dle_started_flg = 0;

_attribute_data_retention_ 	int  mtuExchange_started_flg = 0;


_attribute_data_retention_	u16  final_MTU_size = 23;


#define TEST_DATA_LEN		255
_attribute_data_retention_	u8	app_test_data[TEST_DATA_LEN];
_attribute_data_retention_	u32 app_test_data_tick = 0;

/**
 * @brief      write callback of Attribute of TelinkSppDataClient2ServerUUID
 * @param[in]  p - rf_packet_att_write_t
 * @return     0
 */
int module_onReceiveData(rf_packet_att_write_t *p)
{
	u8 len = p->l2capLen - 3;
	if(len > 0)
	{

#if 1
		blc_gatt_pushHandleValueNotify(BLS_CONN_HANDLE, 0x11, &p->value, len);  //this API can auto handle MTU size
#else
		if( len + 3 <= final_MTU_size){   //opcode: 1 byte; attHandle: 2 bytes
			bls_att_pushNotifyData(0x11, &p->value, len);
		}
		else{
			//can not send this packet, cause MTU size exceed
		}
#endif
	}

	return 0;
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
	connect_event_occurTick = clock_time()|1;

	bls_l2cap_requestConnParamUpdate (8, 8, 19, 200);  //interval=10ms latency=19 timeout=2s
	bls_l2cap_setMinimalUpdateReqSendingTime_after_connCreate(1000);

	//MTU size reset to default 23 bytes every new connection, it can be only updated by MTU size exchange procedure
	final_MTU_size = 23;
}

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_TERMINATE"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
void	task_terminate (u8 e, u8 *p, int n)
{
	connect_event_occurTick = 0;
	mtuExchange_check_tick = 0;

	//MTU size exchange and data length exchange procedure must be executed on every new connection,
	//so when connection terminate, relative flags must be cleared
	dle_started_flg = 0;
	mtuExchange_started_flg = 0;

	//MTU size reset to default 23 bytes when connection terminated
	final_MTU_size = 23;
}

void	task_dle_exchange (u8 e, u8 *p, int n)
{
	ll_data_extension_t* dle_param = (ll_data_extension_t*)p;

	app_test_data_tick = clock_time() | 1;
	dle_started_flg = 1;
}


#define		MY_RF_POWER_INDEX					RF_POWER_INDEX_P2p79dBm

/**
 * @brief      callback function of LinkLayer Event "BLT_EV_FLAG_SUSPEND_EXIT"
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */
_attribute_ram_code_ void  func_suspend_exit (u8 e, u8 *p, int n)
{
	rf_set_power_level_index (MY_RF_POWER_INDEX);
}

/**
 * @brief      callback function of Host Event
 * @param[in]  h - Host Event type
 * @param[in]  para - data pointer of event
 * @param[in]  n - data length of event
 * @return     0
 */
int app_host_event_callback (u32 h, u8 *para, int n)
{

	u8 event = h & 0xFF;

	switch(event)
	{
		case GAP_EVT_SMP_PARING_BEAGIN:
		{

		}
		break;

		case GAP_EVT_SMP_PARING_SUCCESS:
		{
			gap_smp_paringSuccessEvt_t* p = (gap_smp_paringSuccessEvt_t*)para;

			if(p->bonding_result){
				/*save smp key succ*/
			}
			else{
				/*save smp key failed*/
			}
		}
		break;

		case GAP_EVT_SMP_PARING_FAIL:
		{
			gap_smp_paringFailEvt_t* p = (gap_smp_paringFailEvt_t*)para;
		}
		break;

		case GAP_EVT_SMP_CONN_ENCRYPTION_DONE:
		{
			gap_smp_connEncDoneEvt_t* p = (gap_smp_connEncDoneEvt_t*)para;

			if(p->re_connect == SMP_STANDARD_PAIR){  //first paring

			}
			else if(p->re_connect == SMP_FAST_CONNECT){  //auto connect

			}
		}
		break;

		case GAP_EVT_ATT_EXCHANGE_MTU:
		{
			gap_gatt_mtuSizeExchangeEvt_t *pEvt = (gap_gatt_mtuSizeExchangeEvt_t *)para;
			final_MTU_size = pEvt->effective_MTU;
			mtuExchange_started_flg = 1;   //set MTU size exchange flag here
		}
		break;


		default:
		break;
	}

	return 0;
}

/**
 * @brief		user initialization for slave DLE test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_sdle_test_init_normal(void)
{

	//random number generator must be initiated here( in the beginning of user_init_nromal)
	//when deepSleep retention wakeUp, no need initialize again
	random_generator_init();  //this is must



	u8  mac_public[6];
	u8  mac_random_static[6];
	//for 1M  Flash, flash_sector_mac_address equals to 0xFF000
	blc_initMacAddress(flash_sector_mac_address, mac_public, mac_random_static);

	rf_set_power_level_index (MY_RF_POWER_INDEX);


	blc_ll_initTxFifo(app_ll_txfifo, LL_TX_FIFO_SIZE, LL_TX_FIFO_NUM);
	blc_ll_initRxFifo(app_ll_rxfifo, LL_RX_FIFO_SIZE, LL_RX_FIFO_NUM);


	////// Controller Initialization  //////////
	blc_ll_initBasicMCU();   //mandatory
	blc_ll_initStandby_module(mac_public);				//mandatory
	blc_ll_initAdvertising_module(); 	//adv module: 		 mandatory for BLE slave,
	blc_ll_initConnection_module();				//connection module  mandatory for BLE slave/master
	blc_ll_initSlaveRole_module();				//slave module: 	 mandatory for BLE slave,

	////// Host Initialization  //////////
	blc_gap_peripheral_init();    //gap initialization
	extern void my_att_init ();
	my_att_init(); 		  //GATT initialization

	//ATT initialization
	//If not set RX MTU size, default is: 23 bytes.  In this situation, if master send MtuSize Request before slave send MTU size request,
	//slave will response default RX MTU size 23 bytes, then master will not send long packet on host l2cap layer, link layer data length
	//extension feature can not be used.  So in data length extension application, RX MTU size must be enlarged when initialization.
	blc_att_setRxMtuSize(MTU_SIZE_SETTING);

	blc_l2cap_register_handler (blc_l2cap_packet_receive);  	//l2cap initialization


	blc_smp_param_setBondingDeviceMaxNumber(4);    //if not set, default is : SMP_BONDING_DEVICE_MAX_NUM

	//Smp Initialization may involve flash write/erase(when one sector stores too much information,
	//   is about to exceed the sector threshold, this sector must be erased, and all useful information
	//   should re_stored) , so it must be done after battery check
	//Notice:if user set smp parameters: it should be called after usr smp settings
	blc_smp_peripheral_init();

	blc_smp_configSecurityRequestSending(SecReq_IMM_SEND, SecReq_PEND_SEND, 1000);  //if not set, default is:  send "security request" immediately after link layer connection established(regardless of new connection or reconnection )

	//host(GAP/SMP/GATT/ATT) event process: register host event callback and set event mask
	blc_gap_registerHostEventHandler( app_host_event_callback );
	blc_gap_setEventMask( GAP_EVT_MASK_SMP_PARING_BEAGIN 			|  \
						  GAP_EVT_MASK_SMP_PARING_SUCCESS   		|  \
						  GAP_EVT_MASK_SMP_PARING_FAIL				|  \
						  GAP_EVT_MASK_SMP_CONN_ENCRYPTION_DONE 	|  \
						  GAP_EVT_MASK_ATT_EXCHANGE_MTU);


///////////////////// USER application initialization ///////////////////
	/**
	 * @brief	Adv Packet data
	 */
	u8 tbl_advData[] = {
		 0x08, 0x09, 't', 'e', 's', 't', 'D', 'L', 'E',
		};

	/**
	 * @brief	Scan Response Packet data
	 */
	u8	tbl_scanRsp [] = {
			0x08, 0x09, 't', 'e', 's', 't', 'D', 'L', 'E',
		};
	bls_ll_setAdvData( (u8 *)tbl_advData, sizeof(tbl_advData) );
	bls_ll_setScanRspData( (u8 *)tbl_scanRsp, sizeof(tbl_scanRsp));

	bls_ll_setAdvParam( ADV_INTERVAL_30MS, ADV_INTERVAL_40MS,
						ADV_TYPE_CONNECTABLE_UNDIRECTED, OWN_ADDRESS_PUBLIC,
						0,  NULL,  BLT_ENABLE_ADV_ALL, ADV_FP_NONE);

	bls_ll_setAdvEnable(1);  //adv enable

	//ble event call back
	bls_app_registerEventCallback (BLT_EV_FLAG_CONNECT, &task_connect);
	bls_app_registerEventCallback (BLT_EV_FLAG_TERMINATE, &task_terminate);
	bls_app_registerEventCallback (BLT_EV_FLAG_DATA_LENGTH_EXCHANGE, &task_dle_exchange);


#if(FEATURE_PM_ENABLE)
	blc_ll_initPowerManagement_module();

	#if (FEATURE_PM_NO_SUSPEND_ENABLE)
		bls_pm_setSuspendMask ( DEEPSLEEP_RETENTION_ADV | DEEPSLEEP_RETENTION_CONN);
		blc_pm_setDeepsleepRetentionThreshold(3, 3);
		blc_pm_setDeepsleepRetentionEarlyWakeupTiming(350);
	#elif (FEATURE_DEEPSLEEP_RETENTION_ENABLE)
		bls_pm_setSuspendMask (SUSPEND_ADV | DEEPSLEEP_RETENTION_ADV | SUSPEND_CONN | DEEPSLEEP_RETENTION_CONN);
		blc_pm_setDeepsleepRetentionThreshold(50, 50);
		blc_pm_setDeepsleepRetentionEarlyWakeupTiming(200);
	#else
		bls_pm_setSuspendMask (SUSPEND_ADV | SUSPEND_CONN);
	#endif

	bls_app_registerEventCallback (BLT_EV_FLAG_SUSPEND_EXIT, &func_suspend_exit);
#else
	bls_pm_setSuspendMask (SUSPEND_DISABLE);
#endif


}


/**
 * @brief		user initialization for slave DLE test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void feature_sdle_test_init_deepRetn(void)
{
#if (FEATURE_DEEPSLEEP_RETENTION_ENABLE)
	blc_ll_initBasicMCU();   //mandatory
	rf_set_power_level_index (MY_RF_POWER_INDEX);

	blc_ll_recoverDeepRetention();

	irq_enable();

	DBG_CHN0_HIGH;    //debug
#endif
}


/**
 * @brief		This is main_loop function in slave DLE project
 * @param[in]	none
 * @return      none
 */
void feature_sdle_test_mainloop(void)
{
	if(connect_event_occurTick && clock_time_exceed(connect_event_occurTick, 1500000)){  //1.5 S after connection established
		connect_event_occurTick = 0;

		mtuExchange_check_tick = clock_time() | 1;
		if(!mtuExchange_started_flg){  //master do not send MTU exchange request in time
			blc_att_requestMtuSizeExchange(BLS_CONN_HANDLE, MTU_SIZE_SETTING);
			/*After conn 1.5s, S send  MTU size req to the Master.*/
		}
	}


	if(mtuExchange_check_tick && clock_time_exceed(mtuExchange_check_tick, 500000 )){  //2 S after connection established
		mtuExchange_check_tick = 0;

		if(!dle_started_flg){ //master do not send data length request in time
			/*Master hasn't initiated the DLE yet, S send DLE req to the Master.*/
			blc_ll_exchangeDataLength(LL_LENGTH_REQ , DLE_TX_SUPPORTED_DATA_LEN);
		}
	}


	if(dle_started_flg && clock_time_exceed(app_test_data_tick, 3330000)){
		if(BLE_SUCCESS == blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, SPP_SERVER_TO_CLIENT_DP_H, &app_test_data[0], MTU_SIZE_SETTING-3))
		{
			app_test_data_tick = clock_time() | 1;
			app_test_data[0]++;
		}
	}
}


#endif