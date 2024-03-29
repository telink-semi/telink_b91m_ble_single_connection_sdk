/********************************************************************************************************
 * @file     app_config.h
 *
 * @brief    This is the header file for BLE SDK
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

#pragma once

#include "../feature_config.h"

#if (FEATURE_TEST_MODE == TEST_SLAVE_MD)

/////////////////// FEATURE SELECT /////////////////////////////////
/**
 *  @brief  Feature select in bLE Sample project
 */
#define BLE_APP_PM_ENABLE					1
#define PM_DEEPSLEEP_RETENTION_ENABLE		1

#define APP_SECURITY_ENABLE      					1



/**
 *  @brief  DEBUG  Configuration
 */

#define TLKAPI_DEBUG_ENABLE					1
#define TLKAPI_DEBUG_CHANNEL         		TLKAPI_DEBUG_CHANNEL_GSUART
#define TLKAPI_DEBUG_GSUART_BAUDRATE       	1000000  //use 1M later

#define APP_LOG_EN							1
#define APP_SMP_LOG_EN						0
#define APP_KEY_LOG_EN						1
#define APP_CONTR_EVENT_LOG_EN				1  //controller event log





/**
 *  @brief  UI Configuration
 */
#define UI_LED_ENABLE          	 			1
#define	UI_KEYBOARD_ENABLE					1




#if (UI_KEYBOARD_ENABLE)   // if test pure power, kyeScan GPIO setting all disabled
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		//---------------  KeyMatrix PB2/PB3/PB4/PB5 -----------------------------
		#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
		#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

		#define	KB_LINE_HIGH_VALID				0   //drive pin output 0 when scan key, scan pin read 0 is valid



		#define			CR_VOL_UP				0xf0
		#define			CR_VOL_DN				0xf1


		/**
		 *  @brief  Normal keyboard map
		 */
		#define		KB_MAP_NORMAL	{	{CR_VOL_DN,		VK_1},	 \
										{CR_VOL_UP,		VK_2}, }



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
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		//---------------  KeyMatrix PD2/PD7/PD6/PF6 -----------------------------
		#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
		#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

		#define	KB_LINE_HIGH_VALID				0   //drive pin output 0 when scan key, scan pin read 0 is valid



		#define			CR_VOL_UP				0xf0
		#define			CR_VOL_DN				0xf1


		/**
		 *  @brief  Normal keyboard map
		 */
		#define		KB_MAP_NORMAL	{	{CR_VOL_DN,		VK_1},	 \
										{CR_VOL_UP,		VK_2}, }



		//////////////////// KEY CONFIG (EVK board) ///////////////////////////
		#define  KB_DRIVE_PINS  {GPIO_PD6, GPIO_PF6}
		#define  KB_SCAN_PINS   {GPIO_PD2, GPIO_PD7}

		//drive pin as gpio
		#define	PD6_FUNC				AS_GPIO
		#define	PF6_FUNC				AS_GPIO

		//drive pin need 100K pulldown
		#define	PULL_WAKEUP_SRC_PD6		MATRIX_ROW_PULL
		#define	PULL_WAKEUP_SRC_PF6		MATRIX_ROW_PULL

		//drive pin open input to read gpio wakeup level
		#define PD6_INPUT_ENABLE		1
		#define PF6_INPUT_ENABLE		1

		//scan pin as gpio
		#define	PD2_FUNC				AS_GPIO
		#define	PD7_FUNC				AS_GPIO

		//scan  pin need 10K pullup
		#define	PULL_WAKEUP_SRC_PD2		MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PD7		MATRIX_COL_PULL

		//scan pin open input to read gpio level
		#define PD2_INPUT_ENABLE		1
		#define PD7_INPUT_ENABLE		1
	#endif
#endif

#if (UI_LED_ENABLE)
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		/**
		 *  @brief  Definition gpio for led
		 */
		#define	GPIO_LED_WHITE			GPIO_PB6
		#define	GPIO_LED_GREEN			GPIO_PB5
		#define	GPIO_LED_BLUE			GPIO_PB4
		#define GPIO_LED_RED			GPIO_PB7
		#define LED_ON_LEVEL 			1 		//gpio output high voltage to turn on led

		#define PB7_FUNC				AS_GPIO
		#define PB6_FUNC				AS_GPIO
		#define PB5_FUNC				AS_GPIO
		#define PB4_FUNC				AS_GPIO

		#define	PB7_OUTPUT_ENABLE		1
		#define	PB6_OUTPUT_ENABLE		1
		#define PB5_OUTPUT_ENABLE		1
		#define	PB4_OUTPUT_ENABLE		1
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		/**
		 *  @brief  Definition gpio for led
		 */
		#define	GPIO_LED_WHITE			GPIO_PE6
		#define	GPIO_LED_GREEN			GPIO_PD1
		#define	GPIO_LED_BLUE			GPIO_PD0
		#define GPIO_LED_RED			GPIO_PE7
		#define LED_ON_LEVEL 			1 		//gpio output high voltage to turn on led

		#define PE6_FUNC				AS_GPIO
		#define PD1_FUNC				AS_GPIO
		#define PD0_FUNC				AS_GPIO
		#define PE7_FUNC				AS_GPIO

		#define	PE6_OUTPUT_ENABLE		1
		#define	PD1_OUTPUT_ENABLE		1
		#define PD0_OUTPUT_ENABLE		1
		#define	PE7_OUTPUT_ENABLE		1
	#endif
#endif











/**
 *  @brief  Definition for gpio debug
 */
#define DEBUG_GPIO_ENABLE							0

#if(DEBUG_GPIO_ENABLE)
	#if (MCU_CORE_TYPE == MCU_CORE_B91)

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


		#define GPIO_CHN12							GPIO_PC7
		#define GPIO_CHN13							GPIO_PC6
		#define GPIO_CHN14							GPIO_PC5
		#define GPIO_CHN15							GPIO_PC4


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
		#define PC7_OUTPUT_ENABLE					1
		#define PC6_OUTPUT_ENABLE					1
		#define PC5_OUTPUT_ENABLE					1
		#define PC4_OUTPUT_ENABLE					1

	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		#define GPIO_CHN0							GPIO_PA1
		#define GPIO_CHN1							GPIO_PA2
		#define GPIO_CHN2							GPIO_PA3
	    #define GPIO_CHN3							GPIO_PA4
		#define GPIO_CHN4							GPIO_PB1
		#define GPIO_CHN5							GPIO_PB2
		#define GPIO_CHN6							GPIO_PB3
		#define GPIO_CHN7							GPIO_PB4

		#define GPIO_CHN8							GPIO_PB5
		#define GPIO_CHN9							GPIO_PB6
		#define GPIO_CHN10							GPIO_PB7
		#define GPIO_CHN11							GPIO_PC0
		#define GPIO_CHN12							GPIO_PE0
		#define GPIO_CHN13							GPIO_PE1
		#define GPIO_CHN14							GPIO_PE2
		#define GPIO_CHN15							GPIO_PE3


		#define PA1_OUTPUT_ENABLE					1
		#define PA2_OUTPUT_ENABLE					1
		#define PA3_OUTPUT_ENABLE					1
		#define PA4_OUTPUT_ENABLE					1
		#define PB1_OUTPUT_ENABLE					1
		#define PB2_OUTPUT_ENABLE					1
		#define PB3_OUTPUT_ENABLE					1
		#define PB4_OUTPUT_ENABLE					1

		#define PB5_OUTPUT_ENABLE					1
		#define PB6_OUTPUT_ENABLE					1
		#define PB7_OUTPUT_ENABLE					1
		#define PC0_OUTPUT_ENABLE					1
		#define PE0_OUTPUT_ENABLE					1
		#define PE1_OUTPUT_ENABLE					1
		#define PE2_OUTPUT_ENABLE					1
		#define PE3_OUTPUT_ENABLE					1
	#endif
#endif  //end of DEBUG_GPIO_ENABLE



#include "vendor/common/default_config.h"


#endif  //end of (FEATURE_TEST_MODE == ...)
