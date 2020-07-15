/*
 * AES.c
 *
 *  Created on: Jun 17, 2020
 *      Author: Telink
 */
#include "aes.h"
unsigned int aes_data_buff[8];

/**
 * @brief     This function refer to encrypt. AES module register must be used by word.
 * @param[in] Key - the key of encrypt.
 * @param[in] plaintext - the plaintext of encrypt.
 * @param[in] result - the result of encrypt.
 * @return    none
 */
int aes_encrypt(unsigned char *Key, unsigned char* plaintext, unsigned char *result)
{
	unsigned int temp;
	write_reg32(0x140b04, 0xc0200000);  //set the ceva base addr
	write_reg32(0x16000c, 0x00000080);
    //set the key
    for (unsigned char i = 0; i < 4; i++) {
    	temp = Key[16-(4*i)-4]<<24 | Key[16-(4*i)-3]<<16 | Key[16-(4*i)-2]<<8 | Key[16-(4*i)-1];
    	reg_aes_key(i) = temp;
    	aes_data_buff[i] = ((unsigned int*)plaintext)[i];
    }

    reg_aes_ptr = (u32)aes_data_buff;

    reg_aes_mode = FLD_AES_START;   //cipher mode

    while(FLD_AES_START == (reg_aes_mode & FLD_AES_START));

    /* read out the result */
	unsigned char *ptr = (unsigned char *)&aes_data_buff[4];
	for (unsigned char i=0; i<16; i++) {
		result[i] = ptr[i];
	}

    return 1;
}

/**
 * @brief     This function refer to encrypt. AES module register must be used by word.
 * @param[in] Key - the key of decrypt.
 * @param[in] decrypttext - the decrypttext of decrypt.
 * @param[in] result - the result of decrypt.
 * @return    none
 */
int aes_decrypt(unsigned char *Key, unsigned char* decrypttext, unsigned char *result)
{
	unsigned int temp;
	write_reg32(0x140b04, 0xc0200000);   //set the ceva base addr
	write_reg32(0x16000c, 0x80);
    //set the key
    for (unsigned char i = 0; i < 4; i++) {
    	temp = Key[16-(4*i)-4]<<24 | Key[16-(4*i)-3]<<16 | Key[16-(4*i)-2]<<8 | Key[16-(4*i)-1];
    	reg_aes_key(i) = temp;
    	aes_data_buff[i] = ((unsigned int*)decrypttext)[i];
    }

    reg_aes_ptr = (u32)aes_data_buff;

    reg_aes_mode = FLD_AES_START | FLD_AES_MODE;   //cipher mode

    while(FLD_AES_START == (reg_aes_mode & FLD_AES_START));

    /* read out the result */
	unsigned char *ptr = (unsigned char *)&aes_data_buff[4];
	for (unsigned char i=0; i<16; i++) {
		result[i] = ptr[i];
	}

	return 1;
}

