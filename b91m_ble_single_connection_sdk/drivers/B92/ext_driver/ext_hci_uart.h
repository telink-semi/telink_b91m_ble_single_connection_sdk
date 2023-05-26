/********************************************************************************************************
 * @file     ext_hci_uart.h
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

#ifndef DRIVERS_B91_EXT_DRIVER_EXT_HCI_UART_H_
#define DRIVERS_B91_EXT_DRIVER_EXT_HCI_UART_H_


#include "common/types.h"
#include "uart.h"

#define UART_DMA_CHN_RX  DMA2
#define UART_DMA_CHN_TX  DMA3

#define UART_DMA_RX_IRQ_CHN  FLD_DMA_CHANNEL2_IRQ
#define UART_DMA_TX_IRQ_CHN  FLD_DMA_CHANNEL3_IRQ

/*** RTS ***/
#define RTS_INVERT    1 /*!< 0: RTS PIN will change from low to high. !! use for RTS auto mode(default auto mode)*/
#define RTS_THRESH    5 /*!< RTS trigger threshold. range: 1-16. */
/*** CTS ***/
#define CTS_POLARITY  1 /*!< 0: low level stop TX; 1: high level stop TX.*/

void HCI_UartSetPin(uart_num_e uartId,u16 txPin, u16 rxPin);

void HCI_UartInit(uart_num_e uartId,u32 baudrate, u8 *txBuf, u32 len);

void HCI_UartSetFlowCtrl(uart_num_e uartId, u16 ctsPin, u16 rtsPin);

#endif /* DRIVERS_B91_EXT_DRIVER_EXT_HCI_UART_H_ */
