/********************************************************************************************************
 * @file	le_crypto.h
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
#ifndef TN_DTLS_H
#define TN_DTLS_H

#define		biL			32

#define BITS_TO_LIMBS(i)  ( (i) / 32 + ( (i) % 32 != 0 ) )



#define TNDTLS_ERR_MPI_BAD_INPUT_DATA                    -0x0004  // Bad input parameters to function.
#define TNDTLS_ERR_MPI_INVALID_CHARACTER                 -0x0006  // There is an invalid character in the digit string.
#define TNDTLS_ERR_MPI_BUFFER_TOO_SMALL                  -0x0008  // The buffer is too small to write to.
#define TNDTLS_ERR_MPI_NEGATIVE_VALUE                    -0x000A  // The input arguments are negative or result in illegal output.
#define TNDTLS_ERR_MPI_DIVISION_BY_ZERO                  -0x000C  // The input argument for division is zero, which is not allowed.
#define TNDTLS_ERR_MPI_NOT_ACCEPTABLE                    -0x000E  // The input arguments are not acceptable.


typedef struct {
	int	s;
	int n;
	unsigned int p[8];
} tn_mpi;

typedef struct {
	int	s;
	int n;
	unsigned int p[16];
} tn_mpi16;

typedef struct
{
    tn_mpi X;
    tn_mpi Y;
    tn_mpi Z;
} tn_ecp_point;

typedef struct
{
	tn_ecp_point	X1X3;			// XsXc
	tn_ecp_point	X2X4;			//
	tn_ecp_point	GaGb;			// Peer's G
    tn_mpi			m2m4;			// private key
    tn_mpi			xcxs;			// key exchange
    tn_mpi			s;				// passphrase
} tn_ecjpake_context;

extern const unsigned int tn_p256_nq[9];

extern const unsigned int tn_p256_n[10];

extern const unsigned int tn_p256_pq[9];
 
extern const unsigned int tn_p256_pr[8];

extern const unsigned int tn_p256_p[10];

extern const tn_ecp_point	tn_t[16];

extern const tn_ecp_point tn_p256_ecp_g;

void tn_mpi_init( tn_mpi *X, int n);
void tn_mpi_free( tn_mpi *X );
int tn_mpi_copy( tn_mpi *X, const tn_mpi *Y );
int tn_mpi_cmp_mpi( const tn_mpi *X, const tn_mpi *Y );
void tn_mpi_modn_add (tn_mpi * px);
void tn_mpi_modp_add (tn_mpi * px);
void tn_mpi_modp_sub (tn_mpi * px);
int tn_mpi_mul_mpi_modp( tn_mpi *X, const tn_mpi *A, const tn_mpi *B );
int tn_mpi_mul_int_modp( tn_mpi *X, const tn_mpi *A,  int n );
int tn_mpi_add_mpi_modp( tn_mpi *X, const tn_mpi *A, const tn_mpi *B );
int tn_mpi_sub_mpi_modp( tn_mpi *X, const tn_mpi *A, const tn_mpi *B );
int tn_mpi_sub_mpi_modn( tn_mpi *X, const tn_mpi *A, const tn_mpi *B );
int tn_mpi_shift_l_modp( tn_mpi *X, int count );
int tn_mpi_add_mpi( tn_mpi *X, const tn_mpi *A, const tn_mpi *B );
int tn_mpi_sub_mpi( tn_mpi *X, const tn_mpi *A, const tn_mpi *B );
int tn_mpi_cmp_int( tn_mpi *X, int z );
int tn_mpi_lset( tn_mpi *X, int z );
int tn_mpi_bitlen( const tn_mpi *X );
int tn_mpi_get_bit( const tn_mpi *X, int pos );
int tn_mpi_modp( tn_mpi *R, const tn_mpi *A);
int tn_mpi_safe_cond_assign( tn_mpi *X, const tn_mpi *Y, unsigned char assign );
int tn_mpi_inv_mod( tn_mpi *X, const tn_mpi *A );
int tn_mpi_mul_mpi_modn( tn_mpi *X, const tn_mpi *A, const tn_mpi *B );
int tn_mpi_neg_modn( tn_mpi *X );

//////////////////////////////////////////////////////////////////////////////////
int tn_ecp_copy( tn_ecp_point *P, const tn_ecp_point *Q );
int tn_ecp_double_jac( tn_ecp_point *R, const tn_ecp_point *P );
int tn_ecp_add_mixed( tn_ecp_point *R, const tn_ecp_point *P, const tn_ecp_point *Q );
int tn_ecp_normalize_jac( tn_ecp_point *pt );
int tn_ecp_mul(tn_ecp_point *R, const tn_mpi *m, const tn_ecp_point *P);
int tn_ecp_muladd( tn_ecp_point *R, const tn_mpi *m, const tn_ecp_point *P, const tn_mpi *n, const tn_ecp_point *Q );

extern void tn_ecp_gen_keypair (tn_mpi *d, tn_ecp_point *Q);
extern void tn_p256_keypair (unsigned char *s, unsigned char *x, unsigned char *y);
extern void tn_p256_dhkey (unsigned char *r, unsigned char *s, unsigned char * x, unsigned char *y);

extern void tn_aes_128(unsigned char *key, unsigned char *plaintext, unsigned char *result);

/**
 * @brief	RFC-44931 defines the Cipher-based Message Authentication Code (CMAC)
 * 			that uses AES-128 as the block cipher function, also known as AES-CMAC.
 * @return	none.
 */
extern void tn_aes_cmac ( unsigned char *key, unsigned char *input, int length,
                  unsigned char *mac );

/**
 * @brief	This function is used to compute confirm value by function f4
 * @return	0
 */
extern int tn_crypto_f4 (unsigned char *r, unsigned char u[32], unsigned char v[32], unsigned char x[16], unsigned char z);

/**
 * @brief	This function is used to generate the numeric comparison values during authentication
 * 			stage 1 of the LE Secure Connections pairing process by function g2
 * @return	0
 */
extern unsigned int tn_crypto_g2 (unsigned char u[32], unsigned char v[32], unsigned char x[16], unsigned char y[16]);

/**
 * @brief	This function is used to generate derived keying material in order to create the LTK
 * 			and keys for the commitment function f6 by function f5
 * @return	0
 */
extern int tn_crypto_f5 (unsigned char *mac, unsigned char *ltk, unsigned char w[32], unsigned char n1[16], unsigned char n2[16],
				  unsigned char a1[7], unsigned char a2[7]);

/**
 * @brief	This function is used to generate check values during authentication stage 2 of the
 * 			LE Secure Connections pairing process by function f6
 * @return	0
 */
extern int tn_crypto_f6 (unsigned char *e, unsigned char w[16], unsigned char n1[16], unsigned char n2[16],
				  unsigned char r[16], unsigned char iocap[3], unsigned char a1[7], unsigned char a2[7]);

/**
 * @brief	This function is used to convert keys of a given size from one key type to another
 * 			key type with equivalent strength
 * @return	0
 */
extern int tn_crypto_h6 (unsigned char *r, unsigned char key[16], unsigned char id[4]);

/**
 * @brief	This function is used to test secure connection crypto function
 * @return	0: success
 * 			1: fail
 */
extern int test_crypto_func ();

/**
 * @brief	This function is used to test secure connection dhkey function
 * @return	0: success
 * 			1: fail
 */
extern int test_dhkey ();

#endif 
