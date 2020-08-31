/********************************************************************************************************
 * @file     s7816.c 
 *
 * @brief    This is the source file for TLSR8278
 *
 * @author	 Driver Group
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
#include "s7816.h"
#include "dma.h"
#include "plic.h"
#include "uart.h"

/**
 * @brief      	This function is used to initiate 7816 module of MCU
 * @param[in]  	Div	-set the divider of clock of 7816 module:
 * 				7816clk = sysclk/(0x1401f0[6:4]*2),	0x1401f0[7]:enable 7816clk
 * 				7816clk:  0x40-2Mhz   0x20-4Mhz
 * 				baudrate: 0x40-16194  0x20-32388
 * @return     	none
 */
void s7816_set_clk(unsigned char Div)
{
	//---Set 7816 CLK divider
    //caution:7816 clk module only controlled by the highest bit of 7816 clk mode
	reg_7816_clk_div&=0x0f;
	reg_7816_clk_div|=(unsigned char)Div;
}


/**
 * @brief      	This function is used to initiate 7816 module of MCU
 * @param[in]  	Pin_7816_TRX	-select the I/O 	pin of 7816 module
 * @param[in]	Pin_7816_RST	-select the RST 	pin of 7816 module
 * @param[in]	Pin_7816_VCC	-select the VCC 	pin of 7816 module
 * @return     	none
 */
void s7816_set_pin(S7816_RTx_PinDef Pin_7816_TRX)
{

	//---enable UART 7816 CLK(GPIO_PA0
    reg_gpio_func_mux(GPIO_PA0)=(reg_gpio_func_mux(GPIO_PA0)&(~BIT_RNG(0,1)))|BIT(0);
    gpio_function_dis(GPIO_PA0);
	//---enable UART 7816 RXT(Optional)

    u8 val = 0;
    u8 mask = 0xff;
	switch(Pin_7816_TRX)
	{
		case S7816_UART0_RTX_A4://take care the details of every pin when using this pin

			gpio_set_up_down_res(S7816_UART0_RTX_A4, GPIO_PIN_PULLUP_10K);
			/***  set PA4 pin as s7816 function ****/
			mask = (unsigned char)~(BIT(0)|BIT(1));
			val = BIT(0);
			reg_gpio_func_mux(S7816_UART0_RTX_A4)=(reg_gpio_func_mux(S7816_UART0_RTX_A4)& mask)|val;
			gpio_function_dis(S7816_UART0_RTX_A4);


			gpio_input_en(S7816_UART0_RTX_A4);
			break;
		case S7816_UART0_RTX_B3:
			gpio_set_up_down_res(S7816_UART0_RTX_B3, GPIO_PIN_PULLUP_10K);

			mask = (unsigned char)~(BIT(6)|BIT(7));
			val = BIT(7);
			reg_gpio_pad_mul_sel|=BIT(0);
			reg_gpio_func_mux(S7816_UART0_RTX_B3)=(reg_gpio_func_mux(S7816_UART0_RTX_B3)& mask)|val;
			gpio_function_dis(S7816_UART0_RTX_B3);


			gpio_input_en(S7816_UART0_RTX_B3);
			break;
		case S7816_UART0_RTX_D3:
			gpio_set_up_down_res(S7816_UART0_RTX_D3, GPIO_PIN_PULLUP_10K);

			mask = (unsigned char)~(BIT(6)|BIT(7));
			val = 0;
		  reg_gpio_func_mux(S7816_UART0_RTX_D3)=(reg_gpio_func_mux(S7816_UART0_RTX_D3)& mask)|val;
		  gpio_function_dis(S7816_UART0_RTX_D3);
			//gpio_set_func(GPIO_PD3,AS_UART0_RX);

		  gpio_input_en(S7816_UART0_RTX_D3);
			break;

		case S7816_UART1_RTX_C7://take care the details of every pin when using this pin
			gpio_set_up_down_res(S7816_UART1_RTX_C7, GPIO_PIN_PULLUP_10K);

			mask = (unsigned char)~(BIT(6)|BIT(7));
			val = BIT(7);
			reg_gpio_pad_mul_sel|=BIT(0);
			reg_gpio_func_mux(S7816_UART1_RTX_C7)=(reg_gpio_func_mux(S7816_UART1_RTX_C7)& mask)|val;
			gpio_function_dis(S7816_UART1_RTX_C7);
			//gpio_set_func(GPIO_PC7,AS_UART1_RX);

			gpio_input_en(S7816_UART1_RTX_C7);
			break;
		case S7816_UART1_RTX_D7:
			gpio_set_up_down_res(S7816_UART1_RTX_D7, GPIO_PIN_PULLUP_10K);

			mask = (unsigned char)~(BIT(6)|BIT(7));
			val = 0;
		   reg_gpio_func_mux(S7816_UART1_RTX_D7)=(reg_gpio_func_mux(S7816_UART1_RTX_D7)& mask)|val;
		   gpio_function_dis(S7816_UART1_RTX_D7);
			//gpio_set_func(GPIO_PD7,AS_UART1_RX);

		   gpio_input_en(S7816_UART1_RTX_D7);
			break;
		case S7816_UART1_RTX_E2:
			gpio_set_up_down_res(S7816_UART1_RTX_E2, GPIO_PIN_PULLUP_10K);

			mask = (unsigned char)~(BIT(4)|BIT(5));
			val = BIT(4);
			reg_gpio_func_mux(S7816_UART1_RTX_E2)=(reg_gpio_func_mux(S7816_UART1_RTX_E2)& mask)|val;
			gpio_function_dis(S7816_UART1_RTX_E2);
			//gpio_set_func(GPIO_PE2,AS_UART1_RX);

			gpio_input_en(S7816_UART1_RTX_E2);
			break;
		default:
			break;
	}
}



/**
 * @brief      	This function is used to transform half duplex mode of 7816
 * @param[in]  	mode	- half_duplex_mode_TX/RX is transformed by setting 0x9b[5]
 * 						0x14008b[5]=1:half_duplex_mode_TX;0x14008b[5]=0:half_duplex_mode_RX
 * @return     	none
 */
void  s7816_set_half_duplex(S7816_Half_Duplex_ModeDef mode,S7816_UART_CHN chn)
{
	if(mode==S7816_TX)
	{
		reg_uart_rx_timeout1(chn)|=FLD_UART_P7816_EN;
	}

	else if(mode==S7816_RX){
		reg_uart_rx_timeout1(chn)&=~FLD_UART_P7816_EN;
	}

}
