/********************************************************************************************************
 * @file     debug_cfg.h
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

#ifndef STACK_BLE_DEBUG_DEBUG_CFG_H_
#define STACK_BLE_DEBUG_DEBUG_CFG_H_









/*****************************************************************************************************************

												Dump message

*****************************************************************************************************************/
#ifndef	DEBUG_PAIRING_ENCRYPTION
#define DEBUG_PAIRING_ENCRYPTION								0
#endif

/* BLE smp trans.. log enable */
#ifndef	SMP_DBG_EN
#define SMP_DBG_EN												0
#endif

#ifndef	TX_PUSH_DATA_LOG
#define TX_PUSH_DATA_LOG										0
#endif

#ifndef	RX_L2CAP_DATA_LOG
#define RX_L2CAP_DATA_LOG										0
#endif

#ifndef	DBG_BOUNDARY_RX
#define DBG_BOUNDARY_RX											0
#endif

#ifndef	DBG_LL_CTRL_LOG_EN
#define DBG_LL_CTRL_LOG_EN									    0
#endif


#ifndef	DBG_IAL_EN
#define DBG_IAL_EN									   		 	0
#endif


#ifndef	DBG_PRVC_RL_EN
#define DBG_PRVC_RL_EN										0
#endif

#ifndef	DBG_PRVC_LEGADV_EN
#define DBG_PRVC_LEGADV_EN									0
#endif

#ifndef	DBG_PRVC_LEGSCAN_EN
#define DBG_PRVC_LEGSCAN_EN									0
#endif

#ifndef	DBG_PRVC_LEGINIT_EN
#define DBG_PRVC_LEGINIT_EN									0
#endif

#ifndef	DBG_PRVC_CONN_EN
#define DBG_PRVC_CONN_EN									0
#endif


#ifndef	DBG_PRVC_EXTADV_EN
#define DBG_PRVC_EXTADV_EN									0
#endif

#ifndef	DBG_PRVC_EXTSCAN_EN
#define DBG_PRVC_EXTSCAN_EN									0
#endif

#ifndef	DBG_PRVC_EXTINIT_EN
#define DBG_PRVC_EXTINIT_EN									0
#endif


#ifndef	DBG_EXTSCAN_REPORT
#define DBG_EXTSCAN_REPORT										0
#endif

#ifndef	DBG_EXTSCAN_ERR_PKT_EN
#define DBG_EXTSCAN_ERR_PKT_EN									0
#endif


#ifndef	DEB_CIG_MST_EN
#define DEB_CIG_MST_EN											0
#endif

#ifndef	DEB_CIG_SLV_EN
#define DEB_CIG_SLV_EN											0
#endif

#ifndef	DBG_CIS_DISCONN_EN
#define DBG_CIS_DISCONN_EN										0
#endif

#ifndef	DEB_BIG_BCST_EN
#define DEB_BIG_BCST_EN											0
#endif

#ifndef	DEB_BIG_SYNC_EN
#define DEB_BIG_SYNC_EN											0
#endif

#ifndef	DEB_BIG_SYNC_TIMESTAM_EN
#define DEB_BIG_SYNC_TIMESTAM_EN								0
#endif

#ifndef DBG_ISO_TEST_EN
#define	DBG_ISO_TEST_EN											0
#endif



#ifndef	DBG_CIS_TERMINATE
#define DBG_CIS_TERMINATE									   	0
#endif


#ifndef	DBG_CIS_1ST_AP_TIMING_EN
#define DBG_CIS_1ST_AP_TIMING_EN								0
#endif


#ifndef	DBG_NUM_COM_PKT
#define DBG_NUM_COM_PKT											0
#endif


#ifndef	DBG_CIS_PARAM
#define DBG_CIS_PARAM											0
#endif

#ifndef	DBG_CIS_MASTER_PARAM
#define DBG_CIS_MASTER_PARAM									0
#endif

#ifndef	DBG_SET_CIG_PARAMS
#define DBG_SET_CIG_PARAMS										0
#endif

#ifndef DBG_CIS_TX_DATA
#define	DBG_CIS_TX_DATA											0
#endif

#ifndef DBG_CIS_RX_DATA
#define	DBG_CIS_RX_DATA											0
#endif


#ifndef DBG_HCI_CIS_TEST
#define	DBG_HCI_CIS_TEST										0
#endif


#ifndef	DBG_SUBRATE_EN
#define DBG_SUBRATE_EN											0
#endif



#ifndef DBG_LL_PAST_EN
#define DBG_LL_PAST_EN                                          0
#endif

#ifndef DBG_LL_PCL_EN
#define DBG_LL_PCL_EN                                           0
#endif

#ifndef DBG_LL_CC_EN
#define DBG_LL_CC_EN                                            0
#endif


#ifndef STACK_USB_LOG_EN
#define STACK_USB_LOG_EN											0
#endif

#ifndef IUT_HCI_LOG_EN
#define IUT_HCI_LOG_EN												0
#endif


#ifndef BLC_LL_LOG_EN
#define BLC_LL_LOG_EN												0
#endif
/*****************************************************************************************************************

												VCD

*****************************************************************************************************************/

/* BLE rf irq timing && log enable */
#define	SL_STACK_IRQ_TIMING_EN				1





#define SL_STACK_SCHE_TIMING_EN				1



/* ACL connection */
#define	SL_STACK_ACL_BASIC_TIMING_EN		1
#define SL_STACK_ACL_TX_FIFO_EN				0



#define	SL_STACK_CIS_BASIC_TIMING_EN		0
#define SL_STACK_BIS_BASIC_TIMING_EN		0


#define SL_STACK_FSM_TIMING_EN				0




/* controller,BQB,IUT, critical command & event */
#define	SL_STACK_IUT_CMD_EVT				1




#define	SL_STACK_ISO_DATA_EN				1



#define SL_STACK_EXT_PRD_BASE_TIMING_EN     0
#define	SL_STACK_EXTSCAN_BASE_TIMING_EN     0













#if (DBG_SLAVE_CONN_UPDATE)
#define			SLET_upt_cmd_1				10
#define 		SLET_upt_cmd_2				11
#define 		SLET_upt_cmd_3				12
#define 		SLET_upt_cmd_4				13
#define			SLET_upt_sync_1				20
#define 		SLET_upt_sync_2				21
#define 		SLET_upt_sync_3				22
#define 		SLET_upt_sync_4				23
#endif

#if 0
#define 		SLET_05_rx_crc				5
#define 		SLET_06_rx_1st				6
#define 		SLET_07_rx_new				7
#define			SLET_10_tx					10
#define 		SLET_11_c_cmdDone			11
#define 		SLET_12_c_1stRxTmt			12
#define 		SLET_13_c_rxTmt				13
#define 		SLET_14_c_rxCrc2			14
#endif





#if 0
#define			SLEV_txFifo_push			17
#define			SLEV_txFifo_empty			18
#define			SLEV_txFifo_RX				19
#define			SLEV_txFifo_post			20
#define 		SLEV_test_event				31

#define			SL16_tf_hw_push				1
#define			SL16_tf_sw_push				2
#define			SL16_tf_hw_load1			3
#define			SL16_tf_sw_load1			4
#define			SL16_tf_hw_load2			5
#define			SL16_tf_sw_load2			6
#define			SL16_tf_hw_RX				7
#define			SL16_tf_sw_RX				8
#define			SL16_tf_hw_TX				9
#define			SL16_tf_sw_TX				10
#define			SL16_tf_hw_post				11
#define			SL16_tf_sw_post				12
#define			SL16_tf_save				13

#define			SL16_seq_notify				15
#define			SL16_seq_write				16
#define			SL16_test_2B				17
#endif







#endif /* STACK_BLE_DEBUG_DEBUG_CFG_H_ */
