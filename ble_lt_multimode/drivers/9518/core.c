/********************************************************************************************************
 * @file     core.c
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
#include "core.h"

#include "plic.h"
#include "compiler.h"
/** @brief Enable interrupts globally in the system.
 * This macro must be used when the initialization phase is over and the interrupts
 * can start being handled by the system.
 */
 void core_enable_interrupt(void)
{
	set_csr(NDS_MSTATUS,1<<3);
	set_csr(NDS_MIE,1<<11 | 1 << 7 | 1 << 3);
#if (SUPPORT_PFT_ARCH)
	plic_set_feature(FLD_FEATURE_PREEMPT_PRIORITY_INT_EN | FLD_FEATURE_VECTOR_MODE_EN);//enable vectored in PLIC
#else
	plic_set_feature(FLD_FEATURE_VECTOR_MODE_EN);//enable vectored in PLIC
#endif
}



