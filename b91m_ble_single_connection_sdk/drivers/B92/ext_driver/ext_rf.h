/********************************************************************************************************
 * @file     ext_rf.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         11,2022
 *
 * @par     Copyright (c) 2022, Telink Semiconductor (Shanghai) Co., Ltd. ("TELINK")
 *
 *          Licensed under the Apache License, Version 2.0 (the "License");
 *          you may not use this file except in compliance with the License.
 *          You may obtain a copy of the License at
 *
 *              http://www.apache.org/licenses/LICENSE-2.0
 *
 *          Unless required by applicable law or agreed to in writing, software
 *          distributed under the License is distributed on an "AS IS" BASIS,
 *          WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *          See the License for the specific language governing permissions and
 *          limitations under the License.
 *******************************************************************************************************/

#ifndef DRIVERS_B92_DRIVER_EXT_EXT_RF_H_
#define DRIVERS_B92_DRIVER_EXT_EXT_RF_H_

#include "compiler.h"
#include "types.h"

#define RF_POWER_INDEX_P3dBm RF_POWER_INDEX_P3p00dBm

#define FAST_SETTLE			1

enum{
	//BLE mode
	FLD_RF_BRX_SN_INIT	 		=   BIT(4),
	FLD_RF_BRX_NESN_INIT	 	=   BIT(5),
	FLD_RF_BTX_SN_INIT	 		=   BIT(6),
	FLD_RF_BTX_NESN_INIT	 	=   BIT(7),
};

//*********************Note: CRYPT*****************************/
enum{
	FLD_TLK_CRYPT_ENABLE		=   BIT(1),
	FLD_TLK_MST_SLV				=   BIT(2),
};

#define    reg_rf_tlk_sk0       REG_ADDR32(REG_BASEBAND_BASE_ADDR+0xa4)
#define    reg_rf_tlk_sk1       REG_ADDR32(REG_BASEBAND_BASE_ADDR+0xa8)
#define    reg_rf_tlk_sk2       REG_ADDR32(REG_BASEBAND_BASE_ADDR+0xac)
#define    reg_rf_tlk_sk3       REG_ADDR32(REG_BASEBAND_BASE_ADDR+0xb0)

#define    reg_tlk_sk(v)		REG_ADDR32(REG_BASEBAND_BASE_ADDR + 0xa4 + (v*4))



#define    reg_rf_tlk_iv0       REG_ADDR32(REG_BASEBAND_BASE_ADDR+0xb4)
#define    reg_rf_tlk_iv1       REG_ADDR32(REG_BASEBAND_BASE_ADDR+0xb8)

#define    reg_rf_tlk_aad       REG_ADDR8(REG_BASEBAND_BASE_ADDR+0xbc)

#define    reg_ccm_control				REG_ADDR8(REG_BB_LL_BASE_ADDR+0x3f)


enum{
	FLD_R_TXLEN_FLAG			= BIT(0),
};

//39 bits
#define		reg_rf_tx_ccm_pkt_cnt		REG_ADDR32(REG_BB_LL_BASE_ADDR+0x40)
#define		reg_rf_rx_ccm_pkt_cnt		REG_ADDR32(REG_BB_LL_BASE_ADDR+0x48)

//*********************Note: must write and read one word*****************************/
#define REG_CV_LLBT_BASE_ADDR							(0x160000)

#define reg_cv_intcntl									REG_ADDR32(REG_CV_LLBT_BASE_ADDR +0x0c)

#define	reg_cv_llbt_rpase_cntl							REG_ADDR32(REG_CV_LLBT_BASE_ADDR+0x288)

enum
{
	FLD_CV_PRAND			= BIT_RNG(0,23),
	FLD_CV_IRK_NUM			= BIT_RNG(24,27),
	FLD_CV_GEN_RES			= BIT(29),
	FLD_CV_RPASE_START		= BIT(30),
	FLD_CV_PRASE_ENABLE		= BIT(31),
};

#define	reg_cv_llbt_hash_status							REG_ADDR32(REG_CV_LLBT_BASE_ADDR+0x28c)

enum
{
	FLD_CV_hash					= BIT_RNG(0,23),
	FLD_CV_IRK_CNT				= BIT_RNG(24,27),
	FLD_CV_RPASE_STATUS_CLR 	= BIT(28),
	FLD_CV_RPASE_STATUS			= BIT_RNG(29,30),
	FLD_CV_HASH_MATCH			= BIT(31),
};


#define	reg_cv_llbt_irk_ptr								REG_ADDR32(REG_CV_LLBT_BASE_ADDR+0x290)

#define			STOP_RF_STATE_MACHINE							( REG_ADDR8(0x80170200) = 0x80 )


#define DMA_RFRX_LEN_HW_INFO				0	// 826x: 8
#define DMA_RFRX_OFFSET_HEADER				4	// 826x: 12
#define DMA_RFRX_OFFSET_RFLEN				5   // 826x: 13
#define DMA_RFRX_OFFSET_DATA				6	// 826x: 14

#define RF_TX_PAKET_DMA_LEN(rf_data_len)		(((rf_data_len)+3)/4)|(((rf_data_len) % 4)<<22)
#define DMA_RFRX_OFFSET_CRC24(p)			(p[DMA_RFRX_OFFSET_RFLEN]+6)  //data len:3
#define DMA_RFRX_OFFSET_TIME_STAMP(p)		(p[DMA_RFRX_OFFSET_RFLEN]+9)  //data len:4
#define DMA_RFRX_OFFSET_FREQ_OFFSET(p)		(p[DMA_RFRX_OFFSET_RFLEN]+13) //data len:2
#define DMA_RFRX_OFFSET_RSSI(p)				(p[DMA_RFRX_OFFSET_RFLEN]+15) //data len:1, signed

#define	RF_BLE_RF_PAYLOAD_LENGTH_OK(p)					( *((unsigned int*)p) == p[5]+13)
#define	RF_BLE_RF_PACKET_CRC_OK(p)						((p[(p[5]+5 + 11)] & 0x01) == 0x0)
#define	RF_BLE_PACKET_VALIDITY_CHECK(p)					(RF_BLE_RF_PAYLOAD_LENGTH_OK(p) && RF_BLE_RF_PACKET_CRC_OK(p))

#define	RF_BLE_RF_PACKET_CRC_OK_HW_ECC(p)						((p[p[5]+5+11-4] & 0x01) == 0x0)

#define rf_set_tx_packet_address(addr)		(dma_set_src_address(DMA0, convert_ram_addr_cpu2bus(addr)))

typedef enum {
	 RF_ACC_CODE_TRIGGER_AUTO   =    BIT(0),	/**< auto trigger */
	 RF_ACC_CODE_TRIGGER_MANU   =    BIT(1),	/**< manual trigger */
} rf_acc_trigger_mode;



_attribute_ram_code_ void ble_rf_set_rx_dma(unsigned char *buff, unsigned char size_div_16);

_attribute_ram_code_ void ble_rf_set_tx_dma(unsigned char fifo_dep, unsigned char size_div_16);

_attribute_ram_code_ void ble_tx_dma_config(void);

_attribute_ram_code_ void ble_rx_dma_config(void);

void rf_drv_ble_init(void);




/**
 * @brief	  	This function serves to update the value of internal cap.
 * @param[in]  	value   - The value of internal cap which you want to set.
 * @return	 	none.
 */
static inline void 	rf_update_internal_capacitance(unsigned char value)
{
	/*
	 * afe1v_reg10<5:0>		reg_xo_cdac_ana<5:0>		CDAC value (lowest cap to highest cap)
	 * afe1v_reg10<6>		reg_xo_mode_ana				mode control - 0 : AMP_OFF, 1 : AMP_ON.
	 * 													0 is to support dc coupling and 1 is to support ac coupling
	 * afe1v_reg10<7>		reg_xo_cap_off_ana			control of X1 and X2 capacitance values
														0 : cap follows CDAC, 1 : cap OFF
	 */
	analog_write_reg8(0x8A, (analog_read_reg8(0x8A) & 0x40)|(value & 0x3f));
}

/**
 * @brief   This function serves to settle adjust for RF Tx.This function for adjust the differ time
 * 			when rx_dly enable.
 * @param   txstl_us - adjust TX settle time.
 * @return  none
 */
static inline void 	rf_tx_settle_adjust(unsigned short txstl_us)
{
	REG_ADDR16(0x80170204) = txstl_us;
}

/**
 * @brief   This function serves to set RF access code value.
 * @param[in]   ac - the address value.
 * @return  none
 */
static inline void rf_set_ble_access_code_value (unsigned int ac)
{
	write_reg32 (0x80170008, ac);
}

/**
 * @brief   This function serves to set RF access code.
 * @param[in]   p - the address to access.
 * @return  none
 */
static inline void rf_set_ble_access_code (unsigned char *p)
{
	write_reg32 (0x80170008, p[3] | (p[2]<<8) | (p[1]<<16) | (p[0]<<24));
}

/**
 * @brief   This function serves to reset function for RF.
 * @param   none
 * @return  none
 *******************need driver change
 */
static inline void reset_sn_nesn(void)
{
	REG_ADDR8(0x80170201) =  0x01;
}

/**
 * @brief   This function serves to set RF access code advantage.
 * @param   none.
 * @return  none.
 */
static inline void rf_set_ble_access_code_adv (void)
{
	write_reg32 (0x170008, 0xd6be898e);
}


/**
 * @brief   This function serves to triggle accesscode in coded Phy mode.
 * @param   none.
 * @return  none.
 */
static inline void rf_trigger_codedPhy_accesscode(void)
{
	write_reg8(0x170425,read_reg8(0x170425)|0x01);
}

/**
 * @brief     This function performs to enable RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_on ()
{
	write_reg8  (0x80170202, 0x45 | BIT(4));	// TX enable
}

/**
 * @brief     This function performs to done RF Tx.
 * @param[in] none.
 * @return    none.
 */
static inline void rf_ble_tx_done ()
{
	write_reg8  (0x80170202, 0x45);
}

/**
 * @brief   This function serves to set RX first timeout value.
 * @param[in]   tm - timeout, unit: uS.
 * @return  none.
 */
static inline void rf_set_1st_rx_timeout(unsigned int tm)
{
	reg_rf_ll_rx_fst_timeout = tm;
}


/**
 * @brief   This function serves to set RX timeout value.
 * @param[in]   tm - timeout, unit: uS, range: 0 ~ 0xfff
 * @return  none.
 */
static inline void rf_ble_set_rx_timeout(u16 tm)
{
	reg_rf_rx_timeout = tm;
}

static inline void rf_set_dma_tx_addr(unsigned int src_addr)//Todo:need check by sunwei
{
	reg_dma_src_addr(DMA0)=convert_ram_addr_cpu2bus(src_addr);
}

typedef enum{
	FSM_BTX 	= 0x81,
	FSM_BRX 	= 0x82,
	FSM_PTX	= 0x83,
	FSM_PRX    = 0x84,
	FSM_STX 	= 0x85,
	FSM_SRX 	= 0x86,
	FSM_TX2RX	= 0x87,
	FSM_RX2TX	= 0x88,
}fsm_mode_e;


/**
 * @brief     	This function serves to RF trigger RF state machine.
 * @param[in] 	mode  - FSM mode.
 * @param[in] 	tx_addr  - DMA TX buffer, if not TX, must be "NULL"
 * @param[in] 	tick  - FAM Trigger tick.
 * @return	   	none.
 */
void rf_start_fsm(fsm_mode_e mode, void* tx_addr, unsigned int tick);





/**
 * @brief    This function serves to enable zb_rt interrupt source.
 * @return  none
 */
static inline void zb_rt_irq_enable(void)
{
	plic_interrupt_enable(IRQ15_ZB_RT);
}

#define	rf_set_ble_channel	rf_set_ble_chn




#define PRMBL_LENGTH_1M						2	//preamble length for 1M PHY
#define PRMBL_LENGTH_2M						3   //preamble length for 2M PHY
#define PRMBL_LENGTH_Coded					10  //preamble length for Coded PHY, never change this value !!!

#define PRMBL_EXTRA_1M						(PRMBL_LENGTH_1M - 1)	// 1 byte for 1M PHY
#define PRMBL_EXTRA_2M						(PRMBL_LENGTH_2M - 2)	// 2 byte for 2M
#define PRMBL_EXTRA_Coded					0     					// 10byte for Coded, 80uS, no extra byte

///////////////////////////////////////////////////////////////////////////////
//
#if RF_RX_SHORT_MODE_EN//open rx dly//Todo: B92 only. Need check
//TX settle time

/* Tdma + else settle + tx_settle = 150us .
but Tdma is a variable value , it turns shorter with cclk getting faster
so when cclk=64m/96m, 150us turns to 148.5us around, lead to packet loss.
need to add tx settle time to ensure 150us . */

/* TX settle time */
#define			LL_TX_STL_ADV_1M								(124 - PRMBL_EXTRA_1M*8) //116
#define			LL_TX_STL_ADV_2M								(131 - PRMBL_EXTRA_2M*4) //127
#define			LL_TX_STL_ADV_CODED								124


#define 		LL_ADV_LEGADV_SCANRSP_SETTLE					(115 - PRMBL_EXTRA_1M*8) //107


#define 		TX_STL_AUTO_MODE_1M								(127 - PRMBL_EXTRA_1M * 8)
#define 		TX_STL_AUTO_MODE_1M_HIGH_FREQ					(127 - PRMBL_EXTRA_1M * 8)
#define			TX_STL_AUTO_MODE_2M								(133 - PRMBL_EXTRA_2M * 4)
#define			TX_STL_AUTO_MODE_CODED							125

#if FAST_SETTLE
	#define			TX_STL_ADV_1M_FAST_SETTLE						(LL_TX_STL_ADV_1M-57)
	#define			TX_STL_ADV_2M_FAST_SETTLE						(LL_TX_STL_ADV_2M-57)
	#define			TX_STL_ADV_CODED_FAST_SETTLE					(LL_TX_STL_ADV_CODED-57)

	#define 		LL_SCANRSP_TX_SETTLE_FAST_SETTLE				(LL_ADV_LEGADV_SCANRSP_SETTLE-57)

	#define 		TX_STL_AUTO_MODE_1M_FAST_SETTLE					(TX_STL_AUTO_MODE_1M-57)
	#define			TX_STL_AUTO_MODE_2M_FAST_SETTLE					(TX_STL_AUTO_MODE_2M-57)
	#define			TX_STL_AUTO_MODE_CODED_FAST_SETTLE				(TX_STL_AUTO_MODE_CODED-57)

#endif


#else// close rx dly
//TX settle time
#define			LL_TX_STL_ADV_1M								79

#define			LL_TX_STL_ADV_2M								84
#define			LL_ADV_TX_STL_CODED								116

#endif




/* AD convert delay : timing cost on RF analog to digital convert signal process:
 * 					Eagle	1M: 20uS	   2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
 *					Jaguar	1M: 20uS	   2M: 10uS;      500K(S2): 14uS    125K(S8):  14uS
 */
#define AD_CONVERT_DLY_1M											19	//before:20. Jaguar T_IFS need 32M + AD_Convert=19, tested by kai.jia at 2022-11-17
#define AD_CONVERT_DLY_2M											10
#define AD_CONVERT_DLY_CODED										14

#define OTHER_SWITCH_DELAY_1M										0
#define OTHER_SWITCH_DELAY_2M										0
#define OTHER_SWITCH_DELAY_CODED									0


#define HW_DELAY_1M													(AD_CONVERT_DLY_1M + OTHER_SWITCH_DELAY_1M)
#define HW_DELAY_2M													(AD_CONVERT_DLY_2M + OTHER_SWITCH_DELAY_2M)
#define HW_DELAY_CODED												(AD_CONVERT_DLY_CODED + OTHER_SWITCH_DELAY_CODED)

static inline void rf_ble_set_1m_phy(void)
{
	write_reg8(0x17063d,0x61);
	write_reg32(0x170620,0x23200a16);
	write_reg8(0x170420,0x8c);// script cc.BIT[3]continue mode.After syncing to the preamble, it will immediately enter
							  //the sync state again, reducing the probability of mis-syncing.modified by zhiwei,confirmed
							  //by qiangkai and xuqiang.20221205
	write_reg8(0x170422,0x00);
	write_reg8(0x17044d,0x01);
	write_reg8(0x17044e,0x1e);
	write_reg16(0x170436,0x0eb7);
	write_reg16(0x170438,0x71c4);
	write_reg8(0x170473,0x01);

	write_reg8(0x17049a,0x00);//tx_tp_align.
	write_reg16(0x1704c2,0x4b3a);
	write_reg32(0x1704c4,0x7a6e6356);
	write_reg8(0x1704c8,0x39);//bit[0:5]grx_fix
#if 1
	write_reg32(0x170000,0x4440081f | PRMBL_LENGTH_1M<<16);
#else
	write_reg32(0x170000,0x4446081f);
#endif

	write_reg16(0x170004,0x04f5);

	write_reg8(0x1704bb,0x50);//BIT[5]:rxc_chf_sel_ble;1M:0(default) 2M:1 open two stage filter to improve
							  //the sensitivity.modified by zhiwei,confirmed by wenfeng and xuqiang,20230106.
}


static inline void rf_ble_set_2m_phy(void)
{
	write_reg8(0x17063d,0x41);
	write_reg32(0x170620,0x26432a06);
	write_reg8(0x170420,0x8c);// script cc.BIT[3]continue mode.After syncing to the preamble, it will immediately enter
							  //the sync state again, reducing the probability of mis-syncing.modified by zhiwei,confirmed
							  //by qiangkai and xuqiang.20221205
	write_reg8(0x170422,0x01);
	write_reg8(0x17044d,0x01);
	write_reg8(0x17044e,0x1e);
	write_reg16(0x170436,0x0eb7);
	write_reg16(0x170438,0x71c4);
	write_reg8(0x170473,0x01);

	write_reg8(0x17049a,0x00);//tx_tp_align.

	write_reg16(0x1704c2,0x4c3b);
	write_reg32(0x1704c4,0x7b726359);
	write_reg8(0x1704c8,0x39);//bit[0:5]grx_fix
	#if 1
		write_reg32(0x170000,0x4440081f | PRMBL_LENGTH_2M<<16);
	#else
		write_reg32(0x170000,0x4446081f);
	#endif

	write_reg16(0x170004,0x04e5);

	write_reg8(0x1704bb,0x70);//BIT[5]:rxc_chf_sel_ble;1M:0(default) 2M:1 open two stage filter to improve
								  //the sensitivity.modified by zhiwei,confirmed by wenfeng and xuqiang,20230106.
}




static inline void rf_ble_set_coded_phy_common(void)
{
	write_reg8(0x17063d,0x61);
	write_reg32(0x170620,0x23200a16);
	write_reg8(0x170420,0xcd);// script cc.BIT[3]continue mode.After syncing to the preamble, it will immediately enter
							  //the sync state again, reducing the probability of mis-syncing.modified by zhiwei,confirmed
							  //by qiangkai and xuqiang.20221205
	write_reg8(0x170422,0x00);
	write_reg8(0x17044d,0x01);
	write_reg8(0x17044e,0xf0);
	write_reg16(0x170438,0x7dc8);
	write_reg8(0x170473,0xa1);

	write_reg8(0x17049a,0x00);//tx_tp_align.

	write_reg16(0x1704c2,0x4b3a);
	write_reg32(0x1704c4,0x7a6e6356);
	write_reg8(0x1704c8,0x39);//bit[0:5]grx_fix
	#if 1
		write_reg32(0x170000,0x4440081f | PRMBL_LENGTH_Coded<<16);
	#else
		write_reg32(0x170000,0x444a081f);
	#endif

	write_reg8(0x1704bb,0x50);//BIT[5]:rxc_chf_sel_ble;1M:0(default) 2M:1 open two stage filter to improve
							  //the sensitivity.modified by zhiwei,confirmed by wenfeng and xuqiang,20230106.
}


static inline void rf_ble_set_coded_phy_s2(void)
{
	write_reg16(0x170436,0x0cee);
	write_reg16(0x170004,0xa4f5);

}


static inline void rf_ble_set_coded_phy_s8(void)
{
	write_reg16(0x170436,0x0cf6);
	write_reg16(0x170004,0xb4f5);
}



#if FAST_SETTLE
	typedef struct
	{
		u8 LDO_CAL_TRIM;	//0xea[5:0]
		u8 LDO_RXTXHF_TRIM;	//0xee[5:0]
		u8 LDO_RXTXLF_TRIM;	//0xee[7:6]  0xed[3:0]
		u8 LDO_PLL_TRIM;	//0xee[5:0]
		u8 LDO_VCO_TRIM;	//0xee[7:6]  0xef[3:0]
		u8 rsvd;
	}Ldo_Trim;

	typedef struct
	{
		unsigned short cal_tbl[40];
		Ldo_Trim	  ldo_trim;
		unsigned char tx_fast_en;
		unsigned char rx_fast_en;

	}Fast_Settle;
	extern Fast_Settle fast_settle;

	void rf_tx_fast_settle(void);
	void rf_rx_fast_settle(void);
	unsigned short get_rf_hpmc_cal_val(void);
	void set_rf_hpmc_cal_val(unsigned short value);
	unsigned char is_rf_tx_fast_settle_en(void);
	unsigned char is_rf_rx_fast_settle_en(void);
	void get_ldo_trim_val(u8* p);
	void set_ldo_trim_val(u8* p);
	void set_ldo_trim_on(void);

	/**
	 *	@brief	  	this function serve to enable the tx timing sequence adjusted.
	 *	@param[in]	none
	 *	@return	 	none
	*/
	void rf_tx_fast_settle_en(void);

	/**
	 *	@brief	  	this function serve to disable the tx timing sequence adjusted.
	 *	@param[in]	none
	 *	@return	 	none
	*/
	void rf_tx_fast_settle_dis(void);


	/**
	 *	@brief	  	this function serve to enable the rx timing sequence adjusted.
	 *	@param[in]	none
	 *	@return	 	none
	*/
	void rf_rx_fast_settle_en(void);


	/**
	 *	@brief	  	this function serve to disable the rx timing sequence adjusted.
	 *	@param[in]	none
	 *	@return	 	none
	*/
	void rf_rx_fast_settle_dis(void);



#endif

#endif /* DRIVERS_B92_DRIVER_EXT_EXT_RF_H_ */
