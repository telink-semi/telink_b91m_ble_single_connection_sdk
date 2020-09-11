/********************************************************************************************************
 * @file	ble.h
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
#ifndef BLE_H_
#define BLE_H_


#include "ble_config.h"
#include "ble_common.h"
#include "ble_format.h"

#include "l2cap/l2cap.h"
#include "phy/phy_stack.h"
#include "phy/phy.h"
#include "phy/phy_test.h"


#include "attr/att.h"
#include "attr/gatt.h"


#include "smp/smp.h"
#include "smp/smp_stack.h"
#include "smp/smp_central.h"
#include "smp/smp_peripheral.h"
#include "smp/smp_storage.h"

#include "gap/gap.h"
#include "gap/gap_event.h"

#include "crypt/aes_ccm.h"
#include "crypt/le_crypto.h"
#include "crypt/aes/aes_att.h"

#include "hci/hci.h"
#include "hci/hci_const.h"
#include "hci/hci_event.h"
#include "hci/usb_desc.h"

#include "stack/ble/service/ota.h"
#include "stack/ble/service/ota_stack.h"
#include "service/device_information.h"
#include "service/hids.h"
#include "service/uuid.h"

#include "ll/ll.h"
#include "ll/ll_stack.h"
#include "ll/ll_adv.h"
#include "ll/ll_init.h"
#include "ll/ll_pm.h"
#include "ll/ll_scan.h"
#include "ll/ll_whitelist.h"
#include "ll/ll_conn/ll_conn.h"
#include "ll/ll_conn/conn_stack.h"
#include "ll/ll_conn/ll_slave.h"
#include "ll/ll_conn/ll_master.h"
#include "ll/ll_conn/ll_conn_csa.h"

#include "ll/ll_ext_adv.h"


#include "stack/ble/trace.h"




volatile void  stmemset(register char * dest,register int val,register unsigned int len);
volatile void * stmemcpy(register char * out, register char * in, register unsigned int len);





#endif /* BLE_H_ */
