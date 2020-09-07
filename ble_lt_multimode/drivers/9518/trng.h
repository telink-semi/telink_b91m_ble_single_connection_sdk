/********************************************************************************************************
 * @file     trng.h
 *
 * @brief    This is the source file for TLSR9518
 *
 * @author	 Driver Group
 * @date     March 18, 2020
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
 * 			 1.initial release(March 18, 2020)
 *
 * @version  A001
 *
 *******************************************************************************************************/
/**	@page TRNG
 *
 *	Introduction
 *	===============
 *	TLSR9518 supports hardware trng. 
 *
 *	API Reference
 *	===============
 *	Header File: trng.h
 */
#ifndef TRNG_H_
#define TRNG_H_

#include "sys.h"
#include "reg_include/register_9518.h"

/**********************************************************************************************************************
 *                                         global constants                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                           global macro                                                             *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/

/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/**
 * @brief     This function performs to get one random number.If chip in suspend TRNG module should be close.
 *            else its current will be larger.
 * @return    none
 **/
void trng_init(void);

/**
 * @brief     This function performs to get one random number.
 * @return    the value of one random number
 **/
unsigned int trng_rand(void);

/*******************************      BLE Stack Use     ******************************/
#define rand	trng_rand
#define random_generator_init  trng_init
void generateRandomNum(int len, unsigned char *data);
#endif
