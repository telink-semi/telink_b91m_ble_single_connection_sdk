/********************************************************************************************************
 * @file	ota_stack.h
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
#ifndef STACK_BLE_SERVICE_OTA_STACK_H_
#define STACK_BLE_SERVICE_OTA_STACK_H_




#ifndef BLE_OTA_FW_CHECK_EN
#define BLE_OTA_FW_CHECK_EN					1
#endif

#define FW_MAX_SIZE							0x40000			//256K



#define FLAG_FW_CHECK						0x5D
#define FW_CHECK_AGTHM2						0x02



typedef struct{
	u8  ota_start_flag;
	u8  ota_reboot_pending;
#if (BLE_OTA_FW_CHECK_EN)
	u8 	fw_check_en;
	u8  fw_check_match;

	u32 fw_crc_init;

	u16 fw_crc_last_index;
#endif
	u8  ota_forbid_flag;
	u8  ota_response_pending;
}ota_service_t;

extern ota_service_t blcOta;


extern u32	blt_ota_start_tick;
extern u32	blt_ota_receive_pkt_tick;
extern u32	blt_ota_timeout_us;

extern u32	ota_program_offset;
extern int 	ota_firmware_size_k;

extern u32	ota_version_flashAddr;
extern u32	ota_version_number;

extern u8	ota_response_buf[10];
extern u32 	ota_reboot_tick;


void bls_ota_procTimeout(void);
void bls_ota_conn_terminate_clear_flag(void);
void bls_ota_response(void);
void bls_ota_reboot(void);




#endif /* STACK_BLE_SERVICE_OTA_STACK_H_ */
