/********************************************************************************************************
 * @file     gatt.h
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2015
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

#ifndef GATT_H_
#define GATT_H_




/************************************ User Interface  *****************************************/

/**
 * @brief	   This function is used to provide data used in Scanning Packets that have a data field.
 * @param[in]  connHandle -  connection handle
 * @param[in]  attHandle - The number of significant octets in the Scan_Response_Data.
 * @param[in]  *p -  data buffer pointer
 * @param[in]  len - data byte number
 * @return     Status - 0x00: command succeeded; 0x01-0xFF: command failed
 */
ble_sts_t	blc_gatt_pushHandleValueNotify  (u16 connHandle, u16 attHandle, u8 *p, int len);

ble_sts_t	blc_gatt_pushHandleValueIndicate(u16 connHandle, u16 attHandle, u8 *p, int len);


ble_sts_t 	blc_gatt_pushWriteComand (u16 connHandle, u16 attHandle, u8 *p, int len);

ble_sts_t 	blc_gatt_pushWriteRequest (u16 connHandle, u16 attHandle, u8 *p, int len);





ble_sts_t 	blc_gatt_pushFindInformationRequest(u16 connHandle, u16 start_attHandle, u16 end_attHandle);

ble_sts_t 	blc_gatt_pushFindByTypeValueRequest (u16 connHandle, u16 start_attHandle, u16 end_attHandle, u16 uuid, u8* attr_value, int len);

ble_sts_t 	blc_gatt_pushReadByTypeRequest (u16 connHandle, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);

ble_sts_t 	blc_gatt_pushReadRequest (u16 connHandle, u16 attHandle);

ble_sts_t 	blc_gatt_pushReadBlobRequest (u16 connHandle, u16 attHandle, u16 offset);

ble_sts_t 	blc_gatt_pushReadByGroupTypeRequest (u16 connHandle, u16 start_attHandle, u16 end_attHandle, u8 *uuid, int uuid_len);












#endif /* GATT_H_ */
