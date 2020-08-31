/********************************************************************************************************
 * @file     ota.h
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
#ifndef BLE_LL_OTA_H_
#define BLE_LL_OTA_H_



/**
 * @brief	OTA result
 */
enum{
	OTA_SUCCESS = 0,     //success
	OTA_PACKET_LOSS,     //lost one or more OTA PDU
	OTA_DATA_CRC_ERR,     //data CRC err
	OTA_WRITE_FLASH_ERR,  //write OTA data to flash ERR
 	OTA_DATA_UNCOMPLETE,  //lost last one or more OTA PDU
 	OTA_TIMEOUT, 		  //
 	OTA_FW_CHECK_ERR,
};


typedef void (*ota_startCb_t)(void);
typedef void (*ota_versionCb_t)(void);
typedef void (*ota_resIndicateCb_t)(int result);




/**
 * @brief      This function is used to register OTA start command callback
 * @param[in]  cb - callback function
 * @return     none
 */
void bls_ota_registerStartCmdCb(ota_startCb_t cb);


/**
 * @brief      This function is used to register OTA version command callback
 * @param[in]  cb - callback function
 * @return     none
 */
void bls_ota_registerVersionReqCb(ota_versionCb_t cb);


/**
 * @brief      This function is used to register OTA result callback
 * @param[in]  cb - callback function
 * @return     none
 */
void bls_ota_registerResultIndicateCb(ota_resIndicateCb_t cb);




/**
 * @brief      This function is used to set OTA timeout
 * @param[in]  timeout_us - timeout value, unit: uS
 * @return     none
 */
void bls_ota_setTimeout(u32 timeout_us);




//firmware_size_k  must be 4k aligned

/**
 * @brief      This function is used to set maximum firmware size and OTA boot address except address 0
 * @param[in]  firmware_size_k - maximum firmware size, unit: K Byte, must 4K Byte aligned
 * @param[in]  boot_addr - timeout value, unit: uS
 * @return     none
 */
void bls_ota_set_fwSize_and_fwBootAddr(int firmware_size_k, int boot_addr);


/**
 * @brief      This function is used to clear OTA firmware data buffer, ready for next OTA
 * 			   this must be done after every OTA
 * @param[in]  none
 * @return     none
 */
void bls_ota_clearNewFwDataArea(void);


/**
 * @brief      CRC16 used in OTA data check.
 * 			   this must be done after every OTA
 * @param[in]  *pD - data buffer address
 * @param[in]  *len - data length
 * @return     none
 */
unsigned short crc16 (unsigned char *pD, int len);




extern int otaWrite(void * p);
extern int otaRead(void * p);


#endif /* BLE_LL_OTA_H_ */
