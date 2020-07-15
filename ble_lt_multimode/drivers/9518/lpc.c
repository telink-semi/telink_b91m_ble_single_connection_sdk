

/*
 * LPC.c
 *
 *  Created on: Jun 17, 2020
 *      Author: Telink
 */

#include "lpc.h"


/**
 * @brief This function servers to power down/up for low power comparator.
 * @param[in] lpc_power
 * @return none
 */
void lpc_set_power(LPC_Power_TypeDef lpc_power)
{
	if(lpc_power == LPC_POWER_ON)
	{
		analog_write_reg8(0x06,analog_read_reg8(0x06) & 0xfd);
	}
	else if(lpc_power == LPC_POWER_DOWN)
	{
		analog_write_reg8(0x06,analog_read_reg8(0x06) | 0x02);
	}
}

/**
 * @brief This function selects input channel for low power comparator .
 * @param[in] pin-selected input channel.Input derived from external PortB(PB<1>~PB<7>).
 * @return none
 */
void lpc_set_input_chn(LPC_Input_Channel_Typedef pin)
{
	analog_write_reg8(0x0d,analog_read_reg8(0x0d) & 0xf8 | pin);
}

/**
 * @brief This function selects input reference voltage for low power comparator .
 * @param[in] mode-lower power comparator working mode includes normal mode and low power mode.
 * @param[in] ref- selected input reference voltage.
 * @return none
 */
void lpc_set_input_ref(LPC_Mode_TypeDef mode,LPC_Reference_TypeDef ref)
{
	if(mode == LPC_NORMAL)
	{
		analog_write_reg8(0x0b,analog_read_reg8(0x0b)&0xf7);
		analog_write_reg8(0x0d,analog_read_reg8(0x0d)&0x7f);
	}
	else if(mode == LPC_LOWPOWER)
	{
		analog_write_reg8(0x0b,analog_read_reg8(0x0b)|0x08);
		analog_write_reg8(0x0d,analog_read_reg8(0x0d)|0x80);
	}
	analog_write_reg8(0x0d,(analog_read_reg8(0x0d)&0x8f)|(ref<<4));
}

/**
 * @brief This function sets scaling_coefficient  for low power comparator .
 * @param[in] divider-selected scaling coefficient.(%25,%50,%75,%100)
 * @return none
 */
void lpc_set_scaling_coeff(LPC_Scaling_TypeDef divider)
{
	analog_write_reg8(0x0b,(analog_read_reg8(0x0b)&0xcf)|(divider<<4));
}















