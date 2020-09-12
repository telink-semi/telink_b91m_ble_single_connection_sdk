/********************************************************************************************************
 * @file	test_compress.c
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

#ifndef uECC_TEST_NUMBER_OF_ITERATIONS
#define uECC_TEST_NUMBER_OF_ITERATIONS   256
#endif

#if LPC11XX

#include "/Projects/lpc11xx/peripherals/uart.h"
#include "/Projects/lpc11xx/peripherals/time.h"

static uint64_t g_rand = 88172645463325252ull;
int fake_rng(unsigned char *dest, unsigned size) {
    while(size) {
        g_rand ^= (g_rand << 13);
        g_rand ^= (g_rand >> 7);
        g_rand ^= (g_rand << 17);

        unsigned amount = (size > 8 ? 8 : size);
        tmemcpy(dest, &g_rand, amount);
        dest += amount;
        size -= amount;
    }
    return 1;
}

#endif

void vli_print(char *str, unsigned char *vli, unsigned int size) {
    printf("%s ", str);
    while (size) {
        printf("%02X ", (unsigned)vli[size - 1]);
        --size;
    }
    printf("\n");
}

int main() {
#if LPC11XX
    uartInit(BAUD_115200);
        initTime();
        
    uECC_set_rng(&fake_rng);
#endif

    unsigned char public[uECC_BYTES * 2];
    unsigned char private[uECC_BYTES];
    unsigned char compressed_point[uECC_BYTES + 1];
    unsigned char decompressed_point[uECC_BYTES * 2];

    int i;
    printf("Testing compression and decompression of %d random EC points\n", uECC_TEST_NUMBER_OF_ITERATIONS);

    for (i = 0; i < uECC_TEST_NUMBER_OF_ITERATIONS; ++i) {
        printf(".");
    #if !LPC11XX
        fflush(stdout);
    #endif

        /* Generate arbitrary EC point (public) on Curve */
        if (!uECC_make_key(public, private)) {
            printf("uECC_make_key() failed\n");
            continue;
        }

        /* compress and decompress point */
        uECC_compress(public, compressed_point);
        uECC_decompress(compressed_point, decompressed_point);

        if (tmemcmp(public, decompressed_point, 2 * uECC_BYTES) != 0) {
            printf("Original and decompressed points are not identical!\n");
            vli_print("Original point =     ", public, 2 * uECC_BYTES);
            vli_print("Compressed point =   ", compressed_point, uECC_BYTES + 1);
            vli_print("Decompressed point = ", decompressed_point, 2 * uECC_BYTES);
        }
    }
    printf("\n");

    return 0;
}

#endif
