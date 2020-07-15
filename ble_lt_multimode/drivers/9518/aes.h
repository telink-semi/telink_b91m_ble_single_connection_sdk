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
int aes_encrypt(unsigned char *Key, unsigned char* plaintext, unsigned char *result);

/**
 * @brief     This function refer to encrypt. AES module register must be used by word.
 * @param[in] Key - the key of decrypt.
 * @param[in] decrypttext - the decrypttext of decrypt.
 * @param[in] result - the result of decrypt.
 * @return    none
 */
int aes_decrypt(unsigned char *Key, unsigned char* decrypttext, unsigned char *result);

#endif /* _AES_H_ */
