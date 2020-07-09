/********************************************************************************************************
 * @file     user_config.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     Sep. 18, 2015
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

#pragma once

#if (__PROJECT_B91_BLE_REMOTE__)
	#include "../B91_ble_remote/app_config.h"
#elif (__PROJECT_B91_BLE_SAMPLE__)
	#include "../B91_ble_sample/app_config.h"
#elif (__PROJECT_B91_MODULE__ )
	#include "../B91_module/app_config.h"
#elif (__PROJECT_B91_HCI__)
	#include "../B91_hci/app_config.h"
#elif (__PROJECT_B91_FEATURE_TEST__)
	#include "../B91_feature_test/app_config.h"
#elif(__PROJECT_B91_MASTER_KMA_DONGLE__)
	#include "../B91_master_kma_dongle/app_config.h"
#elif(__PROJECT_B91_DEBUG_DEMO__)
	#include "../B91_debug_demo/app_config.h"
#else
	#include "../common/default_config.h"
#endif

