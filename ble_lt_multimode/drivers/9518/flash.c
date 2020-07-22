/********************************************************************************************************
 * @file     flash.c 
 *
 * @brief    This is the source file for TLSR8258
 *
 * @author	 junwei.lu@telink-semi.com;
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


#include "flash.h"
#include "nds_intrinsic.h"
#include "mspi.h"
#include "plic.h"
#include "timer.h"
#include "sys.h"

#define NOP5	asm("nop");asm("nop");asm("nop");asm("nop");asm("nop")
#define NOP10	NOP5;NOP5
#define NOP40	NOP10;NOP10;NOP10;NOP10

static inline int flash_is_busy(){
	return mspi_read() & 0x01;				//  the busy bit, pls check flash spec
}

/**
 * @brief     This function serves to set flash write command.
 * @param[in] cmd - set command.
 * @return    none
 */
_attribute_ram_code_ static void flash_send_cmd(unsigned char cmd){
	mspi_high();
	delay_us(1);
	mspi_low();
	mspi_write(cmd);
	mspi_wait();
}

/**
 * @brief     This function serves to send flash address.
 * @param[in] addr - the flash address.
 * @return    none
 */
_attribute_ram_code_ static void flash_send_addr(unsigned int addr){
	mspi_write((unsigned char)(addr>>16));
	mspi_wait();
	mspi_write((unsigned char)(addr>>8));
	mspi_wait();
	mspi_write((unsigned char)(addr));
	mspi_wait();
}

volatile unsigned char flash_cnt = 1;

/**
 * @brief     This function serves to wait flash done.
 *            (make this a asynchorous version).
 * @param[in] none.
 * @return    none.
 */
_attribute_ram_code_ static void flash_wait_done(void)
{
	delay_us(100);
	flash_send_cmd(FLASH_READ_STATUS_CMD);

	int i;
	for(i = 0; i < 10000000; ++i){
		if(!flash_is_busy()){
			flash_cnt++;
			break;
		}
	}
	mspi_high();
}

/**
 * @brief 		This function serves to erase a sector.
 * @param[in]	addr the start address of the sector needs to erase.
 * @return 		none
 */
_attribute_ram_code_ void flash_erase_sector(unsigned long addr){

	unsigned int r= core_disable_interrupt();
	__asm__("csrci 	mmisc_ctl,8");//disable BTB
	mspi_stop_xip();
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_SECT_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();
	NOP40;
	__asm__("csrsi 	mmisc_ctl,8");//enable BTB
	core_restore_interrupt(r);
}

/**
 * @brief 		This function writes the buffer's content to a page.
 * @param[in]   addr the start address of the page
 * @param[in]   len the length(in byte) of content needs to write into the page
 * @param[in]   buf the start address of the content needs to write into
 * @return 		none
 */
_attribute_ram_code_ void flash_write_page(unsigned long addr, unsigned long len, unsigned char *buf){


	unsigned int r= core_disable_interrupt();//???irq_disable();
	__asm__("csrci 	mmisc_ctl,8");//disable BTB
	mspi_stop_xip();
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_CMD);
	flash_send_addr(addr);

	unsigned int i;
	for(i = 0; i < len; ++i){
		mspi_write(buf[i]);		/* write data */
		mspi_wait();
	}
	mspi_high();
	flash_wait_done();
	NOP40;
	__asm__("csrsi 	mmisc_ctl,8");//enable BTB
	core_restore_interrupt(r);//???irq_restore(r);

}

/**
 * @brief 		This function reads the content from a page to the buf.
 * @param[in]   addr the start address of the page
 * @param[in]   len the length(in byte) of content needs to read out from the page
 * @param[out]  buf the start address of the buffer
 * @return 		none
 */
_attribute_ram_code_ void flash_read_page(unsigned long addr, unsigned long len, unsigned char *buf){

	unsigned int r= core_disable_interrupt();//???irq_disable();
	__asm__("csrci 	mmisc_ctl,8");//disable BTB
	mspi_stop_xip();
	flash_send_cmd(FLASH_READ_CMD);
	flash_send_addr(addr);

	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_fm_rd_en();//reg_mspi_fm |= FLD_MSPI_RD_TRIG_EN;//???mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();
	/* get data */
	for(int i = 0; i < len; ++i){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_fm_rd_dis();			/* off read auto mode */
	mspi_high();
	NOP40;
	__asm__("csrsi 	mmisc_ctl,8");//enable BTB
	core_restore_interrupt(r);//???irq_restore(r);
}

/**
 * @brief 		This function serves to erase a page(256 bytes).
 * @param[in]   addr the start address of the page needs to erase.
 * @return 		none
 */
_attribute_ram_code_ void flash_erase_chip(void)
{
	unsigned int r= core_disable_interrupt();
	__asm__("csrci 	mmisc_ctl,8");//disable BTB
	mspi_stop_xip();
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_CHIP_ERASE_CMD);
	mspi_high();
	flash_wait_done();
	NOP40;
	__asm__("csrsi 	mmisc_ctl,8");//enable BTB
	core_restore_interrupt(r);
}





_attribute_ram_code_ unsigned int flash_get_jedec_id(void)
{
	unsigned int r  = core_disable_interrupt();
	__asm__("csrci 	mmisc_ctl,8");//disable BTB
	flash_send_cmd(FLASH_GET_JEDEC_ID);
	unsigned char manufacturer = mspi_read();
	unsigned char mem_type = mspi_read();
	unsigned char cap_id = mspi_read();
	mspi_high();
	NOP40;
	__asm__("csrsi 	mmisc_ctl,8");//enable BTB
	core_restore_interrupt(r);

	return (unsigned int)((manufacturer << 24 | mem_type << 16 | cap_id));
}






#if(FLASH_TYPE==S25FL512S)
#else

#if 0
/**
 * @brief 		This function serves to erase a page(256 bytes).
 * @param[in]   addr the start address of the page needs to erase.
 * @return 		none
 */
_attribute_ram_code_ void flash_erase_page(unsigned int addr)
{
	unsigned int r=core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_PAGE_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

	core_restore_interrupt(r);
}

/**
 * @brief 		This function serves to erase a block(32k).
 * @param[in]   addr the start address of the block needs to erase.
 * @return 		none
 */
_attribute_ram_code_ void flash_erase_32kblock(unsigned int addr)
{
	unsigned int r= core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_32KBLK_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

	core_restore_interrupt(r);
}

/**
 * @brief 		This function serves to erase a block(64k).
 * @param[in]   addr the start address of the block needs to erase.
 * @return 		none
 */
_attribute_ram_code_ void flash_erase_64kblock(unsigned int addr)
{
	unsigned int r= core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_64KBLK_ERASE_CMD);
	flash_send_addr(addr);
	mspi_high();
	flash_wait_done();

	core_restore_interrupt(r);
}

/**
 * @brief 		This function write the status of flash.
 * @param[in]  	the value of status
 * @return 		status
 */
_attribute_ram_code_ unsigned char flash_write_status(unsigned char data)
{
	unsigned char result;
	unsigned int r= core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_STATUS_CMD);
	mspi_write(data);
	mspi_wait();
	mspi_high();
	flash_wait_done();

	delay_us(100);
	flash_send_cmd(FLASH_READ_STATUS_CMD);

	result = mspi_read();
	mspi_high();

	core_restore_interrupt(r);

	return  result;
}

/**
 * @brief 		This function reads the status of flash.
 * @param[in]  	none
 * @return 		none
 */
_attribute_ram_code_ unsigned char flash_read_status(void){

	unsigned char status =0;
	unsigned int r= core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(FLASH_READ_STATUS_CMD);
	/* get low 8 bit status */
	status = mspi_read();
	mspi_high();
	core_restore_interrupt(r);
	return status;
}

/**
 * @brief  		Deep Power Down mode to put the device in the lowest consumption mode
 * 				it can be used as an extra software protection mechanism,while the device
 * 				is not in active use,since in the mode,  all write,Program and Erase commands
 * 				are ignored,except the Release from Deep Power-Down and Read Device ID(RDI)
 * 				command.This release the device from this mode
 * @param[in] 	none
 * @return 		none.
 */
_attribute_ram_code_ void flash_deep_powerdown(void)
{
	unsigned int r= core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(FLASH_POWER_DOWN);
	mspi_high();
	delay_us(1);

	core_restore_interrupt(r);
}

/**
 * @brief		The Release from Power-Down or High Performance Mode/Device ID command is a
 * 				Multi-purpose command.it can be used to release the device from the power-Down
 * 				State or High Performance Mode or obtain the devices electronic identification
 * 				(ID)number.Release from Power-Down will take the time duration of tRES1 before
 * 				the device will resume normal operation and other command are accepted.The CS#
 * 				pin must remain high during the tRES1(8us) time duration.
 * @param[in] 	none
 * @return 		none.
 */
_attribute_ram_code_ void flash_release_deep_powerdown(void)
{
	unsigned int r= core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(FLASH_POWER_DOWN_RELEASE);
	mspi_high();
	flash_wait_done();
	mspi_high();

	core_restore_interrupt(r);
}

/**
 * @brief	  MAC id. Before reading UID of flash, you must read MID of flash. and then you can
 *            look up the related table to select the idcmd and read UID of flash
 * @param[in] buf - store MID of flash
 * @return    none.
 */
_attribute_ram_code_ void flash_read_mid(unsigned char *buf){
	unsigned char j = 0;
	unsigned int r= core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(FLASH_GET_JEDEC_ID);
	mspi_write(0x00);		/* dummy,  to issue clock */
	mspi_wait();
	mspi_fm_rd_en();//???mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();

	for(j = 0; j < 3; ++j){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_fm_rd_dis();			/* off read auto mode */
	mspi_high();

	core_restore_interrupt(r);
}

/**
 * @brief	  UID. Before reading UID of flash, you must read MID of flash. and then you can
 *            look up the related table to select the idcmd and read UID of flash
 * @param[in] idcmd - get this value to look up the table based on MID of flash
 * @param[in] buf   - store UID of flash
 * @return    none.
 */
_attribute_ram_code_ void flash_read_uid(unsigned char idcmd,unsigned char *buf)
{
	unsigned char j = 0;
	unsigned int r= core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(idcmd);
	if(idcmd==FLASH_GD_PUYA_READ_UID_CMD)				//< GD/puya
	{
		flash_send_addr(0x00);
		mspi_write(0x00);		/* dummy,  to issue clock */
		mspi_wait();
	}
	else if (idcmd==FLASH_XTX_READ_UID_CMD)		//< XTX
	{
		flash_send_addr(0x80);
		mspi_write(0x00);		/* dummy,  to issue clock */
		mspi_wait();

	}
	mspi_write(0x00);			/* dummy,  to issue clock */
	mspi_wait();
	mspi_fm_rd_en();//???mspi_ctrl_write(0x0a);	/* auto mode */
	mspi_wait();

	for(j = 0; j < 16; ++j){
		*buf++ = mspi_get();
		mspi_wait();
	}
	mspi_fm_rd_dis();			/* off read auto mode */
	mspi_high();
	core_restore_interrupt(r);
}

/**
 * @brief 		 This function serves to read flash mid and uid,and check the correctness of mid and uid.
 * @param[out]   flash_mid - Flash Manufacturer ID
 * @param[out]   flash_uid - Flash Unique ID
 * @return       0:error 1:ok

 */
_attribute_ram_code_ int flash_read_mid_uid_with_check( unsigned int *flash_mid ,unsigned char *flash_uid)
{
	  unsigned char no_uid[16]={0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01,0x51,0x01};
	  int i,f_cnt=0;
	  unsigned int mid;

	  mspi_stop_xip();
	  flash_read_mid((unsigned char*)&mid);
	  mid = mid&0xffff;
	  *flash_mid  = mid;
	 //     	  		 CMD        MID
	 //  GD25LD40C 		 0x4b     0x60c8
	 //  GD25LD05C  	 0x4b 	  0x60c8
	 //  P25Q40L   		 0x4b     0x6085
	 //  MD25D40DGIG	 0x4b     0x4051
	  if( (mid == 0x60C8) || (mid == 0x6085) ||(mid == 0x4051)){
		  flash_read_uid(FLASH_GD_PUYA_READ_UID_CMD,(unsigned char *)flash_uid);
	  }else{
		  return 0;
	  }
	  for(i=0;i<16;i++){
		if(flash_uid[i]==no_uid[i]){
			f_cnt++;
		}
	  }
	  if(f_cnt==16){//no uid flash
			  return 0;

	  }else{
		  return  1;
	  }
}

/**
 * @brief 		This function serves to protect data for flash.
 * @param[in]   type - flash type include GD,Puya and XTX
 * @param[in]   data - refer to Driver API Doc.
 * @return 		none
 */
_attribute_ram_code_ void flash_lock(Flash_TypeDef type , unsigned short data)
{
	unsigned int r= core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_STATUS_CMD);
	if ((type == FLASH_TYPE_GD)||(type == FLASH_TYPE_XTX)){
		mspi_write((unsigned char)data);   //8 bit status
	}else if(type == FLASH_TYPE_PUYA){

		mspi_write((unsigned char)data);
		mspi_wait();
		mspi_write((unsigned char)(data>>8));//16bit status

	}
	mspi_wait();
	mspi_high();
	flash_wait_done();
	delay_us(100);
	mspi_high();
	core_restore_interrupt(r);
}

/**
 * @brief 		This function serves to protect data for flash.
 * @param[in]   type - flash type include GD,Puya and XTX
 * @return 		none
 */
_attribute_ram_code_ void flash_unlock(Flash_TypeDef type)
{
	unsigned int r= core_disable_interrupt();

	mspi_stop_xip();
	flash_send_cmd(FLASH_WRITE_ENABLE_CMD);
	flash_send_cmd(FLASH_WRITE_STATUS_CMD);
	if ((type == FLASH_TYPE_GD)||(type == FLASH_TYPE_XTX)){
		mspi_write(0);   //8 bit status
	}else if(type == FLASH_TYPE_PUYA){

		mspi_write(0);
		mspi_wait();
		mspi_write(0);//16bit status

	}
	mspi_wait();
	mspi_high();
	flash_wait_done();
	delay_us(100);
	mspi_high();
	core_restore_interrupt(r);
}
#endif
#endif






