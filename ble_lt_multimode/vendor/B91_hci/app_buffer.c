/********************************************************************************************************
 * @file	 app_buffer.c
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


#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "app_buffer.h"
#include "app_config.h"



_attribute_data_retention_	u8	app_ll_rxfifo[LL_RX_FIFO_SIZE * LL_RX_FIFO_NUM] = {0};
_attribute_data_retention_  u8	app_ll_txfifo[LL_TX_FIFO_SIZE * LL_TX_FIFO_NUM] = {0};



#if (UDB_DEBUG_EN)
	#define	UART_TX_FIFO_SIZE			272
	#define	UART_TX_FIFO_NUM			32

	u8 		 	uart_txfifo_b[UART_TX_FIFO_SIZE * UART_TX_FIFO_NUM];
	my_fifo_t	uart_txfifo = {
														UART_TX_FIFO_SIZE,
														UART_TX_FIFO_NUM,
														0,
														0,
														uart_txfifo_b,};
#endif
