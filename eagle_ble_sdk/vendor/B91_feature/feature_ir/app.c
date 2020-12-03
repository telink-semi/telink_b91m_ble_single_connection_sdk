/********************************************************************************************************
 * @file	app.c
 *
 * @brief	This is the source file for BLE SDK
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
#include "app_config.h"
#include "app.h"
#include "rc_ir.h"
#include "rc_ir_learn.h"


#if (FEATURE_TEST_MODE == TEST_IR)

/**
 * @brief		this function is used to process keyboard matrix status change.
 * @param[in]	none
 * @return      none
 */
_attribute_data_retention_	u32	latest_user_event_tick;
_attribute_data_retention_	int 		key_not_released;

ir_learn_send_t ir_learn_result;
void key_change_proc(void)
{

	latest_user_event_tick = clock_time();  //record latest key change time

	u8 key0 = kb_event.keycode[0];

	key_not_released = 1;
	if (kb_event.cnt == 2)   //two key press, do  not process
	{

	}
#if IR_LEARN_ENABLE
	else if(kb_event.cnt == 1)
	{
		switch(key0)
		{
			case CR_VOL_UP:
				gpio_toggle(GPIO_LED_WHITE);
				ir_learn_init();
				ir_learn_start();
				break;

			case CR_VOL_DN:
				gpio_toggle(GPIO_LED_RED);
				ir_learn_init();
				ir_learn_start();
				break;

			case VK_1:
				gpio_toggle(GPIO_LED_BLUE);
				if(get_ir_learn_state() == 0) // success
				{
					ir_learn_copy_result(&ir_learn_result);
					ir_learn_send_init();
					ir_learn_send(&ir_learn_result);
				}
				break;

			case VK_2:
				gpio_toggle(GPIO_LED_GREEN);
				if(get_ir_learn_state() == 0)  // success
				{
					ir_learn_copy_result(&ir_learn_result);
					ir_learn_send_init();
					ir_learn_send(&ir_learn_result);
				}
				break;

			default:
				break;
		}

	}
#else
	else if(kb_event.cnt == 1)
	{
		DBG_CHN0_TOGGLE;
		switch(key0)
		{
			case CR_VOL_UP:
				gpio_toggle(GPIO_LED_WHITE);
				ir_nec_send(0,1,0);
				break;

			case CR_VOL_DN:
				gpio_toggle(GPIO_LED_RED);
				ir_nec_send(0,1,1);
				break;

			case VK_1:
				gpio_toggle(GPIO_LED_BLUE);
				ir_nec_send(0,1,2);
				break;

			case VK_2:
				gpio_toggle(GPIO_LED_GREEN);
				ir_nec_send(0,1,3);
				break;

			default:
				break;
		}

	}
#endif
	else   //kb_event.cnt == 0,  key release
	{
		gpio_write(GPIO_LED_WHITE,0);
		gpio_write(GPIO_LED_GREEN,0);
		gpio_write(GPIO_LED_RED,0);
		gpio_write(GPIO_LED_BLUE,0);
		key_not_released = 0;
	    ir_send_release();
	}
}


/**
 * @brief      this function is used to detect if key pressed or released.
 * @param[in]  e - LinkLayer Event type
 * @param[in]  p - data pointer of event
 * @param[in]  n - data length of event
 * @return     none
 */


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
#if Debug_IR
	ir_learn_detect();
#endif
}


/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void user_init_deepRetn(void)
{
}

/**
 * @brief		This function serves to User Init
 * @return 		none
 */

void user_init_normal(void)
{
/////////////////////////////////////// ir init ////////////////////////////////////
#if	IR_LEARN_ENABLE
	ir_learn_init();
	ir_learn_start();
#else
	rc_ir_init();
#endif
}

/**
 * @brief		This function serves to main
 * @return 		none
 */
void main_loop(void)
{
////////////////////////////////////// UI entry /////////////////////////////////
	proc_keyboard (0, 0, 0);
}


#endif  //end of (FEATURE_TEST_MODE == ...)
