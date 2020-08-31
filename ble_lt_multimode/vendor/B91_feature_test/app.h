/********************************************************************************************************
 * @file     app.h 
 *
 * @brief    for TLSR chips
 *
 * @author	 public@telink-semi.com;
 * @date     May. 10, 2018
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
