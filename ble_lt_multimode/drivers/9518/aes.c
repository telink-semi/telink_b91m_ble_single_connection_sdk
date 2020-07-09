/*
 * AES.c
 *
 *  Created on: Jun 17, 2020
 *      Author: Telink
 */
#include "aes.h"

int aes_encrypt(unsigned int *Key, unsigned int* cipher_data)
{
//	write_reg32(0x170304, 0xc0200000);
	//reg_aes_mode = 0;   //cipher mode
	write_reg32(0x16000c, 0x00000080);
    //set the key
    for (unsigned char i = 0; i < 4; i++) {
    	reg_aes_key(i) = Key[i];
    }

    reg_aes_ptr = (u32)cipher_data;

    reg_aes_mode = FLD_AES_START;   //cipher mode

    while(FLD_AES_START == (reg_aes_mode & FLD_AES_START));
//    for(volatile unsigned int i = 0; i < 0xffff; i++);

    return 1;
}

int aes_decrypt(unsigned int *Key, unsigned int* cipher_data)
{
	write_reg32(0x16000c, 0x80);
    //set the key
    for (unsigned char i = 0; i < 4; i++) {
    	reg_aes_key(i) = Key[i];
    }

    reg_aes_ptr = (u32)cipher_data;

    reg_aes_mode = FLD_AES_START | FLD_AES_MODE;   //cipher mode

    while(FLD_AES_START == (reg_aes_mode & FLD_AES_START));

	return 1;
}

