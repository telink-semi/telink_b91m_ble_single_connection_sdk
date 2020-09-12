/********************************************************************************************************
 * @file	app.c
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
#include "app.h"
#include <stack/ble/ble.h>
#include "tl_common.h"
#include "drivers.h"
#include "app_config.h"
#include "vendor/common/blt_led.h"
#include "application/keyboard/keyboard.h"
#include "vendor/common/blt_soft_timer.h"


/**
 * @brief		user initialization when MCU power on or wake_up from deepSleep mode
 * @param[in]	none
 * @return      none
 */
void user_init_normal(void)
{
#if (   FEATURE_TEST_MODE == TEST_ADVERTISING_ONLY || FEATURE_TEST_MODE == TEST_SCANNING_ONLY \
	 || FEATURE_TEST_MODE == TEST_ADVERTISING_IN_CONN_SLAVE_ROLE \
	 || FEATURE_TEST_MODE==TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE \
	 || FEATURE_TEST_MODE == TEST_ADVERTISING_SCANNING_IN_CONN_SLAVE_ROLE)

	feature_linklayer_state_test_init_normal();

#elif (FEATURE_TEST_MODE == TEST_POWER_ADV)

	feature_adv_power_test_init_normal();

#elif (FEATURE_TEST_MODE == TEST_SMP_SECURITY)

	feature_security_test_init_normal();

#elif (FEATURE_TEST_MODE == TEST_GATT_SECURITY)

	feature_gatt_security_test_init_normal();

#elif (FEATURE_TEST_MODE == TEST_SDATA_LENGTH_EXTENSION)
	feature_sdle_test_init_normal();

#elif (FEATURE_TEST_MODE == TEST_MDATA_LENGTH_EXTENSION)
	feature_mdle_test_init_normal();

#elif (FEATURE_TEST_MODE == TEST_USER_BLT_SOFT_TIMER)

	feature_soft_timer_test_init_normal();

#elif (FEATURE_TEST_MODE == TEST_WHITELIST)

	feature_whitelist_test_init_normal();

#elif (FEATURE_TEST_MODE == TEST_BLE_PHY)

	feature_phytest_init_normal();

#elif (FEATURE_TEST_MODE == TEST_EXTENDED_ADVERTISING)

	feature_ext_adv_init_normal();

#elif (FEATURE_TEST_MODE == TEST_2M_CODED_PHY_EXT_ADV)

	feature_2m_coded_phy_adv_init_normal();

#elif (FEATURE_TEST_MODE == TEST_2M_CODED_PHY_CONNECTION)

	feature_2m_coded_phy_conn_init_normal();

#elif (FEATURE_TEST_MODE == TEST_CSA2)

	feature_csa2_init_normal();

#else

#endif
}



/**
 * @brief		user initialization when MCU wake_up from deepSleep_retention mode
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void user_init_deepRetn(void)
{

#if (   FEATURE_TEST_MODE == TEST_ADVERTISING_ONLY || FEATURE_TEST_MODE == TEST_SCANNING_ONLY \
	 || FEATURE_TEST_MODE == TEST_ADVERTISING_IN_CONN_SLAVE_ROLE \
	 || FEATURE_TEST_MODE==TEST_SCANNING_IN_ADV_AND_CONN_SLAVE_ROLE \
	 || FEATURE_TEST_MODE == TEST_ADVERTISING_SCANNING_IN_CONN_SLAVE_ROLE)

	feature_linklayer_state_test_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_POWER_ADV)

	feature_adv_power_test_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_SMP_SECURITY)

	feature_security_test_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_GATT_SECURITY)

	feature_gatt_security_test_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_SDATA_LENGTH_EXTENSION)
	feature_sdle_test_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_MDATA_LENGTH_EXTENSION)
	feature_mdle_test_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_USER_BLT_SOFT_TIMER)

	feature_soft_timer_test_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_WHITELIST)

	feature_whitelist_test_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_BLE_PHY)


#elif (FEATURE_TEST_MODE == TEST_EXTENDED_ADVERTISING)

	feature_ext_adv_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_2M_CODED_PHY_EXT_ADV)

	feature_2m_coded_phy_adv_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_2M_CODED_PHY_CONNECTION)

	feature_2m_coded_phy_conn_init_deepRetn();

#elif (FEATURE_TEST_MODE == TEST_CSA2)

	feature_csa2_init_deepRetn();

#else

#endif
}





u32 tick_loop=0;
/*----------------------------------------------------------------------------*/
/*-------- Main Loop                                                ----------*/
/*----------------------------------------------------------------------------*/
/**
 * @brief		This is main_loop function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ void main_loop (void)
{
	tick_loop++;

#if (FEATURE_TEST_MODE == TEST_USER_BLT_SOFT_TIMER)
	blt_soft_timer_process(MAINLOOP_ENTRY);
#endif

	blt_sdk_main_loop();

#if (FEATURE_TEST_MODE == TEST_SMP_SECURITY)
	feature_security_test_mainloop();
#elif (FEATURE_TEST_MODE == TEST_SDATA_LENGTH_EXTENSION)
	feature_sdle_test_mainloop();
#elif (FEATURE_TEST_MODE == TEST_MDATA_LENGTH_EXTENSION)
	feature_mdle_test_mainloop();
#elif (FEATURE_TEST_MODE == TEST_2M_CODED_PHY_CONNECTION)
	feature_2m_coded_phy_conn_mainloop();
#endif
}

/*----------------------------- End of File ----------------------------------*/


