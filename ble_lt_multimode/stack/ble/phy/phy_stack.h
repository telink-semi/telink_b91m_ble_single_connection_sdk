/********************************************************************************************************
 * @file	phy_stack.h
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
#ifndef PHY_STACK_H_
#define PHY_STACK_H_


#include "tl_common.h"
#include "stack/ble/hci/hci_cmd.h"


/******************************* phy start *************************************************************************/


/********************* Macro & Enumeration variables for Stack, user can not use!!!!  **********************/
#define PHY_USED_AUXPTR_LE_1M									0
#define PHY_USED_AUXPTR_LE_2M									1
#define PHY_USED_AUXPTR_LE_CODED								2



typedef struct{
	u8	llid;
	u8  rf_len;
	u8	opcode;
	u8	tx_phys;
	u8	rx_phys;
}rf_pkt_ll_phy_req_rsp_t;   //phy_req, phy_rsp


typedef struct{
	u8	llid;
	u8  rf_len;
	u8	opcode;
	u8	m_to_s_phy;
	u8	s_to_m_phy;
	u8 	instant0;
	u8 	instant1;
}rf_pkt_ll_phy_update_ind_t;   //phy_req, phy_rsp



typedef enum{
	LE_CODED_S2 = 2,
	LE_CODED_S8 = 8,
}le_coding_ind_t;



typedef struct {
	u8	dft_tx_prefer_phys;
	u8 	dft_rx_prefer_phys;
	u8	dft_prefer_phy;
	u8	dft_CI;

	//for Extended ADV
	u8	cur_llPhy;
	u8	cur_CI;  //cur_coding_ind
	u16 rsvd;
}ll_phy_t;
extern _attribute_aligned_(4) ll_phy_t		bltPHYs;



//do not support Asymmetric PHYs, conn_phys = tx_phys & rx_phys
typedef struct {
	u8	conn_prefer_phys;  // conn_prefer_phys = tx_prefer_phys & rx_prefer_phys
	u8	conn_cur_phy;	   //
	u8	conn_next_phy;	   //
	u8	conn_cur_CI;	   // CI: coding_ind

	u8	conn_next_CI;
	u8	phy_req_trigger;  // 1: means current device triggers phy_req, due to API "blc_ll_setPhy" called by Host or Application
	u8	phy_req_pending;
	u8	phy_update_pending;

	u32	conn_update_phy;

	#if 0
		u8	tx_prefer_phys;		//phy set
		u8 	rx_prefer_phys;
		u8  tx_next_phys;
		u8 	rx_next_phys;

		u8	cur_tx_phy;		//phy using
		u8	cur_rx_phy;
		u16 rsvd;
	#endif

}ll_conn_phy_t;




typedef ll_conn_phy_t* (*ll_get_conn_phy_ptr_callback_t)(u16);
extern ll_get_conn_phy_ptr_callback_t  ll_get_conn_phy_ptr_cb;


extern u8   tx_settle_adv[4];
extern u8	tx_settle_slave[4];
extern u8	tx_settle_master[4];




int 	blt_reset_conn_phy_param(ll_conn_phy_t* pconn_phy);
void 	rf_ble_switch_phy(le_phy_type_t phy, le_coding_ind_t coding_ind);

typedef void (*ll_phy_switch_callback_t)(le_phy_type_t, le_coding_ind_t);
extern ll_phy_switch_callback_t		ll_phy_switch_cb;

/******************************* phy end ***************************************************************************/






/******************************* phy_test start *************************************************************************/

int 	  blc_phy_test_main_loop(void);

int 	  blc_phytest_cmd_handler (u8 *p, int n);

ble_sts_t blc_phy_setReceiverTest (u8 rx_chn);
ble_sts_t blc_phy_setTransmitterTest (u8 tx_chn, u8 length, u8 pkt_type);
ble_sts_t blc_phy_setPhyTestEnd(u8 *pkt_num);

ble_sts_t blc_phy_reset(void);
void blc_phy_preamble_length_set(unsigned char len);
void phy_test_driver_init(rf_mode_e rf_mode);




ble_sts_t 	blc_ll_setDefaultPhy(le_phy_prefer_mask_t all_phys, le_phy_prefer_type_t tx_phys, le_phy_prefer_type_t rx_phys);

ble_sts_t	blc_ll_readPhy(u16 connHandle, hci_le_readPhyCmd_retParam_t *);


ble_sts_t	blc_ll_setDefaultConnCodingIndication(le_ci_prefer_t prefer_CI);


/******************************* phy_test end ***************************************************************************/



#endif /* PHY_STACK_H_ */


