/********************************************************************************************************
 * @file     smp_storage.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#ifndef SMP_STORAGE_H_
#define SMP_STORAGE_H_









typedef enum {
	Index_Update_by_Pairing_Order = 0,     //default value
	Index_Update_by_Connect_Order = 1,
} index_updateMethod_t;


/*
 * smp parameter need save to flash.
 * */

#define DEVICE_IDX_MASK				0x03
#define IS_PEER_ADDR_RES_SUPPORT(peerAddrResSuppFlg)	(!(peerAddrResSuppFlg & BIT(7)))

typedef union {
	struct{
		u8 	device_idx	 :2;
		u8 	rsvd		 :6;
	};
	u8 cflg_pack;
}comb_flg_t;  //combine flag type


typedef struct {  //82
	u8		flag;
	u8		peer_addr_type;  //address used in link layer connection
	u8		peer_addr[6];

	comb_flg_t	cflg_union;
	u8		peer_id_adrType; //peer identity address information in key distribution, used to identify
	u8		peer_id_addr[6];


	u8 		own_ltk[16];      //own_ltk[16]
	u8		peer_irk[16];
	u8		local_irk[16];

}smp_param_save_t;


/**
 * @brief      This function is used to get the number of currently bound devices.
 * @param[in]  none.
 * @return     The number of currently bound devices.
 */
u8			blc_smp_param_getCurrentBondingDeviceNumber(void);


/**
 * @brief      This function is used to configure the bonding storage address.
 * @param[in]  addr - SMP bonding storage start address.
 * @return     none.
 */
void 		bls_smp_configParingSecurityInfoStorageAddr (int addr);


/**
 * @brief      This function is used to obtain device binding information based on Index.
 * @param[in]  index - Device bonding index number.
 * @param[out] smp_param_load - The value can refer to the structure 'smp_param_save_t'.
 * @return     0: Failed to load binding information;
 *             others: FLASH address of the information area.
 */
u32 		bls_smp_param_loadByIndex(u8 index, smp_param_save_t* smp_param_load);


/**
 * @brief      This function is used to obtain binding information according to the master address and address type.
 * @param[in]  device_num - Set the maximum number of devices that can be bound.
 * @param[in]  adr_type - Address type.
 * @param[in]  addr - Address.
 * @param[out] smp_param_load - The value can refer to the structure 'smp_param_save_t'.
 * @return     0: Failed to load binding information;
 *             others: FLASH address of the information area.
 */
u32			bls_smp_param_loadByAddr(u8 addr_type, u8* addr, smp_param_save_t* smp_param_load);


/**
 * @brief      This function is used to configure the storage order of binding information.
 * @param[in]  method - The storage order of binding info method value can refer to the structure 'index_updateMethod_t'.
 *                      0: Index update by pairing order;
 *                      1: Index update by connect order.
 * @return     none.
 */
void		bls_smp_setIndexUpdateMethod(index_updateMethod_t method);


/**
 * @brief      This function is used for the slave device to clear all binding information stored in the local FLASH.
 * @param[in]  none.
 * @return     none.
 */
void		bls_smp_eraseAllParingInformation(void);








/**
 * @brief      This function is used to get the number of currently bonding peer devices for multiple local device application.
 * @param[in]  local_dev_idx - local device index
 * @return     The number of currently bound devices.
 */
int			blc_smp_multi_device_param_getCurrentBondingDeviceNumber(int local_dev_idx);



/**
 * @brief      This function is used to obtain peer device binding information based on index for multiple local device application.
 * @param[in]  bond_dev_idx - bonding peer device index.
 * @param[in]  local_dev_idx - local device index
 * @param[out] smp_param_load - The value can refer to the structure 'smp_param_save_t'.
 * @return     0: Failed to load binding information;
 *             others: FLASH address of the information area.
 */
u32 		bls_smp_multi_device_param_loadByIndex(int local_dev_idx, int bond_dev_idx, smp_param_save_t* smp_param_load);


int			blc_smp_setPeerAddrResSupportFlg(u32 flash_addr, u8 support);



#endif /* SMP_STORAGE_H_ */
