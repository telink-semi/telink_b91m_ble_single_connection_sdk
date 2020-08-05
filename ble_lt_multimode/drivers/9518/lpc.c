

/*
 * LPC.c
 *
 *  Created on: Jun 17, 2020
 *      Author: Telink
 */

#include "lpc.h"



/**
 * @brief This function powers down low power comparator.
 * @param[in] none
 * @return none
 */
void lpc_power_down(void)
{
	analog_write_reg8(0x07,(analog_read_reg8(0x07))|0x08);
}

/**
 * @brief This function servers to power down/up for low power comparator.
 * @param[in] lpc_power
 * @return none
 */
void lpc_power_on(void)
{
	analog_write_reg8(0x06,analog_read_reg8(0x06) & 0xfd);
}

/**
 * @brief This function selects input channel for low power comparator .
 * @param[in] pin-selected input channel.Input derived from external PortB(PB<1>~PB<7>).
 * @return none
 */
void lpc_set_input_chn(lpc_input_channel_e pin)
{
	analog_write_reg8(0x0d,(analog_read_reg8(0x0d) & 0xf8) | pin);
}

/**
 * @brief This function selects input reference voltage for low power comparator .
 * @param[in] mode-lower power comparator working mode includes normal mode and low power mode.
 * @param[in] ref- selected input reference voltage.
 * @return none
 */
void lpc_set_input_ref(lpc_mode_e mode,lpc_reference_e ref)
{
	if(mode == LPC_LOWPOWER)
	{
		analog_write_reg8(0x0b,analog_read_reg8(0x0b)&0xf7);
		analog_write_reg8(0x0d,analog_read_reg8(0x0d)&0x7f);
	}
	else if(mode == LPC_NORMAL)
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
void lpc_set_scaling_coeff(lpc_scaling_e divider)
{
	analog_write_reg8(0x0b,(analog_read_reg8(0x0b)&0xcf)|(divider<<4));
}















