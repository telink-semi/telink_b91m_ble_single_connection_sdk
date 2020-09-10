/********************************************************************************************************
 * @file	pwm.c
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
#include "pwm.h"


dma_config_st pwm_tx_dma_config={
	.dst_req_sel= DMA_REQ_PWM_TX,//tx req
	.src_req_sel=0,
	.dst_addr_ctrl=DMA_ADDR_FIX,
	.src_addr_ctrl=DMA_ADDR_INCREMENT,//increment
	.dstmode=DMA_HANDSHAKE_MODE,//handshake
	.srcmode=DMA_NORMAL_MODE,
	.dstwidth=DMA_CTR_WORD_WIDTH,//must word
	.srcwidth=DMA_CTR_WORD_WIDTH,//must word
	.src_burst_size=0,//must 0
	.read_num_en=0,
	.priority=0,
	.write_num_en=0,
	.auto_en=0,
};


/**
 * @brief     This fuction servers to set pin as pwm0
 * @param[in] pin - selected pin
 * @return	  none.
 */
void pwm_set_pin(pwm_pin_e pin)
{
	u8 val=0;
	u8 start_bit = (BIT_LOW_BIT(pin & 0xff) %4 )<<1;
    u8 mask =(u8) ~BIT_RNG(start_bit , start_bit+1);

    if(pin==PWM_PWM2_PB7){ 																					// Pad Function Mux:0
		 val = 0;
		 BM_CLR(reg_gpio_pad_mul_sel, BIT(3));
	}else if((pin==PWM_PWM0_PB4) || (pin==PWM_PWM4_PD7) ||(pin==PWM_PWM2_N_PE6) ||(pin==PWM_PWM3_N_PE7)){   // Pad Function Mux:1
		 val = 1<<(start_bit);
	}else{																									// Pad Function Mux:2
		 val = 2<<(start_bit);
		 reg_gpio_pad_mul_sel|=BIT(0);
	}

    reg_gpio_func_mux(pin)=(reg_gpio_func_mux(pin)& mask)|val;
    gpio_function_dis(pin);
}


/**
 * @brief     This function servers to configure DMA channel and some configures.
 * @param[in] chn - to select the DMA channel.
 * @return    none
 */
void pwm_set_dma_config(dma_chn_e chn)
{
	dma_config(chn,&pwm_tx_dma_config);
}


/**
 * @brief     This function servers to configure DMA channel address and length.
 * @param[in] chn - to select the DMA channel.
 * @param[in] buf_addr - the address where DMA need to get data from SRAM.
 * @param[in] len - the length of data in SRAM.
 * @return    none
 */
void pwm_set_dma_buf(dma_chn_e chn,u32 buf_addr,u32 len)
{
	dma_set_address( chn,convert_ram_addr_cpu2bus(buf_addr),reg_pwm_data_buf_adr);
	dma_set_size(chn,len,DMA_WORD_WIDTH);
}


/**
 * @brief     This function servers to enable DMA channel.
 * @param[in] chn - to select the DMA channel.
 * @return    none
 */
void pwm_ir_dma_mode_start(dma_chn_e chn)
{
	dma_chn_en(chn);
}



/**
 * @brief     This function servers to configure DMA head node.
 * @param[in] chn - to select the DMA channel.
 * @param[in] src_addr - to configure DMA source address.
 * @param[in] data_len - to configure DMA length.
 * @param[in] head_of_list - to configure the address of the next node configure.
 * @return    none
 */
void pwm_set_dma_chain_llp(dma_chn_e chn,u16 * src_addr, u32 data_len,dma_chain_config_st * head_of_list)
{
	 dma_config(chn,&pwm_tx_dma_config);
	 dma_set_address( chn,convert_ram_addr_cpu2bus(src_addr),reg_pwm_data_buf_adr);
	 dma_set_size(chn,data_len,DMA_WORD_WIDTH);
	 reg_dma_llp(chn)=(u32)convert_ram_addr_cpu2bus(head_of_list);
}


/**
 * @brief     This function servers to configure DMA cycle chain node.
 * @param[in] chn - to select the DMA channel.
 * @param[in] config_addr  - to servers to configure the address of the current node.
 * @param[in] llponit - to configure the address of the next node configure.
 * @param[in] src_addr - to configure DMA source address.
 * @param[in] data_len - to configure DMA length.
 * @return    none
 */
void pwm_set_tx_dma_add_list_element(dma_chn_e chn,dma_chain_config_st *config_addr,dma_chain_config_st *llponit ,u16 * src_addr,u32 data_len)
{
	config_addr->dma_chain_ctl= reg_dma_ctrl(chn)|BIT(0);
	config_addr->dma_chain_src_addr=(u32)convert_ram_addr_cpu2bus(src_addr);
	config_addr->dma_chain_dst_addr=reg_pwm_data_buf_adr;
    config_addr->dma_chain_data_len=dma_cal_size(data_len,DMA_WORD_WIDTH);
	config_addr->dma_chain_llp_ptr=(u32)convert_ram_addr_cpu2bus(llponit);
}

