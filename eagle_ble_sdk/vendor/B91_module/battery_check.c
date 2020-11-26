/********************************************************************************************************
 * @file     battery_check.c 
 *
 * @brief    for TLSR chips
 *
 * @author	 BLE Group
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
/*
 * battery_check.c
 *
 *  Created on: 2018-8-3
 *      Author: Administrator
 */

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"

#include "battery_check.h"


#if (BATT_CHECK_ENABLE)




//_attribute_data_retention_	u8		adc_first_flg = 1;
_attribute_data_retention_	u8 		lowBattDet_enable = 1;
							u8      adc_hw_initialized = 0;   //note: can not be retention variable
_attribute_data_retention_  u16     batt_vol_mv;






int app_suspend_enter_low_battery (void)
{
	if (!gpio_read(GPIO_WAKEUP_MODULE)) //gpio low level
	{
		analog_write(USED_DEEP_ANA_REG,  analog_read(USED_DEEP_ANA_REG)|LOW_BATT_FLG);  //mark
		return 1;//allow enter cpu_sleep_wakeup
	}

	analog_write(USED_DEEP_ANA_REG,  analog_read(USED_DEEP_ANA_REG)&(~LOW_BATT_FLG));  //clr
	return 0; //forbidden enter cpu_sleep_wakeup
}


void battery_set_detect_enable (int en)
{
	lowBattDet_enable = en;

	if(!en){
		adc_hw_initialized = 0;   //need initialized again
	}

}


int battery_get_detect_enable (void)
{
	return lowBattDet_enable;
}




_attribute_ram_code_ int app_battery_power_check(u16 alram_vol_mv)
{


	//when MCU powered up or wakeup from deep/deep with retention, adc need be initialized
	if(!adc_hw_initialized){

		adc_hw_initialized = 1;
#if VBAT_CHANNEL_EN
		adc_battery_voltage_sample_init();
#else
		adc_gpio_sample_init(ADC_INPUT_PIN_CHN,ADC_VREF_1P2V,ADC_PRESCALE_1F8,ADC_SAMPLE_FREQ_96K);
#endif
		adc_power_on();
	}


	u32 t0 = clock_time();

	while(!clock_time_exceed(t0, 25));  //wait at least 2 sample cycle(f = 96K, T = 10.4us)

	u32 adc_result;
	u32 adc_average;
	adc_average = adc_get_code();


#if 1
	adc_result = adc_average;
#else  	//history data filter
	if(adc_first_flg){
		adc_result = adc_average;
		adc_first_flg = 0;
	}
	else{
		adc_result = ( (adc_result*3) + adc_average + 2 )>>2;  //filter
	}
#endif
//////////////// adc sample data convert to voltage(mv) ////////////////
	batt_vol_mv  = adc_calculate_voltage(adc_result);

	if(batt_vol_mv < alram_vol_mv){

		GPIO_WAKEUP_MODULE_LOW;
		bls_pm_registerFuncBeforeSuspend( &app_suspend_enter_low_battery );
//		bls_pm_registerFuncBeforeSuspend( NULL );

		cpu_set_gpio_wakeup (GPIO_WAKEUP_MODULE, Level_High, 1);  //drive pin pad high wakeup deepsleep

		cpu_sleep_wakeup(DEEPSLEEP_MODE, PM_WAKEUP_PAD, 0);  //deepsleep
//		cpu_sleep_wakeup(DEEPSLEEP_MODE, 0, 0);  //deepsleep
		return 1;
	}
	else{ // batt level > alarm level
		analog_write(USED_DEEP_ANA_REG,  analog_read(USED_DEEP_ANA_REG)&(~LOW_BATT_FLG));  //clr
		return 0;
	}
}

#endif
