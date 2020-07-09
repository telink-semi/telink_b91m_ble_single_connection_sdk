

#include "analog.h"
#include "compiler.h"
#include "plic.h"

dma_config_st analog_tx_dma_config={
	.dst_req_sel 		= DMA_REQ_ALGM_TX,//tx req
	.src_req_sel 		= 0,
	.dst_addr_ctrl		= DMA_ADDR_FIX,
	.src_addr_ctrl	 	= DMA_ADDR_INCREMENT,//increment
	.dstmode		 	= DMA_HANDSHAKE_MODE,//handshake
	.srcmode			= DMA_NORMAL_MODE,
	.dstwidth 			= DMA_CTR_WORD_WIDTH,
	.srcwidth 			= DMA_CTR_WORD_WIDTH,
	.src_burst_size 	= 0,//must 0
	.read_num_en		= 0,
	.priority 			= 0,
	.write_num_en		= 0,
	.auto_en 			= 0,//must 0
};
dma_config_st analog_rx_dma_config={
	.dst_req_sel 		= 0,//tx req
	.src_req_sel 		= DMA_REQ_ALGM_RX,
	.dst_addr_ctrl 		= DMA_ADDR_INCREMENT,
	.src_addr_ctrl 		= DMA_ADDR_FIX,
	.dstmode 			= DMA_NORMAL_MODE,
	.srcmode 			= DMA_HANDSHAKE_MODE,
	.dstwidth 			= DMA_CTR_WORD_WIDTH,//must word
	.srcwidth 			= DMA_CTR_WORD_WIDTH,////must word
	.src_burst_size 	= 0,
	.read_num_en 		= 0,
	.priority 			= 0,
	.write_num_en 		= 0,
	.auto_en 			= 0,//must 0
};


static inline void analog_wait(){
	while(reg_ana_ctrl & FLD_ANA_BUSY){}
}


/**
 * @brief      This function serves to analog register read by byte.
 * @param[in]  addr - address need to be read.
 * @return     the result of read.
 */
u8 analog_read_reg8(u8 addr){
	unsigned int r= core_disable_interrupt();
	reg_ana_addr = addr;
	reg_ana_len=0x1;
    reg_ana_ctrl = FLD_ANA_CYC;
    analog_wait();
	unsigned char data= reg_ana_data(0);
	core_restore_interrupt(r);
	return data;

}

/**
 * @brief      This function serves to analog register write by byte.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
void analog_write_reg8(u8 addr, u8 data){
	unsigned int r= core_disable_interrupt();
	reg_ana_addr = addr;
    reg_ana_data(0) = data;
    reg_ana_ctrl = (FLD_ANA_CYC | FLD_ANA_RW);
      analog_wait();
      reg_ana_ctrl =0x00;
      core_restore_interrupt(r);
}

/**
 * @brief      This function serves to analog register write by halfword.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
void analog_write_reg16(u8 addr, u16 data)
{   unsigned int r= core_disable_interrupt();
	reg_ana_addr = addr;
	reg_ana_addr_data16	 = data;
    reg_ana_ctrl = (FLD_ANA_CYC | FLD_ANA_RW);
	analog_wait();
	core_restore_interrupt(r);
}

/**
 * @brief      This function serves to analog register write by halfword.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
u16 analog_read_reg16(u8 addr)
{
	unsigned int r= core_disable_interrupt();
	reg_ana_len=2;
	reg_ana_addr = addr;
	reg_ana_ctrl = FLD_ANA_CYC;
	analog_wait();
	unsigned short data=reg_ana_addr_data16;
	core_restore_interrupt(r);
	 return data;
}



/**
 * @brief      This function serves to analog register write by word.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */

u32 analog_read_reg32(u8 addr)
{  unsigned int r= core_disable_interrupt();
	reg_ana_len=4;
	reg_ana_addr = addr;
	reg_ana_ctrl = FLD_ANA_CYC;
	analog_wait();
	unsigned int data=reg_ana_addr_data32;
	core_restore_interrupt(r);
	 return data;
}


/**
 * @brief      This function serves to analog register write by word.
 * @param[in]  addr - address need to be write.
 * @param[in]  data - the value need to be write.
 * @return     none.
 */
void analog_write_reg32(u8 addr, u32 data)
{ unsigned int r= core_disable_interrupt();
	reg_ana_addr = addr;
	reg_ana_addr_data32	= data;
	reg_ana_ctrl = (FLD_ANA_CYC | FLD_ANA_RW);
	analog_wait();
	core_restore_interrupt(r);
}


void analog_dma_write_reg32(dma_chn_e chn, unsigned char addr, void * pdat)
{
	unsigned char r = core_disable_interrupt();
	reg_ana_addr = addr;
	reg_dma_src_addr(chn)=(unsigned int)pdat-0x80000+0xc0200000;
	reg_dma_dst_addr(chn)= 0x80140184;
	dma_set_size(chn,4,DMA_WORD_WIDTH);
	analog_tx_dma_config.dstwidth = DMA_CTR_WORD_WIDTH;
	analog_tx_dma_config.srcwidth = DMA_CTR_WORD_WIDTH;
	dma_config(chn, &analog_tx_dma_config);
	dma_chn_en(chn);
//	dma_chn_ctr(chn,0,1,0,2,0,DMA_REQ_ALGM_TX);
	reg_ana_ctrl = FLD_ANA_CYC | FLD_ANA_RW;
	analog_wait();
	core_restore_interrupt(r);
}



void analog_dma_read_reg32(dma_chn_e chn, unsigned char addr,void * pdat)
{
	unsigned char r = core_disable_interrupt();
	reg_ana_len=0x04;
	reg_ana_addr = addr;
	reg_ana_ctrl = FLD_ANA_CYC;
	reg_dma_src_addr(chn)=0x80140184;
	reg_dma_dst_addr(chn)= (unsigned int)pdat-0x80000+0xc0200000;
	//dma_set_chn_size_byte(DMA0,0x4 );
	dma_set_size(chn,4,DMA_WORD_WIDTH);
	analog_rx_dma_config.dstwidth = DMA_CTR_WORD_WIDTH;
	analog_rx_dma_config.srcwidth = DMA_CTR_WORD_WIDTH;
	dma_config(chn, &analog_rx_dma_config);
	dma_chn_en(chn);
//	dma_chn_ctr(chn,1,0,2,0,DMA_REQ_ALGM_RX,0);
	analog_wait();
	core_restore_interrupt(r);
}

/**
 * @brief      This function serves to analog register read.
 * @param[in]  addr - address need to be read.
 * @param[in]  *buff - the value need to be read.
 * @param[in]  len - the length of read value.2,3,4
 * @return     none.
 */
void analog_dma_write_buff(dma_chn_e chn, unsigned char addr, unsigned char * pdat, u32 len)
{
	unsigned char r = core_disable_interrupt();
	reg_ana_addr = addr;
	reg_dma_src_addr(chn)=(unsigned int)pdat-0x80000+0xc0200000;
	reg_dma_dst_addr(chn)= 0x80140184;
	dma_set_size(chn,len,DMA_WORD_WIDTH);
	analog_tx_dma_config.dstwidth = DMA_CTR_WORD_WIDTH;
	analog_tx_dma_config.srcwidth = DMA_CTR_WORD_WIDTH;
	dma_config(chn, &analog_tx_dma_config);
	dma_chn_en(chn);
//	dma_chn_ctr(chn,0,1,0,2,0,DMA_REQ_ALGM_TX);
	reg_ana_ctrl =0x60;
	analog_wait();
	core_restore_interrupt(r);
}


/**
 * @brief      This function serves to analog register write.
 * @param[in]  addr - address need to be write.
 * @param[in]  *v - the value need to be write.
 * @param[in]  len - the length of write value.
 * @return     none.
 */

void analog_dma_read_buff(dma_chn_e chn, unsigned char addr,unsigned char * pdat, u32 len)
{
	unsigned char r = core_disable_interrupt();
	reg_ana_len=len;
	reg_ana_addr = addr;
	reg_ana_ctrl = FLD_ANA_CYC;
	reg_dma_src_addr(chn)=0x80140184;
	reg_dma_dst_addr(chn)= (unsigned int)pdat-0x80000+0xc0200000;
	dma_set_size(chn,len,DMA_WORD_WIDTH);
	analog_rx_dma_config.dstwidth = DMA_CTR_WORD_WIDTH;
	analog_rx_dma_config.srcwidth = DMA_CTR_WORD_WIDTH;
	dma_config(chn, &analog_rx_dma_config);
	dma_chn_en(chn);
//	dma_chn_ctr(chn,1,0,2,0,DMA_REQ_ALGM_RX,0);
	analog_wait();
	core_restore_interrupt(r);
}


void analog_dma_write_addr_data(dma_chn_e chn, void * pdat, int len)
{
	unsigned char r = core_disable_interrupt();
//	reg_ana_addr = addr;

	reg_dma_src_addr(chn)=(unsigned int)pdat-0x80000+0xc0200000;
	reg_dma_dst_addr(chn)= 0x80140184;
//	dma_chn_ctr(chn,0,1,0,2,0,DMA_REQ_ALGM_TX);
	dma_set_size(chn,len,DMA_WORD_WIDTH);

	analog_tx_dma_config.dstwidth = DMA_CTR_WORD_WIDTH;
	analog_tx_dma_config.srcwidth = DMA_CTR_WORD_WIDTH;
	dma_config(chn, &analog_tx_dma_config);
	dma_chn_en(chn);

	reg_ana_ctrl =  FLD_ANA_RW | FLD_ANA_CYC;

	reg_ana_dma_ctl=FLD_ANA_CYC1|FLD_ANA_DMA_EN;
    analog_wait();
    reg_ana_ctrl =0x60;
    reg_ana_dma_ctl = 0;
    core_restore_interrupt(r);
}



