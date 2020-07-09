/********************************************************************************************************
 * @file     selection_sort.h
 *
 * @brief    This is the source file for TLSR9518
 *
 * @author   BLE Group
 * @date     Jun 3, 2020
 *
 * @par      Copyright (c) 2019, Telink Semiconductor (Shanghai) Co., Ltd.
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
 *           1.initial release(Mar. 16, 2020)
 *
 * @version  A001
 *
 *******************************************************************************************************/
#ifndef COMMON_SELECTION_SORT_H_
#define COMMON_SELECTION_SORT_H_

#pragma once

void selection_sort_char(unsigned char * arr, int size);
void selection_sort_int(unsigned int * arr, int size);

#endif /* COMMON_SELECTION_SORT_H_ */
