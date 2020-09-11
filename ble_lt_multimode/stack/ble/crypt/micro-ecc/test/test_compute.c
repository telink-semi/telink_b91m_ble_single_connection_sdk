/********************************************************************************************************
 * @file	test_compute.c
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
/* Copyright 2014, Kenneth MacKay. Licensed under the BSD 2-clause license. */

#ifdef MICRO_ECC_TEST

#include "uECC.h"

#include <stdio.h>
#include <string.h>

void vli_print(unsigned char *vli, unsigned int size) {
    while (size) {
        printf("%02X ", (unsigned)vli[size - 1]);
        --size;
    }
}

int main() {
    int i;
    int success;
    unsigned char private[uECC_BYTES];
    unsigned char public[uECC_BYTES * 2];
    unsigned char public_computed[uECC_BYTES * 2];

    printf("Testing 256 random private key pairs\n");
    for (i = 0; i < 256; ++i) {
        printf(".");
    #if !LPC11XX
        fflush(stdout);
    #endif

        success = uECC_make_key(public, private);
        if (!success) {
            printf("uECC_make_key() failed\n");
            return 1;
        }

        success = uECC_compute_public_key(private, public_computed);
        if (!success) {
            printf("uECC_compute_public_key() failed\n");
        }

        if (tmemcmp(public, public_computed, sizeof(public)) != 0) {
            printf("Computed and provided public keys are not identical!\n");
            printf("Computed public key = ");
            vli_print(public_computed, uECC_BYTES);
            printf("\n");
            printf("Provided public key = ");
            vli_print(public, uECC_BYTES);
            printf("\n");
            printf("Private key = ");
            vli_print(private, uECC_BYTES);
            printf("\n");
        }
    }

    printf("\n");
    printf("Testing private key = 0\n");

    tmemset(private, 0, uECC_BYTES);
    success = uECC_compute_public_key(private, public_computed);
    if (success) {
        printf("uECC_compute_public_key() should have failed\n");
    }

    return 0;
}

#endif

