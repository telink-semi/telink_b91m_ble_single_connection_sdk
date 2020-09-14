/********************************************************************************************************
 * @file	aes_ccm.h
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
#pragma  once

#include <stack/ble/ll/ll.h>
#include "stack/ble/ll/ll_conn/conn_stack.h"

#define AES_BLOCK_SIZE     16


//#define		SUCCESS			0
enum {
    AES_SUCC = SUCCESS,
    AES_NO_BUF,               
    AES_FAIL,
};


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

/**
 * @brief   	this function is used to calculate the aes_ccm value
 * @param[in]   micLen - mic_lenth (should be 4)
 * @param[in]   *key - aes key
 * @param[in]   iv - initial vector (should be 13 bytes nonce)
 * @param[in]   mStr - plaint_text
 * @param[in]   mStrLen - plaint_text length
 * @param[in]   aStr -  a string  (should be AAD the data channel PDU header first octet with NESN, SN and MD bits masked to 0)
 * @param[in]   aStrLen - a string length (should be 1)
 * @param[in]   result - result (result)
 * @return  	0
 */
u8 aes_ccmAuthTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u16 aStrLen, u8 *result);

/**
 * @brief   	this function is used to decrypt the aes_ccm value
 * @param[in]   micLen - mic_lenth (should be 4)
 * @param[in]   *key - aes key
 * @param[in]   iv - initial vector (should be 13 bytes nonce)
 * @param[in]   mStr - plaint_text
 * @param[in]   mStrLen - plaint_text length
 * @param[in]   aStr -  a string  (should be AAD the data channel PDU header first octet with NESN, SN and MD bits masked to 0)
 * @param[in]   aStrLen - a string length (should be 1)
 * @param[in]   *mic - compare with the result of aes_ccmAuthTran
 * @return  	0 : success
 * 				-1 : fail
 */
u8 aes_ccmDecAuthTran(u8 micLen, u8 *key, u8 *iv, u8 *mStr, u16 mStrLen, u8 *aStr, u8 aStrLen, u8 *mic);

/**
 * @brief   	this function is used to initialize the key for AES module
 * @param[in]   *key - aes key
 * @return  	0 : success
 */
u8 aes_initKey(u8 *key);

/**
 * @brief   	this function is used to encrypt the aes_ccm value
 * @param[in]   *key - aes key
 * @param[in]   iv - initial vector (should be 13 bytes nonce)
 * @param[in]   aStr -  a string  (should be AAD the data channel PDU header first octet with NESN, SN and MD bits masked to 0)
 * @param[in]   *mic - the mic of aes_ccmAuthTran
 * @param[in]   mStrLen - plaint_text length
 * @param[in]   *mStr - plaint_text
 * @param[in]   *result - return mStr
 * @return  	0 : success
 */
u8 aes_ccm_encryption(u8 *key, u8 *iv, u8 *aStr, u8 *mic, u8 mStrLen, u8 *mStr, u8 *result);

/**
 * @brief   	this function is used to decrypt the aes_ccm value
 * @param[in]   *key - aes key
 * @param[in]   iv - initial vector (should be 13 bytes nonce)
 * @param[in]   aStr -  a string  (should be AAD the data channel PDU header first octet with NESN, SN and MD bits masked to 0)
 * @param[in]   *mic - the mic of aes_ccmAuthTran
 * @param[in]   mStrLen - plaint_text length
 * @param[in]   *mStr - plaint_text
 * @param[in]   *result - return mStr
 * @return  	0 : success
 */
u8 aes_ccm_decryption(u8 *key, u8 *iv, u8 *aStr, u8 *mic, u8 mStrLen, u8 *mStr, u8 *result);




/**
 * @brief   	this function is used to decrypt the aes_ccm value
 * @param[in]   *key - aes key
 * @param[in]   *plaintext - plaint_text
 * @param[in]   *result - return encrypt result
 * @return  	none.
 */
void aes_ll_encryption(u32* key, u32* plaintext, u8 *result);

/**
 * @brief   	this function is used to encrypt the packet for ATT
 * @param[in]   *key - aes key
 * @param[in]   *iv - initial vector
 * @param[in]   *mic - the mic of aes_ccmAuthTran
 * @param[in]   *micLen - mic_lenth
 * @param[in]   *ps - att packet
 * @param[in]   len - the length of packet
 * @return  	1
 */
u8 aes_att_encryption_packet(u8 *key, u8 *iv, u8 *mic, u8 mic_len, u8 *ps, u8 len);

/**
 * @brief   	this function is used to decrypt the packet for ATT
 * @param[in]   *key - aes key
 * @param[in]   *iv - initial vector
 * @param[in]   *mic - the mic of aes_ccmAuthTran
 * @param[in]   *micLen - mic_lenth
 * @param[in]   *ps - att packet
 * @param[in]   len - the length of packet
 * @return  	1
 */
u8 aes_att_decryption_packet(u8 *key, u8 *iv, u8 *mic, u8 mic_len, u8 *ps, u8 len);

/**
 * @brief   	this function is used to encrypt the packet for link layer
 * @param[in]   *key - aes key
 * @param[in]   *r - the plaintext
 * @param[in]   *p1 - packet buffer2
 * @param[in]   *p2 - packet buffer2
 * @param[in]   *result - the result of encrypt
 * @return  	none.
 */
void aes_ll_c1(u8 * key, u8 * r, u8 *p1, u8 *p2, u8 * result);

/**
 * @brief   	this function is used to encrypt the packet for link layer
 * @param[in]   *key - aes key
 * @param[in]   *r1 - the plaintext1
 * @param[in]   *r2 - the plaintext2
 * @param[in]   *result - the result of encrypt
 * @return  	none.
 */
void aes_ll_s1(u8 * key, u8 * r1, u8 * r2, u8 * result);

