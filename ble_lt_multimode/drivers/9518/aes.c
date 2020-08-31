/*
 * AES.c
 *
 *  Created on: Jun 17, 2020
 *      Author: Telink
 */
#include "aes.h"
#include "compiler.h"
_attribute_aes_data_sec_ unsigned int aes_data_buff[8];
unsigned int aes_base_addr = 0xc0000000;

/**
 * @brief     This function refer to crypt. AES module register must be used by word.
 * @param[in] Key - the key of encrypt.
 * @param[in] plaintext - the plaintext of encrypt.
 * @return    none
 */
void aes_set_key_data(unsigned char *key, unsigned char* decrypttext)
{
	unsigned int temp;
	reg_embase_addr = aes_base_addr;  //set the ceva base addr
	for (unsigned char i = 0; i < 4; i++) {
		temp = key[16-(4*i)-4]<<24 | key[16-(4*i)-3]<<16 | key[16-(4*i)-2]<<8 | key[16-(4*i)-1];
		reg_aes_key(i) = temp;
		temp = decrypttext[16-(4*i)-4]<<24 | decrypttext[16-(4*i)-3]<<16 | decrypttext[16-(4*i)-2]<<8 | decrypttext[16-(4*i)-1];
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
 * @brief     This function refer to wait aes crypt done.
 * @param[in] none.
 * @return    none
 */
static inline void wait_aes_done(void)
{
	while(FLD_AES_START == (reg_aes_mode & FLD_AES_START));
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

    wait_aes_done();

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

    wait_aes_done();

    aes_get_result(result);

	return 1;
}
/**
 * @brief     This function refer to set the base addr of data which use in CEVA module
 * @param[in] addr - the base addr of CEVA data.
 * @return    none
 */
void aes_set_em_base_addr(unsigned int addr){
	aes_base_addr = addr;   //set the ceva base addr
}
