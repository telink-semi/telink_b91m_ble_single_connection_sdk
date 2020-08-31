/********************************************************************************************************
 * @file     clock.h
 *
 * @brief    This is the header file for B91
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

#ifndef CLOCK_H_
#define CLOCK_H_

#include "reg_include/register_9518.h"
#include "compiler.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/
#define  	CCLK_16M_HCLK_16M_PCLK_16M		clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV12_TO_CCLK, CCLK_DIV1_TO_HCLK, HCLK_DIV1_TO_PCLK, PLL_DIV4_TO_MSPI_CLK)
#define		CCLK_24M_HCLK_24M_PCLK_24M		clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV8_TO_CCLK, CCLK_DIV1_TO_HCLK, HCLK_DIV1_TO_PCLK, PLL_DIV4_TO_MSPI_CLK)
#define		CCLK_32M_HCLK_32M_PCLK_16M		clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV6_TO_CCLK, CCLK_DIV1_TO_HCLK, HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK)
#define		CCLK_48M_HCLK_48M_PCLK_24M		clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV4_TO_CCLK, CCLK_DIV1_TO_HCLK, HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK)
#define		CCLK_64M_HCLK_32M_PCLK_16M		clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV3_TO_CCLK, CCLK_DIV2_TO_HCLK, HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK)
#define		CCLK_96M_HCLK_48M_PCLK_24M		clock_init(PLL_CLK_192M, PAD_PLL_DIV, PLL_DIV2_TO_CCLK, CCLK_DIV2_TO_HCLK, HCLK_DIV2_TO_PCLK, PLL_DIV4_TO_MSPI_CLK)

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**
 *  @brief  Define sys_clk struct.
 */
typedef struct {
	unsigned short pll_clk;		/**< pll clk */
	unsigned char cclk;			/**< cpu clk */
	unsigned char hclk;			/**< hclk */
	unsigned char pclk;			/**< pclk */
	unsigned char mspi_clk;		/**< mspi_clk */
}sys_clk_st;


/**
 * @brief system clock type
 */
typedef enum{                                        /**<     <1:0>         <2:6>   <15:8> */
	PLL_CLK_48M 	= (0 | (16 << 2) | (48 << 8)),   /**< ana_09<3:2> | ana_80<4:0> | clk  */
	PLL_CLK_54M 	= (0 | (17 << 2) | (54 << 8)),
	PLL_CLK_60M 	= (0 | (18 << 2) | (54 << 8)),
	PLL_CLK_66M 	= (0 | (19 << 2) | (66 << 8)),
	PLL_CLK_96M 	= (1 | (16 << 2) | (96 << 8)),
	PLL_CLK_108M 	= (1 | (17 << 2) | (108 << 8)),
	PLL_CLK_120M 	= (1 | (18 << 2) | (120 << 8)),
	PLL_CLK_132M 	= (1 | (19 << 2) | (132 << 8)),
	PLL_CLK_192M 	= (2 | (16 << 2) | (192 << 8)),
	PLL_CLK_216M 	= (2 | (17 << 2) | (216 << 8)),
	PLL_CLK_240M 	= (2 | (18 << 2) | (240 << 8)),
	PLL_CLK_264M 	= (2 | (19 << 2) | (264 << 8)),
}sys_pll_clk_e;

/**
 * @brief system clock type.
 */
typedef enum{
	RC24M,
	PAD24M,
	PAD_PLL_DIV,
	PAD_PLL,
}sys_clock_src_e;

/**
 * @brief 32K clock type.
 */

typedef enum{
	CLK_32K_RC   =0,
	CLK_32K_XTAL =1,
}clk_32k_type_e;
/**
 * @brief pll div to cclk.
 */
typedef enum{
	PLL_DIV2_TO_CCLK    =    2,
	PLL_DIV3_TO_CCLK    =    3,
	PLL_DIV4_TO_CCLK    =    4,
	PLL_DIV5_TO_CCLK    =    5,
	PLL_DIV6_TO_CCLK    =    6,
	PLL_DIV7_TO_CCLK    =    7,
	PLL_DIV8_TO_CCLK    =    8,
	PLL_DIV9_TO_CCLK    =    9,
	PLL_DIV10_TO_CCLK   =    10,
	PLL_DIV11_TO_CCLK   =    11,
	PLL_DIV12_TO_CCLK   =    12,
	PLL_DIV13_TO_CCLK   =    13,
	PLL_DIV14_TO_CCLK   =    14,
	PLL_DIV15_TO_CCLK   =    15,
}sys_pll_div_to_cclk_e;

/**
 * @brief pll div to mspi_clk.
 */
typedef enum{
	CCLK_TO_MSPI_CLK       	=    1,
	PLL_DIV2_TO_MSPI_CLK    =    2,
	PLL_DIV3_TO_MSPI_CLK    =    3,
	PLL_DIV4_TO_MSPI_CLK    =    4,
	PLL_DIV5_TO_MSPI_CLK    =    5,
	PLL_DIV6_TO_MSPI_CLK    =    6,
	PLL_DIV7_TO_MSPI_CLK    =    7,
	PLL_DIV8_TO_MSPI_CLK    =    8,
	PLL_DIV9_TO_MSPI_CLK    =    9,
	PLL_DIV10_TO_MSPI_CLK   =    10,
	PLL_DIV11_TO_MSPI_CLK   =    11,
	PLL_DIV12_TO_MSPI_CLK   =    12,
	PLL_DIV13_TO_MSPI_CLK   =    13,
	PLL_DIV14_TO_MSPI_CLK   =    14,
	PLL_DIV15_TO_MSPI_CLK   =    15,
}sys_pll_div_to_mspi_clk_e;

/**
 * @brief cclk div to hclk.
 */
typedef enum{
	HCLK_DIV1_TO_PCLK    =    1,
	HCLK_DIV2_TO_PCLK    =    2,
	HCLK_DIV4_TO_PCLK    =    4,
}sys_hclk_div_to_pclk_e;

/**
 * @brief cclk div to hclk.
 */
typedef enum{
	CCLK_DIV1_TO_HCLK    =    1,
	CCLK_DIV2_TO_HCLK    =    2,
}sys_cclk_div_to_hclk_e;



/**
 *  @brief  Define sys_clk struct.
 */
typedef enum {
	RC_24M_CAL_DISABLE=0,
	RC_24M_CAL_ENABLE,

}rc_24M_cal_e;


/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/
extern sys_clk_st sys_clk;
extern clk_32k_type_e g_clk_32k_src;

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief       This function use to select the system clock source.
 * @param[in]   pll - pll clock.
 * @param[in]	src - cclk source.
 * @param[in]	cclk_div - the cclk divide from pll.it is useless if src is not PAD_PLL_DIV. cclk max is 96M
 * @param[in]	hclk_div - the hclk divide from cclk.hclk max is 48M.
 * @param[in]	pclk_div - the pclk divide from hclk.pclk max is 24M.
 * @param[in]	mspi_clk_div - mspi_clk has two source. pll div and hclk.mspi max is 64M.
 * @return      none
 */
_attribute_ram_code_sec_noinline_ void clock_init(sys_pll_clk_e pll,
		sys_clock_src_e src,
		sys_pll_div_to_cclk_e cclk_div,
		sys_cclk_div_to_hclk_e hclk_div,
		sys_hclk_div_to_pclk_e pclk_div,
		sys_pll_div_to_mspi_clk_e mspi_clk_div);

/**
 * @brief   This function serves to set 32k clock source.
 * @param[in]   variable of 32k type.
 * @return  none.
 */
void clock_32k_init(clk_32k_type_e src);

/**
 * @brief   	This function serves to kick 32k xtal.
 * @param[in]   xtal_times - kick times.
 * @return  	1 success, 0 error.
 */
unsigned char clock_kick_32k_xtal(unsigned char xtal_times);

/**
 * @brief     This function performs to select 24M as the system clock source.
 * @param[in] none.
 * @return    none.
 */
void clock_cal_24m_rc (void);

/**
 * @brief     This function performs to select 32K as the system clock source.
 * @param[in] none.
 * @return    none.
 */
void clock_cal_32k_rc (void);

/**
 * @brief  This function serves to get the 32k tick.
 * @param  none.
 * @return none.
 */
_attribute_ram_code_sec_  unsigned int clock_get_32k_tick (void);

/**
 * @brief  This function serves to set the 32k tick.
 * @param  none.
 * @return none.
 */
_attribute_ram_code_sec_ void clock_set_32k_tick(unsigned int tick);
#endif

