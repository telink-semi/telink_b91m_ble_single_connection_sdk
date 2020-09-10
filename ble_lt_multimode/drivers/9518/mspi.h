/********************************************************************************************************
 * @file	mspi.h
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
_attribute_ram_code_sec_ static inline void mspi_wait(void){
	while(reg_mspi_status & FLD_MSPI_BUSY);
}

/**
 * @brief     This function servers to set the spi high level.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_sec_ static inline  void mspi_fm_rd_en(void){
	reg_mspi_fm |= FLD_MSPI_RD_TRIG_EN;
}

/**
 * @brief     This function servers to set the spi high level.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_sec_ static inline void mspi_fm_rd_dis(void){
	reg_mspi_fm &= ~FLD_MSPI_RD_TRIG_EN;
}

/**
 * @brief     This function servers to set the spi high level.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_sec_ static inline void mspi_high(void){
	reg_mspi_fm |= FLD_MSPI_CSN;
}

/**
 * @brief     This function servers to set the spi low level.
 * @param[in] none
 * @return    none
 */
_attribute_ram_code_sec_ static inline void mspi_low(void){
	reg_mspi_fm &= ~FLD_MSPI_CSN;
}

/**
 * @brief     This function servers to gets the spi data.
 * @param[in] none.
 * @return    the spi data.
 */
_attribute_ram_code_sec_ static inline unsigned char mspi_get(void){
	return reg_mspi_data;
}

/**
 * @brief     This function servers to write the spi.
 * @param[in] c - the char need to be write.
 * @return    none
 */
_attribute_ram_code_sec_ static inline  void mspi_write(unsigned char c){
	reg_mspi_data = c;
}

/**
 * @brief     This function servers to control the write.
 * @param[in] c - need to be write.
 * @return    none
 */
_attribute_ram_code_sec_ static inline void mspi_fm_write(unsigned char c){
	reg_mspi_fm = c;
}

/**
 * @brief     This function servers to spi read.
 * @param[in] none.
 * @return    read reault.
 */
_attribute_ram_code_sec_ static inline unsigned char mspi_read(void){
	mspi_write(0);		// dummy, issue clock
	mspi_wait();
	return mspi_get();
}

/**
 * @brief     This function serves to Stop XIP operation before flash.
 * @param[in] none.
 * @return    none.
 */
_attribute_ram_code_sec_ static inline void mspi_stop_xip(void)
{
	mspi_wait();	//wait xip busy=0
	mspi_high();	//mspi_cn=1, stop xip read
	while(gpio_get_level(GPIO_PF3) == 0);	//wait cn=1
}

#endif


