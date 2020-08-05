/********************************************************************************************************
 * @file     app_config.h
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

#pragma once



/////////////////// FEATURE SELECT /////////////////////////////////
#define BLE_APP_PM_ENABLE					0
#define PM_DEEPSLEEP_RETENTION_ENABLE		0
#define TEST_CONN_CURRENT_ENABLE            0 //test connection current, disable UI to have a pure power
#define BLE_REMOTE_SECURITY_ENABLE      	1
#define REMOTE_IR_ENABLE					0
#define BLE_AUDIO_ENABLE					1
#define BLE_REMOTE_OTA_ENABLE				0
////////////////////////// AUDIO CONFIG (RCU board) /////////////////////////////
#if (BLE_AUDIO_ENABLE)

#define	ADPCM_PACKET_LEN				   128

#define TL_MIC_ADPCM_UNIT_SIZE			   248

#define TL_MIC_BUFFER_SIZE				   1984

#define BLE_DMIC_ENABLE						1
//DMIC 使用时需要跳线：DDATA-->PB2；  DCLK1-->PB3
#if	BLE_DMIC_ENABLE
	#define PB2_FUNC						AS_DMIC_DI
	#define PB3_FUNC						AS_DMIC_SCK

	#define PB2_INPUT_ENABLE				1
	#define PB3_INPUT_ENABLE				1
#endif

#endif

#define	UDB_DEBUG_EN						0


#if (UDB_DEBUG_EN)
	#define PA5_FUNC						AS_USB_DM
	#define PA6_FUNC						AS_USB_DP
	#define PA5_INPUT_ENABLE				1
	#define PA6_INPUT_ENABLE				1

	#define BLE_ADV_DBG_EN					1
	#define BLE_IRQ_DBG_EN					1
#endif



#if 1 //(SAMPLE_BOARD_SELECT == EVK_C1T139A30_V1P2)
	#if (TEST_CONN_CURRENT_ENABLE) //test current, disable keyboard
			#define	UI_KEYBOARD_ENABLE						0
	#else
			#define	UI_KEYBOARD_ENABLE						1
	#endif


	#if (UI_KEYBOARD_ENABLE)   // if test pure power, kyeScan GPIO setting all disabled
			//---------------  KeyMatrix PB2/PB3/PB4/PB5 -----------------------------
			#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
			#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

			#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid



			#define			CR_VOL_UP				0xf0  ////
			#define			CR_VOL_DN				0xf1
			#define			VOICE					0xc0



			#define		KB_MAP_NORMAL	{	CR_VOL_UP,		VK_1,	 \
											CR_VOL_DN,		VOICE }

#if REMOTE_IR_ENABLE
			#define			IR_VOL_UP		0x0b
			#define 		IR_VK_1			0x01
			#define			IR_VOL_DN		0x0c
			#define 		IR_HOME			0x24
#endif
			//////////////////// KEY CONFIG (EVK board) ///////////////////////////
			#define  KB_DRIVE_PINS  {GPIO_PC2, GPIO_PC0}
			#define  KB_SCAN_PINS   {GPIO_PC3, GPIO_PC1}

			//drive pin as gpio
			#define	PC2_FUNC				AS_GPIO
			#define	PC0_FUNC				AS_GPIO

			//drive pin need 100K pulldown
			#define	PULL_WAKEUP_SRC_PC2		MATRIX_ROW_PULL
			#define	PULL_WAKEUP_SRC_PC0		MATRIX_ROW_PULL

			//drive pin open input to read gpio wakeup level
			#define PC2_INPUT_ENABLE		1
			#define PC0_INPUT_ENABLE		1

			//scan pin as gpio
			#define	PC3_FUNC				AS_GPIO
			#define	PC1_FUNC				AS_GPIO

			//scan  pin need 10K pullup
			#define	PULL_WAKEUP_SRC_PC3		MATRIX_COL_PULL
			#define	PULL_WAKEUP_SRC_PC1		MATRIX_COL_PULL

			//scan pin open input to read gpio level
			#define PC3_INPUT_ENABLE		1
			#define PC1_INPUT_ENABLE		1



			#define		KB_MAP_NUM		KB_MAP_NORMAL
			#define		KB_MAP_FN		KB_MAP_NORMAL


	#endif
#define     UI_LED_ENABLE           1
#if UI_LED_ENABLE
	#define	GPIO_LED_WHITE			GPIO_PB6
	#define	GPIO_LED_GREEN			GPIO_PB5
#if (HARDWARE_BOARD_SELECT == HW_C1T219A20_V1_0_64_EVB)
	#define GPIO_LED_RED			GPIO_PB7
#elif (HARDWARE_BOARD_SELECT == HW_C1T217A20_V1_0_48_EVB)
	#define GPIO_LED_RED			GPIO_PB2
#elif (HARDWARE_BOARD_SELECT == HW_C1T213A20_V1_0_80_EVB)
	#define GPIO_LED_RED			GPIO_PB7
#endif
	#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led

	#define PB7_FUNC				AS_GPIO
	#define PB6_FUNC				AS_GPIO
	#define PB5_FUNC				AS_GPIO

	#define	PB7_OUTPUT_ENABLE		1
	#define	PB6_OUTPUT_ENABLE		1
	#define PB5_OUTPUT_ENABLE		1
#endif
#elif (SAMPLE_BOARD_SELECT == DONGLE_C1T139A3_V2P0A)
	#undef  PM_DEEPSLEEP_RETENTION_ENABLE
	#define PM_DEEPSLEEP_RETENTION_ENABLE					0    //dongle demo no need deepSleepRetention

	#define	UI_BUTTON_ENABLE								1
	#define UI_LED_ENABLE									1

	#if (UI_BUTTON_ENABLE)
			//---------------  Button -------------------------------
			#define	SW1_GPIO				GPIO_PD5
			#define	SW2_GPIO				GPIO_PD6
			#define PD5_FUNC				AS_GPIO
			#define PD6_FUNC				AS_GPIO
			#define PD5_INPUT_ENABLE		1
			#define PD6_INPUT_ENABLE		1
			#define PULL_WAKEUP_SRC_PD5     PM_PIN_PULLUP_10K
			#define PULL_WAKEUP_SRC_PD6     PM_PIN_PULLUP_10K
	#endif



	#if (UI_LED_ENABLE)
			//---------------  LED ----------------------------------
			#define	GPIO_LED_RED			GPIO_PA3
			#define	GPIO_LED_WHITE			GPIO_PB1
			#define	GPIO_LED_GREEN			GPIO_PA2
			#define	GPIO_LED_BLUE			GPIO_PB0
			#define	GPIO_LED_YELLOW			GPIO_PA4

			#define PA3_FUNC				AS_GPIO
			#define PB1_FUNC				AS_GPIO
			#define PA2_FUNC				AS_GPIO
			#define PB0_FUNC				AS_GPIO
			#define PA4_FUNC				AS_GPIO

			#define	PA3_OUTPUT_ENABLE		1
			#define	PB1_OUTPUT_ENABLE		1
			#define PA2_OUTPUT_ENABLE		1
			#define	PB0_OUTPUT_ENABLE		1
			#define	PA4_OUTPUT_ENABLE		1

			#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led
	#endif
#endif



#ifndef	UI_KEYBOARD_ENABLE
#define UI_KEYBOARD_ENABLE									0
#endif

#ifndef	UI_BUTTON_ENABLE
#define UI_BUTTON_ENABLE									0
#endif




/////////////////// DEEP SAVE FLG //////////////////////////////////
#define USED_DEEP_ANA_REG                   DEEP_ANA_REG0 //u8,can save 8 bit info when deep
#define CONN_DEEP_FLG	                    BIT(0) //if 1: conn deep, 0: adv deep






/////////////////// Clock  /////////////////////////////////
#define CLOCK_SYS_CLOCK_HZ  	24000000

enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};



/////////////////// watchdog  //////////////////////////////
#define MODULE_WATCHDOG_ENABLE		0
#define WATCHDOG_INIT_TIMEOUT		500  //ms





#define UART_PRINT_DEBUG_ENABLE			0
/////////////////////////////////////// PRINT DEBUG INFO ///////////////////////////////////////
#if (UART_PRINT_DEBUG_ENABLE)
	//the baud rate should not bigger than 1M(system timer clock is constant 16M)
	#define PRINT_BAUD_RATE             					1000000		//1000000
	#define DEBUG_INFO_TX_PIN           					GPIO_PA0
	#define PULL_WAKEUP_SRC_PA0         					PM_PIN_PULLUP_10K
	#define PA0_OUTPUT_ENABLE         						1
	#define PA0_DATA_OUT                                    1 			//must
	#include "../../application/print/u_printf.h"

#endif



#define DEBUG_GPIO_ENABLE							0

#if(DEBUG_GPIO_ENABLE)


	#define GPIO_CHN0							GPIO_PE1
	#define GPIO_CHN1							GPIO_PE2
//	#define GPIO_CHN2							GPIO_PA0
    #define GPIO_CHN3							GPIO_PA4
	#define GPIO_CHN4							GPIO_PA3
	#define GPIO_CHN5							GPIO_PB0
//	#define GPIO_CHN6							GPIO_PB2
	#define GPIO_CHN7							GPIO_PE0

	#define GPIO_CHN8							GPIO_PA2
	#define GPIO_CHN9							GPIO_PA1
	#define GPIO_CHN10							GPIO_PB1
//	#define GPIO_CHN11							GPIO_PB3

	//PDx bug(baoyi)
	#define GPIO_CHN12							GPIO_PD7
	#define GPIO_CHN13							GPIO_PD6
	#define GPIO_CHN14							GPIO_PD5
	#define GPIO_CHN15							GPIO_PD4


	#define PE1_OUTPUT_ENABLE					1
	#define PE2_OUTPUT_ENABLE					1
//	#define PA0_OUTPUT_ENABLE					1
	#define PA4_OUTPUT_ENABLE					1
	#define PA3_OUTPUT_ENABLE					1
	#define PB0_OUTPUT_ENABLE					1
//	#define PB2_OUTPUT_ENABLE					1
	#define PE0_OUTPUT_ENABLE					1

	#define PA2_OUTPUT_ENABLE					1
	#define PA1_OUTPUT_ENABLE					1
	#define PB1_OUTPUT_ENABLE					1
//	#define PB3_OUTPUT_ENABLE					1
	#define PD7_OUTPUT_ENABLE					1
	#define PD6_OUTPUT_ENABLE					1
	#define PD5_OUTPUT_ENABLE					1
	#define PD4_OUTPUT_ENABLE					1


	#define DBG_CHN0_LOW		gpio_write(GPIO_CHN0, 0)
	#define DBG_CHN0_HIGH		gpio_write(GPIO_CHN0, 1)
	#define DBG_CHN0_TOGGLE		gpio_toggle(GPIO_CHN0)
	#define DBG_CHN1_LOW		gpio_write(GPIO_CHN1, 0)
	#define DBG_CHN1_HIGH		gpio_write(GPIO_CHN1, 1)
	#define DBG_CHN1_TOGGLE		gpio_toggle(GPIO_CHN1)
	#define DBG_CHN2_LOW		//gpio_write(GPIO_CHN2, 0)
	#define DBG_CHN2_HIGH		//gpio_write(GPIO_CHN2, 1)
	#define DBG_CHN2_TOGGLE		//gpio_toggle(GPIO_CHN2)
	#define DBG_CHN3_LOW		gpio_write(GPIO_CHN3, 0)
	#define DBG_CHN3_HIGH		gpio_write(GPIO_CHN3, 1)
	#define DBG_CHN3_TOGGLE		gpio_toggle(GPIO_CHN3)
	#define DBG_CHN4_LOW		gpio_write(GPIO_CHN4, 0)
	#define DBG_CHN4_HIGH		gpio_write(GPIO_CHN4, 1)
	#define DBG_CHN4_TOGGLE		gpio_toggle(GPIO_CHN4)
	#define DBG_CHN5_LOW		gpio_write(GPIO_CHN5, 0)
	#define DBG_CHN5_HIGH		gpio_write(GPIO_CHN5, 1)
	#define DBG_CHN5_TOGGLE		gpio_toggle(GPIO_CHN5)
	#define DBG_CHN6_LOW		//gpio_write(GPIO_CHN6, 0)
	#define DBG_CHN6_HIGH		//gpio_write(GPIO_CHN6, 1)
	#define DBG_CHN6_TOGGLE		//gpio_toggle(GPIO_CHN6)
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
	#define DBG_CHN11_LOW		//gpio_write(GPIO_CHN11, 0)
	#define DBG_CHN11_HIGH		//gpio_write(GPIO_CHN11, 1)
	#define DBG_CHN11_TOGGLE	//gpio_toggle(GPIO_CHN11)
	#define DBG_CHN12_LOW		gpio_write(GPIO_CHN12, 0)
	#define DBG_CHN12_HIGH		gpio_write(GPIO_CHN12, 1)
	#define DBG_CHN12_TOGGLE	gpio_toggle(GPIO_CHN12)
	#define DBG_CHN13_LOW		gpio_write(GPIO_CHN13, 0)
	#define DBG_CHN13_HIGH		gpio_write(GPIO_CHN13, 1)
	#define DBG_CHN13_TOGGLE	gpio_toggle(GPIO_CHN13)
	#define DBG_CHN14_LOW		gpio_write(GPIO_CHN14, 0)
	#define DBG_CHN14_HIGH		gpio_write(GPIO_CHN14, 1)
	#define DBG_CHN14_TOGGLE	gpio_toggle(GPIO_CHN14)
	#define DBG_CHN15_LOW		gpio_write(GPIO_CHN15, 0)
	#define DBG_CHN15_HIGH		gpio_write(GPIO_CHN15, 1)
	#define DBG_CHN15_TOGGLE	gpio_toggle(GPIO_CHN15)
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
	#define DBG_CHN12_LOW
	#define DBG_CHN12_HIGH
	#define DBG_CHN12_TOGGLE
	#define DBG_CHN13_LOW
	#define DBG_CHN13_HIGH
	#define DBG_CHN13_TOGGLE
	#define DBG_CHN14_LOW
	#define DBG_CHN14_HIGH
	#define DBG_CHN14_TOGGLE
	#define DBG_CHN15_LOW
	#define DBG_CHN15_HIGH
	#define DBG_CHN15_TOGGLE
#endif  //end of DEBUG_GPIO_ENABLE



#include "../common/default_config.h"
