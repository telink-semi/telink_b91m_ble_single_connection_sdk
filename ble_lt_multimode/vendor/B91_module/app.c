/********************************************************************************************************
 * @file     app.c 
 *
 * @brief    This is the source file for TLSR8258
 *
 * @author	 junwei.lu@telink-semi.com;
 * @date     May 8, 2018
 *
 * @par      Copyright (c) 2018, Telink Semiconductor (Shanghai) Co., Ltd.
 *           All rights reserved.
 *
 *           The information contained herein is confidential property of Telink
 *           Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *           of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *           Co., Ltd. and the licensee or the terms described here-in. This heading
 *           MUST NOT be removed from this file.
 *
 *           Licensees are granted free, non-transferable use of the information in this
 *           file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 * @par      History:
 * 			 1.initial release(DEC. 26 2018)
 *
 * @version  A001
 *         
 *******************************************************************************************************/
#include "../../drivers.h"

// Hardware board select
#define HW_C1T217A20_V1_0_48_EVB		1
#define HW_C1T219A20_V1_0_64_EVB		2
#define HW_C1T213A20_V1_0_80_EVB		3
#define HARDWARE_BOARD_SELECT			HW_C1T219A20_V1_0_64_EVB

// Common GPIO
#define GPIO_LED_BLUE		GPIO_PB4
#define GPIO_LED_GREEN		GPIO_PB5
#define GPIO_LED_WHITE		GPIO_PB6

#if (HARDWARE_BOARD_SELECT == HW_C1T219A20_V1_0_64_EVB)
	#define GPIO_LED_RED		GPIO_PB7
#elif (HARDWARE_BOARD_SELECT == HW_C1T217A20_V1_0_48_EVB)
	#define GPIO_LED_RED		GPIO_PB2
#elif (HARDWARE_BOARD_SELECT == HW_C1T213A20_V1_0_80_EVB)
	#define GPIO_LED_RED		GPIO_PB7
#endif


#if (TEST_MODE == LED_TEST)

u32 system_timer_tick = 0;

void user_init()
{
	gpio_set_gpio_en(GPIO_LED_BLUE|GPIO_LED_GREEN|GPIO_LED_WHITE|GPIO_LED_RED);
	gpio_set_output_en(GPIO_LED_BLUE|GPIO_LED_GREEN|GPIO_LED_WHITE|GPIO_LED_RED);
	system_timer_tick = sys_get_stimer_tick();
	gpio_write(GPIO_LED_BLUE,1);
}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{
	static u8 led_mode = 0;
	if((led_mode == 0) && sys_timeout(system_timer_tick,500*1000)){
		gpio_toggle(GPIO_LED_BLUE);
		led_mode = 1;
	}
	else if((led_mode == 1) && sys_timeout(system_timer_tick,1000*1000)){
		gpio_toggle(GPIO_LED_GREEN);
		led_mode = 2;
	}
	else if((led_mode == 2) && sys_timeout(system_timer_tick,1500*1000)){
		gpio_toggle(GPIO_LED_WHITE);
		led_mode = 3;
	}
	else if((led_mode == 3) && sys_timeout(system_timer_tick,2000*1000)){
		gpio_toggle(GPIO_LED_RED);
		led_mode = 0;
		system_timer_tick = sys_get_stimer_tick();
	}

}


#else

void user_init()
{

}

/////////////////////////////////////////////////////////////////////
// main loop flow
/////////////////////////////////////////////////////////////////////
void main_loop (void)
{

}

#endif


