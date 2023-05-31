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

#include "config.h"


/////////////////// MODULE /////////////////////////////////
#define BLE_MODULE_PM_ENABLE				0
#define PM_DEEPSLEEP_RETENTION_ENABLE		0
#define TELINK_SPP_SERVICE_ENABLE			1
#define BLE_MODULE_INDICATE_DATA_TO_MCU		1
#define BATT_CHECK_ENABLE       			0   //enable or disable battery voltage detection
#define BLT_APP_LED_ENABLE					1

#define UART_DMA_USE						1


/**
 *  @brief  flash firmware check
 */
#define FLASH_FIRMWARE_CHECK_ENABLE			0


/**
 *  @brief  firmware signature check
 */
#define FIRMWARE_SIGNATURE_ENABLE			0


/**
 *  @brief  DEBUG  Configuration
 */
#define DEBUG_GPIO_ENABLE						1

#define TLKAPI_DEBUG_ENABLE					1
#define TLKAPI_DEBUG_CHANNEL         		TLKAPI_DEBUG_CHANNEL_UDB
#define TLKAPI_DEBUG_GSUART_BAUDRATE       	1000000  //use 1M later




//////////////// SMP SETTING  //////////////////////////////
#define BLE_SECURITY_ENABLE 			   	1

////////////////////////UART INIT///////////////////////////
#if (MCU_CORE_TYPE == MCU_CORE_B91)
	#define PB2_OUTPUT_ENABLE					1
	#define PB2_INPUT_ENABLE					0

	#define PB3_OUTPUT_ENABLE					0
	#define PB3_INPUT_ENABLE					1
#elif (MCU_CORE_TYPE == MCU_CORE_B92)
	#define UART0_TX_PIN            GPIO_FC_PB1
	#define UART0_RX_PIN            GPIO_FC_PB2

	#define PB1_OUTPUT_ENABLE					1
	#define PB1_INPUT_ENABLE					0

	#define PB2_OUTPUT_ENABLE					0
	#define PB2_INPUT_ENABLE					1
#endif
/////////////////// DEEP SAVE FLG //////////////////////////////////
#define USED_DEEP_ANA_REG                   DEEP_ANA_REG1 //u8,can save 8 bit info when deep
#define	LOW_BATT_FLG					    BIT(0)

#if (BATT_CHECK_ENABLE)
#define VBAT_CHANNEL_EN						0

#if VBAT_CHANNEL_EN
	/**		The battery voltage sample range is 1.8~3.5V    **/
#else
	/** 	if the battery voltage > 3.6V, should take some external voltage divider	**/
	#define GPIO_BAT_DETECT					GPIO_PB0
	#define PB0_FUNC						AS_GPIO
	#define PB0_INPUT_ENABLE				0
	#define PB0_DATA_OUT					0
	#define ADC_INPUT_PIN_CHN				ADC_GPIO_PB0
#endif
#endif

//////////////////// LED CONFIG (EVK board) ///////////////////////////
#if BLT_APP_LED_ENABLE
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




//////////////////////////// MODULE PM GPIO	(EVK board) /////////////////////////////////
#define GPIO_WAKEUP_MODULE					GPIO_PA2   //mcu wakeup module
#define	PA2_FUNC							AS_GPIO
#define PA2_INPUT_ENABLE					1
#define	PA2_OUTPUT_ENABLE					0
#define	PA2_DATA_OUT						0
#define GPIO_WAKEUP_MODULE_HIGH				gpio_setup_up_down_resistor(GPIO_WAKEUP_MODULE, PM_PIN_PULLUP_10K);
#define GPIO_WAKEUP_MODULE_LOW				gpio_setup_up_down_resistor(GPIO_WAKEUP_MODULE, PM_PIN_PULLDOWN_100K);

#define GPIO_WAKEUP_MCU						GPIO_PA1   //module wakeup mcu
#define	PA1_FUNC							AS_GPIO
#define PA1_INPUT_ENABLE					1
#define	PA1_OUTPUT_ENABLE					1
#define	PA1_DATA_OUT						0
#define GPIO_WAKEUP_MCU_HIGH				do{gpio_set_output_en(GPIO_WAKEUP_MCU, 1); gpio_write(GPIO_WAKEUP_MCU, 1);}while(0)
#define GPIO_WAKEUP_MCU_LOW					do{gpio_set_output_en(GPIO_WAKEUP_MCU, 1); gpio_write(GPIO_WAKEUP_MCU, 0);}while(0)
#define GPIO_WAKEUP_MCU_FLOAT				do{gpio_set_output_en(GPIO_WAKEUP_MCU, 0); gpio_write(GPIO_WAKEUP_MCU, 0);}while(0)










/////////////////////HCI UART variables///////////////////////////////////////
/*----------------------------------------------*
*	SPP TX FIFO  = 2 Bytes LEN + n Bytes Data.	*
*												*
*	T_txdata_buf = 4 Bytes LEN + n Bytes Data.	*
*												*
*	SPP_TXFIFO_SIZE = 2 + n.					*
*												*
*	UART_DATA_LEN = n.							*
*												*
*	UART_DATA_LEN = SPP_TXFIFO_SIZE - 2.		*
*-----------------------------------------------*/
#define SPP_RXFIFO_SIZE		72
#define SPP_RXFIFO_NUM		2

#define SPP_TXFIFO_SIZE		72
#define SPP_TXFIFO_NUM		8

#define UART_DATA_LEN    	(SPP_TXFIFO_SIZE - 2)   // data max 252
typedef struct{
    unsigned int len; // data max 252
    unsigned char data[UART_DATA_LEN];
}uart_data_t;







///////////////////////////////////// ATT  HANDLER define ///////////////////////////////////////
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
	OTA_PS_H, 								//UUID: 2800, 	VALUE: telink ota service uuid
	OTA_CMD_OUT_CD_H,						//UUID: 2803, 	VALUE:  			Prop: read | write_without_rsp | Notify
	OTA_CMD_OUT_DP_H,						//UUID: telink ota uuid,  VALUE: otaData
	OTA_CMD_INPUT_CCB_H,					//UUID: 2902, 	VALUE: otaDataCCC
	OTA_CMD_OUT_DESC_H,						//UUID: 2901, 	VALUE: otaName

	ATT_END_H,

}ATT_HANDLE;





#if(DEBUG_GPIO_ENABLE)
	#if (MCU_CORE_TYPE == MCU_CORE_B91)

		#define GPIO_CHN0							GPIO_PE1
		#define GPIO_CHN1							GPIO_PE2
	//	#define GPIO_CHN2							GPIO_PA0
	    #define GPIO_CHN3							GPIO_PA4
		#define GPIO_CHN4							GPIO_PA3
		#define GPIO_CHN5							GPIO_PB0
	//	#define GPIO_CHN6							GPIO_PB2
		#define GPIO_CHN7							GPIO_PE0

	//	#define GPIO_CHN8							GPIO_PA2
		#define GPIO_CHN9							GPIO_PA1
		#define GPIO_CHN10							GPIO_PB1
	//	#define GPIO_CHN11							GPIO_PB3


		#define PE1_OUTPUT_ENABLE					1
		#define PE2_OUTPUT_ENABLE					1
	//	#define PA0_OUTPUT_ENABLE					1
		#define PA4_OUTPUT_ENABLE					1
		#define PA3_OUTPUT_ENABLE					1
		#define PB0_OUTPUT_ENABLE					1
	//	#define PB2_OUTPUT_ENABLE					1
		#define PE0_OUTPUT_ENABLE					1

	//	#define PA2_OUTPUT_ENABLE					1
		#define PA1_OUTPUT_ENABLE					1
		#define PB1_OUTPUT_ENABLE					1
	//	#define PB3_OUTPUT_ENABLE					1
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


/////////////////// set default   ////////////////

#include "vendor/common/default_config.h"
