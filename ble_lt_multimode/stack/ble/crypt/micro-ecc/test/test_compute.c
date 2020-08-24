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

