
/*
 * lpc.h
 *
 *  Created on: Jun 17, 2020
 *      Author: Telink
 */

#ifndef _LPC_H_
#define _LPC_H_

#include "analog.h"


/**
 * define input IO.
 */
typedef enum{
	LPC_INPUT_PB1=1	  ,
	LPC_INPUT_PB2=2   ,
	LPC_INPUT_PB3=3   ,
	LPC_INPUT_PB4=4   ,
	LPC_INPUT_PB5=5   ,
	LPC_INPUT_PB6=6   ,
	LPC_INPUT_PB7=7   ,
}lpc_input_channel_e;

/**
 * define work mode.
 */
typedef enum{
	LPC_NORMAL=0,
	LPC_LOWPOWER,
}lpc_mode_e;


/**
 * define Reference voltage.
 */
typedef enum{
	LPC_REF_974MV  = 1,
	LPC_REF_923MV  = 2,
	LPC_REF_872MV  = 3,
	LPC_REF_820MV  = 4,
	LPC_REF_PB0    = 5,
	LPC_REF_PB3    = 6,
}lpc_reference_e;

/**
 * define scale.
 */
typedef enum{
	LPC_SCALING_PER25 =0 ,
	LPC_SCALING_PER50 =1 ,
	LPC_SCALING_PER75 =2 ,
	LPC_SCALING_PER100=3 ,
}lpc_scaling_e;



/**
 * @brief This function powers down low power comparator.
 * @param[in] none
 * @return none
 */
extern void lpc_power_down(void);

/**
 * @brief This function servers to power down/up for low power comparator.
 * @param[in] none
 * @return none
 */
extern void lpc_power_on(void);

/**
 * @brief This function selects input channel for low power comparator .
 * @param[in] pin-selected input channel.Input derived from external PortB(PB<1>~PB<7>).
 * @return none
 */
extern void lpc_set_input_chn(lpc_input_channel_e pin);

/**
 * @brief 		This function selects input reference voltage for low power comparator .
 * @param[in] 	mode - lower power comparator working mode includes normal mode and low power mode.
 * @param[in] 	ref	 - selected input reference voltage.
 * @return 		none
 */
extern void lpc_set_input_ref(lpc_mode_e mode,lpc_reference_e ref);


/**
 * @brief 		This function serves to set scaling_coefficient for low power comparator .
 * @param[in] 	divider - selected scaling coefficient.(%25,%50,%75,%100)
 * @return 		none
 */
extern void lpc_set_scaling_coeff(lpc_scaling_e divider);


/**
 * @brief 		This function serves to get the comparison results.if Vin>Vref 0x88[6]=0,else 0x88[6]=1.
 * @param[in] 	none
 * @return 		none
 */
static inline unsigned char lpc_get_result(void)
{
	return ((analog_read_reg8(0x88)&0x40)>>6);
}



#endif
