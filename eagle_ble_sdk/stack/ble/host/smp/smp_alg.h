/********************************************************************************************************
 * @file	smp_alg.h
 *
 * @brief	This is the header file for BLE SDK
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

#ifndef SMP_ALG_H_
#define SMP_ALG_H_




/**
 * @brief   	this function is used to generate the confirm values
 * @param[in]   *key - aes key, little--endian.
 * @param[in]   *r - the plaintext, little--endian.
 * @param[in]   *pres - packet buffer2, little--endian.
 * @param[in]   *preq - packet buffer2, little--endian.
 * @param[in]   iat - initiate address type
 * @param[in]   *ia - packet buffer2, little--endian.
 * @param[in]   rat - response address type
 * @param[in]   *ra - packet buffer2, little--endian.
 * @param[out]  *c1 - the confirm value, little--endian.
 * @return  	none.
 * @Note		Input data requires strict Word alignment
 */
void 			blt_smp_alg_c1(u8 key[16], u8 r[16], u8 pres[7], u8 preq[7], u8 iat, u8 ia[6], u8 rat, u8 ra[6], u8 c1[16]);

/**
 * @brief   	this function is used to generate the STK during the LE legacy pairing process.
 * @param[in]   *key - aes key, little--endian.
 * @param[in]   *r1 - the plaintext1, little--endian.
 * @param[in]   *r2 - the plaintext2, little--endian.
 * @param[out]  *STK - the result of encrypt, little--endian.
 * @return  	none.
 * @Note		Input data requires strict Word alignment
 */
void			blt_smp_alg_s1(u8 key[16], u8 r1[16], u8 r2[16], u8 stk[16]);

/**
 * @brief		This function is used to compute confirm value by function f4
 * 				---  Ca: f4(U, V, X, Z) = AES-CMACX (U || V || Z)  ---
 * @param[in]   u: 	is the 256-bits, big--endian.
 * @param[in]   v: 	is the 256-bits, big--endian.
 * @param[in]   x:  is the 128-bits key, big--endian.
 * @param[in]   z:  is the 8-bits
 * @param[out]  *r: the output of the confirm value:128-bits, big--endian.
 * @return	none.
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




