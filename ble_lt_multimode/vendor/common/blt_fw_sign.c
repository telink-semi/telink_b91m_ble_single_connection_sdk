/*
 * blt_fw_signature.c
 *
 *  Created on: 2019-5-5
 *      Author: Administrator
 */

#if 1
#include "tl_common.h"
#include "drivers.h"
#include "blt_fw_sign.h"
#include "stack/ble/ble_config.h"
#include "blt_common.h"
#include "proj_lib/firmware_encrypt.h"

/**
 * @brief		This function is used to check digital signature of firmware
 * @param[in]	none
 * @return      none
 */
void blt_firmware_signature_check(void)
{
		unsigned int flash_mid;
		unsigned char flash_uid[16];
		unsigned char signature_enc_key[16];
		int flag = flash_read_mid_uid_with_check(&flash_mid, flash_uid);

		if(flag==0){  //reading flash UID error
			while(1);
		}

		firmware_encrypt_based_on_uid (flash_uid, signature_enc_key);

		if(tmemcmp(signature_enc_key, (u8*)(FLASH_R_BASE_ADDR | (flash_sector_calibration + CALIB_OFFSET_FIRMWARE_SIGNKEY)), 16)){  //signature not match
			while(1);   //user can change the code here to stop firmware running
		}
}
#endif
