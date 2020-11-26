/********************************************************************************************************
 * @file	smp_alg.h
 *
 * @brief	This is the source file for BLE SDK
 *
 * @author	BLE GROUP
 * @date	2020.06
 *
 * @par		Copyright (c) 2020, Telink Semiconductor (Shanghai) Co., Ltd.
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

#ifndef SMP_ALG_H_
#define SMP_ALG_H_




/**
 * @brief   	this function is used to encrypt the packet for link layer
 * @param[in]   *key - aes key
 * @param[in]   *r - the plaintext
 * @param[in]   *p1 - packet buffer2
 * @param[in]   *p2 - packet buffer2
 * @param[in]   *result - the result of encrypt
 * @return  	none.
 */
void			blt_smp_alg_c1(u8 * key, u8 * r, u8 *p1, u8 *p2, u8 * result);

/**
 * @brief   	this function is used to encrypt the packet for link layer
 * @param[in]   *key - aes key
 * @param[in]   *r1 - the plaintext1
 * @param[in]   *r2 - the plaintext2
 * @param[in]   *result - the result of encrypt
 * @return  	none.
 */
void			blt_smp_alg_s1(u8 * key, u8 * r1, u8 * r2, u8 * result);

/**
 * @brief	This function is used to compute confirm value by function f4
 * @return	0
 */
int				blt_smp_alg_f4 (unsigned char *r, unsigned char u[32], unsigned char v[32], unsigned char x[16], unsigned char z);

/**
 * @brief	This function is used to generate the numeric comparison values during authentication
 * 			stage 1 of the LE Secure Connections pairing process by function g2
 * @return	0
 */
unsigned int	blt_smp_alg_g2 (unsigned char u[32], unsigned char v[32], unsigned char x[16], unsigned char y[16]);

/**
 * @brief	This function is used to generate derived keying material in order to create the LTK
 * 			and keys for the commitment function f6 by function f5
 * @return	0
 */
int 			blt_smp_alg_f5 (unsigned char *mac, unsigned char *ltk, unsigned char w[32], unsigned char n1[16], unsigned char n2[16],
								unsigned char a1[7], unsigned char a2[7]);

/**
 * @brief	This function is used to generate check values during authentication stage 2 of the
 * 			LE Secure Connections pairing process by function f6
 * @return	0
 */
int 			blt_smp_alg_f6 (unsigned char *e, unsigned char w[16], unsigned char n1[16], unsigned char n2[16],
								unsigned char r[16], unsigned char iocap[3], unsigned char a1[7], unsigned char a2[7]);

/**
 * @brief	This function is used to convert keys of a given size from one key type to another
 * 			key type with equivalent strength
 * @return	0
 */
int 			blt_smp_alg_h6 (unsigned char *r, unsigned char key[16], unsigned char id[4]);


/**
 * @brief	This function is used to convert keys of a given size from one key type to another
 * 			key type with equivalent strength
 * 			--- h7(SALT, W) = AES-CMACsalt(W) ---
 * @return	0
 */
int 			blt_smp_alg_h7 (unsigned char *r, unsigned char salt[16], unsigned char w[16]);

/**
 * @brief	This function is used to generate the Group Session Key (GSK) for encrypting or
 * 			decrypting payloads of an encrypted BIS.
 * 			--- h8(K, S, keyID) = AES-CMACik(keyID) ---
 * @return	0
 */
int 			blt_smp_alg_h8 (unsigned char *r, unsigned char k[16], unsigned char s[16], unsigned char keyId[4]);


#endif /* SMP_ALG_H_ */




