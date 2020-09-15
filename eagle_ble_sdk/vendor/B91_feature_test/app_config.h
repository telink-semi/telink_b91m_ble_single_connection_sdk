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

/* Enable C linkage for C++ Compilers: */
#if defined(__cplusplus)
extern "C" {
#endif

// Hardware board select
#define HW_C1T219A20_V1_0_64_EVB		1
#define HW_C1T217A20_V1_0_48_EVB		2
#define HW_C1T213A20_V1_0_80_EVB		3
#define HARDWARE_BOARD_SELECT			HW_C1T213A20_V1_0_80_EVB


/////////////////// TEST FEATURE SELECTION /////////////////////////////////

//ble link layer test
#define	TEST_ADVERTISING_ONLY							1
#define TEST_SCANNING_ONLY								2
#define TEST_ADVERTISING_IN_CONN_SLAVE_ROLE				3
#define TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE        4
#define TEST_ADVERTISING_SCANNING_IN_CONN_SLAVE_ROLE	5


//power test
#define TEST_POWER_ADV									10



//smp test
#define TEST_SMP_SECURITY								20 //If testing SECURITY, such as Passkey Entry or Numric_Comparison, we use the remote control board for testing

//gatt secure test
#define TEST_GATT_SECURITY								21 //If testing SECURITY, such as Passkey Entry or Numric_Comparison, we use the remote control board for testing

//slave data length exchange test
#define TEST_SDATA_LENGTH_EXTENSION						22

//master data length exchange test
#define TEST_MDATA_LENGTH_EXTENSION						23

//other test
#define TEST_USER_BLT_SOFT_TIMER						30
#define TEST_WHITELIST									31
 //phy test
#define TEST_BLE_PHY									32		// BQB PHY_TEST demo


#define TEST_EXTENDED_ADVERTISING						40		//  Extended ADV demo


#define TEST_2M_CODED_PHY_EXT_ADV						50		//  2M/Coded PHY used on Extended ADV

#define TEST_2M_CODED_PHY_CONNECTION					60		//  2M/Coded PHY used on Legacy_ADV and Connection

#define	TEST_CSA2										80



#define FEATURE_TEST_MODE								TEST_SDATA_LENGTH_EXTENSION




#define 		PHYTEST_MODE_THROUGH_2_WIRE_UART		1   //Direct Test Mode through a 2-wire UART interface
#define 		PHYTEST_MODE_OVER_HCI_WITH_USB			2   //Direct Test Mode over HCI(USB hardware interface)
#define 		PHYTEST_MODE_OVER_HCI_WITH_UART			3   //Direct Test Mode over HCI(UART hardware interface)

#if (FEATURE_TEST_MODE == TEST_BLE_PHY)

	#define BLE_PHYTEST_MODE					    PHYTEST_MODE_THROUGH_2_WIRE_UART

#elif (FEATURE_TEST_MODE == TEST_USER_BLT_SOFT_TIMER)

	#define BLT_SOFTWARE_TIMER_ENABLE				1

#elif (FEATURE_TEST_MODE == TEST_SMP_SECURITY)

	////////////////////////// BOARD SELECT ////////////////////////////////////////
	#define EVK_BOARD							0    			//Ver:C1T213A20_V1_1
	#define REMOTE_BOARD						1    			//Ver:provide later
	#define	BOARD_SELECT						EVK_BOARD


	#if (BOARD_SELECT == EVK_BOARD)
		#define	MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
		#define	MATRIX_COL_PULL					PM_PIN_PULLUP_10K

		#define	KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid

		#define			CR_VOL_UP				0xf0  ////
		#define			CR_VOL_DN				0xf1

		/**
		 *  @brief  Normal keyboard map
		 */
		#define		KB_MAP_NORMAL	{	CR_VOL_DN,		VK_1,	 \
										CR_VOL_UP,		VK_2 }



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


		// LED GPIO
		/**
		 *  @brief  Definition gpio for led
		 */
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
		#define GPIO_LED				GPIO_LED_GREEN
		#define	LED_ON_LEVAL			1 	//gpio output high voltage to turn on led

	#elif(BOARD_SELECT == REMOTE_BOARD)
		//LED gpio cfg
		#define		LED_ON_LEVAL						1 			//gpio output high voltage to turn on led
		#define		GPIO_LED							GPIO_PC6
		#define 	PC6_FUNC							AS_GPIO

		//PB3 IRout 100K pulldown when  IR not working,  when IR begin, disable this 100K pulldown
		#define			PULL_WAKEUP_SRC_PB3				PM_PIN_PULLDOWN_100K

		//////////////////////////// KEYSCAN/MIC  GPIO //////////////////////////////////
		#define			MATRIX_ROW_PULL					PM_PIN_PULLDOWN_100K
		#define			MATRIX_COL_PULL					PM_PIN_PULLUP_10K
		#define			KB_LINE_HIGH_VALID				0   //dirve pin output 0 when keyscan, scanpin read 0 is valid

		//repeat key
		#define 		KB_REPEAT_KEY_ENABLE			0
		#define			KB_REPEAT_KEY_INTERVAL_MS		200
		#define 		KB_REPEAT_KEY_NUM				1
		#define 		KB_MAP_REPEAT					{VK_1, }

		#define			CR_VOL_UP						0xf0  ////
		#define			CR_VOL_DN						0xf1
		#define			CR_VOL_MUTE						0xf2
		#define			CR_CHN_UP						0xf3
		#define			CR_CHN_DN						0xf4  ////
		#define			CR_POWER						0xf5
		#define			CR_SEARCH						0xf6
		#define			CR_RECORD						0xf7
		#define			CR_PLAY							0xf8  ////
		#define			CR_PAUSE						0xf9
		#define			CR_STOP							0xfa
		#define			CR_FAST_BACKWARD				0xfb
		#define			CR_FAST_FORWARD					0xfc  ////
		#define			CR_HOME							0xfd
		#define			CR_BACK							0xfe
		#define			CR_MENU							0xff
		//key map
		#define			KB_MAP_NORMAL	{\
							VK_B,		CR_POWER,		VK_NONE,		VK_C,				CR_HOME,	 \
							VK_NONE,	VK_NONE,		VK_NONE,		CR_VOL_UP,			CR_VOL_DN,	 \
							VK_2,		VK_RIGHT,		CR_VOL_DN,		VK_3,				VK_1,	 \
							VK_5,		VK_ENTER,		CR_VOL_UP,		VK_6,				VK_4,	 \
							VK_8,		VK_DOWN,		VK_UP ,			VK_9,				VK_7,	 \
							VK_0,		CR_BACK,		VK_LEFT,		CR_VOL_MUTE,		CR_MENU,	 }

		#define  		KB_DRIVE_PINS  {GPIO_PD5, GPIO_PD2, GPIO_PD4, GPIO_PD6, GPIO_PD7}
		#define  		KB_SCAN_PINS   {GPIO_PC5, GPIO_PA0, GPIO_PB2, GPIO_PA4, GPIO_PA3, GPIO_PD3}

		//drive pin as gpio
		#define	PD5_FUNC				AS_GPIO
		#define	PD2_FUNC				AS_GPIO
		#define	PD4_FUNC				AS_GPIO
		#define	PD6_FUNC				AS_GPIO
		#define	PD7_FUNC				AS_GPIO
		//drive pin need 100K pulldown
		#define	PULL_WAKEUP_SRC_PD5		MATRIX_ROW_PULL
		#define	PULL_WAKEUP_SRC_PD2		MATRIX_ROW_PULL
		#define	PULL_WAKEUP_SRC_PD4		MATRIX_ROW_PULL
		#define	PULL_WAKEUP_SRC_PD6		MATRIX_ROW_PULL
		#define	PULL_WAKEUP_SRC_PD7		MATRIX_ROW_PULL
		//drive pin open input to read gpio wakeup level
		#define PD5_INPUT_ENABLE		1
		#define PD2_INPUT_ENABLE		1
		#define PD4_INPUT_ENABLE		1
		#define PD6_INPUT_ENABLE		1
		#define PD7_INPUT_ENABLE		1
		//scan pin as gpio
		#define	PC5_FUNC				AS_GPIO
		#define	PA0_FUNC				AS_GPIO
		#define	PB2_FUNC				AS_GPIO
		#define	PA4_FUNC				AS_GPIO
		#define	PA3_FUNC				AS_GPIO
		#define	PD3_FUNC				AS_GPIO
		//scan  pin need 10K pullup
		#define	PULL_WAKEUP_SRC_PC5		MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PA0		MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PB2		MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PA4		MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PA3		MATRIX_COL_PULL
		#define	PULL_WAKEUP_SRC_PD3		MATRIX_COL_PULL
		//scan pin open input to read gpio level
		#define PC5_INPUT_ENABLE		1
		#define PA0_INPUT_ENABLE		1
		#define PB2_INPUT_ENABLE		1
		#define PA4_INPUT_ENABLE		1
		#define PA3_INPUT_ENABLE		1
		#define PD3_INPUT_ENABLE		1

	#endif
	#define	KB_MAP_NUM			    KB_MAP_NORMAL
	#define	KB_MAP_FN			    KB_MAP_NORMAL

	#define GPIO_WAKEUP_KEYPROC_CNT	3
	#define LONG_PRESS_KEY_POWER_OPTIMIZE 1
	#define IDLE_KEY	   			0
	#define CONSUMER_KEY   	   		1
	#define KEYBOARD_KEY   	   		2
	#define IR_KEY   	   			3
#elif (FEATURE_TEST_MODE == TEST_GATT_SECURITY)

#elif (FEATURE_TEST_MODE == TEST_MDATA_LENGTH_EXTENSION)
	#define BLE_HOST_SMP_ENABLE				0  //1 for standard security management
	#define BLE_HOST_SIMPLE_SDP_ENABLE		0  //simple service discovery
	#define UI_BUTTON_ENABLE				0
	#define UI_LED_ENABLE					1

	//Button gpio cfg
	/**
	 *  @brief  Definition gpio for button detection
	 */
	#define	SW1_GPIO						GPIO_PC6
	#define	SW2_GPIO						GPIO_PC7
	#define PC6_FUNC						AS_GPIO
	#define PC7_FUNC						AS_GPIO
	#define PC6_INPUT_ENABLE				1
	#define PC7_INPUT_ENABLE				1
	#define PULL_WAKEUP_SRC_PC6				PM_PIN_PULLUP_10K
	#define PULL_WAKEUP_SRC_PC7				PM_PIN_PULLUP_10K

	// LED GPIO
	/**
	 *  @brief  Definition gpio for led
	 */
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

	//Clock
	#define CLOCK_SYS_CLOCK_HZ  			16000000

#elif (FEATURE_TEST_MODE == TEST_SDATA_LENGTH_EXTENSION)

#endif

/////////////////// Clock  /////////////////////////////////
/**
 *  @brief  MCU system clock
 */
#ifndef CLOCK_SYS_CLOCK_HZ
#define CLOCK_SYS_CLOCK_HZ  	16000000
#endif

/**
 *  @brief  Definition the number of system tick for 1s/1ms/1us
 */
enum{
	CLOCK_SYS_CLOCK_1S = CLOCK_SYS_CLOCK_HZ,
	CLOCK_SYS_CLOCK_1MS = (CLOCK_SYS_CLOCK_1S / 1000),
	CLOCK_SYS_CLOCK_1US = (CLOCK_SYS_CLOCK_1S / 1000000),
};

#define UART_DATA_LEN    68      // data max 252
typedef struct{
    unsigned int len;        // data max 252
    unsigned char data[UART_DATA_LEN];
}uart_data_t;





///////////////////////////////////// ATT  HANDLER define ///////////////////////////////////////
/**
 *  @brief  Attribute handler define
 */
typedef enum
{
	ATT_H_START = 0,


	//// Gap ////
	/**********************************************************************************************/
	GenericAccess_PS_H, 					//UUID: 2800, 	VALUE: uuid 1800
	GenericAccess_DeviceName_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
	GenericAccess_DeviceName_DP_H,			//UUID: 2A00,   VALUE: device name
	GenericAccess_Appearance_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read
	GenericAccess_Appearance_DP_H,			//UUID: 2A01,	VALUE: appearance
	CONN_PARAM_CD_H,						//UUID: 2803, 	VALUE:  			Prop: Read
	CONN_PARAM_DP_H,						//UUID: 2A04,   VALUE: connParameter


	//// gatt ////
	/**********************************************************************************************/
	GenericAttribute_PS_H,					//UUID: 2800, 	VALUE: uuid 1801
	GenericAttribute_ServiceChanged_CD_H,	//UUID: 2803, 	VALUE:  			Prop: Indicate
	GenericAttribute_ServiceChanged_DP_H,   //UUID:	2A05,	VALUE: service change
	GenericAttribute_ServiceChanged_CCB_H,	//UUID: 2902,	VALUE: serviceChangeCCC


	//// device information ////
	/**********************************************************************************************/
	DeviceInformation_PS_H,					 //UUID: 2800, 	VALUE: uuid 180A
	DeviceInformation_pnpID_CD_H,			 //UUID: 2803, 	VALUE:  			Prop: Read
	DeviceInformation_pnpID_DP_H,			 //UUID: 2A50,	VALUE: PnPtrs

	#if(FEATURE_TEST_MODE == TEST_SDATA_LENGTH_EXTENSION || FEATURE_TEST_MODE == TEST_GATT_SECURITY)
		//// SPP ////
		/**********************************************************************************************/
		SPP_PS_H, 							 //UUID: 2800, 	VALUE: telink spp service uuid

		//server to client
		SPP_SERVER_TO_CLIENT_CD_H,		     //UUID: 2803, 	VALUE:  			Prop: read | Notify
		SPP_SERVER_TO_CLIENT_DP_H,			 //UUID: telink spp s2c uuid,  VALUE: SppDataServer2ClientData
		SPP_SERVER_TO_CLIENT_CCB_H,			 //UUID: 2902, 	VALUE: SppDataServer2ClientDataCCC
		SPP_SERVER_TO_CLIENT_DESC_H,		 //UUID: 2901, 	VALUE: TelinkSPPS2CDescriptor

		//client to server
		SPP_CLIENT_TO_SERVER_CD_H,		     //UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp
		SPP_CLIENT_TO_SERVER_DP_H,			 //UUID: telink spp c2s uuid,  VALUE: SppDataClient2ServerData
		SPP_CLIENT_TO_SERVER_DESC_H,		 //UUID: 2901, 	VALUE: TelinkSPPC2SDescriptor

		//// Ota ////
		/**********************************************************************************************/
		OTA_PS_H, 							 //UUID: 2800, 	VALUE: telink ota service uuid
		OTA_CMD_OUT_CD_H,					 //UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp
		OTA_CMD_OUT_DP_H,					 //UUID: telink ota uuid,  VALUE: otaData
		OTA_CMD_OUT_DESC_H,					 //UUID: 2901, 	VALUE: otaName
	#else
		//// HID ////
		/**********************************************************************************************/
		HID_PS_H, 								//UUID: 2800, 	VALUE: uuid 1812

		//include
		HID_INCLUDE_H,							//UUID: 2802, 	VALUE: include

		//protocol
		HID_PROTOCOL_MODE_CD_H,					//UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp
		HID_PROTOCOL_MODE_DP_H,					//UUID: 2A4E,	VALUE: protocolMode

		//boot keyboard input report
		HID_BOOT_KB_REPORT_INPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
		HID_BOOT_KB_REPORT_INPUT_DP_H,			//UUID: 2A22, 	VALUE: bootKeyInReport
		HID_BOOT_KB_REPORT_INPUT_CCB_H,			//UUID: 2902, 	VALUE: bootKeyInReportCCC

		//boot keyboard output report
		HID_BOOT_KB_REPORT_OUTPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | write| write_without_rsp
		HID_BOOT_KB_REPORT_OUTPUT_DP_H,		    //UUID: 2A32, 	VALUE: bootKeyOutReport

		//consume report in
		HID_CONSUME_REPORT_INPUT_CD_H,			//UUID: 2803, 	VALUE:  			Prop: Read | Notify
		HID_CONSUME_REPORT_INPUT_DP_H,			//UUID: 2A4D, 	VALUE: reportConsumerIn
		HID_CONSUME_REPORT_INPUT_CCB_H,			//UUID: 2902, 	VALUE: reportConsumerInCCC
		HID_CONSUME_REPORT_INPUT_REF_H, 		//UUID: 2908    VALUE: REPORT_ID_CONSUMER, TYPE_INPUT

		//keyboard report in
		HID_NORMAL_KB_REPORT_INPUT_CD_H,		//UUID: 2803, 	VALUE:  			Prop: Read | Notify
		HID_NORMAL_KB_REPORT_INPUT_DP_H,		//UUID: 2A4D, 	VALUE: reportKeyIn
		HID_NORMAL_KB_REPORT_INPUT_CCB_H,		//UUID: 2902, 	VALUE: reportKeyInInCCC
		HID_NORMAL_KB_REPORT_INPUT_REF_H, 		//UUID: 2908    VALUE: REPORT_ID_KEYBOARD, TYPE_INPUT

		//keyboard report out
		HID_NORMAL_KB_REPORT_OUTPUT_CD_H,		//UUID: 2803, 	VALUE:  			Prop: Read | write| write_without_rsp
		HID_NORMAL_KB_REPORT_OUTPUT_DP_H,  		//UUID: 2A4D, 	VALUE: reportKeyOut
		HID_NORMAL_KB_REPORT_OUTPUT_REF_H, 		//UUID: 2908    VALUE: REPORT_ID_KEYBOARD, TYPE_OUTPUT

		// report map
		HID_REPORT_MAP_CD_H,					//UUID: 2803, 	VALUE:  			Prop: Read
		HID_REPORT_MAP_DP_H,					//UUID: 2A4B, 	VALUE: reportKeyIn
		HID_REPORT_MAP_EXT_REF_H,				//UUID: 2907 	VALUE: extService

		//hid information
		HID_INFORMATION_CD_H,					//UUID: 2803, 	VALUE:  			Prop: read
		HID_INFORMATION_DP_H,					//UUID: 2A4A 	VALUE: hidInformation

		//control point
		HID_CONTROL_POINT_CD_H,					//UUID: 2803, 	VALUE:  			Prop: write_without_rsp
		HID_CONTROL_POINT_DP_H,					//UUID: 2A4C 	VALUE: controlPoint


		//// battery service ////
		/**********************************************************************************************/
		BATT_PS_H, 								//UUID: 2800, 	VALUE: uuid 180f
		BATT_LEVEL_INPUT_CD_H,					//UUID: 2803, 	VALUE:  			Prop: Read | Notify
		BATT_LEVEL_INPUT_DP_H,					//UUID: 2A19 	VALUE: batVal
		BATT_LEVEL_INPUT_CCB_H,					//UUID: 2902, 	VALUE: batValCCC


		//// Ota ////
		/**********************************************************************************************/
		OTA_PS_H, 								//UUID: 2800, 	VALUE: telink ota service uuid
		OTA_CMD_OUT_CD_H,						//UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp
		OTA_CMD_OUT_DP_H,						//UUID: telink ota uuid,  VALUE: otaData
		OTA_CMD_OUT_DESC_H,						//UUID: 2901, 	VALUE: otaName
	#endif

	ATT_END_H,

}ATT_HANDLE;

/**
 *  @brief  Definition for gpio debug
 */
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

	//PDx bug(baoyi)
	#define GPIO_CHN12							GPIO_PD7
	#define GPIO_CHN13							GPIO_PD6
	#define GPIO_CHN14							GPIO_PD5
	#define GPIO_CHN15							GPIO_PD4


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
/////////////////// set default   ////////////////

#include "../common/default_config.h"

/* Disable C linkage for C++ Compilers: */
#if defined(__cplusplus)
}
#endif
