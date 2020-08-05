#ifndef     RF_H
#define     RF_H

#include "gpio.h"
#include "sys.h"

/*******************************      Eagle RF      ******************************/
#define RF_CHN_TABLE 		0x8000


//extern  rf_mode_e g_RFMode;

/**
 *  @brief  select status of rf.
 */
typedef enum {
    RF_MODE_TX = 0,
    RF_MODE_RX = 1,
    RF_MODE_AUTO=2
} rf_status_e;

/**
 *  @brief  select RX_CYC2LNA and TX_CYC2PA pin;
 */

typedef enum {
	RF_RFFE_RX_PB1 = GPIO_PB1,
    RF_RFFE_RX_PD6 = GPIO_PD6,
    RF_RFFE_RX_PE4 = GPIO_PE4
} rf_lna_rx_pin_e;


typedef enum {
	RF_RFFE_TX_PB0 = GPIO_PB0,
	RF_RFFE_TX_PB6 = GPIO_PB6,
    RF_RFFE_TX_PD7 = GPIO_PD7,
    RF_RFFE_TX_PE5 = GPIO_PE5
} rf_pa_tx_pin_e;


/**
 *  @brief  Define power list of RF
 */
typedef enum {
	 /*VBAT*/
	 RF_POWER_P9p11dBm = 63, //  9.1 dbm
	 RF_POWER_P8p57dBm  = 45, //   8.6 dbm
	 RF_POWER_P8p05dBm  = 35, //   8.1 dbm
	 RF_POWER_P7p45dBm  = 27, //   7.5 dbm
	 RF_POWER_P6p98dBm  = 23, //   7.0 dbm
	 RF_POWER_P5p68dBm  = 18, //   6.0 dbm
	 /*VANT*/
	 RF_POWER_P4p35dBm  = BIT(7) | 63,   //   4.4 dbm
	 RF_POWER_P3p83dBm  = BIT(7) | 50,   //   3.8 dbm
	 RF_POWER_P3p25dBm  = BIT(7) | 41,   //   3.3 dbm
	 RF_POWER_P2p79dBm  = BIT(7) | 36,   //   2.8 dbm
	 RF_POWER_P2p32dBm  = BIT(7) | 32,   //   2.3 dbm
	 RF_POWER_P1p72dBm  = BIT(7) | 26,   //   1.7 dbm
	 RF_POWER_P0p80dBm  = BIT(7) | 22,   //   0.8 dbm
	 RF_POWER_P0p01dBm  = BIT(7) | 20,   //   0.0 dbm
	 RF_POWER_N0p53dBm  = BIT(7) | 18,   //  -0.5 dbm
	 RF_POWER_N1p37dBm  = BIT(7) | 16,   //  -1.4 dbm
	 RF_POWER_N2p01dBm  = BIT(7) | 14,   //  -2.0 dbm
	 RF_POWER_N3p37dBm  = BIT(7) | 12,   //  -3.4 dbm
	 RF_POWER_N4p77dBm  = BIT(7) | 10,   //  -4.8 dbm
	 RF_POWER_N6p54dBm = BIT(7) | 8,     //  -6.5 dbm
	 RF_POWER_N8p78dBm = BIT(7) | 6,     //  -8.8 dbm
	 RF_POWER_N12p06dBm = BIT(7) | 4,    //  -12.1 dbm
	 RF_POWER_N17p83dBm = BIT(7) | 2,    //  -17.8 dbm
	 RF_POWER_N23p54dBm = BIT(7) | 1,    //  -23.5 dbm

	 RF_POWER_N30dBm    = 0xff,          //  -30 dbm
	 RF_POWER_N50dBm    = BIT(7) | 0,    //  -50 dbm

} rf_power_level_e;

extern const rf_power_level_e rf_power_Level_list[60];
/**
 *  @brief  Define power index list of RF
 */
typedef enum {
	 /*VBAT*/
	 RF_POWER_INDEX_P9p11dBm,
	 RF_POWER_INDEX_P8p57dBm,
	 RF_POWER_INDEX_P8p05dBm,
	 RF_POWER_INDEX_P7p45dBm,
	 RF_POWER_INDEX_P6p98dBm,
	 RF_POWER_INDEX_P5p68dBm,
	 /*VANT*/
	 RF_POWER_INDEX_P4p35dBm,
	 RF_POWER_INDEX_P3p83dBm,
	 RF_POWER_INDEX_P3p25dBm,
	 RF_POWER_INDEX_P2p79dBm,
	 RF_POWER_INDEX_P2p32dBm,
	 RF_POWER_INDEX_P1p72dBm,
	 RF_POWER_INDEX_P0p80dBm,
	 RF_POWER_INDEX_P0p01dBm,
	 RF_POWER_INDEX_N0p53dBm,
	 RF_POWER_INDEX_N1p37dBm,
	 RF_POWER_INDEX_N2p01dBm,
	 RF_POWER_INDEX_N3p37dBm,
	 RF_POWER_INDEX_N4p77dBm,
	 RF_POWER_INDEX_N6p54dBm,
	 RF_POWER_INDEX_N8p78dBm,
	 RF_POWER_INDEX_N12p06dBm,
	 RF_POWER_INDEX_N17p83dBm,
	 RF_POWER_INDEX_N23p54dBm,
} rf_power_level_index_e;



#define DMA_RFRX_LEN_HW_INFO				0	// 826x: 8
#define DMA_RFRX_OFFSET_HEADER				4	// 826x: 12
#define DMA_RFRX_OFFSET_RFLEN				5   // 826x: 13
#define DMA_RFRX_OFFSET_DATA				6	// 826x: 14

#define RF_TX_PAKET_DMA_LEN(rf_data_len)		(((rf_data_len)+3)/4)|(((rf_data_len) % 4)<<22)
#define DMA_RFRX_OFFSET_CRC24(p)			(p[DMA_RFRX_OFFSET_RFLEN]+6)  //data len:3
#define DMA_RFRX_OFFSET_TIME_STAMP(p)		(p[DMA_RFRX_OFFSET_RFLEN]+9)  //data len:4
#define DMA_RFRX_OFFSET_FREQ_OFFSET(p)		(p[DMA_RFRX_OFFSET_RFLEN]+13) //data len:2
#define DMA_RFRX_OFFSET_RSSI(p)				(p[DMA_RFRX_OFFSET_RFLEN]+15) //data len:1, signed

#define    RF_ZIGBEE_PACKET_LENGTH_OK(p)    			(p[0]  == p[4]+9)
#define    RF_ZIGBEE_PACKET_CRC_OK(p)       			((p[p[0]+3] & 0x51) == 0x0)

#define		RF_BLE_PACKET_LENGTH_OK(p)				( *((unsigned int*)p) == p[5]+13)    			//dma_len must 4 byte aligned
#define		RF_BLE_PACKET_CRC_OK(p)					((p[*((unsigned int*)p) + 3] & 0x01) == 0x0)
#define     RF_NRF_ESB_PACKET_LENGTH_OK(p)          (p[0] == (p[4] & 0x3f) + 11)
#define     RF_NRF_ESB_PACKET_CRC_OK(p)             ((p[p[0]+3] & 0x01) == 0x00)
#define     RF_NRF_SB_PACKET_CRC_OK(p)              ((p[p[0]+3] & 0x01) == 0x00)
#define 	reg_rf_irq_mask							REG_ADDR16(0x0140a1c)

#define    tx_tl_ctrl									REG_ADDR8(0x140c9a)

#define TCMD_UNDER_BOTH			0xc0
#define TCMD_UNDER_RD			0x80
#define TCMD_UNDER_WR			0x40

#define TCMD_MASK				0x3f

#define TCMD_WRITE				0x3
#define TCMD_WAIT				0x7
#define TCMD_WAREG				0x8

/**
 *  command table for special registers
 */
typedef struct tbl_cmd_set_st {
	unsigned int  	ADR;
	unsigned char	DAT;
	unsigned char	CMD;
} tbl_cmd_set_st;

/**
 *  @brief  Define RF mode
 */
typedef enum {
	RF_MODE_BLE_2M =    		BIT(0),
	RF_MODE_BLE_1M = 			BIT(1),
    RF_MODE_BLE_1M_NO_PN   =    BIT(2),
	RF_MODE_ZIGBEE_250K    =    BIT(3),
    RF_MODE_LR_S2_500K     =    BIT(4),
    RF_MODE_LR_S8_125K     =    BIT(5),
    RF_MODE_PRIVATE_250K   =    BIT(6),
    RF_MODE_PRIVATE_500K   =    BIT(7),
    RF_MODE_PRIVATE_1M     =    BIT(8),
    RF_MODE_PRIVATE_2M     =    BIT(9),
    RF_MODE_ANT     	   =    BIT(10),
    RF_MODE_BLE_2M_NO_PN   =    BIT(11),
    RF_MODE_HYBEE_1M   	   =    BIT(12),
    RF_MODE_HYBEE_2M   	   =    BIT(13),
    RF_MODE_HYBEE_500K     =    BIT(14),
} rf_mode_e;



/**
 *  @brief  Define RF channel
 */
typedef enum {
	 RF_CHANNEL_0   =    BIT(0),
	 RF_CHANNEL_1   =    BIT(1),
	 RF_CHANNEL_2   =    BIT(2),
	 RF_CHANNEL_3   =    BIT(3),
	 RF_CHANNEL_4   =    BIT(4),
	 RF_CHANNEL_5   =    BIT(5),
	 RF_CHANNEL_NONE =   0x00,
	 RF_CHANNEL_ALL =    0x0f,
} rf_channel_e;


/**
 * @brief      This function performs a series of operations of writing digital or analog registers
 *             according to a command table
 * @param[in]  pt - pointer to a command table containing several writing commands
 * @param[in]  size  - number of commands in the table
 * @return     number of commands are carried out
 */

extern int rf_loadtbl_cmd_set_st(const tbl_cmd_set_st * pt, int size);


/**
*	@brief     This function serves to initiate the mode of RF
*	@param[in] rf_mode  -  mode of RF
*	@return	   none.
*/

extern void rf_drv_init (rf_mode_e rf_mode);



/**
 * @brief   This function serves to judge the statue of  RF receive.
 * @param   none.
 * @return  none.
 */
static inline unsigned char rf_is_rf_receiving_pkt(void)
{
	//if the value of [2:0] of the reg_0x140840 isn't 0 , it means that the RF is in the receiving packet phase.(confirmed by junwen)
	return ((read_reg8(0x140840)&0x07) != 0);//todo
}
/**
*	@brief	  	This function serves to set the which irq enable
*	@param[in]	mask:Options that need to be enabled.
*	@return	 	Yes: 1, NO: 0.
*/
static inline void rf_set_irq_mask(u16 mask)
{
	BM_SET(reg_rf_irq_mask,mask);
}

	#define			RF_FAST_MODE			1
	#define			RF_TRX_MODE				0x80
	#define			RF_TRX_OFF				0x45		//f02


/**
*	@brief	  	This function serves to clear the TX/RX irq mask
*   @param      mask:RX/TX irq value.
*	@return	 	none
*/
static inline void rf_clr_irq_mask(u16 mask)
{
	BM_CLR(reg_rf_irq_mask,mask);
}

/**
*	@brief	  	This function serves to determine whether sending a packet of data is finished
*	@param[in]	none.
*	@return	 	Yes: 1, NO: 0.
*/
static inline unsigned short rf_get_irq_status(u16 mask)
{
	return ((unsigned short )BM_IS_SET(reg_rf_irq_status,mask));
}

/**
*	@brief	  	This function serves to clear the Tx/Rx finish flag bit.
*				After all packet data are sent, corresponding Tx finish flag bit
*				will be set as 1.By reading this flag bit, it can check whether
*				packet transmission is finished. After the check, it¡¯s needed to
*				manually clear this flag bit so as to avoid misjudgment.
*   @param      none
*	@return	 	none
*/
static inline void rf_clr_irq_status(u16 mask)
{
	 BM_SET(reg_rf_irq_status, mask);
}
/**
 * @brief   This function serves to settle adjust for RF Tx.
 * @param   txstl_us - adjust TX settle time.
 * @return  none
 */
static inline void 	tx_settle_adjust(unsigned short txstl_us)
{
	REG_ADDR16(0x80140a04) = txstl_us;
}
/**
*	@brief     This function serves to set RF tx DMA setting
*	@param[in] fifo_dep -tx chn deep.
*	@param[in] fifo_byte_size -tx_idx_addr = {tx_chn_adr*bb_tx_size,4'b0}
*	@return	   none.
*/
void rf_set_tx_dma(unsigned char fifo_depth,unsigned char fifo_byte_size);

/**
*	@brief     This function serves to srx dma setting
*	@param[in] buff -DMA rx buffer.
*	@param[in] fifo_dep -rx chn deep
*	@param[in] fifo_byte_size -rx_idx_addr = {tx_chn_adr*bb_tx_size,4'b0}
*	@return	   none.
*/
void rf_set_rx_dma(unsigned char *buff,unsigned char wptr_mask,unsigned char fifo_byte_size);


/**
*	@brief     This function serves to RF trigger stx
*	@param[in] addr -DMA tx buffer.
*	@param[in] schedule_mode_en -0:trigger stx immediately,1:trigger stx after tick.
*	@param[in] tick -Send after tick delay.
*	@return	   none.
*/
void rf_start_stx(void* addr,unsigned char schedule_mode,  unsigned int tick);

void rf_start_stx2rx  (void* addr, unsigned int tick);

/**
*	@brief     This function serves to trigger srx on
*	@param[in] addr -DMA rx buffer.
*	@param[in] schedule_mode_en -0:trigger srx immediately,1:trigger srx after tick.
*	@param[in] tick -Receive after tick delay.
*	@return	   none.
*/
void rf_start_srx(unsigned char schedule_mode,  unsigned int tick);



/**
*	@brief	  	This function serves to get rssi
*   @param[in]      none
*	@return	 	none
*/
signed char rf_rssi_get_154(void);

/**
 * @brief   This function serves to set RF Rx mode.
 * @param   none.
 * @return  none.
 */
void rf_set_rxmode(void);


//void tx_tp_align(void);
/**
 * @brief   This function serves to set RF Tx mode.
 * @param   none.
 * @return  none.
 */
void rf_set_txmode(void);

/**
*	@brief	  	This function serves to set RF Tx packet.
*	@param[in]	addr - the address RF to send packet.
*	@return	 	none.
*/
void rf_tx_pkt(void* addr);

/**
*	@brief	  	This function serves to judge RF Tx/Rx state.
*	@param[in]	rf_status - Tx/Rx status.
*	@param[in]	rf_channel - RF channel.
*	@return	 	failed -1,else success.
*/

int rf_trx_state_set(rf_status_e rf_status, signed char rf_channel);

/**
 * @brief   This function serves to set RF access command.
 * @param[in]  acc - the command.
 * @return  none.
 */
static inline void rf_access_code_comm (unsigned int acc)
{
	reg_rf_access_0 = acc & 0xff;
	reg_rf_access_1 = (acc>>8) & 0xff;
	reg_rf_access_2 = (acc>>16) & 0xff;
	reg_rf_access_3 = (acc>>24) & 0xff;//notice: This state will be reset after reset baseband
}

/**
 * @brief   This function serves to set RF access command.
 * @param[in]   acc - the command.
 * @return  none.
 */
static inline void rf_longrange_access_code_comm (unsigned int acc)
{
	reg_rf_access_0 = acc & 0xff;
	reg_rf_access_1 = (acc>>8) & 0xff;
	reg_rf_access_2 = (acc>>16) & 0xff;
	reg_rf_access_3 = (acc>>24) & 0xff;//notice: This state will be reset after reset baseband

	reg_rf_modem_mode_cfg_rx1_0 |= FLD_RF_LR_TRIG_MODE;
}

/**
*	@brief		this function is to enable/disable each access_code channel for
*				RF Rx terminal.
*	@param[in]	pipe  	Bit0~bit5 correspond to channel 0~5, respectively.
*						0£ºDisable 1£ºEnable
*						If ¡°enable¡± is set as 0x3f (i.e. 00111111),
*						all access_code channels (0~5) are enabled.
*	@return	 	none
*/
static inline void rf_rx_acc_code_enable(rf_channel_e pipe)
{
    write_reg8(0x140c4d, (read_reg8(0x140c4d)&0xc0) | pipe); //rx_access_code_chn_en
}

/**
*	@brief		this function is to select access_code channel for RF Rx terminal.
*	@param[in]	pipe  	Bit0~bit5 correspond to channel 0~5, respectively.
*						0£ºDisable 1£ºEnable
*						If ¡°enable¡± is set as 0x3f (i.e. 00111111),
*						all access_code channels (0~5) are enabled.
*	@return	 	none
*/
static inline void rf_tx_acc_code_select(rf_channel_e pipe)
{
    write_reg8(0x140a15, (read_reg8(0x140a15)&0xf8) | pipe); //Tx_Channel_man[2:0]
}

/**
*	@brief     This function serves to initiate the mode of RF
*	@param[in] rf_mode  -  mode of RF
*	@return	   none.
*/

extern void rf_drv_init (rf_mode_e rf_mode);

/**
 * @brief   	This function serves to set RF baseband channel.
 * @param[in]   chn - channel numbers.
 * @return  	none.
 */
void rf_set_ble_channel(signed char chn_num);

/**
 * @brief   	This function serves to set RF no pn mode baseband channel.
 * @param[in]   chn - freq_num.
 * @return  	none.
 */
void rf_set_channel(signed char chn);

/**
 * @brief   	This function serves to set pri sb mode enable.
 * @param[in]   none.
 * @return  	none.
 */
void rf_private_sb_en(void);


/**
 * @brief   	This function serves to set pri sb mode enable.
 * @param[in]   len-packet length.
 * @return  	none.
 */
void rf_set_private_sb_len(int len);

/**
 * @brief   	This function serves to set zigbee channel.
 * @param[in]   freq-zigbee freq.
 * @return  	none.
 */
void rf_set_zigbee_chn(unsigned int chn);

/**
 * @brief   	This function serves to disable pn of ble mode.
 * @param[in]   none.
 * @return  	none.
 */
void rf_pn_disable(void);

/**
 * @brief   	This function serves to get the right fifo packet.
 * @param[in]   fifo_num-the number of fifo set in dma.
 * @param[in]   fifo_dep-deepth of each fifo set in dma.
 * @param[in]   addr-address of rx packet.
 * @return  	the next rx_packet address.
 */
u8* rf_rx_packet_addr(int fifo_num,int fifo_dep,void* addr);
/**
*	@brief     This function serves to reset RF BaseBand
*	@param[in] none.
*	@return	   none.
*/
static inline void rf_reset_baseband(void)
{
	REG_ADDR8(0x801404e3) = BIT(0);		//rf_reset_baseband
	REG_ADDR8(0x801404e3) = 0;			//release reset signal
}
/**
 * @brief   This function serves to reset function for RF.
 * @param   none
 * @return  none
 */
static inline void reset_sn_nesn(void)
{
	REG_ADDR8(0x80140a01) =  0x01;
}



/**
 * @brief   This function serves to set CRC advantage.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_ble_crc_adv ()
{
	write_reg32 (0x80140824, 0x555555);
}
/**
 * @brief   This function serves to set CRC value for RF.
 * @param[in]  crc - CRC value.
 * @return  none.
 */
static inline void rf_set_ble_crc_value (unsigned int crc)
{
	write_reg32 (0x80140824, crc);
}
/**
 * @brief   This function serves to set RF access code value.
 * @param[in]   ac - the address value.
 * @return  none
 */
static inline void rf_set_ble_access_code_value (unsigned int ac)
{
	write_reg32 (0x80140808, ac);
}
/**
 * @brief   This function serves to reset RF Tx/Rx mode.
 * @param   none.
 * @return  none.
 */

static inline void rf_set_tx_rx_off(void)
{
	write_reg8 (0x80140a16, 0x29);
	write_reg8 (0x80140828, RF_TRX_MODE);	// rx disable
	write_reg8 (0x80140a02, RF_TRX_OFF);	// reset tx/rx state machine
}
/**
 * @brief   This function serves to turn off RF auto mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_tx_rx_off_auto_mode(void)
{
	write_reg8 (0x80140a00, 0x80);
}
/**
*	@brief	  	This function serves to start Rx of auto mode. In this mode,
*				RF module stays in Rx status until a packet is received or it fails to receive packet when timeout expires.
*				Timeout duration is set by the parameter "tick".
*				The address to store received data is set by the function ¡°addr¡±.
*	@param[in]	addr - The address to store received data.
*	@param[in]	tick - Unit is us. It indicates timeout duration in Rx status.Max value: 0xffffff (16777215)
*	@return	 	none
*/
extern void rf_start_brx  (void* addr, unsigned int tick);
extern void rf_start_btx (void* addr, unsigned int tick);
/**
 * @brief   This function serves to set the max length of rx packet.Use byte_len to limit what DMA
 * moves out will not exceed the buffer size we define.And old chip do this through dma size.
 * @param   maxlen:the longest of rx packet.
 * @return  none.
 */
void rf_set_rx_maxlen(unsigned int byte_len);
/**
 * @brief     This function performs to enable RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_on ()
{
	write_reg8  (0x80140a02, 0x45 | BIT(4));	// TX enable
}

/**
 * @brief   This function serves to set RF power level.
 * @param   rf_power_level_e - the RF power types.
 * @return  none.
 */
void rf_set_power_level (rf_power_level_e level);

/**
 * @brief   This function serves to set RF power level index.
 * @param   rf_power_level_e - the RF power types.
 * @return  none.
 */
void rf_set_power_level_index(rf_power_level_index_e idx);


/**
*	@brief	  	This function serves to set pin for RFFE of RF
*   @param[in]     tx_pin - select pin to send
*   @param[in]      rx_pin - select pin to receive
*	@return	 	none
*
*/
extern void rf_rffe_set_pin(rf_pa_tx_pin_e tx_pin, rf_lna_rx_pin_e rx_pin);

/**
 * @brief     This function performs to done RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_done ()
{
	write_reg8  (0x80140a02, 0x45);
}

#if 0

/**
*	@brief     This function serves to set RF ble 1m Register initialization
*	@param[in] none
*	@return	   none.
*/
void ble1m_setup(void);
/**
*	@brief     This function serves to set the tx packet of ble1M
*	@param[in] none
*	@return	   none.
*/
void ble_pktsetup(void);


/**
*	@brief	  	This function is to enable manual rx
*
*	@param[out]	none
*
*	@return	 	none
*/
void tx_manual_on(void);

void txsend(void);

/**
 * @brief   	This function serves to initial RF ble 1m.
 * @param[in]   none.
 * @return  	none.
 */
void rf_set_ble_1M_mode(void);


/**
 * @brief   	This function serves to enable rx manual mode.
 * @param[in]   none.
 * @return  	none.
 */
void rf_rx_manual_en(void);

/**
 * @brief   	This function serves to enable tx manual mode.
 * @param[in]   none.
 * @return  	none.
 */
void rf_tx_manual_en(void);

/**
 * @brief   	This function serves to disable tx manual mode.
 * @param[in]   none.
 * @return  	none.
 */
void rf_tx_rx_maunal_dis(void);



/**
*	@brief     This function serves to reset RF BaseBand
*	@param[in] none.
*	@return	   none.
*/
static inline void rf_baseband_reset(void)
{
	REG_ADDR8(0x61) = BIT(0);		//rf_reset_baseband
	REG_ADDR8(0x61) = 0;			//release reset signal
}


/**
 * @brief   This function serves to set RF power level index.
 * @param   rf_power_level_e - the RF power types.
 * @return  none.
 */
extern void rf_set_power_level_index (rf_power_level_e level);



/**
 * @brief   This function serves to set RF's channel.
 * @param   chn - RF channel.
 * @param   set - the value to set.
 * @return  none.
 */
//extern void rf_set_channel (signed char chn, unsigned short set);

/**
 * @brief   This function serves to set The channel .
 * @param[in]   rf_power_level_e - the RF power types.
 * @return  none.
 */
extern void rf_set_channel_500k(signed short chn, unsigned short set);

/**
*	@brief	  	this function performs to get access_code length.
*
*	@param[in]	len  	Optional range: 3~5
*										Note: The effect for 3-byte access_code is not good.
*
*	@return	 	access_byte_num[2:0]
*/
static inline unsigned char rf_get_acc_code_len(unsigned char len)
{
    return (read_reg8(0x80140805) & 0x07);
}

/**
*	@brief		this function is to select access_code channel for RF Rx terminal.
*	@param[in]	pipe  	Bit0~bit5 correspond to channel 0~5, respectively.
*						0£ºDisable 1£ºEnable
*						If ¡°enable¡± is set as 0x3f (i.e. 00111111),
*						all access_code channels (0~5) are enabled.
*	@return	 	none
*/
static inline void rf_set_tx_acc_code_pipe_en(unsigned char pipe)
{
    write_reg8(0x80140a15, (read_reg8(0x80140a15)&0xf8) | pipe); //Tx_Channel_man[2:0]
}


/**
 * @brief   This function serves to reset RF Tx/Rx mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_tx_rx_off(void)
{
	write_reg8 (0x80140a16, 0x29);
	write_reg8 (0x80140828, 0x80);	// rx disable
	write_reg8 (0x80140a02, 0x45);	// reset tx/rx state machine
}

/**
 * @brief   This function serves to turn off RF auto mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_tx_rx_off_auto(void)
{
	write_reg8 (0x80140a00, 0x80);
}

/**
 * @brief   This function serves to set RF Tx mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_tx_on (void)
{
	write_reg8 (0x80140a02, 0x45);
	write_reg8 (0x80140a02, 0x45 | BIT(4));	//TX enable
}

/**
 * @brief   This function serves to settle adjust for RF Tx.
 * @param   txstl_us - adjust TX settle time.
 * @return  none
 */
static inline void 	rf_set_tx_settle_time(unsigned short txstl_us)
{
	REG_ADDR16(0x80140a04) = txstl_us;
}


/**
 * @brief   This function serves to set pipe for RF Tx.
 * @param   pipe - RF Optional range .
 * @return  none
 */
static inline void rf_set_tx_pipe (unsigned char pipe)
{
	write_reg8 (0x80140a15, 0xf0 | pipe);
}



/**
 * @brief   This function serves to set RF Tx mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_rx_on (void)
{
    write_reg8 (0x140828, 0x80 | BIT(0));	//rx enable
    write_reg8 (0x140a02, 0x45);
    write_reg8 (0x140a02, 0x45 | BIT(5));	// RX enable
}


/**
 * @brief     This function performs to enable RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_on ()
{
	write_reg8  (0x80140a02, 0x45 | BIT(4));	// TX enable
	write_reg32 (0x80140a04, 0x38);
}


/**
 * @brief     This function performs to done RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_done ()
{
	write_reg8  (0x80140a02, 0x45);
	write_reg32 (0x80140a04, 0x50);
}

/**
 * @brief   This function serves to reset function for RF.
 * @param   none
 * @return  none
 */
static inline void rf_ble_sn_nesn_reset(void)
{
	REG_ADDR8(0x80140a01) =  0x01;
}

/**
 * @brief   This function serves to reset the RF sn.
 * @param   none.
 * @return  none.
 */
static inline void rf_ble_sn_reset (void)
{
	write_reg8  (0x80140a01, 0x3f);
	write_reg8  (0x80140a01, 0x00);
}


/**
 * @brief   This function serves to set pipe for RF Tx.
 * @param   p - RF Optional range .
 * @return  none
 */
static inline void rf_ble_set_crc (unsigned char *p)
{
	write_reg32 (0x80140824, p[0] | (p[1]<<8) | (p[2]<<16));
}

/**
 * @brief   This function serves to set CRC value for RF.
 * @param   crc - CRC value.
 * @return  none.
 */
static inline void rf_ble_set_crc_value (unsigned int crc)
{
	write_reg32 (0x80140824, crc);
}


/**
 * @brief   This function serves to set CRC advantage.
 * @param   none.
 * @return  none.
 */
static inline void rf_ble_set_crc_adv ()
{
	write_reg32 (0x80140824, 0x555555);
}


/**
 * @brief   This function serves to set RF access code value.
 * @param   ac - the address value.
 * @return  none
 */
static inline void rf_ble_set_access_code_value (unsigned int ac)
{
	write_reg32 (0x80140808, ac);
}


/**
 * @brief   This function serves to set RF access code advantage.
 * @param   none.
 * @return  none.
 */
static inline void rf_ble_set_access_code_adv (void)
{
	write_reg32 (0x80140808, 0xd6be898e);
}




/**
 * @brief   This function serves to set RF access code 6bit to 32bit.
 * @param   code - the access code.
 * @return  the value of the access code.
 */
static inline unsigned int rf_ble_set_access_code_16to32 (unsigned short code)
{
	unsigned int r = 0;
	for (int i=0; i<16; i++) {
		r = r << 2;
		r |= code & BIT(i) ? 1 : 2;
	}
	return r;
}

/**
 * @brief   This function serves to set RF access code 6bit to 32bit.
 * @param   code - the access code.
 * @return  the value of access code.
 */
static inline unsigned short rf_ble_set_access_code_32to16 (unsigned int code)
{
	unsigned short r = 0;
	for (int i=0; i<16; i++) {
		r = r << 1;

		r |= (code & BIT(i*2)) ? 1 : 0;

	}
	return r;
}


/**
*	@brief	  	this function is to set access code.
*	@param[in]	pipe  	index number for access_code channel.
*	@param[in]	addr    the access code address.
*	@return	 	none
*/
extern void rf_set_acc_code_pipe(unsigned char pipe, const unsigned char *addr);


/**
*	@brief	  	this function is to get access code.
*	@param[in]	pipe  	Set index number for access_code channel.
*	@param[in]	addr  	the access of the code address.
*	@return	 	none
*/
extern void rf_get_acc_code_pipe(unsigned char pipe, unsigned char *addr);


/**
*	@brief	  	This function serves to judge RF Tx/Rx state.
*	@param[in]	rf_status - Tx/Rx status.
*	@param[in]	rf_channel - RF channel.
*	@return	 	failed -1,else success.
*/
extern int rf_set_trx_state(rf_status_e rf_status, signed char rf_channel);


/**
*	@brief	  	This function serves to get RF status.
*	@param[in]	none.
*	@return	 	RF Rx/Tx status.
*/
extern rf_status_e rf_get_trx_state(void);


/**
*	@brief	  	This function serves to start Rx.
*	@param[in]	tick  Tick value of system timer.
*	@return	 	none
*/
//extern void rf_start_srx  (unsigned int tick);


/**
 * @brief   This function serves to set the ble channel.
 * @param   chn_num - channel numbers.
 * @return  none.
 */
extern void 	rf_ble_set_channel (signed char chn_num);


/**
*	@brief	  	This function serves to simulate 100k Tx by 500k Tx
*   @param[in]  *input  		- the content of payload
*   @param[in]	len 			- the length of payload
*   @param[in]  init_val 		- the initial value of CRC
*	@return	 	init_val 		- CRC
*/
extern unsigned short rf_154_ccitt_cal_crc16(unsigned char *input, unsigned int len, unsigned short init_val);


/**
*	@brief	  	This function serves to simulate 100k Tx by 500k Tx
*   @param[in]  *preamble  		- the content of preamble
*   @param[in]	preamble_len 	- the length of preamble
*   @param[in]  *acc_code 		- the content of access code
*   @param[in]  acc_len			- the length of access code
*   @param[in]  *payload		- the content of payload
*   @param[in]	pld_len			- the length of payload
*   @param[in]	*tx_buf			- the data need to be sent
*   @param[in]	crc_init		- the initial value of CRC
*	@return	 	none
*/
extern void rf_154_tx_500k_simulate_100k(unsigned char *preamble, unsigned char preamble_len,
                                     unsigned char *acc_code, unsigned char acc_len,
                                     unsigned char *payload, unsigned char pld_len,
                                     unsigned char *tx_buf, unsigned short crc_init);


/**
*	@brief	  	This function is to stop energy detect and get energy detect value of
*				the current channel for zigbee mode.
*   @param      none
*	@return	 	rf_ed:0x00~0xff
*
*/
extern unsigned char rf_154_stop_ed(void);


/**
*	@brief	  	This function serves to set pin for RFFE of RF
*   @param      tx_pin - select pin to send
*   @param      rx_pin - select pin to receive
*	@return	 	none
*
*/
extern void rf_rffe_set_pin(rf_pa_tx_pin_e tx_pin, rf_lna_rx_pin_e rx_pin);


#endif




void ble_rf_set_rx_dma(unsigned char *buff, unsigned char fifo_byte_size);

void ble_rf_set_tx_dma(unsigned char fifo_dep, unsigned char fifo_byte_size);

void ble_tx_dma_config(void);

void ble_rx_dma_config(void);

#endif
