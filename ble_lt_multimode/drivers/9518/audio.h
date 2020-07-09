/********************************************************************************************************
 * @file     audio.h
 *
 * @brief    This is the Audio driver header file for TLSR8278
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

#ifndef audio_H
#define audio_H


#include "reg_include/register_9518.h"
#include "i2c.h"
#include "audio.h"
/**
 * @brief  enum variable, represents Selection of PWM pin.
 */
typedef enum{
	I2S_BCK_PC3      = GPIO_PC3,
	I2S_ADC_LR_PC4   = GPIO_PC4,
	I2S_ADC_DAT_PC5  = GPIO_PC5,
	I2S_DAC_LR_PC6   = GPIO_PC6,
	I2S_DAC_DAT_PC7  = GPIO_PC7,
}i2s_pin_e;


typedef enum{
	DMIC_GROUPB_B2_DAT_B3_B4_CLK,
	DMIC_GROUPC_C1_DAT_C2_C3_CLK,
	DMIC_GROUPD_D4_DAT_D5_D6_CLK,
}dmic_pin_group_e;


typedef enum{
	CODEC_I2S ,
	IO_CODEC ,
	IO_I2S,
}audio_flow_e;


typedef enum{
	I2S_RJ_MODE ,
	I2S_LJ_MODE ,
	I2S_I2S_MODE,
	I2S_DSP_MODE,
}i2s_mode_select_e;

typedef enum{
	CODEC_PARALLEL_MODE ,
	CODEC_LJ_MODE ,
	CODEC_DSP_MODE,
	CODEC_I2S_MODE,
}codec_mode_select_e;


typedef enum{
	CODEC_ITF_AC,
	CODEC_ITF_PD,
}codec_interface_select_e;

typedef enum{
	I2S_BIT_16_DATA,
	I2S_BIT_20_DATA ,
	I2S_BIT_24_DATA,
	I2S_BIT_32_DATA,
}i2s_data_select_e;

typedef enum{
	CODEC_BIT_16_DATA,
	CODEC_BIT_18_DATA ,
	CODEC_BIT_20_DATA,
	CODEC_BIT_24_DATA,
}codec_data_select_e;


typedef enum{
	I2C_RX_TX_SLAVE ,
	I2C_RX_TX_MASTER,
}i2s_m_s_select_e;
typedef enum{
	CODEC_SLAVE ,
	CODEC_MASTER,
}codec_m_s_mode_e;

typedef enum{
	MCU_WREG,
	I2C_WREG ,
}codec_wreg_mode_e;

typedef enum{
	AMIC_IN,
	DMIC_IN,
	LINE_IN,
}audio_input_mode_e;



typedef enum{
   	BIT_16_MONO,
	BIT_20_OR_24_MONO ,
	BIT_16_STEREO ,
	BIT_20_OR_24_STEREO ,

}audio_in_mode_e;

typedef enum{
   	BIT_16_MONO_FIFO0 ,
	BIT_20_MONO_FIFO0 ,
	BIT_16_STEREO_FIFO0 ,
	BIT_20_STEREO_FIFO0 ,
	BIT_16_MONO_FIFO1 ,
	BIT_20_MONO_FIFO1 ,
	BIT_16_STEREO_FIFO0_AND_FIFO1 ,
	BIT_20_STEREO_FIFO0_AND_FIFO1 ,

}audio_out_mode_e;



typedef enum{
	I2S_DATA_IN_FIFO ,
	USB_DATA_IN_FIFO ,
	SAR_ADC_DATA_IN_FIFO,
}audio_mux_ain_e;

typedef enum{
	I2S_OUT ,
	USB_IOS_OUT ,
	NO_USE=3,
}audio_mux_aout_e;

/**
 * define audio rate value.
 */
typedef enum{
	AUDIO_8K,
	AUDIO_16K=3,
	AUDIO_32K=6,
	AUDIO_44EP1K,
	AUDIO_48K,
}audio_sample_rate_e;

typedef enum{
	AUDIO_RATE_EQUAL,
	AUDIO_RATE_GT,
	AUDIO_RATE_LT,
}audio_rate_match_e;



typedef struct {
	unsigned int audio_dma_ctl;
	unsigned int audio_dma_src_addr;
	unsigned int audio_dma_dst_addr;
	unsigned int audio_dma_data_len;
	unsigned int audio_dma_llp_ptr;
}dma_llp_config_t;



enum{
	CODEC_AMIC_GAIN_0_DB,
	CODEC_AMIC_GAIN_4_DB,
	CODEC_AMIC_GAIN_8_DB,
	CODEC_AMIC_GAIN_12_DB,
	CODEC_AMIC_GAIN_16_DB,
	CODEC_AMIC_GAIN_20_DB,
};


enum{
	CODEC_DMIC_GAIN_0_D=0,
	CODEC_DMIC_GAIN_4_DB=4,
	CODEC_DMIC_GAIN_8_DB=8,
	CODEC_DMIC_GAIN_12_DB=12,
	CODEC_DMIC_GAIN_16_DB=16,
	CODEC_DMIC_GAIN_20_DB=20,
};


/**
 * 	@brief      This function serves to set the clock of i2s
 * 	@param[in]  step - the dividing factor of step.
 * 	@param[in]  mod - the dividing factor of mod.
 * 	@return     none
 */
static inline void audio_set_i2s_clk(u8 step,u8  mod)
{
	reg_i2s_step = (step&FLD_I2S_STEP)|FLD_I2S_CLK_EN;
	reg_i2s_mod = mod;
}

/**
 * 	@brief      This function serves to set the clock of codec
 * 	@param[in]  step - the dividing factor of step.
 * 	@param[in]  mod - the dividing factor of mod.
 * 	@return     none
 */
static inline void audio_set_codec_clk(u8 step,u8  mod)
{
	BM_CLR(reg_dmic_clk_set,BIT(0));//set dmic_div
	reg_dmic_step = (step&FLD_DMIC_STEP)|FLD_DMIC_SEL;
	reg_dmic_mod = mod;
}
/**
 * 	@brief      This function serves to set disable i2s clk
 * 	@param[in]  step - the dividing factor of step.
 * 	@param[in]  mod - the dividing factor of mod.
 * 	@return     none
 */
static inline void audio_i2s_clk_dis()
{
	BM_CLR(reg_i2s_step,FLD_I2S_CLK_EN);
}

/**
 * 	@brief      This function serves to set enable i2s clk
 * 	@param[in]  step - the dividing factor of step.
 * 	@param[in]  mod - the dividing factor of mod.
 * 	@return     none
 */
static inline void audio_i2s_clk_en()
{
	BM_SET(reg_i2s_step,FLD_I2S_CLK_EN);
}


/**
 * 	@brief      This function serves to set disable codec clk
 * 	@param[in]  step - the dividing factor of step.
 * 	@param[in]  mod - the dividing factor of mod.
 * 	@return     none
 */
static inline void audio_codec_clk_dis()
{
	BM_CLR(reg_dmic_step,FLD_DMIC_SEL);
}

/**
 * 	@brief      This function serves to set disable codec clk
 * 	@param[in]  step - the dividing factor of step.
 * 	@param[in]  mod - the dividing factor of mod.
 * 	@return     none
 */
static inline void audio_codec_clk_en()
{
	BM_SET(reg_dmic_step,FLD_DMIC_SEL);
}



/**
 * 	@brief      This function serves to set the bclk
 * 	@param[in]  div bclk=i2s_clk/(div*2),if div=0,  i2s_clk=bclk.
 * 	@return     none
 */
static inline void audio_set_i2s_bclk(u8 div)
{
	reg_pcm_clk_num=div&0x0f;

}

/**
 * 	@brief      This function serves to set the lrclk,
 * 	@param[in]  div=bclk/(div)
 * 	@return     none
 */
static inline void audio_set_lrclk(u16 div)
{
	reg_int_pcm_num=(div-1);
	reg_dec_pcm_num=(div-1);
}


static inline void audio_set_tx_buff_length(u16 len)
{
	reg_tx_max=((len)>>2)-1;
}


static inline void audio_set_rx_buff_length(u16 len)
{
   reg_rx_max=((len)>>2)-1;
}



static inline void audio_write_codec_register(u8 addr,u8  data)
{
	write_reg8(REG_AUDIO_AHB_BASE+((0x80+addr)<<2), data);

}

static inline u8 audio_read_codec_register(u8 addr)
{
	return read_reg8(REG_AUDIO_AHB_BASE+((0x80+addr)<<2));

}



static inline void audio_set_fifo_tx_tri_num(u8 number)
{
	reg_fifo_trig0 &=~FLD_AUDIO_FIFO_AOUT0_TRIG_NUM;
	reg_fifo_trig0 |=(number&FLD_AUDIO_FIFO_AOUT0_TRIG_NUM);
}

static inline void  audio_set_fifo_rx_tri_num(u8 number)
{
	 reg_fifo_trig0 &= ~FLD_AUDIO_FIFO_AIN0_TRIG_NUM;
	 reg_fifo_trig0 |=((number&0x0f)<<4);
}


static inline void  audio_clk_en(u8 i2s_clk_en,u8 mc_clk_en)
{
	reg_audio_en=MASK_VAL( FLD_AUDIO_CLK_EN, i2s_clk_en,\
			      FLD_AUDIO_MC_CLK_EN, mc_clk_en);
}


static inline u16 audio_get_rx_wptr(void)
{
	return reg_rx_wptr;
}


static inline  void audio_set_tx_wptr(u16 ptr)
{
	 reg_tx_wptr=ptr;
}

static inline u16 audio_get_tx_rptr(void)
{
	return reg_tx_rptr;
}

static inline void audio_set_rx_rptr(u16 ptr)
{
	reg_rx_rptr=ptr;
}

static inline void audio_codec_set_sample_rate (int rate)
{
	audio_write_codec_register (0x0b, rate);
}

static inline u32 audio_get_mic_buffer_write_pointer (dma_chn_e chn)
{
	return   reg_dma_ahp_to_cup_addr(reg_dma_dst_addr(chn));
}

static inline u32 audio_get_speaker_buffer_read_pointer (dma_chn_e chn)
{
	return reg_dma_ahp_to_cup_addr(reg_dma_src_addr(chn));
}

void audio_dmic_set_pin(dmic_pin_group_e pin_gp);
void audio_mux_config(audio_flow_e audio_flow, audio_in_mode_e ain0_mode , audio_in_mode_e ain1_mode,audio_out_mode_e i2s_aout_mode);
void codec_init(void);
void audio_codec_dac_config(codec_m_s_mode_e mode,codec_wreg_mode_e wreg_mode, audio_sample_rate_e rate);
void audio_codec_adc_config(codec_m_s_mode_e mode,audio_input_mode_e in_mode,codec_wreg_mode_e wreg_mode, audio_sample_rate_e rate);
void audio_data_path_sel (audio_mux_ain_e ain0_sel, audio_mux_aout_e aout0_sel, audio_mux_ain_e ain1_sel, audio_mux_aout_e aout1_sel);
void audio_i2s_config(i2s_mode_select_e i2s_format,i2s_data_select_e wl,i2s_m_s_select_e  i2s_m_s);
void audio_set_i2s_clock (audio_sample_rate_e audio_rate,audio_rate_match_e  match);
void audio_rx_dma_config(dma_chn_e chn,u16 * dst_addr,u32 data_len,dma_llp_config_t *head_of_list);
void audio_rx_dma_add_list_element(dma_llp_config_t * rx_config,dma_llp_config_t *llpoint ,u16 * dst_addr,u32 data_len);
void audio_tx_dma_config(dma_chn_e chn,u16 * src_addr, u32 data_len,dma_llp_config_t * tx_config);
void audio_tx_dma_add_list_element(dma_llp_config_t *config_addr,dma_llp_config_t *llponit ,u16 * src_addr,u32 data_len);
void audio_rx_dma_en(void);
void audio_tx_dma_en(void);
void codec_set(u8 codec_master_en);

void audio_i2s_codec_config (int sample_rate, u32 * speaker_buff, int speaker_size, u32 *mic_buff, int mic_size);

#endif
