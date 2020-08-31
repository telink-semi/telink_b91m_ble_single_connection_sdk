/*
 * mdec.h
 *
 *  Created on: Apr 16, 2020
 *      Author: Telink
 */

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
 * @brief This function is used to initialize the MDEC module,include clock setting and input IO select.
 * @param[in]    pin - MDEC_PinTypeDef
 * @return none
 */
void mdec_init(mdec_pin_e pin);

/**
 * @brief This function is used to read the receive data of MDEC module's IO.
 * @param[out]   dat - The array to store date.
 * @return       1--decode success,  0--decode failure.
 */
unsigned char mdec_read_dat(unsigned char *dat);

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
#endif /* DRIVERS_9518_MDEC_H_ */
