/********************************************************************************************************
 * @file     ble_host.h
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

#ifndef STACK_BLE_HOST_BLE_HOST_H_
#define STACK_BLE_HOST_BLE_HOST_H_

#include "stack/ble/ble_common.h"
#include "stack/ble/ble_format.h"

#include "stack/ble/host/l2cap/l2cap.h"
#include "stack/ble/host/l2cap/l2cap_signal.h"


#include "stack/ble/host/attr/att.h"
#include "stack/ble/host/attr/gatt.h"


#include "stack/ble/host/smp/smp.h"
#include "stack/ble/host/smp/smp_peripheral.h"
#if (MCU_CORE_TYPE == MCU_CORE_825x || MCU_CORE_TYPE == MCU_CORE_827x)
#include "stack/ble/host/smp/smp_central.h"
#endif
#include "stack/ble/host/smp/smp_storage.h"


#include "stack/ble/host/gap/gap.h"
#include "stack/ble/host/gap/gap_event.h"

#include "stack/ble/device/multi_device.h"




#endif /* STACK_BLE_HOST_BLE_HOST_H_ */
