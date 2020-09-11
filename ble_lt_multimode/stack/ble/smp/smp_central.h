/********************************************************************************************************
 * @file	smp_central.h
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
#ifndef SMP_CENTRAL_H_
#define SMP_CENTRAL_H_



#if (LL_MASTER_MULTI_CONNECTION)
	#define	PAIR_SLAVE_MAX_NUM            		8
#else
	#define	PAIR_SLAVE_MAX_NUM            		1
#endif


// 8 byte rand  2 byte ediv		16 byte ltk
#define PAIR_INFO_SECTOR_SIZE	 				64
#define PAIR_OFFSET_RAND		 				8
#define PAIR_OFFSET_EDIV		 				16
#define PAIR_OFFSET_LTK			 				32
#define PAIR_OFFSET_IRK			 				48

#define SLAVE_TRIGGER_SMP_FIRST_PAIRING			0   //first pair, slave send security_request to trigger master's pairing&encryption
#define MASTER_TRIGGER_SMP_FIRST_PAIRING		BIT(0)

#define SLAVE_TRIGGER_SMP_AUTO_CONNECT			0   //auto connect, slave send security_request to trigger master's encryption
#define MASTER_TRIGGER_SMP_AUTO_CONNECT			BIT(1)





/**
 * @brief      This function is used to configure trigger pairing or back-connection encryption.
 * @param[in]  trigger - BIT(0): MASTER_TRIGGER_SMP_FIRST_PAIRING;
 *                       BIT(1): MASTER_TRIGGER_SMP_AUTO_CONNECT.
 * @return     none.
 */
void    blm_host_smp_setSecurityTrigger(u8 trigger);


/**
 * @brief      This function is used to configure the bonding storage address.
 * @param[in]  addr - SMP bonding storage start address.
 * @return     none.
 */
void 	blm_smp_configParingSecurityInfoStorageAddr (int addr);


/**
 * @brief      This function is used to process the SMP protocol interaction process.
 * @param[in]  conn_handle - Connection handle.
 * @param[in]  p - SMP protocol exchange data.
 * @return     none.
 */
void 	blm_host_smp_handler(u16 conn_handle, u8 *p);


/**
 * @brief      This function is used to check whether the input address and address type have been bound.
 * @param[in]  adr_type - Address type.
 * @param[in]  addr - Address.
 * @return     0: not found; others: Index of the paired device.
 */
int 	tbl_bond_slave_search(u8 adr_type, u8 * addr);


/**
 * @brief      This function is used to delete the device binding info corresponding to the specified address and address type from the bonding table.
 * @param[in]  adr_type - Address type.
 * @param[in]  addr - Address.
 * @return     0: Failed to delete; 1: Successfully deleted.
 */
int 	tbl_bond_slave_delete_by_adr(u8 adr_type, u8 *addr);


/**
 * @brief      This function is used to delete the device binding info corresponding to the specified address and address type from the bonding table.
 * @param[in]  adr_type - Address type.
 * @param[in]  addr - Address.
 * @return     none.
 */
void 	tbl_bond_slave_unpair_proc(u8 adr_type, u8 *addr);


/**
 * @brief      This function is used to process SMP encryption change event.
 *             Refer to <<Core spec>> 7.7.8 Encryption Change event for detailed instructions
 * @param[in]  status - 0: An encryption change has occurred;
 *                      1 to 255: An attempt to change encryption failed.
 * @param[in]  conn_handle - Connection handle.
 * @param[in]  enc_enable - Encryption_Enabled.
 * @return     none.
 */
void	blm_smp_encChangeEvt(u8 status, u16 conn_handle, u8 enc_enable);


/**
 * @brief      This function is used to initialize SMP central device.
 * @param[in]  none.
 * @return     0: Initialization failed;
 *             1: initialization succeeded.
 */
int 	blc_smp_central_init (void);



#endif /* SMP_CENTRAL_H_ */
