/*
 * aes.h
 *
 *  Created on: Jun 17, 2020
 *      Author: Telink
 */

#ifndef _AES_H_
#define _AES_H_

#include "compiler.h"
#include "./reg_include/aes_reg.h"

/**
 * @brief     This function refer to encrypt. AES module register must be used by word.
 * @param[in] Key - the key of encrypt.
 * @param[in] plaintext - the plaintext of encrypt.
 * @param[in] result - the result of encrypt.
 * @return    none
 */
int aes_encrypt(unsigned char *key, unsigned char* plaintext, unsigned char *result);

/**
 * @brief     This function refer to decrypt. AES module register must be used by word.
 * @param[in] Key - the key of decrypt.
 * @param[in] decrypttext - the decrypttext of decrypt.
 * @param[in] result - the result of decrypt.
 * @return    none
 */
int aes_decrypt(unsigned char *key, unsigned char* decrypttext, unsigned char *result);

/**
 * @brief     This function refer to set the base addr of data which use in CEVA module
 * @param[in] addr - the base addr of CEVA data.
 * @return    none
 */
void aes_set_em_base_addr(unsigned int addr);

/**
 * @brief     This function refer to crypt. AES module register must be used by word.
 * @param[in] Key - the key of encrypt.
 * @param[in] data - the plaintext of encrypt.
 * @return    none
 */
void aes_set_key_data(unsigned char *key, unsigned char* data);

/**
 * @brief     This function refer to crypt. AES module register must be used by word.
 * @param[in] result - the result of encrypt.
 * @return    none
 */
void aes_get_result(unsigned char *result);

/**
 * @brief     This function refer to set aes mode.
 * @param[in] mode - the irq mask.
 * @return    none
 */
static inline void aes_set_mode(aes_mode_e mode)
{
	reg_aes_mode = (mode);
}

#endif /* _AES_H_ */
