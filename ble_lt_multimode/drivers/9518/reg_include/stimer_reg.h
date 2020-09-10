/********************************************************************************************************
 * @file	stimer_reg.h
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
#ifndef STIMER_REG_H_
#define STIMER_REG_H_

/*******************************      sys clock registers: 0x140200       ******************************/
#define STIMER_BASE_ADDR			   	0x140200
#define reg_system_tick         		REG_ADDR32(STIMER_BASE_ADDR)

#define reg_system_irq_level         	REG_ADDR32(STIMER_BASE_ADDR+0x4)

#define reg_system_irq_mask				REG_ADDR8(STIMER_BASE_ADDR+0x8)
enum{
	FLD_SYSTEM_IRQ_MASK 	= 	BIT_RNG(0,2),
	FLD_SYSTEM_TRIG_PAST_EN = 	BIT(3),
};

#define reg_system_cal_irq		REG_ADDR8(STIMER_BASE_ADDR+0x9)

typedef enum{
	FLD_SYSTEM_IRQ  		= 	BIT(0),
	FLD_SYSTEM_32K_IRQ  	= 	BIT(1),
}stimer_irq_e;

#define reg_system_ctrl		    REG_ADDR8(STIMER_BASE_ADDR+0xa)
enum{
	FLD_SYSTEM_32K_WR_EN 		= 	BIT(0),
	FLD_SYSTEM_TIMER_EN 	    = 	BIT(1),
	FLD_SYSTEM_TIMER_AUTO 	    = 	BIT(2),
	FLD_SYSTEM_32K_CAL_EN 		= 	BIT(3),
	FLD_SYSTEM_32K_CAL_MODE 	= 	BIT_RNG(4,7),

};

#define reg_system_st		    REG_ADDR8(STIMER_BASE_ADDR+0xb)

enum{

	FLD_SYSTEM_CMD_STOP 			=   BIT(1),
	FLD_SYSTEM_CMD_SYNC		        =   BIT(3),
	FLD_SYSTEM_CLK_32K		        =   BIT(4),
	FLD_SYSTEM_CLR_RD_DONE			=   BIT(5),
	FLD_SYSTEM_RD_BUSY			    =   BIT(6),
	FLD_SYSTEM_CMD_SET_DLY_DONE	    =   BIT(7),

};

#define reg_system_timer_set_32k         	REG_ADDR32(STIMER_BASE_ADDR+0xc)

#define reg_system_timer_read_32k         	REG_ADDR32(STIMER_BASE_ADDR+0x10)

#define reg_system_cal_latch_32k         	REG_ADDR32(STIMER_BASE_ADDR+0x14)

#define reg_system_up_32k					REG_ADDR32(STIMER_BASE_ADDR+0x18)
enum{

	FLD_SYSTEM_UPDATE_UPON_32K 			=   BIT(0),
	FLD_SYSTEM_RUN_UPON_NXT_32K		    =   BIT(1),
};

/*******************************      BLE Stack Use     ******************************/
#define reg_system_tick_irq				reg_system_irq_level

typedef enum {
	STIMER_IRQ_MASK     		=   BIT(0),
	STIMER_32K_CAL_IRQ_MASK     =   BIT(1),
}stimer_irq_mask_e;

typedef enum {
	FLD_IRQ_SYSTEM_TIMER     		=   BIT(0),
}system_timer_irq_mask_e;


typedef enum {
	STIMER_IRQ_CLR	     		=   BIT(0),
	STIMER_32K_CAL_IRQ_CLR     	=   BIT(1),
}stimer_irq_clr_e;

#endif /* STIMER_REG_H_ */
