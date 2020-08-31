/*
 * emi.h
 *
 *  Created on: Jul 13, 2020
 *      Author: telink
 */

#ifndef  EMI_H_
#define  EMI_H_

#include "rf.h"

#define ACCESS_ADDR                      0x140808
#define ACCESS_CODE_EMI                  0x29417671
#define ACCLEN_ADDR                      0x140805

/**
 * @brief   This function serves to set singletone power.
 * @param   level - the power level.
 * @return  none.
 */
void rf_set_power_level_index_singletone (rf_power_level_e level);

/**
 * @brief   This function serves to set singletone power and channel.
 * @param   power_level - the power level.
 * @param   rf_chn - the channel.
 * @return  none.
 */
void rf_emi_single_tone(rf_power_level_e power_level,signed char rf_chn);

/**
 * @brief   This function serves to set rx mode and channel.
 * @param   mode - mode of RF
 * @param   rf_chn - the rx channel.
 * @return  none.
 */
void rf_emi_rx(rf_mode_e mode,signed char rf_chn);

/**
 * @brief   This function serves is receiving service program
 * @param   none.
 * @return  none.
 */
void rf_emi_rx_loop(void);

/**
 * @brief   This function serves to close RF
 * @param   none.
 * @return  none.
 */
void rf_emi_stop(void);

/**
 * @brief   This function serves to get the number of packets received
 * @param   none.
 * @return  the number of packets received.
 */
unsigned int rf_emi_get_rxpkt_cnt(void);

/**
 * @brief   This function serves to get the RSSI of packets received
 * @param   none.
 * @return  the RSSI of packets received.
 */
char rf_emi_get_rssi_avg(void);

/**
 * @brief   This function serves to Set the CD mode correlation register.
 * @param   none.
 * @return  none.
 */
void rf_continue_mode_setup(void);

/**
 * @brief   This function serves to init the CD mode.
 * @param   rf_mode - mode of RF.
 * @param   power_level - power level of RF.
 * @param   rf_chn - channel of RF.
 * @param 	pkt_type - The type of data sent
 * 					   0:random data
 * 					   1:0xf0
 * 					   2:0x55
 * @return  none.
 */
void rf_emi_tx_continue_setup(rf_mode_e rf_mode,rf_power_level_e power_level,signed char rf_chn,unsigned char pkt_type);

/**
 * @brief   This function serves to continue to send CD mode.
 * @param   none.
 * @return  none.
 */
void rf_continue_mode_run(void);

/**
 * @brief   This function serves to generate random number.
 * @param   the old random number.
 * @return  the new random number.
 */
unsigned int pnGen(unsigned int state);

/**
 * @brief   This function serves to send packets in the burst mode
 * @param   rf_mode - mode of RF.
 * @param 	pkt_type - The type of data sent
 * 					   0:random data
 * 					   1:0xf0
 * 					   2:0x55
 * @return  none.
 */
void rf_emi_tx_burst_loop(rf_mode_e rf_mode,unsigned char pkt_type);

/**
 * @brief   This function serves to init the burst mode.
 * @param   rf_mode - mode of RF.
 * @param   power_level - power level of RF.
 * @param   rf_chn - channel of RF.
 * @param 	pkt_type - The type of data sent
 * 					   0:random data
 * 					   1:0xf0
 * 					   2:0x55
 * @return  none.
 */
void rf_emi_tx_burst_setup(rf_mode_e rf_mode,rf_power_level_e power_level,signed char rf_chn,unsigned char pkt_type);

/**
 * @brief   This function serves to generate random packets that need to be sent in burst mode
 * @param   *p - the address of random packets.
 * @param   n - the number of random packets.
 * @return  none.
 */
void rf_phy_test_prbs9 (unsigned char *p, int n);

#endif /* DRIVERS_9518_EMI_H_ */
