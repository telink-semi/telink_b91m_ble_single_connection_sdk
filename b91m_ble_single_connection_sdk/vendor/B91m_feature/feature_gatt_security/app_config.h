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

#if (FEATURE_TEST_MODE == TEST_GATT_SECURITY)

/////////////////// FEATURE SELECT /////////////////////////////////
/**
 *  @brief  Feature select in bLE Sample project
 */
#define BLE_APP_PM_ENABLE					1
#define PM_DEEPSLEEP_RETENTION_ENABLE		1

#define APP_DEFAULT_HID_BATTERY_OTA_ATTRIBUTE_TABLE				0

// LE_Security_Mode_1_Level_1, no authentication and no encryption
#define 	SMP_TEST_NO_SECURITY				1
// LE_Security_Mode_1_Level_2, unauthenticated paring with encryption
#define 	SMP_TEST_LEGACY_PAIRING_JUST_WORKS	2 //JustWorks
#define 	SMP_TEST_SC_PAIRING_JUST_WORKS		3 //JustWorks
// LE_Security_Mode_1_Level_3, authenticated paring with encryption
#define 	SMP_TEST_LEGACY_PASSKEY_ENTRY_SDMI	4 //PK_Resp_Dsply_Init_Input
#define 	SMP_TEST_LEGACY_PASSKEY_ENTRY_MDSI	5 //PK_Init_Dsply_Resp_Input
#define 	SMP_TEST_LEGACY_PASSKEY_ENTRY_MISI	6 //PK_BOTH_INPUT, not test
#define 	SMP_TEST_LEGACY_PASSKEY_ENTRY_OOB	7 //OOB_Authentication, not test
// LE_Security_Mode_1_Level_4, authenticated paring with encryption
#define 	SMP_TEST_SC_NUMERIC_COMPARISON		8 //Numric_Comparison
#define 	SMP_TEST_SC_PASSKEY_ENTRY_SDMI		9 //PK_Resp_Dsply_Init_Input
#define 	SMP_TEST_SC_PASSKEY_ENTRY_MDSI		10//PK_Init_Dsply_Resp_Input
#define 	SMP_TEST_SC_PASSKEY_ENTRY_MISI		11//PK_BOTH_INPUT, not test
#define 	SMP_TEST_SC_PASSKEY_ENTRY_OOB		12//OOB_Authentication, not test

// LE security mode select
#define 	LE_SECURITY_MODE_1_LEVEL_1			SMP_TEST_NO_SECURITY
#define 	LE_SECURITY_MODE_1_LEVEL_2			SMP_TEST_LEGACY_PAIRING_JUST_WORKS
#define 	LE_SECURITY_MODE_1_LEVEL_3			SMP_TEST_LEGACY_PASSKEY_ENTRY_SDMI
#define 	LE_SECURITY_MODE_1_LEVEL_4			SMP_TEST_SC_PASSKEY_ENTRY_SDMI



#define     SMP_TEST_MODE						LE_SECURITY_MODE_1_LEVEL_2

//use app, after connected, enable notify, write some data into characteristic Telink SPP:Phone->Module

//client to server RX character permission
//refer to core5.0 Vol3,Part C, Table 10.2 for more information
#if (SMP_TEST_MODE == LE_SECURITY_MODE_1_LEVEL_1)
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_RDWR
	#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_ENCRYPT_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_AUTHEN_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_SECURE_CONN_RDWR
#elif(SMP_TEST_MODE == LE_SECURITY_MODE_1_LEVEL_2)
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_RDWR
	#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_ENCRYPT_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_AUTHEN_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_SECURE_CONN_RDWR
#elif(SMP_TEST_MODE == LE_SECURITY_MODE_1_LEVEL_3)
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_ENCRYPT_RDWR
	#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_AUTHEN_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_SECURE_CONN_RDWR
#elif(SMP_TEST_MODE == LE_SECURITY_MODE_1_LEVEL_4)
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_ENCRYPT_RDWR
	//#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_AUTHEN_RDWR
	#define     SPP_C2S_ATT_PERMISSIONS_RDWR        ATT_PERMISSIONS_SECURE_CONN_RDWR
#endif



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



#if (UI_LED_ENABLE)
	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		/**
		 *  @brief  Definition gpio for led
		 */
		#define	GPIO_LED_WHITE			GPIO_PB6
		#define	GPIO_LED_GREEN			GPIO_PB5
		#define	GPIO_LED_BLUE			GPIO_PB4
		#define GPIO_LED_RED			GPIO_PB7
		#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led

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
		#define LED_ON_LEVAL 			1 		//gpio output high voltage to turn on led

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









#ifndef UART_PRINT_DEBUG_ENABLE
#define UART_PRINT_DEBUG_ENABLE  			1
#endif
/////////////////////////////////////// PRINT DEBUG INFO ///////////////////////////////////////
#if (UART_PRINT_DEBUG_ENABLE)
	#define PRINT_BAUD_RATE             					115200
	#define DEBUG_INFO_TX_PIN           					GPIO_PA0
	#define PULL_WAKEUP_SRC_PA0         					PM_PIN_PULLUP_10K
	#define PA0_OUTPUT_ENABLE         					1
	#define PA0_DATA_OUT                                      1 //must
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
