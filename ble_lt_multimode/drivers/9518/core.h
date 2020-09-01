/********************************************************************************************************
 * @file     core.h
 *
 * @brief    This is the header file for TLSR9518
 *
 * @author	 Driver Group
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
#ifndef CORE_H
#define CORE_H
#include "nds_intrinsic.h"
#include "../../common/types.h"
#include "../../common/bit.h"
#define  read_csr(reg)		         __nds__csrr(reg)
#define  write_csr(reg, val)	     __nds__csrw(val, reg)
#define  swap_csr(reg, val)	         __nds__csrrw(val, reg)
#define set_csr(reg, bit)	         __nds__csrrs(bit, reg)
#define clear_csr(reg, bit)	         __nds__csrrc(bit, reg)

/*
 * Inline nested interrupt entry/exit macros
 */
/* Svae/Restore macro */
#define save_csr(r)             long __##r = read_csr(r);
#define restore_csr(r)           write_csr(r, __##r);
/* Support PowerBrake (Performance Throttling) feature */
#define	SUPPORT_PFT_ARCH		1

#if SUPPORT_PFT_ARCH

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
#define MSTATUS64_SD        	0x8000000000000000

#define save_mxstatus()         save_csr(NDS_MXSTATUS)
#define restore_mxstatus()      restore_csr(NDS_MXSTATUS)

 /* Nested IRQ entry macro : Save CSRs and enable global interrupt. */
#define core_save_nested_context()                              \
	 save_csr(NDS_MEPC)                              \
	 save_csr(NDS_MSTATUS)                           \
	 save_mxstatus()                                 \
	 set_csr(NDS_MSTATUS, MSTATUS_MIE);

/* Nested IRQ exit macro : Restore CSRs */
#define core_restore_nested_context()                               \
	 clear_csr(NDS_MSTATUS, MSTATUS_MIE);            \
	 restore_csr(NDS_MSTATUS)                        \
	 restore_csr(NDS_MEPC)                           \
	 restore_mxstatus()
#define NDS_FENCE_IORW			__nds__fence(FENCE_IORW,FENCE_IORW)
#else
#define save_mxstatus()
#define restore_mxstatus()
#define core_save_nested_context()
#define core_restore_nested_context()
#endif


typedef enum{
	FLD_FEATURE_PREEMPT_PRIORITY_INT_EN = BIT(0),
	FLD_FEATURE_VECTOR_MODE_EN 			= BIT(1),
}
feature_e;



/* Disable the Machine external, timer and software interrupts until setup is done */
/** @brief Disable interrupts globally in the system.
 * This function must be used when the system wants to disable all the interrupt
 * it could handle.
 */
static inline u32 core_interrupt_disable(void){

	u32 r = read_csr (NDS_MIE);
	clear_csr(NDS_MIE, BIT(3)| BIT(7)| BIT(11));
	return r;
}


/* Enable the Machine External/Timer/Sofware interrupt bit in MIE. */
/** @brief Disable interrupts globally in the system.
 * This function must be used when the system wants to restore all the interrupt
 * it could handle.
 */
static inline u32 core_restore_interrupt(u32 en){

	set_csr(NDS_MIE, en);
	return 0;
}

/** @brief Enable interrupts globally in the system.
 * This macro must be used when the initialization phase is over and the interrupts
 * can start being handled by the system.
 */
void core_interrupt_enable(void);

/*******************************      BLE Stack Use     ******************************/
#define  irq_disable		core_interrupt_disable
#define	 irq_enable			core_interrupt_enable
#define  irq_restore(en)	core_restore_interrupt(en)

#endif
