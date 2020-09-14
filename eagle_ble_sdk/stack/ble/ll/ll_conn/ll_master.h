/********************************************************************************************************
 * @file	ll_master.h
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
#include "conn_stack.h"

#ifndef LL_MASTER_H_
#define LL_MASTER_H_

/**
 * @brief      This function servers for obtaining l2cap data
 * @param[in]  conn - connect handle
 * @param[in]  *raw_pkt - data pointer of l2cap
 * @return     data pointer of l2cap
 */
rf_packet_l2cap_t * blm_l2cap_packet_pack (u16 conn, u8 * raw_pkt);


/**
 * @brief      for user to initialize ACL connection master role.
 * @param	   none
 * @return     none
 */
void blc_ll_initMasterRoleSingleConn_module(void);


/**
 * @brief      this function is used to terminate an existing connection.
 * @param[in]  handle - indicates which connection is to be disconnected.
 * @param[in]  reason - indicates the reason for ending the connection
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blm_ll_disconnect (u16 handle, u8 reason);


/**
 * @brief      this function is used to change the Link Layer connection parameters of a connection.
 * @param[in]  connHandle - Connection_Handle
 * @param[in]  conn_min - Minimum value for the connection interval.
 * @param[in]  conn_max - Maximum value for the connection interval.
 * @param[in]  conn_latency - Slave latency for the connection in number of connection events
 * @param[in]  timeout - Supervision timeout for the LE Link.
 * @param[in]  ce_min - Information parameter about the minimum length of connection event.
 * @param[in]  ce_max - Information parameter about the maximum length of connection event.
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blm_ll_updateConnection (u16 connHandle,
							  	   u16 conn_min, u16 conn_max, u16 conn_latency, u16 timeout,
							       u16 ce_min, u16 ce_max );


/**
 * @brief      this function is used by the Host to specify a channel classification based on its local information.
 * @param[in]  handle - Connection_Handle
 * @param[in]  *map - channel map
 * @return     status, 0x00:  succeed
 * 			           other: failed
 */
ble_sts_t blm_ll_setHostChannel (u16 handle, u8 * map);



/**
 * @brief      this function is used to detect if RF state machine in PHY layer is working.
 * @param	   none
 * @return     1 : RF state machine is working
 * 			   0 : RF state machine is not working
 */
bool 	blm_ll_isRfStateMachineBusy(void);


#endif /* LL_MASTER_H_ */
