/********************************************************************************************************
 * @file	app_config.h
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
#pragma once


// Hardware board select
#define HW_C1T219A20_V1_0_64_EVB		1
#define HW_C1T217A20_V1_0_48_EVB		2
#define HW_C1T213A20_V1_0_80_EVB		3
#define HARDWARE_BOARD_SELECT			HW_C1T213A20_V1_0_80_EVB


#define FLASH_ADR_PARING   				0x78000


/////////////////// MODULE /////////////////////////////////
#define BLE_HOST_SMP_ENABLE							1  //1 for standard security management,  0 for telink referenced paring&bonding(no security)
#define BLE_HOST_SIMPLE_SDP_ENABLE					1  //1  //simple service discovery


#define BLE_MASTER_OTA_ENABLE						1  //slave ota test
#define AUDIO_SDM_ENBALE							0//if using sdm playback, should better disable USB MIC

#define UI_AUDIO_ENABLE								1
#define UI_BUTTON_ENABLE							1
#define UI_UPPER_COMPUTER_ENABLE					0  //work with upper computer

#define BLT_APP_LED_ENABLE							1



#define APPLICATION_DONGLE							1
#if(APPLICATION_DONGLE)
	#define PA5_FUNC				AS_USB_DM
	#define PA6_FUNC				AS_USB_DP
	#define PA5_INPUT_ENABLE		1
	#define PA6_INPUT_ENABLE		1

	#define	USB_PRINTER_ENABLE 		1
	#define	USB_SPEAKER_ENABLE 		0

	#define AUDIO_HOGP				0

	#define	USB_MIC_ENABLE 			1
	#define	USB_MOUSE_ENABLE 		1
	#define	USB_KEYBOARD_ENABLE 	1
	#define	USB_SOMATIC_ENABLE      0   //  when USB_SOMATIC_ENABLE, USB_EDP_PRINTER_OUT disable
	#define USB_CUSTOM_HID_REPORT	1
#endif

#define ID_VENDOR				0x248a			// for report
#define ID_PRODUCT_BASE			0x880c			//AUDIO_HOGP
#define STRING_VENDOR			L"Telink"
#define STRING_PRODUCT			L"BLE Remote KMA Dongle"
#define STRING_SERIAL			L"TLSR9518"

//////////////////// Audio /////////////////////////////////////
#define MIC_RESOLUTION_BIT		16
#define MIC_SAMPLE_RATE			16000
#define MIC_CHANNLE_COUNT		1
#define	MIC_ENOCDER_ENABLE		0


#if (UI_AUDIO_ENABLE)
	#define BLE_DMIC_ENABLE					0  //0: Amic   1: Dmic
	/* Dongle Audio MODE:
	 * TL_AUDIO_DONGLE_ADPCM_GATT_TELINK
	 */
	#define TL_AUDIO_MODE  	     TL_AUDIO_DONGLE_ADPCM_GATT_TELINK
#endif




#if BLT_APP_LED_ENABLE
#define     UI_LED_ENABLE           1
#else
#define     UI_LED_ENABLE           1
#endif

#if UI_LED_ENABLE
	// LED GPIO
	#define GPIO_LED_BLUE		GPIO_PB4
	#define GPIO_LED_GREEN		GPIO_PB5
	#define GPIO_LED_WHITE		GPIO_PB6

	#define PB6_FUNC				AS_GPIO
	#define PB5_FUNC				AS_GPIO
	#define PB4_FUNC				AS_GPIO

	#define	PB6_OUTPUT_ENABLE		1
	#define PB5_OUTPUT_ENABLE		1
	#define	PB4_OUTPUT_ENABLE		1

	#if (HARDWARE_BOARD_SELECT == HW_C1T219A20_V1_0_64_EVB)
		#define GPIO_LED_RED		GPIO_PB7
		#define PB7_FUNC			AS_GPIO
		#define	PB7_OUTPUT_ENABLE		1
	#elif (HARDWARE_BOARD_SELECT == HW_C1T217A20_V1_0_48_EVB)
		#define GPIO_LED_RED		GPIO_PB2
		#define PB2_FUNC			AS_GPIO
		#define	PB2_OUTPUT_ENABLE		1
	#elif (HARDWARE_BOARD_SELECT == HW_C1T213A20_V1_0_80_EVB)
		#define GPIO_LED_RED		GPIO_PB7
		#define PB7_FUNC			AS_GPIO
		#define	PB7_OUTPUT_ENABLE		1
	#endif


	#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led

#endif

//----------------------- GPIO for UI --------------------------------
#if (UI_BUTTON_ENABLE)
	//---------------  Button ----------------------------------
	#define	SW1_GPIO				GPIO_PC6
	#define	SW2_GPIO				GPIO_PC7
	#define PC6_FUNC				AS_GPIO
	#define PC7_FUNC				AS_GPIO
	#define PC6_INPUT_ENABLE		1
	#define PC7_INPUT_ENABLE		1
	#define PULL_WAKEUP_SRC_PC6     PM_PIN_PULLUP_10K
	#define PULL_WAKEUP_SRC_PC7     PM_PIN_PULLUP_10K
#endif


/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  	32000000

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};



/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		0
#define WATCHDOG_INIT_TIMEOUT		500  //ms





#define	UDB_DEBUG_EN						0
#if (UDB_DEBUG_EN)
	#define PA5_FUNC						AS_USB_DM
	#define PA6_FUNC						AS_USB_DP
	#define PA5_INPUT_ENABLE				1
	#define PA6_INPUT_ENABLE				1

	#define BLE_ADV_DBG_EN					1
	#define BLE_IRQ_DBG_EN					1
	#define	UDB_STR_DATA_ENABLE				1
#endif



#define DEBUG_GPIO_ENABLE							0

#if(DEBUG_GPIO_ENABLE)


	#define GPIO_CHN0							GPIO_PE1
	#define GPIO_CHN1							GPIO_PE2
	#define GPIO_CHN2							GPIO_PA0
    #define GPIO_CHN3							GPIO_PA4
	#define GPIO_CHN4							GPIO_PA3
	#define GPIO_CHN5							GPIO_PB0
	#define GPIO_CHN6							GPIO_PB2
	#define GPIO_CHN7							GPIO_PE0

	#define GPIO_CHN8							GPIO_PA2
	#define GPIO_CHN9							GPIO_PA1
	#define GPIO_CHN10							GPIO_PB1
	#define GPIO_CHN11							GPIO_PB3


	#define PE1_OUTPUT_ENABLE					1
	#define PE2_OUTPUT_ENABLE					1
	#define PA0_OUTPUT_ENABLE					1
	#define PA4_OUTPUT_ENABLE					1
	#define PA3_OUTPUT_ENABLE					1
	#define PB0_OUTPUT_ENABLE					1
	#define PB2_OUTPUT_ENABLE					1
	#define PE0_OUTPUT_ENABLE					1

	#define PA2_OUTPUT_ENABLE					1
	#define PA1_OUTPUT_ENABLE					1
	#define PB1_OUTPUT_ENABLE					1
	#define PB3_OUTPUT_ENABLE					1


	#define DBG_CHN0_LOW		gpio_write(GPIO_CHN0, 0)
	#define DBG_CHN0_HIGH		gpio_write(GPIO_CHN0, 1)
	#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
	#define DBG_CHN1_LOW		gpio_write(GPIO_CHN1, 0)
	#define DBG_CHN1_HIGH		gpio_write(GPIO_CHN1, 1)
	#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
	#define DBG_CHN2_LOW		gpio_write(GPIO_CHN2, 0)
	#define DBG_CHN2_HIGH		gpio_write(GPIO_CHN2, 1)
	#define DBG_CHN2_TOGGLE		gpio_toggle(GPIO_CHN2)
	#define DBG_CHN3_LOW		gpio_write(GPIO_CHN3, 0)
	#define DBG_CHN3_HIGH		gpio_write(GPIO_CHN3, 1)
	#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
	#define DBG_CHN4_LOW		gpio_write(GPIO_CHN4, 0)
	#define DBG_CHN4_HIGH		gpio_write(GPIO_CHN4, 1)
	#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
	#define DBG_CHN5_LOW		gpio_write(GPIO_CHN5, 0)
	#define DBG_CHN5_HIGH		gpio_write(GPIO_CHN5, 1)
	#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
	#define DBG_CHN6_LOW		gpio_write(GPIO_CHN6, 0)
	#define DBG_CHN6_HIGH		gpio_write(GPIO_CHN6, 1)
	#define DBG_CHN6_TOGGLE		gpio_toggle(GPIO_CHN6)
	#define DBG_CHN7_LOW		gpio_write(GPIO_CHN7, 0)
	#define DBG_CHN7_HIGH		gpio_write(GPIO_CHN7, 1)
	#define DBG_CHN7_TOGGLE		gpio_toggle(GPIO_CHN7)
	#define DBG_CHN8_LOW		gpio_write(GPIO_CHN8, 0)
	#define DBG_CHN8_HIGH		gpio_write(GPIO_CHN8, 1)
	#define DBG_CHN8_TOGGLE		gpio_toggle(GPIO_CHN8)
	#define DBG_CHN9_LOW		gpio_write(GPIO_CHN9, 0)
	#define DBG_CHN9_HIGH		gpio_write(GPIO_CHN9, 1)
	#define DBG_CHN9_TOGGLE		gpio_toggle(GPIO_CHN9)
	#define DBG_CHN10_LOW		gpio_write(GPIO_CHN10, 0)
	#define DBG_CHN10_HIGH		gpio_write(GPIO_CHN10, 1)
	#define DBG_CHN10_TOGGLE	gpio_toggle(GPIO_CHN10)
	#define DBG_CHN11_LOW		gpio_write(GPIO_CHN11, 0)
	#define DBG_CHN11_HIGH		gpio_write(GPIO_CHN11, 1)
	#define DBG_CHN11_TOGGLE	gpio_toggle(GPIO_CHN11)
#else
	#define DBG_CHN0_LOW
	#define DBG_CHN0_HIGH
	#define DBG_CHN0_TOGGLE
	#define DBG_CHN1_LOW
	#define DBG_CHN1_HIGH
	#define DBG_CHN1_TOGGLE
	#define DBG_CHN2_LOW
	#define DBG_CHN2_HIGH
	#define DBG_CHN2_TOGGLE
	#define DBG_CHN3_LOW
	#define DBG_CHN3_HIGH
	#define DBG_CHN3_TOGGLE
	#define DBG_CHN4_LOW
	#define DBG_CHN4_HIGH
	#define DBG_CHN4_TOGGLE
	#define DBG_CHN5_LOW
	#define DBG_CHN5_HIGH
	#define DBG_CHN5_TOGGLE
	#define DBG_CHN6_LOW
	#define DBG_CHN6_HIGH
	#define DBG_CHN6_TOGGLE
	#define DBG_CHN7_LOW
	#define DBG_CHN7_HIGH
	#define DBG_CHN7_TOGGLE
	#define DBG_CHN8_LOW
	#define DBG_CHN8_HIGH
	#define DBG_CHN8_TOGGLE
	#define DBG_CHN9_LOW
	#define DBG_CHN9_HIGH
	#define DBG_CHN9_TOGGLE
	#define DBG_CHN10_LOW
	#define DBG_CHN10_HIGH
	#define DBG_CHN10_TOGGLE
	#define DBG_CHN11_LOW
	#define DBG_CHN11_HIGH
	#define DBG_CHN11_TOGGLE
#endif  //end of DEBUG_GPIO_ENABLE


#include "../common/default_config.h"
