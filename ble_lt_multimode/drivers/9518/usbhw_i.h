/********************************************************************************************************
 * @file	usbhw_i.h
 *
 * @brief	This is the header file for B91
 *
 * @author	Ble Group
 * @date	2019
 *
 * @par		Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
 *			All rights reserved.
 *
 *			The information contained herein is confidential property of Telink
 *          Semiconductor (Shanghai) Co., Ltd. and is available under the terms
 *          of Commercial License Agreement between Telink Semiconductor (Shanghai)
 *          Co., Ltd. and the licensee or the terms described here-in. This heading
 *          MUST NOT be removed from this file.
 *
 *          Licensee shall not delete, modify or alter (or permit any third party to delete, modify, or  
 *          alter) any information contained herein in whole or in part except as expressly authorized  
 *          by Telink semiconductor (shanghai) Co., Ltd. Otherwise, licensee shall be solely responsible  
 *          for any claim to the extent arising out of or relating to such deletion(s), modification(s)  
 *          or alteration(s).
 *
 *          Licensees are granted free, non-transferable use of the information in this
 *          file under Mutual Non-Disclosure Agreement. NO WARRENTY of ANY KIND is provided.
 *
 *******************************************************************************************************/
//
////#pragma once
//#ifndef     USBHW_H
//#define     USBHW_H
//#include "../../drivers.h"
//#include "reg_include/register_9518.h"
///**
// * @brief     This function servers to reset the pointer of control Endpoint.
// * @param[in] data - the value of control Endpoint
// * @return    none.
// */
//static inline void usbhw_reset_ctrl_ep_ptr(void) {
//	reg_ctrl_ep_ptr = 0;
//}
//
///**
// * @brief     This function servers to get the irq status of control Endpoint.
// * @param[in] none.
// * @return    none.
// */
//static inline unsigned int usbhw_get_ctrl_ep_irq(void) {
//	return reg_ctrl_ep_irq_sta;
//}
//
///**
// * @brief     This function servers to clear the irq status of control Endpoint.
// * @param[in] ep - select the Endpoint
// * @return    none.
// */
//static inline void usbhw_clr_ctrl_ep_irq(int ep) {
//#ifdef WIN32
//	BM_CLR(reg_ctrl_ep_irq_sta, ep);
//#else
//	reg_ctrl_ep_irq_sta = ep;
//#endif
//}
//
///**
// * @brief     This function servers to set the value of control Endpoint.
// * @param[in] data - the value of control Endpoint
// * @return    none.
// */
//static inline void usbhw_write_ctrl_ep_ctrl(unsigned char data) {
//	reg_ctrl_ep_ctrl = data;
//}
//
///**
// * @brief     This function servers to read the data of control Endpoint.
// * @param[in] none.
// * @return    the value of control Endpoint
// */
//static inline unsigned char usbhw_read_ctrl_ep_data(void) {
//	return reg_ctrl_ep_dat;
//}
//
///**
// * @brief     This function servers to write the data of control Endpoint.
// * @param[in] data -  the value of control Endpoint
// * @return    none
// */
//static inline void usbhw_write_ctrl_ep_data(unsigned char data) {
//	reg_ctrl_ep_dat = data;
//}
//
///**
// * @brief     This function servers to determine whether control Endpoint is busy.
// * @param[in] none.
// * @return    1: busy; 0: not busy.
// */
//static inline int usbhw_is_ctrl_ep_busy(void) {
//	return reg_ctrl_ep_irq_sta & FLD_USB_EP_BUSY;
//}
//
//
///**
// * @brief     This function servers to reset the pointer of Endpoint.
// * @param[in] data - the value of control Endpoint
// * @return    none.
// */
//static inline void usbhw_reset_ep_ptr(unsigned int ep) {
//	reg_usb_ep_ptr(ep & 0x07) = 0;
//}
//
//
//
///**
// * @brief     This function servers to get the irq status of Endpoint.
// * @param[in] none.
// * @return    none.
// */
//static inline unsigned int usbhw_get_eps_irq(void) {
//	return reg_usb_irq;
//}
//
//
//
///**
// * @brief     This function servers to clear the irq status of Endpoint.
// * @param[in] ep - select the Endpoint
// * @return    none.
// */
//static inline void usbhw_clr_eps_irq(int ep) {
//	reg_usb_irq = ep;
//}
//
///**
// * @brief     This function servers to read the data of Endpoint.
// * @param[in] none.
// * @return    the value of Endpoint
// */
//static inline unsigned char usbhw_read_ep_data(unsigned int ep) {
//	return reg_usb_ep_dat(ep & 0x07);
//}
//
///**
// * @brief     This function servers to write the data of Endpoint.
// * @param[in] data -  the value of Endpoint
// * @return    none
// */
//static inline void usbhw_write_ep_data(unsigned int ep, unsigned char data) {
//	reg_usb_ep_dat(ep & 0x07) = data;
//}
//
///**
// * @brief     This function servers to enable the specified Endpoint.
// * @param[in] ep -  the value of Endpoint
// * @param[in] en -  1:enable,0:disable
// * @return    none
// */
//static inline void usbhw_set_ep_en(unsigned int ep, unsigned char en) {
//	if(en)
//	{
//		reg_usb_edp_en |= ep;
//	}
//	else
//	{
//		reg_usb_edp_en &= ~(ep);
//	}
//}
//
///**
// * @brief     This function servers to determine whether Endpoint is busy.
// * @param[in] none.
// * @return    1: busy; 0: not busy.
// */
//static inline unsigned int usbhw_is_ep_busy(unsigned int ep) {
//	return reg_usb_ep_ctrl(ep & 0x07) & FLD_USB_EP_BUSY;
//}
//
///**
// * @brief     This function servers to set the specified data EndPoint to ack.
// * @param[in] ep -  select the data EndPoint.
// * @return    none.
// */
//static inline void usbhw_data_ep_ack(unsigned int ep) {
//	reg_usb_ep_ctrl(ep & 0x07) = FLD_USB_EP_BUSY;
//}
//
///**
// * @brief     This function servers to set the specified data EndPoint to stall.
// * @param[in] ep -  select the data EndPoint.
// * @return    none.
// */
//static inline void usbhw_data_ep_stall(unsigned int ep) {
//	reg_usb_ep_ctrl(ep & 0x07) = FLD_USB_EP_STALL;
//}
//
//
///**
// * @brief     This function servers to set the threshold of printer.
// * @param[in] th - set the threshold for printer
// * @return    none.
// */
//static inline void usbhw_set_printer_threshold(unsigned char th) {
//	reg_usb_ep8_send_thre = th;
//}
//#endif
