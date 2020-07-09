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

static inline void aes_reset(void)
{
	REG_ADDR32(0x1600b0) = BIT(7);
}

int aes_encrypt(unsigned int *Key, unsigned int* cipher_data);

int aes_decrypt(unsigned int *Key, unsigned int* cipher_data);

#endif /* _AES_H_ */
