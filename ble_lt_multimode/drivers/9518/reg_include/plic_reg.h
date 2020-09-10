/********************************************************************************************************
 * @file	plic_reg.h
 *
 * @brief	This is the header file for B91
 *
 * @author	Ble Group
 * @date	2019
 *
 * @par		Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *			The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *          alter) any information contained herein in whole or in part except as expressly authorized  
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
#ifndef INTERRUPT_REG_H
#define INTERRUPT_REG_H
#include "../sys.h"
#include "../../../common/bit.h"

/*******************************     interrupt registers:     ******************************/
#define reg_irq_feature		         (*(volatile unsigned long*)(0 + (0xe4000000)))

#define reg_irq_pending(i)             	  (*(volatile unsigned long*)(0 + (0xe4001000+((i>31) ? 4 : 0))))

#define reg_irq_src0             	  (*(volatile unsigned long*)(0 + (0xe4002000)))
#define reg_irq_src1             	  (*(volatile unsigned long*)(0 + (0xe4002004)))

#define reg_irq_src(i)             	  (*(volatile unsigned long*)(0 + (0xe4002000+((i>31) ? 4 : 0) )))

#define reg_irq_threshold             (*(volatile unsigned long*)(0 + (0xe4200000)))
#define reg_irq_done          	  	   (*(volatile unsigned long*)(0 + (0xe4200004)))

#define reg_irq_src_priority(i)       (*(volatile unsigned long*)(0 + 0xe4000000+((i)<<2)))

#endif
