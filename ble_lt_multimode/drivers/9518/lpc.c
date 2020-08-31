

/*
 * LPC.c
 *
 *  Created on: Jun 17, 2020
 *      Author: Telink
 */

#include "lpc.h"









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
		analog_write_reg8(0x0b,analog_read_reg8(0x0b)|0x08);
		analog_write_reg8(0x0d,analog_read_reg8(0x0d)&0x7f);
	}
	else if(mode == LPC_NORMAL)
	{
		analog_write_reg8(0x0b,analog_read_reg8(0x0b)&0xf7);
		analog_write_reg8(0x0d,analog_read_reg8(0x0d)|0x80);
	}
	analog_write_reg8(0x0d,(analog_read_reg8(0x0d)&0x8f)|(ref<<4));
}

















