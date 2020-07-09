/********************************************************************************************************
 * @file     mspi.h
 *
 * @brief    This is the header file for TLSR8258
 *
 * @author	 peng.sun@telink-semi.com;
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

#ifndef MSPI_H
#define MSPI_H
#include "compiler.h"
#include "gpio.h"
#include "reg_include/register_9518.h"

 /**
  * @brief     This function servers to set the spi wait.
  * @param[in] none
  * @return    none
  */
 void mspi_wait(void);

/**
 * @brief     This function servers to set the spi high level.
 * @param[in] none
 * @return    none
 */
void mspi_fm_rd_en(void);

/**
 * @brief     This function servers to set the spi high level.
 * @param[in] none
 * @return    none
 */
void mspi_fm_rd_dis(void);

/**
 * @brief     This function servers to set the spi high level.
 * @param[in] none
 * @return    none
 */
void mspi_high(void);

/**
 * @brief     This function servers to set the spi low level.
 * @param[in] none
 * @return    none
 */
void mspi_low(void);

/**
 * @brief     This function servers to gets the spi data.
 * @param[in] none.
 * @return    the spi data.
 */
unsigned char mspi_get(void);

/**
 * @brief     This function servers to write the spi.
 * @param[in] c - the char need to be write.
 * @return    none
 */
void mspi_write(unsigned char c);

/**
 * @brief     This function servers to control the write.
 * @param[in] c - need to be write.
 * @return    none
 */
void mspi_fm_write(unsigned char c);

/**
 * @brief     This function servers to spi read.
 * @param[in] none.
 * @return    read reault.
 */
unsigned char mspi_read(void);

/**
 * @brief     This function serves to Stop XIP operation before flash.
 * @param[in] none.
 * @return    none.
 */
static inline void mspi_stop_xip(void)
{
	mspi_wait();	//wait xip busy=0
	mspi_high();	//mspi_cn=1, stop xip read
	while(gpio_get_level(GPIO_PF3) == 0);	//wait cn=1
}

#endif


