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
typedef  enum{
	GPIO_PA0_MDEC,
	GPIO_PB7_MDEC,
	GPIO_PC4_MDEC,
	GPIO_PD0_MDEC,
	GPIO_PE0_MDEC
}MDEC_PinTypeDef;
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
void mdec_init(MDEC_PinTypeDef pin);

/**
 * @brief This function is used to read the receive data of MDEC module's IO.
 * @param[out]   dat - The array to store date.
 * @return       1--decode success,  0--decode failure.
 */
unsigned char mdec_read_dat(unsigned char *dat);

#endif /* DRIVERS_9518_MDEC_H_ */
