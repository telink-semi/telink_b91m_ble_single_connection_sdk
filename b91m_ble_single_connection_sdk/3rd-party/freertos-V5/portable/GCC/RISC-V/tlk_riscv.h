/********************************************************************************************************
 * @file     tlk_riscv.h
 *
 * @brief    This is the source file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         11,2022
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

#ifndef _RISC_V_TLK_RISCV_H_
#define _RISC_V_TLK_RISCV_H_

#include "types.h"
#include "vendor/common/user_config.h"
#include "core.h"
#include "driver.h"
#include "ext_driver/ext_pm.h"
#include "compatibility_pack/cmpt.h"

#ifndef FREERTOS_ENABLE
	#define FREERTOS_ENABLE   1
#endif

#if FREERTOS_ENABLE && BLE_APP_PM_ENABLE
	#define OS_PM_EN          1
#else
	#define OS_PM_EN          0
#endif

#define SYSTICK_TO_OSTICK		       (SYSTEM_TIMER_TICK_1S / configTICK_RATE_HZ)
#define OS_TICK_TO_SYSTIMER_TICK(x)    ((x) * (SYSTEM_TIMER_TICK_1S /configTICK_RATE_HZ))
#define SYSTIMER_TICK_TO_OS_TICK(x)    (((x) + SYSTICK_TO_OSTICK/2) / SYSTICK_TO_OSTICK)

#define MSTATUS_UIE         	0x00000001
#define MSTATUS_SIE         	0x00000002
#define MSTATUS_HIE         	0x00000004
#define MSTATUS_MIE         	0x00000008
#define MSTATUS_UPIE        	0x00000010
#define MSTATUS_SPIE        	0x00000020
#define MSTATUS_HPIE        	0x00000040
#define MSTATUS_MPIE        	0x00000080
#define MSTATUS_SPP         	0x00000100
#define MSTATUS_HPP         	0x00000600
#define MSTATUS_MPP         	0x00001800
#define MSTATUS_FS          	0x00006000
#define MSTATUS_XS          	0x00018000
#define MSTATUS_MPRV        	0x00020000
#define MSTATUS_PUM         	0x00040000
#define MSTATUS_MXR				0x00080000
#define MSTATUS_VM          	0x1F000000
#define MSTATUS32_SD        	0x80000000

#if __riscv_xlen == 64
#define MCAUSE_INT          	0x8000000000000000UL
#define MCAUSE_CAUSE        	0x7FFFFFFFFFFFFFFFUL
#else
#define MCAUSE_INT          	0x80000000UL
#define MCAUSE_CAUSE        	0x7FFFFFFFUL
#endif

#define IRQ_S_SOFT          	1
#define IRQ_H_SOFT          	2
#define IRQ_M_SOFT          	3
#define IRQ_S_TIMER         	5
#define IRQ_H_TIMER         	6
#define IRQ_M_TIMER         	7
#define IRQ_S_EXT           	9
#define IRQ_H_EXT           	10
#define IRQ_M_EXT           	11
#define IRQ_COP             	12
#define IRQ_HOST            	13

#define MIP_SSIP            	(1 << IRQ_S_SOFT)
#define MIP_HSIP            	(1 << IRQ_H_SOFT)
#define MIP_MSIP            	(1 << IRQ_M_SOFT)
#define MIP_STIP            	(1 << IRQ_S_TIMER)
#define MIP_HTIP            	(1 << IRQ_H_TIMER)
#define MIP_MTIP            	(1 << IRQ_M_TIMER)
#define MIP_SEIP            	(1 << IRQ_S_EXT)
#define MIP_HEIP            	(1 << IRQ_H_EXT)
#define MIP_MEIP            	(1 << IRQ_M_EXT)


/** @{
 *  Software interrupt
 */
#define PLIC_SW_BASE         0xE6400000
#define PLIC_SW_EN          *(volatile uint32_t*)(PLIC_SW_BASE + 0x2000)
#define PLIC_SW_TRIG        *(volatile uint32_t*)(PLIC_SW_BASE + 0x1000)

typedef struct{
	volatile uint32_t claim_complete;
}plic_sw_cmpl_t;

#define plic_sw_cmpl   ((plic_sw_cmpl_t *)(PLIC_SW_BASE + 0x200004))

static inline void soft_irq_enable(void)
{
	//__asm volatile("csrs mie (1<<11)|(1<<7)(1<<3)");
	//__asm volatile("csrs mstatus (1<<3)");
	PLIC_SW_EN = (1<<1); //BIT(1);
}

static inline  void soft_irq_trigger(void)
{
	PLIC_SW_TRIG = (1<<1);//BIT(1);
}

static inline void soft_irq_complete(void)
{
	plic_sw_cmpl->claim_complete;            //read claim
	plic_sw_cmpl->claim_complete = 1;        //complete
}
/* @} */


/** @{
 *  Machine Timer
 */
typedef struct{
	volatile unsigned long long mtime;
	volatile unsigned long long mtimecmp;
}mtimer_t;
#define mtimer   ((mtimer_t *)(0xE6000000))

static inline unsigned char mtimer_is_irq_enable(void)
{
	uint32_t mie = 0;
	__asm volatile ("csrr %0, mie" :"=r"(mie));
	return (mie & BIT(7)) ? 1:0;
}
/* @ } */

#endif /* _TLK_RISCV_H_ */
