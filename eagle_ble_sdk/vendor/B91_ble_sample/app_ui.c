/********************************************************************************************************
 * @file	app_ui.c
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
#include "application/keyboard/keyboard.h"
#include "application/usbstd/usbkeycode.h"
#include "app_att.h"
#include "app_ui.h"

#if (UI_KEYBOARD_ENABLE)



_attribute_data_retention_	int 	key_not_released;

extern u32	latest_user_event_tick;

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


extern u32	scan_pin_need;



#elif (UI_BUTTON_ENABLE)



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
	/**
	 * @brief 	record the result of key detect
	 */
	typedef	struct{
		u8 	cnt;				//count button num
		u8 	btn_press;
		u8 	keycode[MAX_BTN_SIZE];			//6 btn
	}vc_data_t;
	_attribute_data_retention_	vc_data_t vc_event;

	/**
	 * @brief 	record the status of button process
	 */
	typedef struct{
		u8  btn_history[4];		//vc history btn save
		u8  btn_filter_last;
		u8	btn_not_release;
		u8 	btn_new;					//new btn  flag
	}btn_status_t;
	_attribute_data_retention_	btn_status_t 	btn_status;

	/**
	 * @brief      Debounce processing during button detection
	 * @param[in]  btn_v - vc_event.btn_press
	 * @return     1:Detect new button;0:Button isn't changed
	 */
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

	/**
	 * @brief      This function is key detection processing
	 * @param[in]  read_key - Decide whether to return the key detection result
	 * @return     1:Detect new button;0:Button isn't changed
	 */
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


	/**
	 * @brief		this function is used to detect if button pressed or released.
	 * @param[in]	e - event type when this function is triggered by LinkLayer event
	 * @param[in]	p - event callback data pointer for when this function is triggered by LinkLayer event
	 * @param[in]	n - event callback data length when this function is triggered by LinkLayer event
	 * @return      none
	 */
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