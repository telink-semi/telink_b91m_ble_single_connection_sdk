/********************************************************************************************************
 * @file	 button.c
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
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
#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"
#include "app_att.h"



_attribute_data_retention_	int     ui_mtu_size_exchange_req = 0;
_attribute_data_retention_	int 	key_not_released;

extern u32	latest_user_event_tick;

#define CONSUMER_KEY   	   		1
#define KEYBOARD_KEY   	   		2
_attribute_data_retention_	u8 		key_type;





ble_sts_t  app_debug_pushNotifyData (u16 attHandle, u8 *p, int len)
{

	DBG_CHN4_TOGGLE;

	int n;
							 //llid rfLen   l2capLen     cid    opcode   handle      data[0..19]   mic[0..3]
	u8 pkt_notify_short[36] = {0x02,0x09, 0x05,0x00,  0x04,0x00, 0x1b,  0x0e,0x00,  0x00, 0x00, 0x00, 0x00};    //mic[0..3] on  blt_txfifo


	n = len < 20 ? len : 20;
	pkt_notify_short[0] = 2;				//first data packet
	pkt_notify_short[1] = n + 7;

	*(u16*)(pkt_notify_short + 2) = len + 3;	//l2cap
	*(u16*)(pkt_notify_short + 4) = 0x04;	//chanid

	pkt_notify_short[6] = ATT_OP_HANDLE_VALUE_NOTI;
	pkt_notify_short[7] = U16_LO(attHandle);
	pkt_notify_short[8] = U16_HI(attHandle);

	tmemcpy (pkt_notify_short + 9, p, n);


	bls_ll_pushTxFifo (BLS_CONN_HANDLE, pkt_notify_short);


	return BLE_SUCCESS;
}

#if (BLE_AUDIO_ENABLE)
#include "tl_audio.h"

#define AUDIO_DEBUG			1

#if (AUDIO_DEBUG)

	dma_llp_config_t rx_dma_list_config[2];

	dma_llp_config_t tx_dma_list_config[2];
	extern u8 audio_tx_dma_chn;
	extern u8 audio_rx_dma_chn;
	 void audio_tx_dma_dis()
	 {
	 	dma_chn_dis(audio_tx_dma_chn);
	 }

	 void audio_rx_dma_dis()
	 {
	 	dma_chn_dis(audio_rx_dma_chn);
	 }

	volatile u8 amic_enable;
	void audio_amic_init(void)
	{
		plic_interrupt_enable(IRQ20_DFIFO);
		audio_mux_config(CODEC_I2S,BIT_16_MONO,BIT_16_MONO,BIT_16_MONO_FIFO0);
		audio_i2s_config(I2S_I2S_MODE,I2S_BIT_16_DATA,I2C_RX_TX_SLAVE);
		audio_set_i2s_clock(AUDIO_44EP1K,AUDIO_RATE_EQUAL);
		audio_clk_en(1,1);
		audio_codec_dac_config(CODEC_MASTER,MCU_WREG,AUDIO_16K);
		audio_codec_adc_config(CODEC_MASTER,AMIC_IN,MCU_WREG,AUDIO_16K);
		audio_data_path_sel(I2S_DATA_IN_FIFO,I2S_OUT,NO_USE,NO_USE);
		audio_rx_dma_config(DMA2,(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE, &rx_dma_list_config[0]);
		audio_rx_dma_add_list_element(&rx_dma_list_config[0],&rx_dma_list_config[1],(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE);
		audio_rx_dma_add_list_element(&rx_dma_list_config[1],&rx_dma_list_config[0],(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE);
		audio_rx_dma_en();
#if 0
		audio_tx_dma_config(DMA3,(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE,&tx_dma_list_config[0]);
		audio_tx_dma_add_list_element(&tx_dma_list_config[0],&tx_dma_list_config[1],(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE);
		audio_tx_dma_add_list_element(&tx_dma_list_config[1],&tx_dma_list_config[0],(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE);
		audio_tx_dma_en();
#endif
	}
	void audio_dmic_init()
	{
		plic_interrupt_enable(IRQ20_DFIFO);
	    audio_dmic_set_pin(DMIC_GROUPB_B2_DAT_B3_B4_CLK);
		audio_mux_config(CODEC_I2S,BIT_16_MONO,BIT_16_MONO,BIT_16_MONO_FIFO0);
		audio_i2s_config(I2S_I2S_MODE,I2S_BIT_16_DATA,I2C_RX_TX_SLAVE);
		audio_set_i2s_clock(AUDIO_44EP1K,AUDIO_RATE_EQUAL);
		audio_clk_en(1,1);
		audio_codec_dac_config(CODEC_MASTER,MCU_WREG,AUDIO_16K);
		audio_codec_adc_config(CODEC_MASTER,DMIC_IN,MCU_WREG,AUDIO_16K);
		audio_data_path_sel(I2S_DATA_IN_FIFO,I2S_OUT,NO_USE,NO_USE);
		audio_rx_dma_config(DMA2,(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE, &rx_dma_list_config[0]);
		audio_rx_dma_add_list_element(&rx_dma_list_config[0],&rx_dma_list_config[1],(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE);
		audio_rx_dma_add_list_element(&rx_dma_list_config[1],&rx_dma_list_config[0],(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE);
		audio_rx_dma_en();
#if 0
		audio_tx_dma_config(DMA3,(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE,&tx_dma_list_config[0]);
		audio_tx_dma_add_list_element(&tx_dma_list_config[0],&tx_dma_list_config[1],(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE);
		audio_tx_dma_add_list_element(&tx_dma_list_config[1],&tx_dma_list_config[0],(u16*)(buffer_mic),TL_MIC_BUFFER_SIZE);
		audio_tx_dma_en();
#endif
	}
	void audio_mic_off()//
	{
		audio_clk_en(0,0);
		audio_rx_dma_dis();
		audio_tx_dma_dis();
	}
#endif

	_attribute_data_retention_	u8		ui_mic_enable = 0;

	_attribute_data_retention_	u8 		key_voice_press = 0;

	u32 	key_voice_pressTick = 0;

	void ui_enable_mic (int en)
	{
		ui_mic_enable = en;

		#if (BLT_APP_LED_ENABLE)
			device_led_setup(led_cfg[en ? LED_AUDIO_ON : LED_AUDIO_OFF]);
		#endif
		gpio_write(GPIO_LED_RED,en);
		if(en){  //audio on
			///////////////////// AUDIO initialization///////////////////
			#if (BLE_DMIC_ENABLE)  //Dmic config
				audio_dmic_init();
			#else  //Amic config
				audio_amic_init();
			#endif
		}
		else{  //audio off
			#if (BLE_DMIC_ENABLE)  //Dmic config
			audio_mic_off();
			#else  //audio off
				audio_mic_off();
			#endif
		}

		#if (BATT_CHECK_ENABLE)
			battery_set_detect_enable(!en);
		#endif
	}


	void voice_press_proc(void)
	{
		key_voice_press = 0;
		ui_enable_mic (1);
#if 1
		if(ui_mtu_size_exchange_req && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN){
			ui_mtu_size_exchange_req = 0;
			blc_att_requestMtuSizeExchange(BLS_CONN_HANDLE, 0x009e);
		}
#endif
	}


	void task_audio (void)
	{
		static u32 audioProcTick = 0;
		if(clock_time_exceed(audioProcTick, 500)){
			audioProcTick = clock_time();
		}
		else{
			return;
		}

		///////////////////////////////////////////////////////////////
//		log_event(TR_T_audioTask);
#if  0//
		proc_mic_encoder ();

		int *p = mic_encoder_data_buffer ();
		if(p)
		{
			mic_encoder_data_read_ok();
		}
		//////////////////////////////////////////////////////////////////
#else
		proc_mic_encoder ();

		if (blc_ll_getTxFifoNumber() < 9)
		{
			int *p = mic_encoder_data_buffer ();
			if (p)					//around 3.2 ms @16MHz clock
			{
				if( BLE_SUCCESS == bls_att_pushNotifyData (AUDIO_MIC_INPUT_DP_H, (u8*)p, ADPCM_PACKET_LEN) ){
					mic_encoder_data_read_ok();
					DBG_CHN5_TOGGLE;
				}
			}
		}
#endif
	}



	void blc_checkConnParamUpdate(void)
	{
		extern u32 interval_update_tick;

		if(	 interval_update_tick && clock_time_exceed(interval_update_tick,5*1000*1000) && \
			 blc_ll_getCurrentState() == BLS_LINK_STATE_CONN &&  bls_ll_getConnectionInterval()!= 8 )
		{
			interval_update_tick = clock_time() | 1;
			bls_l2cap_requestConnParamUpdate (8, 8, 99, 400);
		}
	}

#endif





void key_change_proc(void)
{

	latest_user_event_tick = clock_time();  //record latest key change time


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

			app_debug_pushNotifyData(HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
			//blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
		}
		else
		{
			key_type = KEYBOARD_KEY;
			key_buf[2] = key0;
			if(key0 == VK_1)
			{
			}
			else if(key0 == VOICE)
			{
#if (BLE_AUDIO_ENABLE)
				if(ui_mic_enable){  //if voice on, voice off
					ui_enable_mic (0);
				}
				else{ //if voice not on, mark voice key press tick
					key_voice_press = 1;
					key_voice_pressTick = clock_time();
				}
#endif
			}

			app_debug_pushNotifyData(HID_NORMAL_KB_REPORT_INPUT_DP_H, key_buf, 8);
			//blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_NORMAL_KB_REPORT_INPUT_DP_H, key_buf, 8);
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

			app_debug_pushNotifyData(HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
			//blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
		}
		else if(key_type == KEYBOARD_KEY)
		{
			key_buf[2] = 0;
			app_debug_pushNotifyData(HID_NORMAL_KB_REPORT_INPUT_DP_H, key_buf, 8);
			//blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_NORMAL_KB_REPORT_INPUT_DP_H, key_buf, 8); //release
		}
	}


}



_attribute_data_retention_		static u32 keyScanTick = 0;

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

#if (BLE_AUDIO_ENABLE)
	 //long press voice 1 second
//	if(key_voice_press && !ui_mic_enable && blc_ll_getCurrentState() == BLS_LINK_STATE_CONN && \
//		clock_time_exceed(key_voice_pressTick, 1000000)){
//		voice_press_proc();
//	}
	if(key_voice_press && !ui_mic_enable &&clock_time_exceed(key_voice_pressTick, 1000000))
	{
		voice_press_proc();
	}
#endif
}


extern u32	scan_pin_need;



#if (UI_BUTTON_ENABLE)



/////////////////////////////////////////////////////////////////////
	#define MAX_BTN_SIZE			2
	#define BTN_VALID_LEVEL			0

	#define USER_BTN_1				0x01
	#define USER_BTN_2				0x02

	_attribute_data_retention_	u32 ctrl_btn[] = {SW1_GPIO, SW2_GPIO};
	_attribute_data_retention_	u8 btn_map[MAX_BTN_SIZE] = {USER_BTN_1, USER_BTN_2};



	_attribute_data_retention_	int button_not_released = 0;

	_attribute_data_retention_	static int button0_press_flag;
	_attribute_data_retention_	static u32 button0_press_tick;
	_attribute_data_retention_	static int button1_press_flag;
	_attribute_data_retention_	static u32 button1_press_tick;

	_attribute_data_retention_	static int consumer_report = 0;

	typedef	struct{
		u8 	cnt;				//count button num
		u8 	btn_press;
		u8 	keycode[MAX_BTN_SIZE];			//6 btn
	}vc_data_t;
	_attribute_data_retention_	vc_data_t vc_event;

	typedef struct{
		u8  btn_history[4];		//vc history btn save
		u8  btn_filter_last;
		u8	btn_not_release;
		u8 	btn_new;					//new btn  flag
	}btn_status_t;
	_attribute_data_retention_	btn_status_t 	btn_status;


	u8 btn_debounce_filter(u8 *btn_v)
	{
		u8 change = 0;

		for(int i=3; i>0; i--){
			btn_status.btn_history[i] = btn_status.btn_history[i-1];
		}
		btn_status.btn_history[0] = *btn_v;

		if(  btn_status.btn_history[0] == btn_status.btn_history[1] && btn_status.btn_history[1] == btn_status.btn_history[2] && \
			btn_status.btn_history[0] != btn_status.btn_filter_last ){
			change = 1;

			btn_status.btn_filter_last = btn_status.btn_history[0];
		}

		return change;
	}

	u8 vc_detect_button(int read_key)
	{
		u8 btn_changed, i;
		tmemset(&vc_event,0,sizeof(vc_data_t));			//clear vc_event
		//vc_event.btn_press = 0;

		for(i=0; i<MAX_BTN_SIZE; i++){
			if(BTN_VALID_LEVEL != !gpio_read(ctrl_btn[i])){
				vc_event.btn_press |= BIT(i);
			}
		}

		btn_changed = btn_debounce_filter(&vc_event.btn_press);


		if(btn_changed && read_key){
			for(i=0; i<MAX_BTN_SIZE; i++){
				if(vc_event.btn_press & BIT(i)){
					vc_event.keycode[vc_event.cnt++] = btn_map[i];
				}
			}

			return 1;
		}

		return 0;
	}

	void proc_button (u8 e, u8 *p, int n)
	{
		int det_key = vc_detect_button (1);

		if (det_key)  //key change: press or release
		{
			extern u32 latest_user_event_tick;
			latest_user_event_tick = clock_time();

			u8 key0 = vc_event.keycode[0];
//			u8 key1 = vc_event.keycode[1];

			button_not_released = 1;

			if(vc_event.cnt == 2)  //two key press
			{

			}
			else if(vc_event.cnt == 1) //one key press
			{
				if(key0 == USER_BTN_1)
				{
					button0_press_flag = 1;
					button0_press_tick = clock_time();

					//send "Vol+"
					u16 consumer_key = MKEY_VOL_UP;
					blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
					consumer_report = 1;

				}
				else if(key0 == USER_BTN_2)
				{
					button1_press_flag = 1;
					button1_press_tick = clock_time();

					//send "Vol-"
					u16 consumer_key = MKEY_VOL_DN;
					blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
					consumer_report = 1;
				}
			}
			else{  //release
				button_not_released = 0;

				button0_press_flag = 0;
				button1_press_flag = 0;

				//send release Vol+/Vol-
				if(consumer_report){
					consumer_report = 0;
					u16 consumer_key = 0;
					blc_gatt_pushHandleValueNotify (BLS_CONN_HANDLE, HID_CONSUME_REPORT_INPUT_DP_H, (u8 *)&consumer_key, 2);
				}
			}

		}


	}
#endif   //end of UI_BUTTON_ENABLE
