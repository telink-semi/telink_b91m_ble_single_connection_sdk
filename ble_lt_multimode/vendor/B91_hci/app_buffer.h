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

#ifndef VENDOR_B91_HCI_APP_BUFFER_H_
#define VENDOR_B91_HCI_APP_BUFFER_H_




#define LL_RX_FIFO_SIZE		64
#define LL_RX_FIFO_NUM		8

#define LL_TX_FIFO_SIZE		48
#define LL_TX_FIFO_NUM		17  //only 9 and 17 and 33 can be used, TODO: test 33



extern	u8	app_ll_rxfifo[LL_RX_FIFO_SIZE * LL_RX_FIFO_NUM];
extern	u8	app_ll_txfifo[LL_TX_FIFO_SIZE * LL_TX_FIFO_NUM];


extern 	my_fifo_t	uart_txfifo;




#endif /* VENDOR_B91_BLE_SAMPLE_APP_BUFFER_H_ */
