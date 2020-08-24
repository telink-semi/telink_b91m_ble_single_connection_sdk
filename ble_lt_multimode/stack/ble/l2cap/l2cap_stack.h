/*
 * l2cap_stack.h
 *
 *  Created on: Aug 20, 2020
 *      Author: Administrator
 */

#ifndef STACK_BLE_L2CAP_L2CAP_STACK_H_
#define STACK_BLE_L2CAP_L2CAP_STACK_H_



typedef struct{
	u16 	connParaUpReq_minInterval;
	u16 	connParaUpReq_maxInterval;
	u16 	connParaUpReq_latency;
	u16 	connParaUpReq_timeout;

	u8		connParaUpReq_pending;
	u8      rsvd[3];
}para_up_req_t;

extern _attribute_aligned_(4) para_up_req_t	para_upReq;



void 		blt_update_parameter_request (void);
ble_sts_t   blc_l2cap_pushData_2_controller (u16 connHandle, u16 cid, u8 *format, int format_len, u8 *pDate, int data_len);




#endif /* STACK_BLE_L2CAP_L2CAP_STACK_H_ */
