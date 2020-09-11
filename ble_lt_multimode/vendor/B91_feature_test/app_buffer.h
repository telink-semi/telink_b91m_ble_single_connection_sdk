/********************************************************************************************************
 * @file	app_buffer.h
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
	#define LL_TX_FIFO_NUM		17  //only 9 and 17  can be used

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
	#define LL_TX_FIFO_NUM		17  //only 9 and 17  can be used

	#define MTU_SIZE_SETTING   			 23
	#define DLE_TX_SUPPORTED_DATA_LEN    27
#endif


extern	u8	app_ll_rxfifo[LL_RX_FIFO_SIZE * LL_RX_FIFO_NUM];
extern	u8	app_ll_txfifo[LL_TX_FIFO_SIZE * LL_TX_FIFO_NUM];


extern 	my_fifo_t	uart_txfifo;




#endif /* VENDOR_B91_FEATURE_TEST_APP_BUFFER_H_ */
