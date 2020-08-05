/*
 * mdec.c
 *
 *  Created on: Apr 16, 2020
 *      Author: Telink
 */
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
unsigned char buf_flag=0;

void mdec_init(MDEC_PinTypeDef pin)
{
	analog_write_reg8(0x4e, analog_read_reg8(0x4e) & (~BIT(7)));    //turn to 32K_RC
	clock_cal_32k_rc();

	if(pin==GPIO_PA0_MDEC){
		analog_write_reg8(mdec_rst_addr,(analog_read_reg8(mdec_rst_addr) & (~FLD_CLS_MDEC)) | FLD_SELE_PA0);//A0
	    }
    if(pin==GPIO_PB7_MDEC){
    	analog_write_reg8(mdec_rst_addr,(analog_read_reg8(mdec_rst_addr) & (~FLD_CLS_MDEC)) | FLD_SELE_PB7 );//B7
    	}
    if(pin==GPIO_PC4_MDEC){
    	analog_write_reg8(mdec_rst_addr,(analog_read_reg8(mdec_rst_addr) & (~FLD_CLS_MDEC)) | FLD_SELE_PC4 );//C4
    	}
    if(pin==GPIO_PD0_MDEC){
        	analog_write_reg8(mdec_rst_addr,(analog_read_reg8(mdec_rst_addr) & (~FLD_CLS_MDEC)) | FLD_SELE_PD0 );//D0
        }
    if(pin==GPIO_PE0_MDEC){
			analog_write_reg8(mdec_rst_addr,(analog_read_reg8(mdec_rst_addr) & (~FLD_CLS_MDEC)) | FLD_SELE_PE0 );//E0
		}
}

/**
 * @brief This function is used to read the receive data of MDEC module's IO.
 * @param[out]   dat - The array to store date.
 * @return       1--decode success,  0--decode failure. if(alg_reg_sr(0x64)&BIT4) {
 */
unsigned char mdec_read_dat(unsigned char *dat)
{
	unsigned char m0,m1,m2,data_crc;
	if(analog_read_reg8(0x64) & BIT(4) )//
	{
		analog_write_reg8(0x64,0x10);
		buf_flag = dat[0]=analog_read_reg8(0x6a);
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

	}

	return 0;

}


