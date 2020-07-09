/********************************************************************************************************
 * @file     gpio_9518.c
 *
 * @brief    This is the source file for TLSR8258
 *
 * @author	 BLE Team
 * @date     June 16, 2020
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
//#include "bsp.h"
#include "compiler.h"
#include "reg_include/register_9518.h"
#include "analog.h"

#include "gpio.h"

/**
 * @brief      This function servers to initiate pull up-down resistor of all gpio.
 * @param[in]  none
 * @return     none.
 */
static void gpio_analog_resistance_init(void)
{
	//A<3:0>
	analog_write_reg8 (0x0e,  PULL_WAKEUP_SRC_PA0 |
						(PULL_WAKEUP_SRC_PA1<<2) |
						(PULL_WAKEUP_SRC_PA2<<4) |
						(PULL_WAKEUP_SRC_PA3<<6));
	//A<7:4>
	analog_write_reg8 (0x0f,  PULL_WAKEUP_SRC_PA4 |
						(PULL_WAKEUP_SRC_PA5<<2) |
						(PULL_WAKEUP_SRC_PA6<<4) |
						(PULL_WAKEUP_SRC_PA7<<6));
	//B<3:0>
	analog_write_reg8 (0x10,  PULL_WAKEUP_SRC_PB0 |
						(PULL_WAKEUP_SRC_PB1<<2) |
						(PULL_WAKEUP_SRC_PB2<<4) |
						(PULL_WAKEUP_SRC_PB3<<6));
	//B<7:4>
	analog_write_reg8 (0x11,  PULL_WAKEUP_SRC_PB4 |
						(PULL_WAKEUP_SRC_PB5<<2) |
						(PULL_WAKEUP_SRC_PB6<<4) |
						(PULL_WAKEUP_SRC_PB7<<6));

    //C<3:0>
	analog_write_reg8 (0x12,  PULL_WAKEUP_SRC_PC0 |
						(PULL_WAKEUP_SRC_PC1<<2) |
						(PULL_WAKEUP_SRC_PC2<<4) |
						(PULL_WAKEUP_SRC_PC3<<6));
    //C<7:4>
	analog_write_reg8 (0x13,  PULL_WAKEUP_SRC_PC4 |
						(PULL_WAKEUP_SRC_PC5<<2) |
						(PULL_WAKEUP_SRC_PC6<<4) |
						(PULL_WAKEUP_SRC_PC7<<6));

    //D<3:0>
	analog_write_reg8 (0x14,  PULL_WAKEUP_SRC_PD0 |
						(PULL_WAKEUP_SRC_PD1<<2) |
						(PULL_WAKEUP_SRC_PD2<<4) |
						(PULL_WAKEUP_SRC_PD3<<6));
	//D<7:4>
	analog_write_reg8 (0x15,  PULL_WAKEUP_SRC_PD4 |
						(PULL_WAKEUP_SRC_PD5<<2) |
						(PULL_WAKEUP_SRC_PD6<<4) |
						(PULL_WAKEUP_SRC_PD7<<6));
	//E<3:0>
	analog_write_reg8 (0x16,  PULL_WAKEUP_SRC_PE0 |
						(PULL_WAKEUP_SRC_PE1<<2) |
						(PULL_WAKEUP_SRC_PE2<<4) |
						(PULL_WAKEUP_SRC_PE3<<6));
	//E<7:4>
	analog_write_reg8 (0x17,  PULL_WAKEUP_SRC_PE4 |
						(PULL_WAKEUP_SRC_PE5<<2) |
						(PULL_WAKEUP_SRC_PE6<<4) |
						(PULL_WAKEUP_SRC_PE7<<6));
}
/**
 * @brief      This function servers to initialization all gpio.
 * @param[in]  en  -  if mcu wake up from deep retention mode, determine whether it is NOT necessary to reset analog register
 *                    1: set analog register
 *                    0: not set analog register
 * @return     none.
 */
/**Processing methods of unused GPIO
 * Set it to high resistance state and set it to open pull-up or pull-down resistance to
 * let it be in the determined state.When GPIO uses internal pull-up or pull-down resistance,
 * do not use pull-up or pull-down resistance on the board in the process of practical
 * application because it may have the risk of electric leakage .
 */
_attribute_ram_code_ void gpio_init(int anaRes_init_en)
{
	//PA group
	reg_gpio_pa_setting1 =
		(PA0_INPUT_ENABLE<<8) 	| (PA1_INPUT_ENABLE<<9)	| (PA2_INPUT_ENABLE<<10)	| (PA3_INPUT_ENABLE<<11) |
		(PA4_INPUT_ENABLE<<12)	| (PA5_INPUT_ENABLE<<13)	| (PA6_INPUT_ENABLE<<14)	| (PA7_INPUT_ENABLE<<15) |
		((PA0_OUTPUT_ENABLE?0:1)<<16)	| ((PA1_OUTPUT_ENABLE?0:1)<<17) | ((PA2_OUTPUT_ENABLE?0:1)<<18)	| ((PA3_OUTPUT_ENABLE?0:1)<<19) |
		((PA4_OUTPUT_ENABLE?0:1)<<20)	| ((PA5_OUTPUT_ENABLE?0:1)<<21) | ((PA6_OUTPUT_ENABLE?0:1)<<22)	| ((PA7_OUTPUT_ENABLE?0:1)<<23) |
		(PA0_DATA_OUT<<24)	| (PA1_DATA_OUT<<25)	| (PA2_DATA_OUT<<26)	| (PA3_DATA_OUT<<27) |
		(PA4_DATA_OUT<<28)	| (PA5_DATA_OUT<<29)	| (PA6_DATA_OUT<<30)	| (PA7_DATA_OUT<<31) ;
	reg_gpio_pa_setting2 =
		(PA0_DATA_STRENGTH<<8)		| (PA1_DATA_STRENGTH<<9)| (PA2_DATA_STRENGTH<<10)	| (PA3_DATA_STRENGTH<<11) |
		(PA4_DATA_STRENGTH<<12)	| (PA5_DATA_STRENGTH<<13)	| (PA6_DATA_STRENGTH<<14)	| (PA7_DATA_STRENGTH<<15) |
		(PA0_FUNC==AS_GPIO ? BIT(16):0)	| (PA1_FUNC==AS_GPIO ? BIT(17):0)| (PA2_FUNC==AS_GPIO ? BIT(18):0)| (PA3_FUNC==AS_GPIO ? BIT(19):0) |
		(PA4_FUNC==AS_GPIO ? BIT(20):0)	| (PA5_FUNC==AS_GPIO ? BIT(21):0)| (PA6_FUNC==AS_GPIO ? BIT(22):0)| (PA7_FUNC==AS_GPIO ? BIT(23):0);

	//PB group
	reg_gpio_pb_setting1 =
		(PB0_INPUT_ENABLE<<8) 	| (PB1_INPUT_ENABLE<<9)	| (PB2_INPUT_ENABLE<<10)	| (PB3_INPUT_ENABLE<<11) |
		(PB4_INPUT_ENABLE<<12)	| (PB5_INPUT_ENABLE<<13)	| (PB6_INPUT_ENABLE<<14)	| (PB7_INPUT_ENABLE<<15) |
		((PB0_OUTPUT_ENABLE?0:1)<<16)	| ((PB1_OUTPUT_ENABLE?0:1)<<17) | ((PB2_OUTPUT_ENABLE?0:1)<<18)	| ((PB3_OUTPUT_ENABLE?0:1)<<19) |
		((PB4_OUTPUT_ENABLE?0:1)<<20)	| ((PB5_OUTPUT_ENABLE?0:1)<<21) | ((PB6_OUTPUT_ENABLE?0:1)<<22)	| ((PB7_OUTPUT_ENABLE?0:1)<<23) |
		(PB0_DATA_OUT<<24)	| (PB1_DATA_OUT<<25)	| (PB2_DATA_OUT<<26)	| (PB3_DATA_OUT<<27) |
		(PB4_DATA_OUT<<28)	| (PB5_DATA_OUT<<29)	| (PB6_DATA_OUT<<30)	| (PB7_DATA_OUT<<31) ;
	reg_gpio_pb_setting2 =
		(PB0_DATA_STRENGTH<<8)		| (PB1_DATA_STRENGTH<<9)| (PB2_DATA_STRENGTH<<10)	| (PB3_DATA_STRENGTH<<11) |
		(PB4_DATA_STRENGTH<<12)	| (PB5_DATA_STRENGTH<<13)	| (PB6_DATA_STRENGTH<<14)	| (PB7_DATA_STRENGTH<<15) |
		(PB0_FUNC==AS_GPIO ? BIT(16):0)	| (PB1_FUNC==AS_GPIO ? BIT(17):0)| (PB2_FUNC==AS_GPIO ? BIT(18):0)| (PB3_FUNC==AS_GPIO ? BIT(19):0) |
		(PB4_FUNC==AS_GPIO ? BIT(20):0)	| (PB5_FUNC==AS_GPIO ? BIT(21):0)| (PB6_FUNC==AS_GPIO ? BIT(22):0)| (PB7_FUNC==AS_GPIO ? BIT(23):0);

	//PC group
	//ie
	analog_write_reg8(areg_gpio_pc_ie, 	(PC0_INPUT_ENABLE<<0) 	| (PC1_INPUT_ENABLE<<1)	| (PC2_INPUT_ENABLE<<2)	| (PC3_INPUT_ENABLE<<3) |
									(PC4_INPUT_ENABLE<<4)	| (PC5_INPUT_ENABLE<<5) | (PC6_INPUT_ENABLE<<6)	| (PC7_INPUT_ENABLE<<7) );

	//oen
	reg_gpio_pc_oen =
		((PC0_OUTPUT_ENABLE?0:1)<<0)	| ((PC1_OUTPUT_ENABLE?0:1)<<1) | ((PC2_OUTPUT_ENABLE?0:1)<<2)	| ((PC3_OUTPUT_ENABLE?0:1)<<3) |
		((PC4_OUTPUT_ENABLE?0:1)<<4)	| ((PC5_OUTPUT_ENABLE?0:1)<<5) | ((PC6_OUTPUT_ENABLE?0:1)<<6)	| ((PC7_OUTPUT_ENABLE?0:1)<<7);
	//dataO
	reg_gpio_pc_out =
		(PC0_DATA_OUT<<0)	| (PC1_DATA_OUT<<1)	| (PC2_DATA_OUT<<2)	| (PC3_DATA_OUT<<3) |
		(PC4_DATA_OUT<<4)	| (PC5_DATA_OUT<<5)	| (PC6_DATA_OUT<<6)	| (PC7_DATA_OUT<<7) ;

	//ds
	analog_write_reg8(areg_gpio_pc_ds, 	(PC0_DATA_STRENGTH<<0) 	| (PC1_DATA_STRENGTH<<1)  | (PC2_DATA_STRENGTH<<2)	| (PC3_DATA_STRENGTH<<3) |
									(PC4_DATA_STRENGTH<<4)	| (PC5_DATA_STRENGTH<<5)  | (PC6_DATA_STRENGTH<<6)	| (PC7_DATA_STRENGTH<<7) );

	reg_gpio_pc_gpio =
		(PC0_FUNC==AS_GPIO ? BIT(0):0)	| (PC1_FUNC==AS_GPIO ? BIT(1):0)| (PC2_FUNC==AS_GPIO ? BIT(2):0)| (PC3_FUNC==AS_GPIO ? BIT(3):0) |
		(PC4_FUNC==AS_GPIO ? BIT(4):0)	| (PC5_FUNC==AS_GPIO ? BIT(5):0)| (PC6_FUNC==AS_GPIO ? BIT(6):0)| (PC7_FUNC==AS_GPIO ? BIT(7):0);

	//PD group
	reg_gpio_pd_setting1 =
		(PD0_INPUT_ENABLE<<8) 	| (PD1_INPUT_ENABLE<<9)	| (PD2_INPUT_ENABLE<<10)	| (PD3_INPUT_ENABLE<<11) |
		(PD4_INPUT_ENABLE<<12)	| (PD5_INPUT_ENABLE<<13)| (PD6_INPUT_ENABLE<<14)	| (PD7_INPUT_ENABLE<<15) |
		((PD0_OUTPUT_ENABLE?0:1)<<16)	| ((PD1_OUTPUT_ENABLE?0:1)<<17) | ((PD2_OUTPUT_ENABLE?0:1)<<18)	| ((PD3_OUTPUT_ENABLE?0:1)<<19) |
		((PD4_OUTPUT_ENABLE?0:1)<<20)	| ((PD5_OUTPUT_ENABLE?0:1)<<21) | ((PD6_OUTPUT_ENABLE?0:1)<<22)	| ((PD7_OUTPUT_ENABLE?0:1)<<23) |
		(PD0_DATA_OUT<<24)	| (PD1_DATA_OUT<<25)	| (PD2_DATA_OUT<<26)	| (PD3_DATA_OUT<<27) |
		(PD4_DATA_OUT<<28)	| (PD5_DATA_OUT<<29)	| (PD6_DATA_OUT<<30)	| (PD7_DATA_OUT<<31) ;
	reg_gpio_pd_setting2 =
		(PD0_DATA_STRENGTH<<8)	| (PD1_DATA_STRENGTH<<9)	| (PD2_DATA_STRENGTH<<10)	| (PD3_DATA_STRENGTH<<11) |
		(PD4_DATA_STRENGTH<<12)	| (PD5_DATA_STRENGTH<<13)	| (PD6_DATA_STRENGTH<<14)	| (PD7_DATA_STRENGTH<<15) |
		(PD0_FUNC==AS_GPIO ? BIT(16):0)	| (PD1_FUNC==AS_GPIO ? BIT(17):0)| (PD2_FUNC==AS_GPIO ? BIT(18):0)| (PD3_FUNC==AS_GPIO ? BIT(19):0) |
		(PD4_FUNC==AS_GPIO ? BIT(20):0)	| (PD5_FUNC==AS_GPIO ? BIT(21):0)| (PD6_FUNC==AS_GPIO ? BIT(22):0)| (PD7_FUNC==AS_GPIO ? BIT(23):0);

	//PE group
	reg_gpio_pe_setting1 =
		(PE0_INPUT_ENABLE<<8) 	| (PE1_INPUT_ENABLE<<9)	| (PE2_INPUT_ENABLE<<10)	| (PE3_INPUT_ENABLE<<11) |
		(PE4_INPUT_ENABLE<<12)	| (PE5_INPUT_ENABLE<<13)| (PE6_INPUT_ENABLE<<14)	| (PE7_INPUT_ENABLE<<15) |
		((PE0_OUTPUT_ENABLE?0:1)<<16)	| ((PE1_OUTPUT_ENABLE?0:1)<<17) | ((PE2_OUTPUT_ENABLE?0:1)<<18)	| ((PE3_OUTPUT_ENABLE?0:1)<<19) |
		((PE4_OUTPUT_ENABLE?0:1)<<20)	| ((PE5_OUTPUT_ENABLE?0:1)<<21) | ((PE6_OUTPUT_ENABLE?0:1)<<22)	| ((PE7_OUTPUT_ENABLE?0:1)<<23) |
		(PE0_DATA_OUT<<24)	| (PE1_DATA_OUT<<25)	| (PE2_DATA_OUT<<26)	| (PE3_DATA_OUT<<27) |
		(PE4_DATA_OUT<<28)	| (PE5_DATA_OUT<<29)	| (PE6_DATA_OUT<<30)	| (PE7_DATA_OUT<<31) ;
	reg_gpio_pe_setting2 =
		(PE0_DATA_STRENGTH<<8)	| (PE1_DATA_STRENGTH<<9)	| (PE2_DATA_STRENGTH<<10)	| (PE3_DATA_STRENGTH<<11) |
		(PE4_DATA_STRENGTH<<12)	| (PE5_DATA_STRENGTH<<13)	| (PE6_DATA_STRENGTH<<14)	| (PE7_DATA_STRENGTH<<15) |
		(PE0_FUNC==AS_GPIO ? BIT(16):0)	| (PE1_FUNC==AS_GPIO ? BIT(17):0)| (PE2_FUNC==AS_GPIO ? BIT(18):0)| (PE3_FUNC==AS_GPIO ? BIT(19):0) |
		(PE4_FUNC==AS_GPIO ? BIT(20):0)	| (PE5_FUNC==AS_GPIO ? BIT(21):0)| (PE6_FUNC==AS_GPIO ? BIT(22):0)| (PE7_FUNC==AS_GPIO ? BIT(23):0);

	if(anaRes_init_en)
	{
		gpio_analog_resistance_init();
	}
}

/**
 * @brief      This function servers to enable gpio function.
 * @param[in]  pin - the selected pin.
 * @return     none.
 */
void gpio_set_gpio_en(gpio_pin_e pin)
{
	u8	bit = pin & 0xff;
	BM_SET(reg_gpio_func(pin), bit);
}


/**
 * @brief      This function servers to disable gpio function.
 * @param[in]  pin - the selected pin.
 * @return     none.
 */
void gpio_set_gpio_dis(gpio_pin_e pin)
{
	u8	bit = pin & 0xff;
	BM_CLR(reg_gpio_func(pin), bit);
}

/**
 * @brief      This function enable the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function.
 * @param[in]  value - enable or disable the pin's input function(0: disable, 1: enable)
 * @return     none
 */
void gpio_set_input_en(gpio_pin_e pin)
{
	unsigned char	bit = pin & 0xff;
	unsigned short group = pin & 0xf00;

	if(group == GPIO_GROUPA || GPIO_GROUPB|| group == GPIO_GROUPD || group == GPIO_GROUPE)
	{
			BM_SET(reg_gpio_ie(pin), bit);
	}

    else if(group == GPIO_GROUPC)
    {

    	analog_write_reg8(areg_gpio_pc_ie, analog_read_reg8(areg_gpio_pc_ie)|bit);

    }

}

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] gpio - the pin needs to set its pull-up/down resistor
 * @param[in] up_down - the type of the pull-up/down resistor
 * @return    none
 */
void gpio_setup_up_down_resistor(gpio_pin_e gpio, gpio_pull_type_e up_down)
{
	unsigned char r_val = up_down & 0x03;

	unsigned char base_ana_reg = 0x0e + ((gpio >> 8) << 1) + ( (gpio & 0xf0) ? 1 : 0 );  //group = gpio>>8;
	unsigned char shift_num, mask_not;

	if(gpio & 0x11){
		shift_num = 0;
		mask_not = 0xfc;
	}
	else if(gpio & 0x22){
		shift_num = 2;
		mask_not = 0xf3;
	}
	else if(gpio & 0x44){
		shift_num = 4;
		mask_not = 0xcf;
	}
	else if(gpio & 0x88){
		shift_num = 6;
		mask_not = 0x3f;
	}
	else{
		return;
	}

    if(GPIO_DP == gpio){
        //usb_dp_pullup_en (0);
    }

    analog_write_reg8(base_ana_reg, (analog_read_reg8(base_ana_reg) & mask_not) | (r_val << shift_num));
}

/**
 * @brief      This function disable the input function of a pin.
 * @param[in]  pin - the pin needs to set the input function.
 * @return     none
 */
void gpio_set_input_dis(gpio_pin_e pin)
{
	unsigned char	bit = pin & 0xff;
	unsigned short group = pin & 0xf00;

	if(group == GPIO_GROUPA || group == GPIO_GROUPD || group == GPIO_GROUPE|| group == GPIO_GROUPB)
	{

			BM_CLR(reg_gpio_ie(pin), bit);
	}

    else if(group == GPIO_GROUPC)
    {

    	analog_write_reg8(areg_gpio_pc_ie, analog_read_reg8(areg_gpio_pc_ie)&(~bit));
    }

}

/**
 * @brief      This function servers to set the specified GPIO as high resistor.
 * @param[in]  pin  - select the specified GPIO
 * @return     none.
 */
void gpio_shutdown(gpio_pin_e pin)
{
	unsigned short group = pin & 0xf00;
	unsigned char bit = pin & 0xff;
	switch(group)
	{
		case GPIO_GROUPA:
			reg_gpio_pa_oen |= bit;//disable output
			reg_gpio_pa_out &= (!bit);//set low level
			reg_gpio_pa_ie &= (!bit);//disable input
			break;
		case GPIO_GROUPB:
			reg_gpio_pb_oen |= bit;
			reg_gpio_pb_out &= (!bit);
			reg_gpio_pb_ie &= (!bit);
			break;
		case GPIO_GROUPC:
			reg_gpio_pc_oen |= bit;
			reg_gpio_pc_out &= (!bit);
			analog_write_reg8(areg_gpio_pc_ie, analog_read_reg8(areg_gpio_pc_ie) & (!bit));
			break;
		case GPIO_GROUPD:
			reg_gpio_pd_oen |= bit;
			reg_gpio_pd_out &= (!bit);
			reg_gpio_pd_ie &= (!bit);
			break;

		case GPIO_GROUPE:
			reg_gpio_pe_oen |= bit;
			reg_gpio_pe_out &= (!bit);
			reg_gpio_pe_ie &= (!bit);
			break;

		case GPIO_ALL:
		{
			//output disable
			reg_gpio_pa_oen = 0xff;
			reg_gpio_pb_oen = 0xff;
			reg_gpio_pc_oen = 0xff;
			reg_gpio_pd_oen = 0xff;

			//set low level
			reg_gpio_pa_out = 0x00;
			reg_gpio_pb_out = 0x00;
			reg_gpio_pc_out = 0x00;
			reg_gpio_pd_out = 0x00;

			//disable input
			reg_gpio_pa_ie = 0x80;					//SWS
			reg_gpio_pb_ie = 0x00;
			analog_write_reg8(areg_gpio_pc_ie, 0);
			reg_gpio_pd_ie = 0x00;
		}
	}
}


/**
 * @brief     This function set a pin's IRQ.
 * @param[in] pin 			- the pin needs to enable its IRQ
 * @param[in] trigger_type  - gpio interrupt type 0  rising edge 1 falling edge 2 high level 3 low level
 * @return    none
 */
void gpio_set_gpio_irq_trigger_type(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type)
{
	switch(trigger_type)
	{
	case INTR_RISING_EDGE:
		BM_CLR(reg_gpio_pol(pin), pin & 0xff);
		BM_CLR(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO);
	break;
	case INTR_FALLING_EDGE:
		BM_SET(reg_gpio_pol(pin), pin & 0xff);
		BM_CLR(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO);
	break;
	case INTR_HIGH_LEVEL:
		BM_CLR(reg_gpio_pol(pin), pin & 0xff);
		BM_SET(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO);
	break;
	case INTR_LOW_LEVEL:
		BM_SET(reg_gpio_pol(pin), pin & 0xff);
		BM_SET(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO);
	 break;
	}
	reg_gpio_irq_ctrl |= FLD_GPIO_CORE_INTERRUPT_EN;
	reg_gpio_irq_clr = FLD_GPIO_IRQ_CLR|FLD_GPIO_IRQ_GPIO2RISC0_CLR|FLD_GPIO_IRQ_GPIO2RISC1_CLR;//must clear cause to unexpected interrupt.
	BM_SET(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_MASK_GPIO);
}

/**
 * @brief     This function set a pin's IRQ_RISC0.
 * @param[in] pin 			- the pin needs to enable its IRQ
 * @param[in] trigger_type  - gpio interrupt type 0  rising edge 1 falling edge 2 high level 3 low level
 * @return    none
 */
void gpio_set_gpio2risc0_irq_trigger_type(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type)
{
	switch(trigger_type)
	{
	case INTR_RISING_EDGE:
		BM_CLR(reg_gpio_pol(pin), pin & 0xff);
		BM_CLR(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO2RISC0);
	break;
	case INTR_FALLING_EDGE:
		BM_SET(reg_gpio_pol(pin), pin & 0xff);
		BM_CLR(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO2RISC0);
	break;
	case INTR_HIGH_LEVEL:
		BM_CLR(reg_gpio_pol(pin), pin & 0xff);
		BM_SET(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO2RISC0);
		break;
	case INTR_LOW_LEVEL:
		BM_SET(reg_gpio_pol(pin), pin & 0xff);
		BM_SET(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO2RISC0);
	   break;
	}
	reg_gpio_irq_clr = FLD_GPIO_IRQ_CLR|FLD_GPIO_IRQ_GPIO2RISC0_CLR|FLD_GPIO_IRQ_GPIO2RISC1_CLR;//must clear cause to unexpected interrupt.
	BM_SET(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_MASK_GPIO2RISC0);
}
/**
 * @brief     This function set a pin's IRQ_RISC1.
 * @param[in] pin 			- the pin needs to enable its IRQ
 * @param[in] trigger_type  - gpio interrupt type 0  rising edge 1 falling edge 2 high level 3 low level
 * @return    none
 */
void gpio_set_gpio2risc1_irq_trigger_type(gpio_pin_e pin, gpio_irq_trigger_type_e trigger_type)
{
	switch(trigger_type)
	{
	case INTR_RISING_EDGE:
		BM_CLR(reg_gpio_pol(pin), pin & 0xff);
		BM_CLR(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO2RISC1);
	break;
	case INTR_FALLING_EDGE:
		BM_SET(reg_gpio_pol(pin), pin & 0xff);
		BM_CLR(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO2RISC1);
	break;
	case INTR_HIGH_LEVEL:
		BM_CLR(reg_gpio_pol(pin), pin & 0xff);
		BM_SET(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO2RISC1);
		break;
	case INTR_LOW_LEVEL:
		BM_SET(reg_gpio_pol(pin), pin & 0xff);
		BM_SET(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_LVL_GPIO2RISC1);
	   break;
	}
	reg_gpio_irq_clr = FLD_GPIO_IRQ_CLR|FLD_GPIO_IRQ_GPIO2RISC0_CLR|FLD_GPIO_IRQ_GPIO2RISC1_CLR;//must clear cause to unexpected interrupt.
	BM_SET(reg_gpio_irq_risc_mask, FLD_GPIO_IRQ_MASK_GPIO2RISC1);
}
/**
 * @brief      This function serves to enable gpio irq function.
 * @param[in]  none.
 * @return     none.
 */
void gpio_irq_en(gpio_pin_e pin)
{
	BM_SET(reg_gpio_irq_en(pin), pin & 0xff);
}
/**
 * @brief      This function serves to disable gpio irq function.
 * @param[in]  none.
 * @return     none.
 */
void gpio_irq_dis(gpio_pin_e pin)
{
	BM_CLR(reg_gpio_irq_en(pin), pin & 0xff);
}
/**
 * @brief      This function serves to enable gpio risc0 irq function.
 * @param[in]  none.
 * @return     none.
 */
void gpio_gpio2risc0_irq_en(gpio_pin_e pin)
{
	BM_SET(reg_gpio_irq_risc0_en(pin), pin & 0xff);
}
/**
 * @brief      This function serves to disable gpio risc0 irq function.
 * @param[in]  none.
 * @return     none.
 */
void gpio_gpio2risc0_irq_dis(gpio_pin_e pin)
{
	BM_CLR(reg_gpio_irq_risc0_en(pin), pin & 0xff);
}
/**
 * @brief      This function serves to enable gpio risc1 irq function.
 * @param[in]  none.
 * @return     none.
 */
void gpio_gpio2risc1_irq_en(gpio_pin_e pin)
{
	BM_SET(reg_gpio_irq_risc1_en(pin), pin & 0xff);
}
/**
 * @brief      This function serves to disable gpio risc1 irq function.
 * @param[in]  none.
 * @return     none.
 */
void gpio_gpio2risc1_irq_dis(gpio_pin_e pin)
{
	BM_CLR(reg_gpio_irq_risc1_en(pin), pin & 0xff);
}

/**
 * @brief     This function set a pin's pull-up/down resistor.
 * @param[in] gpio - the pin needs to set its pull-up/down resistor
 * @param[in] up_down - the type of the pull-up/down resistor
 * @return    none
 */

void gpio_set_up_down_res(gpio_pin_e pin, gpio_pull_type_e up_down_res)
{
	unsigned char r_val = up_down_res & 0x03;

	unsigned char base_ana_reg = 0x0e + ((pin >> 8) << 1) + ( (pin & 0xf0) ? 1 : 0 );  //group = gpio>>8;
	unsigned char shift_num, mask_not;

	if(pin & 0x11){
			shift_num = 0;
			mask_not = 0xfc;
		}
		else if(pin & 0x22){
			shift_num = 2;
			mask_not = 0xf3;
		}
		else if(pin & 0x44){
			shift_num = 4;
			mask_not = 0xcf;
		}
		else if(pin & 0x88){
			shift_num = 6;
			mask_not = 0x3f;
		}
		else{
			return;
		}
	analog_write_reg8(base_ana_reg, (analog_read_reg8(base_ana_reg) & mask_not) | (r_val << shift_num));
}

