/********************************************************************************************************
 * @file	mdec.h
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
#ifndef MDEC_H_
#define MDEC_H_
#include "analog.h"
#include "reg_include/mdec_reg.h"

#if 1
/**
 * @brief     This function servers to reset the MDEC module.When the system is wakeup by MDEC, you should
 * 			  to reset the MDEC module to clear the flag bit of MDEC wakeup.
 * @param[in] none
 * @return    none
 */
static inline void mdec_reset(void)
{
	analog_write_reg8(mdec_rst_addr,analog_read_reg8(mdec_rst_addr) | FLD_MDEC_RST);
	analog_write_reg8(mdec_rst_addr,analog_read_reg8(mdec_rst_addr) & (~FLD_MDEC_RST));
}
#endif

/**
*	@brief	  	This function serves to set the which irq enable
*	@param[in]	mask:Options that need to be enabled.
*	@return	 	Yes: 1, NO: 0.
*/
static inline void mdec_set_irq_mask(u8 mask)
{
	analog_write_reg8( REG_PG_STATUS_BASE_ADRESS, (analog_read_reg8(REG_PG_STATUS_BASE_ADRESS) | mask));
}

/**
*	@brief	  	This function serves to clear the wake mdec bit.After all packet
*				data are received, corresponding flag bitwill be set as 1. 
*				needed to manually clear this flag bit so as to avoid misjudgment.
*   @param      none
*	@return	 	none
*/
static inline void mdec_clr_irq_status(u8 mask)
{
	analog_write_reg8( REG_PG_STATUS_BASE_ADRESS, (analog_read_reg8(REG_PG_STATUS_BASE_ADRESS) | mask));
}

/**
*	@brief	  	After all packet data are received, it can check whether
*				packet transmission is finished.
*	@param[in]	none.
*	@return	 	Yes: 1, NO: 0.
*/
static inline unsigned char mdec_get_irq_status(u8 mask)
{
	return (analog_read_reg8(REG_PG_STATUS_BASE_ADRESS) & mask);
}

/**
 * @brief 		This function is used to initialize the MDEC module,include clock setting and input IO select.
 * @param[in]   pin - MDEC_PinTypeDef
 * @return 		none
 */
void mdec_init(mdec_pin_e pin);

/**
 * @brief 		This function is used to read the receive data of MDEC module's IO.
 * @param[out]  dat - The array to store date.
 * @return      1--decode success,  0--decode failure.
 */
unsigned char mdec_read_dat(unsigned char *dat);


#endif /* DRIVERS_9518_MDEC_H_ */
