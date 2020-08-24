/********************************************************************************************************
 * @file     ll_ext_adv.h
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Feb. 1, 2018
 *
 * @par      Copyright (c) Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *			 The information contained herein is confidential and proprietary property of Telink
 * 		     Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *			 of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *			 Co., Ltd. and the licensee in separate contract or the terms described here-in.
 *           This heading MUST NOT be removed from this file.
 *
 * 			 Licensees are granted free, non-transferable use of the information in this
 *			 file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/

#ifndef LL_ADV_EXT_H_
#define LL_ADV_EXT_H_


#include <stack/ble/hci/hci_cmd.h>
#include <stack/ble/ll/ll_adv.h>
#include <stack/ble/ll/ll_stack.h>



#define 		MAX_LENGTH_PRIMARY_ADV_PKT						44   //sizeof(rf_pkt_pri_adv_t) = 43

#define 		MAX_LENGTH_SECOND_ADV_PKT						264   //sizeof(rf_pkt_ext_adv_t) = 261





//NOTE: this data structure must 4 bytes aligned
typedef struct
{
    u8		adv_handle;
    u8 		extAdv_en;
    u8 		adv_chn_mask;
    u8		adv_chn_num;

	u8 		own_addr_type;
	u8 		peer_addr_type;
    u8 		pri_phy;
    u8 		sec_phy;


    u8 		max_ext_adv_evt;
    u8 		run_ext_adv_evt;
    u8		unfinish_advData;
    u8		unfinish_scanRsp;


	u8		adv_filterPolicy;
    u8 		scan_req_noti_en;
    u8 		coding_ind;					//s2 or s8
    u8		param_update_flag;


	u8		with_aux_adv_ind;   //ADV_EXT_IND  with AUX_ADV_IND
	u8		with_aux_chain_ind;
	u8 		rand_adr_flg;
    u8 		adv_sid;


    // u8 		s_adv_max_skip;


	u16     adv_did; 	// BIT<11:0>
	u16 	evt_props;
	u16		advInt_use;
	u16		send_dataLen;
    u16 	maxLen_advData;			//for each ADV sets, this value can be different to save SRAM
    u16 	curLen_advData;
    u16 	maxLen_scanRsp;			//for each ADV sets, this value can be different to save SRAM
    u16 	curLen_scanRsp;

    u16		send_dataLenBackup;
    u16		rsvd_16_1;


	u32 	adv_duration_tick;
	u32 	adv_begin_tick;				//24
    u32		adv_event_tick;

	u8*		dat_extAdv;
	u8*		dat_scanRsp;                //Scan response data.
	rf_pkt_pri_adv_t*		primary_adv;
	rf_pkt_ext_adv_t*		secondary_adv;

	u8 		rand_adr[6];
	u8 		peer_addr[6];
}ll_ext_adv_t;


#define ADV_SET_PARAM_LENGTH				(sizeof(ll_ext_adv_t))   //sizeof(ll_ext_adv_t) =  ,  must 4 byte aligned












//initialization
void 		blc_ll_initExtendedAdvertising_module(	u8 *pAdvCtrl, u8 *pPriAdv,int num_sets);

void 		blc_ll_initExtSecondaryAdvPacketBuffer(u8 *pSecAdv, int sec_adv_buf_len);

void 		blc_ll_initExtAdvDataBuffer(u8 *pExtAdvData, int max_len_advData);								//set AdvData buffer for all adv_set
void 		blc_ll_initExtAdvDataBuffer_by_advHandle(u8 *pExtAdvData, u8 advHandle, int max_len_advData);  //set AdvData buffer for specific adv_set

void 		blc_ll_initExtScanRspDataBuffer(u8 *pScanRspData, int max_len_scanRspData);
void 		blc_ll_initExtScanRspDataBuffer_by_advHandle(u8 *pScanRspData,  u8 advHandle, int max_len_scanRspData);



//Set Extended ADV parameters
ble_sts_t	blc_ll_setAdvRandomAddr(u8 advHandle, u8* rand_addr);


ble_sts_t 	blc_ll_setExtAdvParam(  u8 advHandle, 		 			advEvtProp_type_t adv_evt_prop, u32 pri_advIntervalMin, 		u32 pri_advIntervalMax,
									u8 pri_advChnMap,	 			own_addr_type_t ownAddrType, 	u8 peerAddrType, 			u8  *peerAddr,
									adv_fp_type_t advFilterPolicy,  tx_power_t adv_tx_pow,			le_phy_type_t pri_adv_phy, 	u8 sec_adv_max_skip,
									le_phy_type_t sec_adv_phy, 	 	u8 adv_sid, 					u8 scan_req_noti_en);
ble_sts_t	blc_ll_setExtAdvData	(u8 advHandle, data_oper_t operation, data_fragm_t fragment_prefer, u8 adv_dataLen, 	u8 *advdata);
ble_sts_t 	blc_ll_setExtScanRspData(u8 advHandle, data_oper_t operation, data_fragm_t fragment_prefer, u8 scanRsp_dataLen, u8 *scanRspData);
ble_sts_t 	blc_ll_setExtAdvEnable_1(u32 extAdv_en, u8 sets_num, u8 advHandle, 	 u16 duration, 	  u8 max_extAdvEvt);
ble_sts_t	blc_ll_setExtAdvEnable_n(u32 extAdv_en, u8 sets_num, u8 *pData);


ble_sts_t	blc_ll_removeAdvSet(u8 advHandle);
ble_sts_t	blc_ll_clearAdvSets(void);


// if Coded PHY is used, this API set default S2/S8 mode for Extended ADV
void		blc_ll_setDefaultExtAdvCodingIndication(u8 advHandle, le_ci_prefer_t prefer_CI);


void        blc_ll_setAuxAdvChnIdxByCustomers(u8 aux_chn);
void		blc_ll_setMaxAdvDelay_for_AdvEvent(u8 max_delay_ms);    //unit: mS, only 8/4/2/1/0  available








//TODO
void 		blt_clearAdvSetsParam(ll_ext_adv_t		*pEadv);


#endif /* LL_ADV_EXT_H_ */
