/********************************************************************************************************
 * @file	ll_conn.h
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
#ifndef LL_CONN_H_
#define LL_CONN_H_


/**
 * @brief	LE connection handle for slave & master role
 */
#define			BLM_CONN_HANDLE									BIT(7)
#define			BLS_CONN_HANDLE									BIT(6)



/**
 * @brief	LE connection role
 */
#define 		LL_ROLE_MASTER              					0
#define 		LL_ROLE_SLAVE               					1


#define 	BLE_INVALID_CONNECTION_HANDLE    					0xffff
#define 	IS_CONNECTION_HANDLE_VALID(handle)  				( handle != BLE_INVALID_CONNECTION_HANDLE )

/**
 * @brief      for user to initialize ACL connection module.
 * 			   notice that: user must sue this API for both connection slave and master role.
 * @param	   none
 * @return     none
 */
void		blc_ll_initConnection_module(void);


/**
 * @brief      for user to initialize LinkLayer TX FIFO.
 * 			   notice that: size*(number - 1)<= 0xFFF
 * @param[in]  pTxbuf - TX FIFO buffer address.
 * @param[in]  size - TX FIFO size,  must 16 Byte aligned
 * @param[in]  number - TX FIFO number, can only be 9, 17 or 33
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t 	blc_ll_initTxFifo(u8 *pTxbuf, int size, int number);


/**
 * @brief      for user to initialize LinkLayer RX FIFO.
 * @param[in]  pTxbuf - RX FIFO buffer address.
 * @param[in]  size - RX FIFO size
 * @param[in]  number - RX FIFO number, can only be 4, 8, 16 or 32
 * @return     status, 0x00:  succeed
 * 					   other: failed
 */
ble_sts_t	blc_ll_initRxFifo(u8 *pRxbuf, int size, int number);



#endif /* LL_CONN_H_ */
