/********************************************************************************************************
 * @file	 app_buffer.h
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
 * @date     Jun 20, 2020
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

#ifndef VENDOR_B91_FEATURE_TEST_APP_BUFFER_H_
#define VENDOR_B91_FEATURE_TEST_APP_BUFFER_H_


#if (FEATURE_TEST_MODE == TEST_SDATA_LENGTH_EXTENSION) || (FEATURE_TEST_MODE == TEST_MDATA_LENGTH_EXTENSION)
	// DLE Length select
	#define DLE_LENGTH_27_BYTES				0
	#define DLE_LENGTH_52_BYTES				1
	#define DLE_LENGTH_56_BYTES				2
	#define DLE_LENGTH_100_BYTES			3
	#define DLE_LENGTH_200_BYTES			4
	#define DLE_LENGTH_251_BYTES			5
	#define DLE_LENGTH_SELECT				DLE_LENGTH_200_BYTES
#endif


#if (DLE_LENGTH_SELECT == DLE_LENGTH_27_BYTES)
	#define LL_RX_FIFO_SIZE		64
	#define LL_RX_FIFO_NUM		8

	#define LL_TX_FIFO_SIZE		48
	#define LL_TX_FIFO_NUM		17  //only 9 and 17 and 33 can be used, TODO: test 33

	#define MTU_SIZE_SETTING   			 23
	#define DLE_TX_SUPPORTED_DATA_LEN    27
#elif (DLE_LENGTH_SELECT == DLE_LENGTH_52_BYTES)
	#define LL_RX_FIFO_SIZE		80	// rx - 24 = 56
	#define LL_RX_FIFO_NUM		8

	#define LL_TX_FIFO_SIZE		64	// tx - 12 = 52
	#define LL_TX_FIFO_NUM		17

	#define MTU_SIZE_SETTING   			 48
	#define DLE_TX_SUPPORTED_DATA_LEN    52
#elif (DLE_LENGTH_SELECT == DLE_LENGTH_56_BYTES)
	#define LL_RX_FIFO_SIZE		80	// rx - 24 = 56
	#define LL_RX_FIFO_NUM		8

	#define LL_TX_FIFO_SIZE		80	// tx - 12 = 68
	#define LL_TX_FIFO_NUM		17

	#define MTU_SIZE_SETTING   			 52
	#define DLE_TX_SUPPORTED_DATA_LEN    56
#elif (DLE_LENGTH_SELECT == DLE_LENGTH_100_BYTES)
	#define LL_RX_FIFO_SIZE		128	// rx - 24 = 104
	#define LL_RX_FIFO_NUM		8

	#define LL_TX_FIFO_SIZE		112	// tx - 12 = 100
	#define LL_TX_FIFO_NUM		17

	#define MTU_SIZE_SETTING   			 96
	#define DLE_TX_SUPPORTED_DATA_LEN    100
#elif (DLE_LENGTH_SELECT == DLE_LENGTH_200_BYTES)
	#define LL_RX_FIFO_SIZE		224	// rx - 24 = 200
	#define LL_RX_FIFO_NUM		8

	#define LL_TX_FIFO_SIZE		224	// tx - 12 = 212
	#define LL_TX_FIFO_NUM		17

	#define MTU_SIZE_SETTING   			 196
	#define DLE_TX_SUPPORTED_DATA_LEN    200
#elif (DLE_LENGTH_SELECT == DLE_LENGTH_251_BYTES)
	#define LL_RX_FIFO_SIZE		288	// rx - 24 = 264, max:251+24 = 275, 16 align-> 288
	#define LL_RX_FIFO_NUM		8

	#define LL_TX_FIFO_SIZE		272	// tx - 12 = 260, max:251+12 = 264, 16 align-> 272
	#define LL_TX_FIFO_NUM		9	// Note: the total length(FIFO_SIZE*FIFO_NUM) of FIFO must be less than 0xFFF(4096)

	#define MTU_SIZE_SETTING   			 247
	#define DLE_TX_SUPPORTED_DATA_LEN    251
#else
	#define LL_RX_FIFO_SIZE		64
	#define LL_RX_FIFO_NUM		8

	#define LL_TX_FIFO_SIZE		48
	#define LL_TX_FIFO_NUM		17  //only 9 and 17 and 33 can be used, TODO: test 33

	#define MTU_SIZE_SETTING   			 23
	#define DLE_TX_SUPPORTED_DATA_LEN    27
#endif


extern	u8	app_ll_rxfifo[LL_RX_FIFO_SIZE * LL_RX_FIFO_NUM];
extern	u8	app_ll_txfifo[LL_TX_FIFO_SIZE * LL_TX_FIFO_NUM];


extern 	my_fifo_t	uart_txfifo;




#endif /* VENDOR_B91_FEATURE_TEST_APP_BUFFER_H_ */
