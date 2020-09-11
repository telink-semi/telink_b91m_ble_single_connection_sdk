/********************************************************************************************************
 * @file	ll_pm.h
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
/*
 * ll_pm.h
 *
 *  Created on: 2017-3-7
 *      Author: Administrator
 */

#ifndef LL_PM_H_
#define LL_PM_H_




////////////////// Power Management ///////////////////////
#define			SUSPEND_DISABLE				0
#define			SUSPEND_ADV					BIT(0)
#define			SUSPEND_CONN				BIT(1)
#define			DEEPSLEEP_RETENTION_ADV		BIT(2)
#define			DEEPSLEEP_RETENTION_CONN	BIT(3)





typedef struct {
	u8		suspend_mask;
	u8		wakeup_src;
	u8		conn_no_suspend;
	u8 		timer_wakeup;

	u8		latency_off;
	u8		no_latency;
	u16 	latency_en;

	u16		sys_latency;
	u16 	user_latency;
	u16		valid_latency;
	u16 	latency_use;

	u32     deepRet_advThresTick;
	u32     deepRet_connThresTick;
	u32     deepRet_earlyWakeupTick;

	u8 		deepRt_en;
	u8		deepRet_type;
	u8		appWakeup_en;
	u8		appWakeup_flg;

	u8		appWakeup_loop_noLatency;
	u8 		timing_miss;
	u8      timing_synced;
	u8 		rsvd;

	u32 	appWakeup_tick;

	u32     current_wakeup_tick; //The system wake-up tick of the actual transfer of the cpu_sleep_wakeup function.

	u16		pm_border_flag;
	u16     rsvd1;

}st_ll_pm_t;

extern _attribute_aligned_(4) st_ll_pm_t  bltPm;


typedef 	void (*ll_module_pm_callback_t)(void);


typedef 	void (*pm_appWakeupLowPower_callback_t)(int);




/******************************* User Interface  ************************************/
void 		blc_ll_initPowerManagement_module(void);


void		bls_pm_setSuspendMask (u8 mask);
u8 			bls_pm_getSuspendMask (void);
void 		bls_pm_setWakeupSource(u8 source);
u32 		bls_pm_getSystemWakeupTick(void);

void 		bls_pm_setManualLatency(u16 latency); //manual set latency to save power

void 		bls_pm_setAppWakeupLowPower(u32 wakeup_tick, u8 enable);
void 		bls_pm_registerAppWakeupLowPowerCb(pm_appWakeupLowPower_callback_t cb);

void 		blc_pm_setDeepsleepRetentionThreshold(u32 adv_thres_ms, u32 conn_thres_ms);
void 		blc_pm_setDeepsleepRetentionEarlyWakeupTiming(u32 earlyWakeup_us);
void 		blc_pm_setDeepsleepRetentionType(SleepMode_TypeDef sleep_type);

/************************* Stack Interface, user can not use!!! ***************************/




#endif /* LL_PM_H_ */
