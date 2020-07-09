/********************************************************************************************************
 * @file     audio.c
 *
 * @brief    This is the Audio driver file for TLSR8278
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

#include "audio.h"
/********************************************************************************************************
 * @file     audio.c
 *
 * @brief    This is the Audio driver file for TLSR8278
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

#include "audio.h"
#include "pwm.h"
#include "../../common/types.h"
u8 audio_rx_dma_chn;
u8 audio_tx_dma_chn;
//#define    SDM_BUFFER_SIZE    4096
//volatile signed short SDMBuf[SDM_BUFFER_SIZE>>1];
dma_config_st audio_dma_rx_config={
		.dst_req_sel= 0,
		.src_req_sel=DMA_REQ_AUDIO0_RX,//rx req
		.dst_addr_ctrl=DMA_ADDR_INCREMENT,
		.src_addr_ctrl=DMA_ADDR_FIX,
		.dstmode=DMA_NORMAL_MODE,
		.srcmode=DMA_HANDSHAKE_MODE,
		.dstwidth=DMA_CTR_WORD_WIDTH,//must word
		.srcwidth=DMA_CTR_WORD_WIDTH,//must word
		.src_burst_size=0,//must 0
		.read_num_en=0,
		.priority=0,
		.write_num_en=0,
		.auto_en=0,//must 0
		};

dma_config_st audio_dma_tx_config={
		.dst_req_sel= DMA_REQ_AUDIO0_TX,//tx req
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
		.auto_en=0,//must 0
		};


#if 0
void pwm_set(void)
{
	reg_pwm_enable=0;
	reg_pwm0_enable=0;//off pwm0
	pwm_set_pin(PWM_PWM0_PB4);
	reg_pwm_clkdiv = 0;
	pwm_set_pwm0_mode(PWM_NORMAL_MODE);
	pwm_set_tcmp(PWM0_ID,1);
	pwm_set_tmax(PWM0_ID,2);//48/4=12m pwm  mclk to  ext codec clk

}
#endif
/**
 * @brief     This function selects  pin  for i2s.
 * @param[in]  pin - the selected pin.
 * @return    none
 */
void i2s_set_pin_mux(i2s_pin_e pin)
{

	u8 val=0;
	u8 start_bit = (BIT_LOW_BIT(pin & 0xff) %4 )<<1;
	u8 mask =(u8) ~BIT_RNG(start_bit , start_bit+1);
	if(pin==I2S_BCK_PC3)
	{
		val = 0;//function 0
	}
	else if((pin==I2S_ADC_LR_PC4)||(pin==I2S_ADC_DAT_PC5)||(pin==I2S_DAC_LR_PC6)||(pin==I2S_DAC_DAT_PC7))
	{
		val = 1<<(start_bit);//function 1
	}
	reg_gpio_func_mux(pin)=(reg_gpio_func_mux(pin)& mask)|val;
	gpio_set_gpio_dis(pin);

}

/**
 * @brief     This function configures i2s pin.
 * @param[in] none
 * @return    none
 */
void i2s_set_pin(void)
{

	i2s_set_pin_mux(I2S_BCK_PC3);
	i2s_set_pin_mux(I2S_ADC_LR_PC4);
	i2s_set_pin_mux(I2S_ADC_DAT_PC5);
	i2s_set_pin_mux(I2S_DAC_LR_PC6);
	i2s_set_pin_mux(I2S_DAC_DAT_PC7);

}

/**
 * @brief     This function configures dmic pin.
 * @param[in] pin_gp the group in
 * @return    none
 */
void audio_dmic_set_pin(dmic_pin_group_e pin_gp)
{

	if(pin_gp==DMIC_GROUPB_B2_DAT_B3_B4_CLK)
	{
		reg_gpio_pad_mul_sel=BIT(2);
		reg_gpio_pb_fuc_h=reg_gpio_pb_fuc_h&(~BIT_RNG(0,1));//PB4 fuc
		reg_gpio_pb_fuc_l=(reg_gpio_pb_fuc_h&(~BIT_RNG(4,7)))|0x0f;//PB2  PB3 fuc
		gpio_set_gpio_dis(GPIO_PB2|GPIO_PB3|GPIO_PB4);
	}
	else if(DMIC_GROUPC_C1_DAT_C2_C3_CLK)////can not use
	{
		reg_gpio_pc_fuc_l=(reg_gpio_pc_fuc_l&(~BIT_RNG(2,7)))|((2<<2)|(2<<4)|(2<<6));
		gpio_set_gpio_dis(GPIO_PC1|GPIO_PC2|GPIO_PC3);

	}
	else if(DMIC_GROUPD_D4_DAT_D5_D6_CLK)//can not use
	{
		reg_gpio_pd_fuc_h=(reg_gpio_pd_fuc_h&(~BIT_RNG(0,5)))|((1<<0)|(1<<2)|(1<<4));
		gpio_set_gpio_dis(GPIO_PD4|GPIO_PD5|GPIO_PD6);
	}

}


void aduio_i2c_set_pin(void)
{
	reg_i2c_sct0=0x01;
	reg_i2c_sp=0x1f;//set i2c clock
	reg_i2c_id=0x34;//set i2c id

	i2c_set_pin(I2C_GPIO_SDA_B3,I2C_GPIO_SCL_B2);
}



/**
 * @brief     This function serves to set rx_dam channel and config dma rx default.
 * @param[in] dma_chn_e: dma channel.
 * @return    none
 */
 void audio_rx_dma_en(void)
{
	 dma_chn_en(audio_rx_dma_chn);
}

 void audio_tx_dma_en(void)
{
	 dma_chn_en(audio_tx_dma_chn);
}


void audio_rx_dma_config(dma_chn_e chn,u16 *dst_addr,u32 data_len,dma_llp_config_t *head_of_list)
{
	audio_rx_dma_chn=chn;
	audio_set_rx_buff_length(data_len);
	dma_config(audio_rx_dma_chn,&audio_dma_rx_config);
	dma_set_address( chn,REG_AUDIO_AHB_BASE,(u32)reg_dma_addr(dst_addr));//SRC_addr  //dst addr ch0 adc
	dma_set_size(chn,data_len,DMA_WORD_WIDTH);
	reg_dma_llp(chn)=(u32)reg_dma_addr(head_of_list);

}

void audio_rx_dma_add_list_element(dma_llp_config_t *config_addr,dma_llp_config_t *llponit ,u16 * dst_addr,u32 data_len)
{
	config_addr->audio_dma_ctl=reg_dma_ctrl(audio_rx_dma_chn)|BIT(0);
	config_addr->audio_dma_src_addr=REG_AUDIO_AHB_BASE;
	config_addr->audio_dma_dst_addr=(u32)reg_dma_addr(dst_addr);
	config_addr->audio_dma_data_len=(( data_len+3)/4)|( ( data_len % 4)<<22);
	config_addr->audio_dma_llp_ptr=(u32)reg_dma_addr(llponit);
}



void audio_tx_dma_config(dma_chn_e chn,u16 * src_addr, u32 data_len,dma_llp_config_t * head_of_list)
{
	 audio_tx_dma_chn=chn;
	 audio_set_tx_buff_length(data_len);
	 dma_config(audio_tx_dma_chn,&audio_dma_tx_config);
	 dma_set_address( chn,(u32)reg_dma_addr(src_addr),REG_AUDIO_AHB_BASE);
	 dma_set_size(chn,data_len,DMA_WORD_WIDTH);
	 reg_dma_llp(chn)=(u32)reg_dma_addr(head_of_list);
}


void audio_tx_dma_add_list_element(dma_llp_config_t *config_addr,dma_llp_config_t *llponit ,u16 * src_addr,u32 data_len)
{
	config_addr->audio_dma_ctl=reg_dma_ctrl(audio_tx_dma_chn)|BIT(0);
	config_addr->audio_dma_src_addr=(u32)reg_dma_addr(src_addr);
	config_addr->audio_dma_dst_addr=REG_AUDIO_AHB_BASE;
	config_addr->audio_dma_data_len=(( data_len+3)/4)|( ( data_len % 4)<<22);
	config_addr->audio_dma_llp_ptr=(u32)reg_dma_addr(llponit);
}


#if 0
void i2c_init(void)
{
	reg_i2c_sct0=0x01;
	reg_i2c_sp=0x1f;//set i2c clock
	////AUDIO_I2S_TO_EXT_CODEC ///
#if 0
	reg_i2c_id=0x34<<1;//set i2c id
#endif

	reg_audio_i2c_addr=0x34;
}



//////////  Codec Config by I2C
u8 i2c_codec_read(u8 addr)//todo
{
	reg_i2c_data_buf(0)=addr;
	reg_i2c_len=0x01;//rx_len
	reg_i2c_sct1=FLD_I2C_LS_ID|FLD_I2C_LS_ADDR|FLD_I2C_LS_START;

	while(i2c_master_busy()); //wait busy=0
	while(reg_i2c_mst & FLD_I2C_ACK_IN);//wait ack=0

	reg_i2c_sct1=FLD_I2C_LS_ID|FLD_I2C_LS_DATAR|FLD_I2C_LS_START|FLD_I2C_LS_ID_R|FLD_I2C_LS_ACK;
	while(i2c_master_busy()); //wait busy=0
	u8 rdat8= reg_i2c_data_buf(0);
	reg_i2c_sct1=FLD_I2C_LS_STOP|FLD_I2C_LS_ID_R;
	while(i2c_master_busy()); //wait busy=0
	return rdat8;
}


u8 i2c_codec_read_ext(u8 addr)//todo
{
	reg_i2c_data_buf(0)=addr;
	reg_i2c_len=0x01;//rx_len
	reg_i2c_sct1=FLD_I2C_LS_ID|FLD_I2C_LS_DATAW|FLD_I2C_LS_START|FLD_I2C_LS_STOP;

	while(i2c_master_busy()); //wait busy=0
	while(reg_i2c_mst & FLD_I2C_ACK_IN);//wait ack=0

	reg_i2c_sct1=FLD_I2C_LS_ID|FLD_I2C_LS_DATAR|FLD_I2C_LS_START|FLD_I2C_LS_STOP|FLD_I2C_LS_ID_R;
	while(i2c_master_busy()); //wait busy=0
	while(reg_i2c_mst & FLD_I2C_ACK_IN);//wait ack=0
	u8 rdat8= reg_i2c_data_buf(0);
	return rdat8;
}



//////////  Codec Config by I2C
void i2c_codec_write(u8 addr ,u8 wdat)
{

	reg_i2c_data_buf(0)=addr;
	reg_i2c_data_buf(1)=wdat;
	reg_i2c_len=2;//tx_len
	reg_i2c_sct1=FLD_I2C_LS_ID|FLD_I2C_LS_DATAW| FLD_I2C_LS_START| FLD_I2C_LS_STOP;
	while(i2c_master_busy()); //wait busy=0
	while(reg_i2c_mst & FLD_I2C_ACK_IN);//wait ack=0
}





//////////  Codec Config by I2C
void i2c_codec_set(u8 addr, u8 wdat)
	{
	while(!(i2c_codec_read(0x00)&0x80));


	i2c_codec_write(0x23<<1,0x80);

	i2c_codec_write(0x24<<1,0x80);
	//write_reg8(REG_AUDIO_AHB_BASE+(0xa4<<2), 0x80);//adc mute

	i2c_codec_write(0x06<<1,0x00);
	//write_reg8(REG_AUDIO_AHB_BASE+(0x86<<2), 0x00);//out of analog sleep mode


	/*code master*/
	#if 0
	i2c_codec_write(0x08<<1,0x03);
	//	write_reg8(REG_AUDIO_AHB_BASE+(0x88<<2), 0x03);//codec dac master mode
	i2c_codec_write(0x09<<1,0x03);
	//write_reg8(REG_AUDIO_AHB_BASE+(0x89<<2), 0x03);//codec adc master mode
	#else
	/*code slave*/
	i2c_codec_write(0x08<<1,0x23);//codec dac slave mode
	i2c_codec_write(0x09<<1,0x23);//codec adc slave mode
	#endif

	i2c_codec_write(0x0a<<1,0x00);//audio interface active

	i2c_codec_write(0x0b<<1,0x08);//dac sample rate: 48


	i2c_codec_write(0x0f<<1,0xf8);//adc hpf_en, adc sample rate: 48k


	while(!(i2c_codec_read(0x00)==0x98));//wait codec adc/dac locked


	i2c_codec_write(0x23<<1,0x00);//dac unmute


	i2c_codec_write(0x24<<1,0x00);//adc unmute


}



////////// set ext Codec Config by I2C
void i2c_ext_codec_set(void)
{
	u8 wdat8;
	i2c_codec_write(0x1e,0x00);//reset device

	wdat8 = (0x10<<0 | 0<<7);   //0: LINVOL(max:0x1F);  0x7:LINMUTE
	i2c_codec_write(0x00,wdat8); //left line in

	wdat8 = (0x10<<0 | 0<<7);   //0: RINVOL(max:0x1F);  0x7:RINMUTE
	i2c_codec_write(0x02,wdat8); //right line in

	wdat8 = (1<<0 | 1<<1 | 0<<2 | 0<<3 | 1<<4); //0:MICBOOST; 1:MUTEMIC; 2:INSEL MIC; 3:BYPASS; 4:DACSEL
	i2c_codec_write(0x08,wdat8); //ana audio path ctrl

	wdat8 = (0<<3);        //3:DACMUTE
	i2c_codec_write(0x0a,wdat8);

	wdat8 = (0<<0 | 1<<1 | 0<<2 | 0<<3 | 0<<4 | 1<<5 | 0<<6 | 0<<7);   //0:LINEINPD; 1:MICPD; 2:ADCPD; 3:DACPD; 4:OUTPD; 5:OSCPD; 6:CLKOUTPD; 7:POWEROFF
	i2c_codec_write(0x0c, wdat8);  //pwdn ctrl,

	wdat8 = (2<<0 | 0<<2 | 0<<6); //0:I2S FORMAT; 2:DATA WDTH; 6:MASTER MODE
	i2c_codec_write(0x0e, wdat8);  //I2S format::

	//wdat8 = (0<<0 | 0<<1 | 0<<2 | 0<<6 | 0<<7);  //0:USB MODE; 1:BOSR; 2:SR; 6:CLKDIV2; 7:CLKODIV2;    SR:0--->48KHz
	wdat8 = (1<<0 | 0<<1 | 6<<2 | 0<<6 | 0<<7);  //0:USB MODE; 1:BOSR; 2:SR; 6:CLKDIV2; 7:CLKODIV2;    SR:6--->32KHz
	i2c_codec_write(0x10, wdat8);   //ADC&DAC sample rate:

	i2c_codec_write(0x12, 0x01);  //activate device

	BM_SET(reg_pwm0_enable, BIT(0));
	//pwm_start(PWM0_ID);
}

#endif

void audio_adc_power_down(void)
{
	audio_write_codec_register(0x24,0x80);//adc mute
	audio_write_codec_register(0x06,0x07);//active power down mode
	audio_i2s_clk_dis();
	audio_codec_clk_dis();

}

void audio_adc_power_restore(void)
{
	audio_i2s_clk_en();
	audio_codec_clk_en();
	audio_write_codec_register(0x06,0x06);//analog sleep mode
	audio_write_codec_register(0x24,0x00);//adc unmute
}


void audio_codec_dac_config(codec_m_s_mode_e mode,codec_wreg_mode_e wreg_mode, audio_sample_rate_e rate)
{

	audio_set_codec_clk(1,16);////from ppl 192/16=12M
	audio_write_codec_register(0x06,0x04);//active analog sleep mode
	while(!(audio_read_codec_register(0x00)&0x80));//wait codec can be configed

	audio_write_codec_register(0x23,0x80);//dac mute

	audio_write_codec_register(0x06,0x00);//out of analog sleep mode

	reg_audio_codec_dac_ctr= MASK_VAL( FLD_AUDIO_CODEC_A_DAC_FORMAT, CODEC_I2S_MODE,\
		                             	FLD_AUDIO_CODEC_DAC_SB, CODEC_ITF_AC, \
			                             FLD_AUDIO_CODEC_A_DAC_SLAVE, (!mode), \
					                     FLD_AUDIO_CODEC_A_DAC_WL, CODEC_BIT_16_DATA);

	audio_write_codec_register(0x0a,0x00);//audio interface active
	audio_write_codec_register(0x0b, rate);//dac sample rate

	while(!(audio_read_codec_register(0x00)==0x98));//wait codec adc/dac locked
	audio_write_codec_register(0x23,0x00);//dac unmute
}

void audio_codec_adc_config(codec_m_s_mode_e mode,  audio_input_mode_e in_mode,codec_wreg_mode_e wreg_mode, audio_sample_rate_e rate)
{

	audio_set_codec_clk(1,16);////from ppl 192/16=12M
	audio_write_codec_register(0x06,0x04);//active analog sleep mode
	while(!(audio_read_codec_register(0x00)&0x80));//wait codec can be configed
	audio_write_codec_register(0x24,0x80);//adc mute
	audio_write_codec_register(0x06,0x00);//out of analog sleep mode
if (in_mode==AMIC_IN)
{
	audio_write_codec_register(0x17,0x00);//mic1, single_ended
	audio_write_codec_register(0x18,0x00);//mic1, single_ended
	reg_audio_codec_mic_l_R_gain= MASK_VAL( FLD_AUDIO_CODEC_AMIC_L_GAIN, CODEC_AMIC_GAIN_16_DB,\
			                                FLD_AUDIO_CODEC_AMIC_R_GAIN, CODEC_AMIC_GAIN_16_DB);
}
else if(in_mode==DMIC_IN)
{
	reg_audio_dmic_12=MASK_VAL( FLD_AUDIO_CODEC_ADC_DMIC_SEL2, 1,\
			                    FLD_AUDIO_CODEC_ADC_DMIC_SEL1,1, \
								FLD_AUDIO_CODEC_DMIC2_SB,CODEC_ITF_AC,\
								FLD_AUDIO_CODEC_DMIC1_SB, CODEC_ITF_AC);

	reg_audio_adc1_gain=MASK_VAL(  FLD_AUDIO_CODEC_ADC_GID1,1,\
			                       FLD_AUDIO_CODEC_ADC_LRGID,CODEC_DMIC_GAIN_20_DB);//0db~43db  1db step
}
	reg_audio_codec_adc_ctr= MASK_VAL( FLD_AUDIO_CODEC_A_DAC_FORMAT, CODEC_I2S_MODE,\
			                          FLD_AUDIO_CODEC_A_DAC_SLAVE, (!mode), \
								      FLD_AUDIO_CODEC_A_DAC_WL, CODEC_BIT_16_DATA);

	audio_write_codec_register(0x0a,0x00);//audio adc interface active

	audio_write_codec_register(0x0f,((rate&0xf)|BIT(4)));//adc hpf_en, adc sample rate

	while(!(audio_read_codec_register(0x00)==0x98));//wait codec adc/dac locked

	audio_write_codec_register(0x24,0x00);//adc unmute
}

void audio_mux_config(audio_flow_e audio_flow, audio_in_mode_e ain0_mode , audio_in_mode_e ain1_mode,audio_out_mode_e i2s_aout_mode)
{

	reg_audio_ctrl |= audio_flow;
	reg_audio_tune= MASK_VAL( FLD_AUDIO_I2S_I2S_AIN0_COME, ain0_mode,\
							FLD_AUDIO_I2S_I2S_AIN1_COME, ain1_mode, \
							FLD_AUDIO_I2S_I2S_AOUT_COME, i2s_aout_mode);

}


void audio_data_path_sel (audio_mux_ain_e ain0_sel, audio_mux_aout_e aout0_sel, audio_mux_ain_e ain1_sel, audio_mux_aout_e aout1_sel)
{

	reg_audio_sel = MASK_VAL( FLD_AUDIO_AIN0_SEL, ain0_sel,\
								FLD_AUDIO_AOUT0_SEL, aout0_sel, \
								FLD_AUDIO_AIN1_SEL, ain1_sel, \
								FLD_AUDIO_AOUT1_SEL, aout1_sel);

}


void audio_i2s_config(i2s_mode_select_e i2s_format,i2s_data_select_e wl,i2s_m_s_select_e  i2s_m_s)
{

    reg_i2s_cfg = MASK_VAL( FLD_AUDIO_I2S_FORMAT, i2s_format,\
		   FLD_AUDIO_I2S_WL, wl, \
		   FLD_AUDIO_I2S_LRP, 1, \
		   FLD_AUDIO_I2S_LRSWAP, 0, \
		   FLD_AUDIO_I2S_ADC_DCI_MS, i2s_m_s, \
		   FLD_AUDIO_I2S_DAC_DCI_MS, i2s_m_s);

}

void audio_set_i2s_clock (audio_sample_rate_e audio_rate,audio_rate_match_e  match)
{

	switch(audio_rate)
	{
		case AUDIO_8K :
			audio_set_i2s_clk(1,8);//set i2s clk 24M
			audio_set_i2s_bclk(12);//24/(2*12) = 1m bclk
			audio_set_lrclk(125); //bclk/125=8k
			break;
		case AUDIO_16K:
			audio_set_i2s_clk(1,8);//set i2s clk 24M
			audio_set_i2s_bclk(6);//24/(2*6) = 2m bclk
			audio_set_lrclk(125); //bclk/125=16k
			break;
		case AUDIO_32K:
			audio_set_i2s_clk(1,8);//set i2s clk 24M
			audio_set_i2s_bclk(3);//24/(2*3) = 4m bclk
			audio_set_lrclk(125); //bclk/125=32k
			break;
		case AUDIO_48K:
			audio_set_i2s_clk(1,8);//set i2s clk 24M
			audio_set_i2s_bclk(2);//24/(2*2) = 6m bclk
			audio_set_lrclk(125); //bclk/125=48k
			break;
		case AUDIO_44EP1K:
			if(match==AUDIO_RATE_EQUAL)//44099.9
			{
				audio_set_i2s_clk(8,215);
				audio_set_i2s_bclk(0);
				audio_set_lrclk(162);
			}
			else if(match==AUDIO_RATE_GT)//44110.2
			{
				audio_set_i2s_clk(11,228);
				audio_set_i2s_bclk(0);
				audio_set_lrclk(210);
			}
			else if(match==AUDIO_RATE_LT)//44094.4
			{
				audio_set_i2s_clk(7,254);
				audio_set_i2s_bclk(0);
				audio_set_lrclk(120);
			}

		break;
	}

}


void pwm_set(void)
{
	reg_pwm_enable=0;
	reg_pwm0_enable=0;//off pwm0
	pwm_set_pin(PWM_PWM0_PB4);
	reg_pwm_clkdiv = 0;
	pwm_set_pwm0_mode(PWM_NORMAL_MODE);
	pwm_set_tcmp(PWM0_ID,1);
	pwm_set_tmax(PWM0_ID,2);//48/4=12m pwm  mclk to  ext codec clk

}

dma_llp_config_t audio_tx_dma_list_cfg;
dma_llp_config_t audio_rx_dma_list_cfg;

void	audio_i2s_codec_config (int sample_rate, u32 * speaker_buff, int speaker_size, u32 *mic_buff, int mic_size)
{
	int rate = 	sample_rate == 16000 ? AUDIO_16K :
				sample_rate == 32000 ? AUDIO_32K :
				sample_rate == 48000 ? AUDIO_48K : AUDIO_44EP1K;

	audio_mux_config(CODEC_I2S,BIT_16_MONO,BIT_16_MONO,BIT_16_MONO_FIFO0);
	audio_i2s_config(I2S_I2S_MODE,I2S_BIT_16_DATA,I2C_RX_TX_SLAVE);
	audio_set_i2s_clock(rate, AUDIO_RATE_EQUAL);

	audio_clk_en(1,1);

	audio_codec_adc_config(CODEC_MASTER,LINE_IN,MCU_WREG, AUDIO_16K);
	audio_codec_dac_config(CODEC_MASTER,MCU_WREG, rate);
	audio_data_path_sel(I2S_DATA_IN_FIFO,I2S_OUT,NO_USE,NO_USE);

	if (speaker_buff && speaker_size)
	{
		audio_tx_dma_config(DMA3, (u16*)speaker_buff, speaker_size, &audio_tx_dma_list_cfg);
		audio_tx_dma_add_list_element(&audio_tx_dma_list_cfg, &audio_tx_dma_list_cfg, (u16*)speaker_buff, speaker_size);
		audio_tx_dma_en();
	}

	if (mic_buff && mic_size)
	{
		audio_rx_dma_config(DMA2,(u16*)mic_buff, mic_size, &audio_rx_dma_list_cfg);
		audio_rx_dma_add_list_element(&audio_rx_dma_list_cfg, &audio_rx_dma_list_cfg,(u16*)mic_buff, mic_size);
		audio_rx_dma_en();
	}
}



