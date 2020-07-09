

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
    gpio_set_gpio_dis(pin);
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

	reg_dma_src_addr(chn)= reg_dma_addr(buf_addr);
	reg_dma_dst_addr(chn)=reg_pwm_data_buf_adr;

	reg_dma_size(chn) =((len+DMA_WORD_WIDTH-1)/DMA_WORD_WIDTH)|( (len % DMA_WORD_WIDTH)<<22);
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










