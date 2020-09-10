/********************************************************************************************************
 * @file	mdec.c
 *
 * @brief	This is the source file for B91
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
#include"mdec.h"
#include "compiler.h"
#include "reg_include/mdec_reg.h"
#include "analog.h"
#include "clock.h"
//#include "interrupt.h"
/**
 * @brief This function is used to initialize the MDEC module,include clock setting and input IO select.
 * @param[in]    pin - MDEC_PinTypeDef. In order to distinguish which pin the data is input from,
                 only one input pin can be selected one time.
 * @return none
 */

void mdec_init(mdec_pin_e pin)
{
	analog_write_reg8(mdec_rst_addr,(analog_read_reg8(mdec_rst_addr) & (~FLD_CLS_MDEC)) | pin);//A0/B7/C4/D0/E0
}

/**
 * @brief This function is used to read the receive data of MDEC module's IO.
 * @param[out]   dat - The array to store date.
 * @return       1--decode success,  0--decode failure. if(alg_reg_sr(0x64)&BIT4) {
 */
unsigned char mdec_read_dat(unsigned char *dat)
{
	unsigned char m0,m1,m2,data_crc;
		dat[0]=analog_read_reg8(0x6a);
		dat[1]=analog_read_reg8(0x6b);
		dat[2]=analog_read_reg8(0x6c);
		dat[3]=analog_read_reg8(0x6d);
		dat[4]=analog_read_reg8(0x6e);

		m0= ((dat[0]>>5)<<4);
		m1= dat[0]&0x07;
		m2= m0+m1;
		data_crc=(((m2+dat[1])^dat[2])+dat[3])^0xa5;
	//	write_reg32(reg_irq_src(0), read_reg32(reg_irq_src(0)) | IRQ62_PM_TM);
	//	plic_interrupt_enable(IRQ62_PM_TM);
		if(data_crc==dat[4]){
			return 1;
		}
	return 0;

}


