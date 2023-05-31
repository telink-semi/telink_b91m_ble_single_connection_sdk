/********************************************************************************************************
 * @file     main.c
 *
 * @brief    This is the source file for BLE SDK
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

#include "tl_common.h"
#include "drivers.h"
#include "stack/ble/ble.h"
#include "app.h"

#include "app_att.h"

#if(FREERTOS_ENABLE)
#include "FreeRTOS.h"
#include "task.h"
#include "tlk_riscv.h"

PRIVILEGED_DATA TaskHandle_t hProtoTask;

/*! function statement */
static void led_task(void *pvParameters);
void proto_task( void *pvParameters );

#endif

/**
 * @brief		BLE SDK RF interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void rf_irq_handler(void)
{
	DBG_CHN14_HIGH;

	irq_blt_sdk_handler ();

	DBG_CHN14_LOW;
}

/**
 * @brief		BLE SDK System timer interrupt handler.
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_
void stimer_irq_handler(void)
{
	DBG_CHN15_HIGH;

	irq_blt_sdk_handler ();

	DBG_CHN15_LOW;
}



/**
 * @brief		This is main function
 * @param[in]	none
 * @return      none
 */
_attribute_ram_code_ int main (void)   //must on ramcode
{
	/* this function must called before "sys_init()" when:
	 * (1). For all IC: using 32K RC for power management,
  	   (2). For B91 only: even no power management */
	blc_pm_select_internal_32k_crystal();

	#if (MCU_CORE_TYPE == MCU_CORE_B91)
		sys_init(DCDC_1P4_LDO_1P8,VBAT_MAX_VALUE_GREATER_THAN_3V6);
	#elif (MCU_CORE_TYPE == MCU_CORE_B92)
		sys_init(LDO_1P2_LDO_2P0,VBAT_MAX_VALUE_GREATER_THAN_3V6);
	#endif
	/* detect if MCU is wake_up from deep retention mode */
	int deepRetWakeUp = pm_is_MCU_deepRetentionWakeup();  //MCU deep retention wakeUp

	CCLK_24M_HCLK_24M_PCLK_24M;

	rf_drv_ble_init();

	gpio_init(!deepRetWakeUp);
	#if (MCU_CORE_TYPE == MCU_CORE_B92)
		wd_32k_stop();
	#endif


	if( deepRetWakeUp ){ //MCU wake_up from deepSleep retention mode
		user_init_deepRetn ();
	}
	else{ //MCU power_on or wake_up from deepSleep mode
		user_init_normal();
	}

#if (FREERTOS_ENABLE)
	extern void vPortRestoreTask();
	if( deepRetWakeUp ){
		tlkapi_send_string_data(APP_LOG_EN,"enter restore work.",0,0);
		vPortRestoreTask();
	}
	else{

		#if(!TEST_CONN_CURRENT_ENABLE)
			xTaskCreate( led_task, "tLed", configMINIMAL_STACK_SIZE, (void*)0, (tskIDLE_PRIORITY+1), 0 );
		#endif
		xTaskCreate( proto_task, "tProto", 2*configMINIMAL_STACK_SIZE, (void*)0, (tskIDLE_PRIORITY+2), &hProtoTask );
		//xTaskCreate( ui_task, "tUI", configMINIMAL_STACK_SIZE, (void*)0, tskIDLE_PRIORITY + 1, 0 );

		/* !!! must */
		extern void os_give_sem(void);
		os_give_sem();

		vTaskStartScheduler();
	}
#else
	irq_enable();

	while (1) {
		main_loop ();
	}
	return 0;
#endif
}

/**
 ***************************************************************************************
 * OS Start
 ***************************************************************************************
 */
#if (FREERTOS_ENABLE)

#if(!TEST_CONN_CURRENT_ENABLE)
static void led_task(void *pvParameters)
{
	while(1)
	{
		gpio_toggle(GPIO_LED_GREEN);
		vTaskDelay(pdMS_TO_TICKS(200));
	}
}
#endif


//  !!! should notify those tasks that ulTaskNotifyTake long time and should be wakeup every time PM wakeup
void vPortWakeupNotify()
{

}

RAM_CODE
void proto_task( void *pvParameters )
{
	while (1)
	{
		uint32_t timeout = portMAX_DELAY;

		DBG_CHN8_TOGGLE;

		if(blc_ll_getCurrentState() == BLS_LINK_STATE_ADV)
		{
			u32 adv_rst_time = (u32)(clock_time() - blc_ll_getAdvExpectTick());
			if(adv_rst_time < BIT(31) )
			{
				blt_sdk_main_loop();
			}
			timeout = SYSTIMER_TICK_TO_OS_TICK((u32)(blc_ll_getAdvExpectTick() - clock_time()));
		}
		else if( blc_ll_getCurrentState() == BLS_LINK_STATE_CONN ){
			blt_sdk_main_loop();

			/* data test */
			//u8 keycode[8] = {0x0a, 0x00, 0x00, 0x00,0x00, 0x00, 0x00,0x00};
			//blc_gatt_pushHandleValueNotify(BLS_CONN_HANDLE, HID_NORMAL_KB_REPORT_INPUT_DP_H, keycode, sizeof(keycode));
		}

		ulTaskNotifyTake( pdTRUE, timeout );
	}

}

_attribute_ram_code_
void os_give_sem_from_isr(void)
{
	if(hProtoTask == NULL)
		return;
	BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
	vTaskNotifyGiveFromISR(hProtoTask, &pxHigherPriorityTaskWoken);
}

_attribute_ram_code_
void os_give_sem(void)
{
	if(hProtoTask == NULL)
		return;
	xTaskNotifyGive(hProtoTask);
}

#endif
