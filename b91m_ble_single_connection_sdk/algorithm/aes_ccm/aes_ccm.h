/********************************************************************************************************
 * @file     aes_ccm.h
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
#pragma  once

#include "stack/ble/ble_format.h"

#define AES_BLOCK_SIZE     16


//#define		SUCCESS			0
enum {
    AES_SUCC = SUCCESS,
    AES_NO_BUF,               
    AES_FAIL,
};


typedef struct {
	u32		pkt;
	u8		dir;
	u8		iv[8];
} ble_crypt_nonce_t;

#if (MCU_CORE_TYPE == MCU_CORE_B92) 
enum{
	HW_AES_ECC_DISABLE=0,
	HW_AES_ECC_PENDING,
	HW_AES_ECC_ENABLE,
};
#endif

typedef struct {
	u64					enc_pno;
	u64					dec_pno;
	u8                  ltk[16];
	u8					sk[16];			//session key
	ble_crypt_nonce_t	nonce;
	u8					st;
	u8					enable;			//1: slave enable; 2: master enable
#if (MCU_CORE_TYPE == MCU_CORE_B92) 
	u8					hw_aes_ccm_flg;	//0: disable, 1: pending, 2:enable
#endif
	u8					mic_fail;
} ble_crypt_para_t;


struct CCM_FLAGS_TAG {
    union {
        struct {
            u8 L : 3;
            u8 M : 3;
            u8 aData :1;
            u8 reserved :1;            
        } bf;
        u8 val;
    };
};

typedef struct CCM_FLAGS_TAG ccm_flags_t;


typedef struct {
    union {
        u8 A[AES_BLOCK_SIZE];
        u8 B[AES_BLOCK_SIZE];
    } bf;
    
    u8 tmpResult[AES_BLOCK_SIZE];
    u8 newAstr[AES_BLOCK_SIZE];
} aes_enc_t;


enum{
	CRYPT_NONCE_TYPE_ACL = 0,
	CRYPT_NONCE_TYPE_CIS = 1,
	CRYPT_NONCE_TYPE_BIS = 2,
};


extern volatile int aes_enc_dec_busy;


/**
 * @brief   	this function is used to initialize the aes_ccm initial value
 * @param[in]   ltk - encryption key, LTK
 * @param[in]   skdm -
 * @param[in]   skds -
 * @param[in]   ivm -
 * @param[in]   ivs -
 * @param[in]   pd - Reference structure ble_crypt_para_t
 * @return  	none
 */
void aes_ll_ccm_encryption_init (u8 *ltk, u8 *skdm, u8 *skds, u8 *ivm, u8 *ivs, ble_crypt_para_t *pd);


/**
 * @brief   	this function is used to encrypt the aes_ccm value
 * @param[in]   pkt - plaint_text
 * @param[in]   role - ll_ccm_enc: Master role must use 1, Slave role must use 0;
 *                     ll_ccm_dec: Master role must use 0, Slave role must use 1;
 * @param[in]   ll_type -  CRYPT_NONCE_TYPE_ACL = 0,
 *                         CRYPT_NONCE_TYPE_CIS = 1,
 *                         CRYPT_NONCE_TYPE_BIS = 2,
 * @param[in]   pd - Reference structure ble_crypt_para_t
 * @return  	none
 */
void aes_ll_ccm_encryption(llPhysChnPdu_t *pllPhysChnPdu, u8 role, u8 ll_type, ble_crypt_para_t *pd);


/**
 * @brief   	this function is used to decrypt the aes_ccm value
 * @param[in]   pkt - plaint_text
 * @param[in]   role - ll_ccm_enc: Master role must use 1, Slave role must use 0;
 *                     ll_ccm_dec: Master role must use 0, Slave role must use 1;
 * @param[in]   ll_type -  CRYPT_NONCE_TYPE_ACL = 0,
 *                         CRYPT_NONCE_TYPE_CIS = 1,
 *                         CRYPT_NONCE_TYPE_BIS = 2,
 * @param[in]   pd - Reference structure ble_crypt_para_t
 * @return  	0: decryption succeeded; 1: decryption failed
 */
int aes_ll_ccm_decryption(llPhysChnPdu_t *pllPhysChnPdu, u8 role, u8 ll_type, ble_crypt_para_t *pd);






