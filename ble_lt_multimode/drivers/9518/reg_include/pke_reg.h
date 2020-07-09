/********************************************************************************************************
 * @file     i2c_reg.h
 *
 * @brief    This is the source file for TLSR9518
 *
 * @author	 Driver Group
 * @date     February 21, 2020
 *
 * @par      Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
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
 * 			 1.initial release(September 16, 2019)
 *
 * @version  A001
 *
 *******************************************************************************************************/
#ifndef PKE_REG_H_
#define PKE_REG_H_
#include "../sys.h"
#include "../../../common/bit.h"


/*******************************      pke registers: 0x110000      ******************************/

#define REG_PKE_BASE		     0x110000

#define reg_pke_ctrl             REG_ADDR32(REG_PKE_BASE+0x00)
enum{
	FLD_PKE_CTRL_START = 		 BIT(0),
	FLD_PKE_CTRL_STOP = 		 BIT(16),
};

#define reg_pke_conf             REG_ADDR32(REG_PKE_BASE+0x04)
enum{
	FLD_PKE_CONF_IRQ_EN = 		 BIT(8),
	FLD_PKE_CONF_PARTIAL_RADIX = BIT_RNG(16,23),
	FLD_PKE_CONF_BASE_RADIX	=    BIT_RNG(24,26),
};

#define reg_pke_mc_ptr           REG_ADDR32(REG_PKE_BASE+0x10)

#define reg_pke_stat             REG_ADDR32(REG_PKE_BASE+0x20)
enum{
	FLD_PKE_STAT_DONE = 		 BIT(0),
};

#define reg_pke_rt_code          REG_ADDR32(REG_PKE_BASE+0x24)
enum{
	FLD_PKE_RT_CODE_STOP_LOG =	 BIT_RNG(0,3),
};

#define reg_pke_exe_conf         REG_ADDR32(REG_PKE_BASE+0x50)
enum{
	FLD_PKE_EXE_CONF_IAFF_R0 = 	 BIT(0),
	FLD_PKE_EXE_CONF_IMON_R0 = 	 BIT(1),
	FLD_PKE_EXE_CONF_IAFF_R1 = 	 BIT(2),
	FLD_PKE_EXE_CONF_IMON_R1 = 	 BIT(3),
	FLD_PKE_EXE_CONF_OAFF = 	 BIT(4),
	FLD_PKE_EXE_CONF_OMON = 	 BIT(5),
	FLD_PKE_EXE_CONF_ME_SCA_EN = BIT_RNG(8,9),
};





#endif










