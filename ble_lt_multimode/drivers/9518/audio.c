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

dma_llp_config_t audio_tx_dma_list_cfg;
dma_llp_config_t audio_rx_dma_list_cfg;

aduio_i2s_codec_config_st audio_i2s_codec_config=
{
   .audio_in_mode 		=BIT_16_MONO,
   .audio_out_mode		=BIT_16_MONO_FIFO0,
   .i2s_data_select		=I2S_BIT_16_DATA,
   .codec_data_select	=CODEC_BIT_16_DATA,
   .i2s_codec_m_s_mode	=I2S_M_CODEC_S,
   .in_digital_gain		=CODEC_IN_D_GAIN_0_DB,
   .in_analog_gain      =CODEC_IN_A_GAIN_0_DB,
   .out_digital_gain    =CODEC_OUT_D_GAIN_0_DB,
   .out_analog_gain     =CODEC_OUT_A_GAIN_6_DB,
};

dma_config_st audio_dma_rx_config=
{
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

dma_config_st audio_dma_tx_config=
{
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


/**
 * 	@brief      This function serves to set in path digital and analog gain  .
 * 	@param[in]  d_gain : digital gain value
 * 	@param[in]  a_gain : analog  gain value
 * 	@return     none
 */
void audio_set_codec_in_path_a_d_gain (codec_in_path_digital_gain_e d_gain,codec_in_path_analog_gain_e a_gain )
{
	audio_i2s_codec_config.in_digital_gain=d_gain;
	audio_i2s_codec_config.in_analog_gain=a_gain;
}


/**
 * 	@brief      This function serves to set out path digital and analog gain  .
 * 	@param[in]  d_gain : digital gain value
 * 	@param[in]  a_gain : analog  gain value
 * 	@return     none
 */
 void audio_set_codec_out_path_a_d_gain (codec_out_path_digital_gain_e d_gain,codec_out_path_analog_gain_e a_gain)
{
	 audio_i2s_codec_config.out_digital_gain=d_gain;
	 audio_i2s_codec_config.out_analog_gain=a_gain;
}


 /**
  * 	@brief      This function serves to choose which is master to provide clock.
  * 	@param[in]  m_s : I2S_S_CODEC_M: i2s as slave ,codec as master; I2S_M_CODEC_S: i2s as  master, codec  asslave
  * 	@return     none
  */
void audio_set_i2s_codec_m_s (i2s_codec_m_s_mode_e m_s)
{
	audio_i2s_codec_config.i2s_codec_m_s_mode=m_s;
}

/**
 * 	@brief      This function serves to choose which is master to provide clock.
 * 	@param[in]  chn_wl: select word  length and audio channel number
 * 	@return     none
 */
void aduio_set_chn_wl(audio_channel_wl_mode_e chn_wl)
{
	switch (chn_wl)
	{
		case MONO_BIT_16:
			audio_i2s_codec_config.audio_in_mode=BIT_16_MONO;
			audio_i2s_codec_config.audio_out_mode=BIT_16_MONO_FIFO0;
			audio_i2s_codec_config.i2s_data_select=I2S_BIT_16_DATA;
			audio_i2s_codec_config.codec_data_select=CODEC_BIT_16_DATA;
		break;

		case MONO_BIT_20:
			audio_i2s_codec_config.audio_in_mode=BIT_20_OR_24_MONO;
			audio_i2s_codec_config.audio_out_mode=BIT_20_OR_24_MONO_FIFO0;
			audio_i2s_codec_config.i2s_data_select=I2S_BIT_20_DATA;
			audio_i2s_codec_config.codec_data_select=CODEC_BIT_20_DATA;
		break;

		case MONO_BIT_24:
			audio_i2s_codec_config.audio_in_mode=BIT_20_OR_24_MONO;
			audio_i2s_codec_config.audio_out_mode=BIT_20_OR_24_MONO_FIFO0;
			audio_i2s_codec_config.i2s_data_select=I2S_BIT_24_DATA;
			audio_i2s_codec_config.codec_data_select=CODEC_BIT_24_DATA;
		break;

		case STEREO_BIT_16:
			audio_i2s_codec_config.audio_in_mode=BIT_16_STEREO;
			audio_i2s_codec_config.audio_out_mode=BIT_16_STEREO_FIFO0;
			audio_i2s_codec_config.i2s_data_select=I2S_BIT_16_DATA;
			audio_i2s_codec_config.codec_data_select=CODEC_BIT_16_DATA;
		break;

		case STEREO_BIT_20:
			audio_i2s_codec_config.audio_in_mode=BIT_20_OR_24_STEREO;
			audio_i2s_codec_config.audio_out_mode=BIT_20_OR_24_STEREO_FIFO0;
			audio_i2s_codec_config.i2s_data_select=I2S_BIT_20_DATA;
			audio_i2s_codec_config.codec_data_select=CODEC_BIT_20_DATA;
		break;

		case STEREO_BIT_24:
			audio_i2s_codec_config.audio_in_mode=BIT_20_OR_24_STEREO;
			audio_i2s_codec_config.audio_out_mode=BIT_20_OR_24_STEREO_FIFO0;
			audio_i2s_codec_config.i2s_data_select=I2S_BIT_24_DATA;
			audio_i2s_codec_config.codec_data_select=CODEC_BIT_24_DATA;
		break;
	}

}


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
 * @param[in] pin_gp: the group of dmic pin
 * @return    none
 */
void audio_set_dmic_pin(dmic_pin_group_e pin_gp)
{

	if(pin_gp==DMIC_GROUPB_B2_DAT_B3_B4_CLK)
	{
		reg_gpio_pad_mul_sel=BIT(2);
//		reg_gpio_pb_fuc_h=reg_gpio_pb_fuc_h&(~BIT_RNG(0,1));//PB4 fuc
		reg_gpio_pb_fuc_l=(reg_gpio_pb_fuc_h&(~BIT_RNG(4,7)))|0x0f;//PB2  PB3 fuc
//		gpio_set_gpio_dis(GPIO_PB2|GPIO_PB3|GPIO_PB4);
		gpio_set_gpio_dis(GPIO_PB2|GPIO_PB3);
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


/**
 * @brief     This function serves to enable rx_dma channel.
 * @param[in] none.
 * @return    none
 */
 void audio_rx_dma_en(void)
{
	 dma_chn_en(audio_rx_dma_chn);
}

 /**
  * @brief     This function serves to enable tx_dma channel.
  * @param[in] none.
  * @return    none
  */
 void audio_tx_dma_en(void)
{
	 dma_chn_en(audio_tx_dma_chn);
}


 /**
  * @brief     This function serves to config  rx_dma channel.
  * @param[in] dma_chn_e: dma channel
  * @param[in] dst_addr:the dma address of destination
  * @param[in] data_len:the length of dma rx size by byte
  * @param[in] head_of_list:the head address of dma llp.
  * @return    none
  */
void audio_rx_dma_config(dma_chn_e chn,u16 *dst_addr,u32 data_len,dma_llp_config_t *head_of_list)
{
	audio_rx_dma_chn=chn;
	audio_set_rx_buff_len(data_len);
	dma_config(audio_rx_dma_chn,&audio_dma_rx_config);
	dma_set_address( chn,REG_AUDIO_AHB_BASE,(u32)reg_dma_addr(dst_addr));//SRC_addr  //dst addr ch0 adc
	dma_set_size(chn,data_len,DMA_WORD_WIDTH);
	reg_dma_llp(chn)=(u32)reg_dma_addr(head_of_list);

}

/**
 * @brief     This function serves to set rx dma chain transfer
 * @param[in] config_addr:the head of list of llp_pointer.
 * @param[in] llpointer:the next element of llp_pointer.
 * @param[in] dst_addr:the dma address of destination.
 * @param[in] data_len:the length of dma size by byte.
 * @return    none
 */
void audio_rx_dma_add_list_element(dma_llp_config_t *config_addr,dma_llp_config_t *llpointer ,u16 * dst_addr,u32 data_len)
{
	config_addr->audio_dma_ctl=reg_dma_ctrl(audio_rx_dma_chn)|BIT(0);
	config_addr->audio_dma_src_addr=REG_AUDIO_AHB_BASE;
	config_addr->audio_dma_dst_addr=(u32)reg_dma_addr(dst_addr);
	config_addr->audio_dma_data_len=(( data_len+3)/4)|( ( data_len % 4)<<22);
	config_addr->audio_dma_llp_ptr=(u32)reg_dma_addr(llpointer);
}


/**
 * @brief     This function serves to config  tx_dma channel.
 * @param[in] dma_chn_e: dma channel
 * @param[in] src_addr:the address of source
 * @param[in] data_len:the length of dma rx size by byte
 * @param[in] head_of_list:the head address of dma llp.
 * @return    none
 */
void audio_tx_dma_config(dma_chn_e chn,u16 * src_addr, u32 data_len,dma_llp_config_t * head_of_list)
{
	 audio_tx_dma_chn=chn;
	 audio_set_tx_buff_len(data_len);
	 dma_config(audio_tx_dma_chn,&audio_dma_tx_config);
	 dma_set_address( chn,(u32)reg_dma_addr(src_addr),REG_AUDIO_AHB_BASE);
	 dma_set_size(chn,data_len,DMA_WORD_WIDTH);
	 reg_dma_llp(chn)=(u32)reg_dma_addr(head_of_list);
}

/**
 * @brief     This function serves to set tx dma chain transfer
 * @param[in] config_addr:the head of list of llp_pointer.
 * @param[in] llpointer:the next element of llp_pointer.
 * @param[in] src_addr:the address of source
 * @param[in] data_len:the length of dma size by byte.
 * @return    none
 */
void audio_tx_dma_add_list_element(dma_llp_config_t *config_addr,dma_llp_config_t *llpointer ,u16 * src_addr,u32 data_len)
{
	config_addr->audio_dma_ctl=reg_dma_ctrl(audio_tx_dma_chn)|BIT(0);
	config_addr->audio_dma_src_addr=(u32)reg_dma_addr(src_addr);
	config_addr->audio_dma_dst_addr=REG_AUDIO_AHB_BASE;
	config_addr->audio_dma_data_len=(( data_len+3)/4)|( ( data_len % 4)<<22);
	config_addr->audio_dma_llp_ptr=(u32)reg_dma_addr(llpointer);
}



/**
 * @brief     This function serves to disable i2s clk and codec clk.
 * @return    none
 */
void audio_adc_power_down(void)
{
	BM_SET(reg_audio_codec_adc12_ctr,FLD_AUDIO_CODEC_ADC12_SOFT_MUTE);//adc mute
	audio_i2s_clk_dis();
	audio_codec_clk_dis();
}


/**
 * @brief     This function serves to enable i2s clk and enable ande set coxdec to digital record mode with analog power-down state.
 * @return    none
 */
void audio_adc_dmic_power_restore(void)
{
	audio_i2s_clk_en();
	audio_codec_clk_en();
	/*digital record mode with analog power-down*/
	reg_audio_codec_vic_ctr= MASK_VAL( FLD_AUDIO_CODEC_SB, CODEC_ITF_AC,\
									FLD_AUDIO_CODEC_SB_ANALOG, CODEC_ITF_PD, \
									FLD_AUDIO_CODEC_SLEEP_ANALOG, CODEC_ITF_PD);
	BM_CLR(reg_audio_codec_adc12_ctr,FLD_AUDIO_CODEC_ADC12_SOFT_MUTE);//adc unmute
}


/**
 * @brief     This function serves to  initialise audio by muc
 * @param[in] flow_mode:
 * @param[in] audio_sample_rate_e:audio sampling rate.
 * @param[in] channel_wl:word length and channel number.
 * @return    none
 */
void audio_init(audio_flow_mode_e flow_mode,audio_sample_rate_e rate,audio_channel_wl_mode_e channel_wl)
{
	aduio_set_chn_wl(channel_wl);
	audio_set_codec_clk(1,16);////from ppl 192/16=12M
	audio_mux_config(CODEC_I2S,audio_i2s_codec_config.audio_in_mode,audio_i2s_codec_config.audio_in_mode,audio_i2s_codec_config.audio_out_mode);
	audio_i2s_config(I2S_I2S_MODE,audio_i2s_codec_config.i2s_data_select,audio_i2s_codec_config.i2s_codec_m_s_mode);
	audio_set_i2s_clock(rate,AUDIO_RATE_EQUAL,0);
	audio_clk_en(1,1);
	reg_audio_codec_vic_ctr=FLD_AUDIO_CODEC_SLEEP_ANALOG;//active analog sleep mode
	while(!(reg_audio_codec_stat_ctr&FLD_AUDIO_CODEC_PON_ACK));//wait codec can be configed
	if(flow_mode<OUT_ONLY)
	{
		audio_codec_adc_config(audio_i2s_codec_config.i2s_codec_m_s_mode,(flow_mode%3),rate,audio_i2s_codec_config.codec_data_select,MCU_WREG);
	}

	if(flow_mode>LINE_IN_ONLY)
	{
		audio_codec_dac_config(audio_i2s_codec_config.i2s_codec_m_s_mode,rate,audio_i2s_codec_config.codec_data_select,MCU_WREG);
	}
	while(!(reg_audio_codec_stat_ctr==(FLD_AUDIO_CODEC_ADC12_LOCKED|FLD_AUDIO_CODEC_DAC_LOCKED|FLD_AUDIO_CODEC_PON_ACK)));//wait codec adc/dac locked


	audio_data_path_sel(I2S_DATA_IN_FIFO,I2S_OUT,NO_USE,NO_USE);
}


/**
 * @brief     This function serves to  initialise audio by i2c
 * @param[in] flow_mode:
 * @param[in] audio_sample_rate_e:audio sampling rate.
 * @param[in] channel_wl:word length and channel number.
 * @return    none
 */
void audio_init_i2c(audio_flow_mode_e flow_mode,audio_sample_rate_e rate,audio_channel_wl_mode_e channel_wl)
{
	aduio_set_chn_wl(channel_wl);
	audio_set_codec_clk(1,16);////from ppl 192/16=12M
	audio_mux_config(CODEC_I2S,audio_i2s_codec_config.audio_in_mode,audio_i2s_codec_config.audio_in_mode,audio_i2s_codec_config.audio_out_mode);
	audio_i2s_config(I2S_I2S_MODE,audio_i2s_codec_config.i2s_data_select,audio_i2s_codec_config.i2s_codec_m_s_mode);
	audio_set_i2s_clock(rate,AUDIO_RATE_EQUAL,0);
	audio_clk_en(1,1);
	i2c_init(INNER_CODEC);
	i2c_codec_write(addr_audio_codec_vic_ctr,FLD_AUDIO_CODEC_SLEEP_ANALOG);//active analog sleep mode
	while(!(i2c_codec_read(addr_audio_codec_stat_ctr)&FLD_AUDIO_CODEC_PON_ACK));//wait codec can be configed
	if(flow_mode<OUT_ONLY)
	{
		audio_codec_adc_config(audio_i2s_codec_config.i2s_codec_m_s_mode,(flow_mode%3),rate,audio_i2s_codec_config.codec_data_select,I2C_WREG);
	}
	if(flow_mode>LINE_IN_ONLY)
	{
		audio_codec_dac_config(audio_i2s_codec_config.i2s_codec_m_s_mode,rate,audio_i2s_codec_config.codec_data_select,I2C_WREG);
	}
	while(!(i2c_codec_read(addr_audio_codec_stat_ctr)==(FLD_AUDIO_CODEC_ADC12_LOCKED|FLD_AUDIO_CODEC_DAC_LOCKED|FLD_AUDIO_CODEC_PON_ACK)));//wait codec adc/dac locked
	audio_data_path_sel(I2S_DATA_IN_FIFO,I2S_OUT,NO_USE,NO_USE);
}

/**
 * @brief     This function serves to config codec for dac.
 * @param[in] mode :select i2s as master or slave
 * @param[in] wreg_mode:config codec register by mc
 * @param[in] rate: audio sampling rate
 * @param[in] wreg_mode: mcu or i2c config codec
 * @return    none
 */
void audio_codec_dac_config(i2s_codec_m_s_mode_e mode,audio_sample_rate_e rate,codec_data_select_e data_select,codec_wreg_mode_e  wreg_mode)
{

	if(wreg_mode==MCU_WREG)
	{
		BM_SET(reg_audio_codec_dac_ctr,FLD_AUDIO_CODEC_DAC_SOFT_MUTE); //dac mute

		BM_CLR(reg_audio_codec_dac_ctr,FLD_AUDIO_CODEC_DAC_SB|FLD_AUDIO_CODEC_DAC_LEFT_ONLY);//active DAC power,active left and right channel


		BM_CLR(reg_audio_codec_vic_ctr,FLD_AUDIO_CODEC_SB|FLD_AUDIO_CODEC_SB_ANALOG|FLD_AUDIO_CODEC_SLEEP_ANALOG);//disable sleep mode ,disable sb_analog,disable global standby

		/* data word length ,interface master/slave selection, audio interface protocol selection ,active dac audio interface*/
		reg_audio_codec_dac_itf_ctr= MASK_VAL( FLD_AUDIO_CODEC_FORMAT, CODEC_I2S_MODE,\
											FLD_AUDIO_CODEC_DAC_ITF_SB, CODEC_ITF_AC, \
											FLD_AUDIO_CODEC_SLAVE, mode, \
											FLD_AUDIO_CODEC_WL, data_select);

		 /*disable DAC digital gain coupling, Left channel DAC digital gain*/
		reg_audio_codec_dacl_gain=MASK_VAL(FLD_AUDIO_CODEC_DAC_LRGOD,0,\
												FLD_AUDIO_CODEC_DAC_GODL,audio_i2s_codec_config.out_digital_gain);

		reg_audio_codec_dacr_gain=MASK_VAL(FLD_AUDIO_CODEC_DAC_GODR,audio_i2s_codec_config.out_digital_gain); /*Right channel DAC digital gain*/

		 /*disable Headphone gain coupling, set Left channel HP amplifier gain*/
		reg_audio_codec_hpl_gain=MASK_VAL(FLD_AUDIO_CODEC_HPL_LRGO,0,\
											FLD_AUDIO_CODEC_HPL_GOL,audio_i2s_codec_config.out_analog_gain);

		reg_audio_codec_hpr_gain=MASK_VAL(FLD_AUDIO_CODEC_HPR_GOR, audio_i2s_codec_config.out_analog_gain); /* Right channel HP amplifier gain programming*/

		reg_audio_codec_dac_freq_ctr=(FLD_AUDIO_CODEC_DAC_FREQ&rate);

		BM_CLR(reg_audio_codec_dac_ctr,FLD_AUDIO_CODEC_DAC_SOFT_MUTE); /*dac mute*/
	}

	else if (wreg_mode==I2C_WREG)
	{
		/*active DAC power,active left and right channel,dac mute*/
		i2c_codec_write(addr_audio_codec_dac_ctr,MASK_VAL( FLD_AUDIO_CODEC_DAC_SB, 0,\
				FLD_AUDIO_CODEC_DAC_LEFT_ONLY, 0, \
				FLD_AUDIO_CODEC_DAC_SOFT_MUTE, 1));


		/*disable sleep mode ,disable sb_analog,disable global standby*/
		i2c_codec_write(addr_audio_codec_vic_ctr,MASK_VAL( FLD_AUDIO_CODEC_SB, 0,\
				FLD_AUDIO_CODEC_SB_ANALOG, 0, \
				FLD_AUDIO_CODEC_SLEEP_ANALOG, 0));

		/*data word length ,interface master/slave selection, audio interface protocol selection ,active dac audio interface */
		i2c_codec_write(addr_audio_codec_dac_itf_ctr,MASK_VAL( FLD_AUDIO_CODEC_FORMAT, CODEC_I2S_MODE,\
											FLD_AUDIO_CODEC_DAC_ITF_SB, CODEC_ITF_AC, \
											FLD_AUDIO_CODEC_SLAVE, mode, \
											FLD_AUDIO_CODEC_WL, data_select));

		/* disable DAC digital gain coupling, Left channel DAC digital gain */
		i2c_codec_write(addr_audio_codec_dacl_gain,MASK_VAL(FLD_AUDIO_CODEC_DAC_LRGOD,0,\
				FLD_AUDIO_CODEC_DAC_GODL,audio_i2s_codec_config.out_digital_gain));


		i2c_codec_write(addr_audio_codec_dacr_gain,MASK_VAL(FLD_AUDIO_CODEC_DAC_GODR,audio_i2s_codec_config.out_digital_gain));/* Right channel DAC digital gain */


		/* disable Headphone gain coupling, set Left channel HP amplifier gain */
		i2c_codec_write(addr_audio_codec_hpl_gain,MASK_VAL(FLD_AUDIO_CODEC_HPL_LRGO,0,\
	        	FLD_AUDIO_CODEC_HPL_GOL,audio_i2s_codec_config.out_analog_gain));

		i2c_codec_write(addr_audio_codec_hpr_gain,MASK_VAL(FLD_AUDIO_CODEC_HPR_GOR, audio_i2s_codec_config.out_analog_gain));/*  Right channel HP amplifier gain programming*/

		i2c_codec_write(addr_audio_codec_dac_freq_ctr,(FLD_AUDIO_CODEC_DAC_FREQ&rate));

		/*dac mute*/
		i2c_codec_write(addr_audio_codec_dac_ctr,MASK_VAL( FLD_AUDIO_CODEC_DAC_SB, 0,\
				FLD_AUDIO_CODEC_DAC_LEFT_ONLY, 0, \
				FLD_AUDIO_CODEC_DAC_SOFT_MUTE, 0));
	}

}



/**
 * @brief     This function serves to config codec for adc.
 * @param[in] mode :select i2s as master or slave
 * @param[in] in_mode: line_in/amic/dmic input mode select
 * @param[in] wreg_mode:config codec register by mc
 * @param[in] rate: audio sampling rate
 * @param[in] wreg_mode: mcu or i2c config codec
 * @return    none
 */
void audio_codec_adc_config(i2s_codec_m_s_mode_e mode,audio_input_mode_e in_mode,audio_sample_rate_e rate,codec_data_select_e data_select,codec_wreg_mode_e  wreg_mode)
{

	if(wreg_mode==MCU_WREG)
	{
		BM_SET(reg_audio_codec_adc12_ctr,FLD_AUDIO_CODEC_ADC12_SOFT_MUTE); /*adc mute*/

		BM_CLR(reg_audio_codec_adc12_ctr,FLD_AUDIO_CODEC_ADC1_SB|FLD_AUDIO_CODEC_ADC2_SB);/*active anc0 and adc1  channel, if mono only active adc1*/

		BM_CLR(reg_audio_codec_vic_ctr,FLD_AUDIO_CODEC_SB|FLD_AUDIO_CODEC_SB_ANALOG|FLD_AUDIO_CODEC_SLEEP_ANALOG);/*disable sleep mode ,disable sb_analog,disable global standby*/

		if (in_mode==AMIC_IN)
		{
		/*Microphone 1 input selection ,Microphone biasing active,Single-ended input,MICBIAS1 output=2.08V,*/
		reg_audio_codec_mic1_ctr= MASK_VAL( FLD_AUDIO_CODEC_MIC1_SEL, 0,\
										FLD_AUDIO_CODEC_MICBIAS1_SB, 0, \
										FLD_AUDIO_CODEC_MIC_DIFF1, 0,\
										FLD_AUDIO_CODEC_MICBIAS1_V, 0);
		/*Microphone 2 input selection,Single-ended input*/
		reg_audio_codec_mic2_ctr= MASK_VAL( FLD_AUDIO_CODEC_MIC2_SEL, 0,\
										FLD_AUDIO_CODEC_MIC_DIFF2, 0);

		/*analog 0/4/8/12/16/20 dB boost gain*/
		reg_audio_codec_mic_l_R_gain= MASK_VAL( FLD_AUDIO_CODEC_AMIC_L_GAIN, audio_i2s_codec_config.in_analog_gain,\
											FLD_AUDIO_CODEC_AMIC_R_GAIN, audio_i2s_codec_config.in_analog_gain);
		}
		else if(in_mode==DMIC_IN)
		{
		reg_audio_dmic_12=MASK_VAL( FLD_AUDIO_CODEC_ADC_DMIC_SEL2, 1,\
								FLD_AUDIO_CODEC_ADC_DMIC_SEL1,1, \
								FLD_AUDIO_CODEC_DMIC2_SB,CODEC_ITF_AC,\
								FLD_AUDIO_CODEC_DMIC1_SB, CODEC_ITF_AC);
		}

		else if(in_mode==LINE_IN)
		{
		/*analog 0/4/8/12/16/20 dB boost gain*/
		reg_audio_codec_mic_l_R_gain= MASK_VAL( FLD_AUDIO_CODEC_AMIC_L_GAIN, audio_i2s_codec_config.in_analog_gain,\
												FLD_AUDIO_CODEC_AMIC_R_GAIN, audio_i2s_codec_config.in_analog_gain);
		}

		   /*0db~43db  1db step ,digital programmable gain*/
		reg_audio_adc1_gain=MASK_VAL(  FLD_AUDIO_CODEC_ADC_LRGID,1,\
								FLD_AUDIO_CODEC_ADC_GID1,audio_i2s_codec_config.in_digital_gain);
		  /*data word length ,interface master/slave selection, audio interface protocol selection  */
		reg_audio_codec_adc_itf_ctr= MASK_VAL( FLD_AUDIO_CODEC_FORMAT, CODEC_I2S_MODE,\
										FLD_AUDIO_CODEC_SLAVE, mode, \
										 FLD_AUDIO_CODEC_WL, data_select);
		/*audio adc interface active*/
		BM_CLR(reg_audio_codec_adc2_ctr,FLD_AUDIO_CODEC_ADC12_SB);

		/*  adc high pass filter active, set adc sample rate   */
		reg_audio_codec_adc_freq_ctr=MASK_VAL(  FLD_AUDIO_CODEC_ADC12_HPF_EN,1,\
										  FLD_AUDIO_CODEC_ADC_FREQ,rate);

		BM_CLR(reg_audio_codec_adc12_ctr,FLD_AUDIO_CODEC_ADC12_SOFT_MUTE);/*adc unmute*/
	}

	else if(wreg_mode==I2C_WREG)
	{


		/*active adc0 and adc1  channel, if mono only active adc1,adc mute*/
		i2c_codec_write(addr_audio_codec_adc12_ctr,MASK_VAL( FLD_AUDIO_CODEC_ADC1_SB, 0,\
				    FLD_AUDIO_CODEC_ADC2_SB, 0, \
					FLD_AUDIO_CODEC_ADC12_SOFT_MUTE, 1));

		/*disable sleep mode ,disable sb_analog,disable global standby*/
		i2c_codec_write(addr_audio_codec_vic_ctr,MASK_VAL( FLD_AUDIO_CODEC_SB, 0,\
				FLD_AUDIO_CODEC_SB_ANALOG, 0, \
				FLD_AUDIO_CODEC_SLEEP_ANALOG, 0));


		if (in_mode==AMIC_IN)
		{
			 /*Microphone 1 input selection ,Microphone biasing active,Single-ended input,MICBIAS1 output=2.08V,*/
			i2c_codec_write(addr_audio_codec_mic1_ctr,MASK_VAL( FLD_AUDIO_CODEC_MIC1_SEL, 0,\
					FLD_AUDIO_CODEC_MICBIAS1_SB, 0, \
					FLD_AUDIO_CODEC_MIC_DIFF1, 0,\
					FLD_AUDIO_CODEC_MICBIAS1_V, 0));

			 /*Microphone 2 input selection,Single-ended input*/
			i2c_codec_write(addr_audio_codec_mic2_ctr,MASK_VAL( FLD_AUDIO_CODEC_MIC2_SEL, 0,\
					FLD_AUDIO_CODEC_MIC_DIFF2, 0));

			/*analog 0/4/8/12/16/20 dB boost gain*/
			i2c_codec_write(addr_audio_codec_mic_l_R_gain, MASK_VAL( FLD_AUDIO_CODEC_AMIC_L_GAIN, audio_i2s_codec_config.in_analog_gain,\
					FLD_AUDIO_CODEC_AMIC_R_GAIN, audio_i2s_codec_config.in_analog_gain));

		}
		else if(in_mode==DMIC_IN)
		{
			i2c_codec_write(addr_audio_dmic_12,MASK_VAL( FLD_AUDIO_CODEC_ADC_DMIC_SEL2, 1,\
					FLD_AUDIO_CODEC_ADC_DMIC_SEL1,1, \
					FLD_AUDIO_CODEC_DMIC2_SB,CODEC_ITF_AC,\
					FLD_AUDIO_CODEC_DMIC1_SB, CODEC_ITF_AC));
		}

		else if(in_mode==LINE_IN)
		{
			 /*analog 0/4/8/12/16/20 dB boost gain*/
			i2c_codec_write(addr_audio_codec_mic_l_R_gain, MASK_VAL( FLD_AUDIO_CODEC_AMIC_L_GAIN, audio_i2s_codec_config.in_analog_gain,\
					FLD_AUDIO_CODEC_AMIC_R_GAIN, audio_i2s_codec_config.in_analog_gain));
		}

			  /*0db~43db  1db step ,digital programmable gain*/
			i2c_codec_write(addr_audio_adc1_gain,MASK_VAL(  FLD_AUDIO_CODEC_ADC_LRGID,1,\
					FLD_AUDIO_CODEC_ADC_GID1,audio_i2s_codec_config.in_digital_gain));


			i2c_codec_write(addr_audio_codec_adc_itf_ctr,MASK_VAL( FLD_AUDIO_CODEC_FORMAT, CODEC_I2S_MODE,\
					FLD_AUDIO_CODEC_SLAVE, mode, \
					 FLD_AUDIO_CODEC_WL, data_select));

			i2c_codec_write(addr_audio_codec_adc2_ctr,~FLD_AUDIO_CODEC_ADC12_SB);  /*audio adc interface active*/


			/*  adc high pass filter active, set adc sample rate   */
			i2c_codec_write(addr_audio_codec_adc_freq_ctr,MASK_VAL(  FLD_AUDIO_CODEC_ADC12_HPF_EN,1,\
					FLD_AUDIO_CODEC_ADC_FREQ,rate));

			/*dac mute*/
			i2c_codec_write(addr_audio_codec_adc12_ctr,MASK_VAL( FLD_AUDIO_CODEC_ADC1_SB, 0,\
					FLD_AUDIO_CODEC_ADC2_SB, 0, \
					FLD_AUDIO_CODEC_ADC12_SOFT_MUTE, 0));

	}


}


/**
 * @brief     This function serves to set data path.
 * @param[in] audio_flow :audio flow  select
 * @param[in] ain0_mode:fifo0 input mode select
 * @param[in] ain1_mode:fifo1 input mode select
 * @param[in] i2s_aout_mode:fifo output source select
 * @return    none
 */
void audio_mux_config(audio_flow_e audio_flow, audio_in_mode_e ain0_mode , audio_in_mode_e ain1_mode,audio_out_mode_e i2s_aout_mode)
{
	reg_audio_ctrl |= audio_flow;
	reg_audio_tune= MASK_VAL( FLD_AUDIO_I2S_I2S_AIN0_COME, ain0_mode,\
							FLD_AUDIO_I2S_I2S_AIN1_COME, ain1_mode, \
						FLD_AUDIO_I2S_I2S_AOUT_COME, i2s_aout_mode);
}


/**
 * @brief     This function serves to set data path .
 * @param[in] ain0_sel :fifo0 input source select
 * @param[in] aout0_sel:fifo0 output source select
 * @param[in] ain0_sel :fifo1 input source select
 * @param[in] aout0_sel:fifo1 output source select
 * @return    none
 */
void audio_data_path_sel (audio_mux_ain_e ain0_sel, audio_mux_aout_e aout0_sel, audio_mux_ain_e ain1_sel, audio_mux_aout_e aout1_sel)
{

	reg_audio_sel = MASK_VAL( FLD_AUDIO_AIN0_SEL, ain0_sel,\
								FLD_AUDIO_AOUT0_SEL, aout0_sel, \
								FLD_AUDIO_AIN1_SEL, ain1_sel, \
								FLD_AUDIO_AOUT1_SEL, aout1_sel);
}

/**
 * @brief     This function serves to config interface, word length, and m/s .
 * @param[in] i2s_format :interface protocol
 * @param[in] wl:audio data word length
 * @param[in] i2s_m_s:select i2s as master or slave
 * @return    none
 */
void audio_i2s_config(i2s_mode_select_e i2s_format,i2s_data_select_e wl,i2s_codec_m_s_mode_e  m_s)
{

    reg_i2s_cfg = MASK_VAL( FLD_AUDIO_I2S_FORMAT, i2s_format,\
		   FLD_AUDIO_I2S_WL, wl, \
		   FLD_AUDIO_I2S_LRP, 0, \
		   FLD_AUDIO_I2S_LRSWAP, 0, \
		   FLD_AUDIO_I2S_ADC_DCI_MS, m_s, \
		   FLD_AUDIO_I2S_DAC_DCI_MS, m_s);
}

/**
 * @brief     This function serves to set i2s clock and audio sampling rate when i2s as master.
 * @param[in] audio_rate: audio sampling rate
 * @param[in] match:the match of audio rate.
 * @param[in] match_en: initial must 0, then change rate must 1
 * @return    none
 */
_attribute_ram_code_ void  audio_set_i2s_clock (audio_sample_rate_e audio_rate,audio_rate_match_e  match,u8 match_en)
{
	reg_tx_wptr=0xffff;//enable tx_rptr
	u16  tx_rptr_old;
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
			if(match_en)
			{
				tx_rptr_old = reg_tx_rptr;
				while(tx_rptr_old==reg_tx_rptr);
			}
			if(match==AUDIO_RATE_EQUAL)//48000
			{
				audio_set_i2s_clk(2,125);//
				audio_set_i2s_bclk(0);//3.072 m bclk
				audio_set_lrclk(64);
			}

			else if(match==AUDIO_RATE_GT_L0)//48004
			{
				audio_set_i2s_clk(3,169);
				audio_set_i2s_bclk(0);
				audio_set_lrclk(71);
			}

			else if(match==AUDIO_RATE_GT_L1)//48012.0
			{
				audio_set_i2s_clk(4,129);
				audio_set_i2s_bclk(0);
				audio_set_lrclk(124);
			}
			else if(match==AUDIO_RATE_LT_L0)
			{
				audio_set_i2s_clk(2,63);//47994.0
				audio_set_i2s_bclk(0);
				audio_set_lrclk(127);
			}

			else if(match==AUDIO_RATE_LT_L1)
			{
				audio_set_i2s_clk(4,165);//47985.0
				audio_set_i2s_bclk(0);
				audio_set_lrclk(97);
			}
		break;

	case AUDIO_44EP1K:
		if(match_en)
		{
			tx_rptr_old = reg_tx_rptr;
			while(tx_rptr_old==reg_tx_rptr);
		}

		if(match==AUDIO_RATE_EQUAL)//44099.9
		{
			audio_set_i2s_clk(8,215);
			audio_set_i2s_bclk(0);
			audio_set_lrclk(162);
		}
		else if(match==AUDIO_RATE_GT_L0)//44110.2
		{
			audio_set_i2s_clk(11,228);
			audio_set_i2s_bclk(0);
			audio_set_lrclk(210);
		}

		else if(match==AUDIO_RATE_GT_L1)//44117.6
		{
			audio_set_i2s_clk(5,170);
			audio_set_i2s_bclk(0);
			audio_set_lrclk(128);
		}

		else if(match==AUDIO_RATE_LT_L0)//44094.4
		{
			audio_set_i2s_clk(7,254);
			audio_set_i2s_bclk(0);
			audio_set_lrclk(120);
		}

		else if(match==AUDIO_RATE_LT_L1)//44081.6
		{
			audio_set_i2s_clk(9,245);
			audio_set_i2s_bclk(0);
			audio_set_lrclk(160);
		}
	break;
	}

}


/**
 * @brief     This function serves to set dma rx_buff.
 * @param[in] chn: dma_chn_e: dma channel
 * @param[in] in_buff:the pointer of rx_buff.
 * @param[in] buff_size:the size of rx_buff.
 * @return    none
 */
 void audio_rx_dma_chain_init (dma_chn_e chn,u16 * in_buff,u32 buff_size )
{
	audio_rx_dma_config(chn,(u16*)in_buff, buff_size, &audio_rx_dma_list_cfg);
	audio_rx_dma_add_list_element(&audio_rx_dma_list_cfg, &audio_rx_dma_list_cfg,(u16*)in_buff, buff_size);
	audio_rx_dma_en();
}


/**
 * @brief     This function serves to set dma tx_buff.
 * @param[in] chn: dma_chn_e: dma channel
 * @param[in] out_buff:the pointer of tx_buff.
 * @param[in] buff_size:the size of tx_buff.
 * @return    none
 */
void audio_tx_dma_chain_init (dma_chn_e chn,u16 * out_buff,u32 buff_size)
{
	audio_tx_dma_config(chn, (u16*)out_buff, buff_size, &audio_tx_dma_list_cfg);
	audio_tx_dma_add_list_element(&audio_tx_dma_list_cfg, &audio_tx_dma_list_cfg, (u16*)out_buff, buff_size);
	audio_tx_dma_en();
}




#define		WM8731_ANA_AUDIO_PATH_CTRL			0x08		//Analogue Audio Path Control
#define		WM8731_DIG_AUDIO_PATH_CTRL			0x0a		//Digital Audio Path Control
#define		WM8731_POWER_DOWN_CTRL				0x0c		//Power Down Control
#define	    WM8731_ST_LINE_VOL                  0x00        //Set linmute volume
#define	    WM8731_ST_RINE_VOL                  0x02        //Set rinmute volume
#define		WM8731_DIG_AUDIO_INTERFACE_FORMAT	0x0e		//Digital Audio Interface Format
#define		WM8731_SAMPLING_CTRL 				0x10		//Sampling Control
#define		WM8731_ACTIVE_CTRL 					0x12		//Active Control
#define		WM8731_RESET_CTRL 					0x1e		//Reset Register




u8 LineIn_To_I2S_CMD_TAB[9][2]={	 		{WM8731_RESET_CTRL, 				0x00},
		                                    {WM8731_ST_LINE_VOL,			    0x10},
		                                    {WM8731_ST_RINE_VOL,                0x10},
											{WM8731_ANA_AUDIO_PATH_CTRL,        0x13},
											{WM8731_DIG_AUDIO_PATH_CTRL,        0x00},
											{WM8731_POWER_DOWN_CTRL,            0x22},
											{WM8731_DIG_AUDIO_INTERFACE_FORMAT, 0x02},
											{WM8731_SAMPLING_CTRL,              0x19},
											{WM8731_ACTIVE_CTRL,                0x01},

};

/**
 * @brief     This function serves to  set external  codec by i2c
 * @return    none
 */
void audio_set_ext_codec(void)
{
	for (u8 i=0;i<9;i++)
	{
		i2c_codec_write(LineIn_To_I2S_CMD_TAB[i][0]>>1,LineIn_To_I2S_CMD_TAB[i][1]);
	}
}


/**
 * @brief     This function serves to  set pwm0 as external codec mclk
 * @return    none
 */
void pwm_set(void)
{
	//select pd4 as PWM0 output.
	reg_pwm_enable=0;
	reg_pwm0_enable=0;//off pwm0
	pwm_set_pin(PWM_PWM0_PB4);
	reg_pwm_clkdiv = 0;//set pwm clk equal pclk 24M
	pwm_set_pwm0_mode(PWM_NORMAL_MODE);
	pwm_set_tcmp(PWM0_ID,1);
	pwm_set_tmax(PWM0_ID,2);//24M/2=12M pwm  mclk to  ext codec clk
   pwm_start(PWM0_ID);

}
/**
 * @brief     This function serves to  initialise audio by i2c
 * @param[in] flow_mode:
 * @param[in] audio_sample_rate_e:audio sampling rate.
 * @param[in] channel_wl:word length and channel number.
 * @return    none
 */
void audio_i2s_init(void)
{
	pwm_set();
	i2s_set_pin();
	i2c_init(EXT_CODEC);
	aduio_set_chn_wl(MONO_BIT_16);
	audio_mux_config(IO_I2S,BIT_16_MONO,BIT_16_MONO,BIT_16_MONO_FIFO0);
	audio_i2s_config(I2S_I2S_MODE,I2S_BIT_16_DATA,I2S_M_CODEC_S);
	audio_set_i2s_clock(AUDIO_32K,AUDIO_RATE_EQUAL,0);
	audio_clk_en(1,1);
	audio_set_ext_codec();
	audio_data_path_sel(I2S_DATA_IN_FIFO,I2S_OUT,NO_USE,NO_USE);

}
/**
 * @brief     This function serves to read data from codec register.
 * @param[in] addr: the address of codec register
 * @return    none
 */
u8 i2c_codec_read(u8 addr)
{
	reg_i2c_data_buf(0)=addr<<1;
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

/**
 * @brief     This function serves to write data to  codec register.
 * @param[in] addr: the address of codec register
 * @return    none
 */
void i2c_codec_write(u8 addr ,u8 wdat)
{

	reg_i2c_data_buf(0)=addr<<1;
	reg_i2c_data_buf(1)=wdat;
	reg_i2c_len=2;//tx_len
	reg_i2c_sct1=FLD_I2C_LS_ID|FLD_I2C_LS_DATAW| FLD_I2C_LS_START| FLD_I2C_LS_STOP;
	while(i2c_master_busy()); //wait busy=0
	while(reg_i2c_mst & FLD_I2C_ACK_IN);//wait ack=0
}


/**
 * @brief      This function serves to enable i2c master for codec i2c slave .
 * @param[in]  none.
 * @return     none.
 */
void i2c_init(codec_type_e codec_type)
{
	i2c_master_init();
	//i2c_set_master_clk((unsigned char)(sys_clk.pclk*1000*1000/(4*20000)));//set i2c frequency 400K.
	reg_i2c_sp=0x1e;//200k sys_clk.pclk=24M
	if(codec_type==INNER_CODEC)
	{
		reg_audio_i2c_mode=0x05;//codec config by i2c
		reg_i2c_id=(0x34<<1);//set i2c id
		reg_audio_i2c_addr=0x34;
	}
	else if(codec_type==EXT_CODEC)
	{
		reg_i2c_id=0x34;
		i2c_set_pin(I2C_GPIO_SDA_B3,I2C_GPIO_SCL_B2);
	}
}





