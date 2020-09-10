/********************************************************************************************************
 * @file	app.h
 *
 * @brief	This is the header file for B91
 *
 * @author	Ble Group
 * @date	2019
 *
 * @par     Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
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
#ifndef _APP_H
#define _APP_H

#include "tl_common.h"
#include "drivers.h"

/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void user_init_normal(void);

/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void user_init_deepRetn(void);

/**
 * @brief		This is main_loop function
 * @param[in]	none
 * @return      none
 */
void main_loop (void);

/**
 * @brief		user initialization for link layer state test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_linklayer_state_test_init_normal(void);

/**
 * @brief		user initialization for link layer state test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_linklayer_state_test_init_deepRetn(void);

/**
 * @brief		user initialization for adv_power test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_adv_power_test_init_normal(void);

/**
 * @brief		user initialization for adv_power test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_adv_power_test_init_deepRetn(void);

/**
 * @brief		user initialization for security test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_security_test_init_normal(void);

/**
 * @brief		user initialization for security test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_security_test_init_deepRetn(void);

/**
 * @brief		This is main_loop function in security test project
 * @param[in]	none
 * @return      none
 */
void feature_security_test_mainloop(void);

/**
 * @brief		user initialization for GATT security test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_gatt_security_test_init_normal(void);

/**
 * @brief		user initialization for GATT security test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_gatt_security_test_init_deepRetn(void);

/**
 * @brief		user initialization for software timer test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_soft_timer_test_init_normal(void);

/**
 * @brief		user initialization for software timer test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_soft_timer_test_init_deepRetn(void);

/**
 * @brief		user initialization for white list timer test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_whitelist_test_init_normal(void);

/**
 * @brief		user initialization for white list timer test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_whitelist_test_init_deepRetn(void);

/**
 * @brief		PHY test project interrupt handle
 * @param[in]	none
 * @return      none
 */
void app_phytest_irq_proc(void);


/**
 * @brief		user initialization for slave DLE test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_sdle_test_init_normal(void);

/**
 * @brief		user initialization for slave DLE test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_sdle_test_init_deepRetn(void);

/**
 * @brief		This is main_loop function in slave DLE project
 * @param[in]	none
 * @return      none
 */
void feature_sdle_test_mainloop(void);

/**
 * @brief		user initialization for master DLE test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_mdle_test_init_normal(void);

/**
 * @brief		user initialization for master DLE test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_mdle_test_init_deepRetn(void);

/**
 * @brief		This is main_loop function in master DLE project
 * @param[in]	none
 * @return      none
 */
void feature_mdle_test_mainloop(void);

/**
 * @brief		user initialization for extended adv test timer test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_ext_adv_init_normal(void);

/**
 * @brief		user initialization for extended adv test timer test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_ext_adv_init_deepRetn(void);

/**
 * @brief		user initialization for BLE 2M/Coded PHY adv test timer test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_2m_coded_phy_adv_init_normal(void);

/**
 * @brief		user initialization for BLE 2M/Coded PHY adv test timer test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_2m_coded_phy_adv_init_deepRetn(void);
/**
 * @brief		user initialization for BLE 2M/Coded PHY connect test timer test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_2m_coded_phy_conn_init_normal(void);

/**
 * @brief		user initialization for BLE 2M/Coded PHY connect test timer test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_2m_coded_phy_conn_init_deepRetn(void);

/**
 * @brief		This is main_loop function in BLE 2M/Coded PHY connect project
 * @param[in]	none
 * @return      none
 */
void feature_2m_coded_phy_conn_mainloop(void);

/**
 * @brief		user initialization for Channel Selection Algorithm 2 test timer test project when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void feature_csa2_init_normal(void);

/**
 * @brief		user initialization for Channel Selection Algorithm 2 test timer test project when MCU power on or wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
void feature_csa2_init_deepRetn(void);

#endif /* APP_H_ */
