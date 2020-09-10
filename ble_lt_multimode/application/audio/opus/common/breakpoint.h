/********************************************************************************************************
 * @file	breakpoint.h
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
#pragma once
#include "../../../../tl_common.h"

//  breakpoint simulation
#define MODULE_BREAKPOINT_ENABLE	0

#if(MODULE_BREAKPOINT_ENABLE)
extern volatile int bp_continue;		// 
extern volatile int bp_counter;			// use for breakpoint on counter
extern volatile int bp_pos;				// to indicate where the breakpoint is, may be useful in BP_ON_COND
extern volatile int bp_enable;			// enable breakpoint,  default to 1

#define BP_RST_COUNTER		do{bp_counter = 0;}while(0)

// no idea why gcc optimize off the multline version,  so use the oneline version instead
#if 1
#define BP_ALWAYS(p)		if(bp_enable){bp_pos = p;bp_continue = 0;reg_system_tick_ctrl=FLD_SYSTEM_TICK_STOP;do{}while(bp_enable && 0 == bp_continue);reg_system_tick_ctrl=FLD_SYSTEM_TICK_START;}
#define BP_ON_COUNTER(p,c)	if(bp_enable){bp_pos = p;++bp_counter;reg_system_tick_ctrl=FLD_SYSTEM_TICK_STOP;do{}while(bp_enable && (bp_counter == c));reg_system_tick_ctrl=FLD_SYSTEM_TICK_START;}
#define BP_ON_COND(p,c)		if(bp_enable){bp_pos = p;bp_continue = 0;reg_system_tick_ctrl=FLD_SYSTEM_TICK_STOP;do{}while(bp_enable && (0 == bp_continue) && (c));reg_system_tick_ctrl=FLD_SYSTEM_TICK_START;}
#else
#define BP_ALWAYS(p)		if(bp_enable)										\
							{													\
								bp_pos = p;										\
								bp_continue = 0; 								\
								CLR_FLD(reg_tmr_ctrl,FLD_TMR0_EN); 				\
								do{												\
								}while(bp_enable && 0 == bp_continue);			\
								SET_FLD(reg_tmr_ctrl,FLD_TMR0_EN);				\
							}
#define BP_ON_COUNTER(p,c)	if(bp_enable)										\
							{													\
								bp_pos = p; 									\
								++bp_counter; 									\
								CLR_FLD(reg_tmr_ctrl,FLD_TMR0_EN); 				\
								do{												\
								}while(bp_enable && bp_counter == c);			\
								SET_FLD(reg_tmr_ctrl,FLD_TMR0_EN);				\
							}

#define BP_ON_COND(p,c)		if(bp_enable)										\
							{													\
								bp_pos = p; 									\
								bp_continue = 0; 								\
								CLR_FLD(reg_tmr_ctrl,FLD_TMR0_EN); 				\
								do{												\
								}while(bp_enable && (0 == bp_continue) && (c));	\
								SET_FLD(reg_tmr_ctrl,FLD_TMR0_EN);				\
							}

#endif

#else

#define BP_RST_COUNTER

#define BP_ALWAYS
#define BP_ON_COUNTER(p, c)
#define BP_ON_COND(p, c)

#endif

