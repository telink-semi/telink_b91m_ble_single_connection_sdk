/********************************************************************************************************
 * @file	aes.c
 *
 * @brief	This is the source file for B91
 *
 * @author	Ble Group
 * @date	2019
 *
 * @par		Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *			The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *          alter) any information contained herein in whole or in part except as expressly authorized  
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#include "aes.h"
#include "compiler.h"

/**********************************************************************************************************************
 *                                			  local constants                                                       *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                           	local macro                                                        *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                             local data type                                                     *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                              global variable                                                       *
 *********************************************************************************************************************/
_attribute_aes_data_sec_ unsigned int aes_data_buff[8];
unsigned int aes_base_addr = 0xc0000000;
/**********************************************************************************************************************
 *                                              local variable                                                     *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                          local function prototype                                               *
 *********************************************************************************************************************/
/**
 * @brief     This function refer to wait aes crypt done.
 * @param[in] none.
 * @return    none
 */
static inline void aes_wait_done(void);
/**********************************************************************************************************************
 *                                         global function implementation                                             *
 *********************************************************************************************************************/
/**
 * @brief     This function refer to crypt. AES module register must be used by word.
 * @param[in] Key - the key of encrypt.
 * @param[in] data - the plaintext of encrypt.
 * @return    none
 */
void aes_set_key_data(unsigned char *key, unsigned char* data)
{
	unsigned int temp;
	reg_embase_addr = aes_base_addr;  //set the ceva base addr
	for (unsigned char i = 0; i < 4; i++) {
		temp = key[16-(4*i)-4]<<24 | key[16-(4*i)-3]<<16 | key[16-(4*i)-2]<<8 | key[16-(4*i)-1];
		reg_aes_key(i) = temp;
		temp = data[16-(4*i)-4]<<24 | data[16-(4*i)-3]<<16 | data[16-(4*i)-2]<<8 | data[16-(4*i)-1];
		aes_data_buff[i] = temp;
	}

	reg_aes_ptr = (u32)aes_data_buff;
}

/**
 * @brief     This function refer to crypt. AES module register must be used by word.
 * @param[in] result - the result of encrypt.
 * @return    none
 */
void aes_get_result(unsigned char *result)
{
	/* read out the result */
	unsigned char *ptr = (unsigned char *)&aes_data_buff[4];
	for (unsigned char i=0; i<16; i++) {
		result[i] = ptr[15 - i];
	}
}



/**
 * @brief     This function refer to encrypt. AES module register must be used by word.
 * @param[in] Key - the key of encrypt.
 * @param[in] plaintext - the plaintext of encrypt.
 * @param[in] result - the result of encrypt.
 * @return    none
 */
int aes_encrypt(unsigned char *key, unsigned char* plaintext, unsigned char *result)
{

	//set the key
	aes_set_key_data(key, plaintext);

    aes_set_mode(FLD_AES_START);      //cipher mode

    aes_wait_done();

    aes_get_result(result);

    return 1;
}

/**
 * @brief     This function refer to decrypt. AES module register must be used by word.
 * @param[in] Key - the key of decrypt.
 * @param[in] decrypttext - the decrypttext of decrypt.
 * @param[in] result - the result of decrypt.
 * @return    none
 */
int aes_decrypt(unsigned char *key, unsigned char* decrypttext, unsigned char *result)
{
    //set the key
	aes_set_key_data(key, decrypttext);

    aes_set_mode(FLD_AES_START | FLD_AES_MODE);      //decipher mode

    aes_wait_done();

    aes_get_result(result);

	return 1;
}
/**********************************************************************************************************************
  *                    						local function implementation                                             *
  *********************************************************************************************************************/
/**
 * @brief     This function refer to set the base addr of data which use in CEVA module
 * @param[in] addr - the base addr of CEVA data.
 * @return    none
 */
void aes_set_em_base_addr(unsigned int addr){
	aes_base_addr = addr;   //set the ceva base addr
}

/**
 * @brief     This function refer to wait aes crypt done.
 * @param[in] none.
 * @return    none
 */
static inline void aes_wait_done(void)
{
	while(FLD_AES_START == (reg_aes_mode & FLD_AES_START));
}

