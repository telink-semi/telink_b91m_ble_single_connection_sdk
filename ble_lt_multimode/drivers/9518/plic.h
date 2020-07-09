/********************************************************************************************************
 * @file     analog.h
 *
 * @brief    This is the source file for TLSR9518
 *
 * @author	 Driver Group
 * @date     September 16, 2019
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
 * 			 1.initial release(September 16, 2019)
 *
 * @version  A001
 *
 *******************************************************************************************************/


#ifndef  INTERRUPT_H
#define  INTERRUPT_H
#include "core.h"
#include "../../common/bit.h"
#include "reg_include/register_9518.h"

enum{
	INTCNTL_PRIO_LOW,
	INTCNTL_PRIO_HIGH,
};

typedef enum{
	FLD_IRQ_EXCEPTION_EN ,
	FLD_IRQ_SYSTIMER_EN,
	FLD_IRQ_ALG_EN,
	FLD_IRQ_TIMER1_EN,
	FLD_IRQ_TIMER0_EN,
	FLD_IRQ_DMA_EN,
	FLD_IRQ_BMC_EN,
	FLD_IRQ_USB_CTRL_EP_SETUP_EN,
	FLD_IRQ_USB_CTRL_EP_DATA_EN,
	FLD_IRQ_USB_CTRL_EP_STATUS_EN,
	FLD_IRQ_USB_CTRL_EP_SETINF_EN,
	FLD_IRQ_USB_ENDPOINT_EN,
	FLD_IRQ_ZB_DM_EN,
	FLD_IRQ_ZB_BLE_EN,
	FLD_IRQ_ZB_BT_EN,
	FLD_IRQ_ZB_RT_EN,
	FLD_IRQ_PWM_EN,
	FLD_IRQ_PKE_EN,//add
	FLD_IRQ_UART1_EN,
	FLD_IRQ_UART0_EN,
	FLD_IRQ_DFIFO_EN,
	FLD_IRQ_I2C_EN,
	FLD_IRQ_SPI_APB_EN,
	FLD_IRQ_USB_PWDN_EN,
	FLD_IRQ_EN,
	FLD_IRQ_GPIO2RISC0_EN,
	FLD_IRQ_GPIO2RISC1_EN,
	FLD_IRQ_SOFT_EN,

	FLD_IRQ_NPE_BUS0_EN,
	FLD_IRQ_NPE_BUS1_EN,
	FLD_IRQ_NPE_BUS2_EN,
	FLD_IRQ_NPE_BUS3_EN,
	FLD_IRQ_NPE_BUS4_EN,

	FLD_IRQ_USB_250US_EN,
	FLD_IRQ_USB_RESET_EN,
	FLD_IRQ_NPE_BUS7_EN,
	FLD_IRQ_NPE_BUS8_EN,

	FLD_IRQ_NPE_BUS13_EN=42,
	FLD_IRQ_NPE_BUS14_EN,
	FLD_IRQ_NPE_BUS15_EN,

	FLD_IRQ_NPE_BUS17_EN=46,

	FLD_IRQ_NPE_BUS21_EN=50,
	FLD_IRQ_NPE_BUS22_EN,
	FLD_IRQ_NPE_BUS23_EN,
	FLD_IRQ_NPE_BUS24_EN,
	FLD_IRQ_NPE_BUS25_EN,
	FLD_IRQ_NPE_BUS26_EN,
	FLD_IRQ_NPE_BUS27_EN,
	FLD_IRQ_NPE_BUS28_EN,
	FLD_IRQ_NPE_BUS29_EN,
	FLD_IRQ_NPE_BUS30_EN,
	FLD_IRQ_NPE_BUS31_EN,

	FLD_IRQ_NPE_COMB_EN,
	FLD_IRQ_PM_TM_EN,
	FLD_IRQ_EOC_EN,

};

typedef enum{
	IRQ0_EXCEPTION ,
	IRQ1_SYSTIMER,
	IRQ2_ALG,
	IRQ3_TIMER1,
	IRQ4_TIMER0,
	IRQ5_DMA,
	IRQ6_BMC,
	IRQ7_USB_CTRL_EP_SETUP,
	IRQ8_USB_CTRL_EP_DATA,
	IRQ9_USB_CTRL_EP_STATUS,
	IRQ10_USB_CTRL_EP_SETINF,
	IRQ11_USB_ENDPOINT,
	IRQ12_ZB_DM,
	IRQ13_ZB_BLE,
	IRQ14_ZB_BT,
	IRQ15_ZB_RT,
	IRQ16_PWM,
	IRQ17_PKE,//add
	IRQ18_UART1,
	IRQ19_UART0,
	IRQ20_DFIFO,
	IRQ21_I2C,
	IRQ22_SPI_AHB,
	IRQ23_SPI_APB,
	IRQ24_USB_PWDN,
	IRQ25_GPIO,
	IRQ26_GPIO2RISC0,
	IRQ27_GPIO2RISC1,
	IRQ28_SOFT,

	IRQ29_NPE_BUS0,
	IRQ30_NPE_BUS1,
	IRQ31_NPE_BUS2,
	IRQ32_NPE_BUS3,
	IRQ33_NPE_BUS4,

	IRQ34_USB_250US,
	IRQ35_USB_RESET,
	IRQ36_NPE_BUS7,
	IRQ37_NPE_BUS8,

	IRQ42_NPE_BUS13=42,
	IRQ43_NPE_BUS14,
	IRQ44_NPE_BUS15,

	IRQ46_NPE_BUS17=46,

	IRQ50_NPE_BUS21=50,
	IRQ51_NPE_BUS22,
	IRQ52_NPE_BUS23,
	IRQ53_NPE_BUS24,
	IRQ54_NPE_BUS25,
	IRQ55_NPE_BUS26,
	IRQ56_NPE_BUS27,
	IRQ57_NPE_BUS28,
	IRQ58_NPE_BUS29,
	IRQ59_NPE_BUS30,
	IRQ60_NPE_BUS31,

	IRQ61_NPE_COMB,
	IRQ62_PM_TM,
	IRQ63_EOC,

} irq_source_e;

/**
 * @brief    This function serves to set plic feature.
 * @return  none
 */
static inline void plic_set_feature (feature_e feature)
{
 reg_irq_feature = feature;//enable vectored in PLIC
}



static inline void plic_set_pending (irq_source_e src)
{
	reg_irq_pending(src)=BIT(src%32);
}


static inline void plic_set_threshold (u32 threshold)
{
	reg_irq_threshold=threshold;
}


static inline void plic_set_priority (irq_source_e src, u32 priority)
{
	reg_irq_src_priority(src)=priority;

}


/**
 * @brief    This function serves to enable plic interrupt source.
 * @param[in]   src- interrupt source.
 * @return  none
 */
static inline void plic_interrupt_enable(irq_source_e  src)
{
	reg_irq_src(src) |= BIT(src%32);

}

/**
 * @brief    This function serves to disable plic interrupt source.
 * @param[in]   src- interrupt source.
 * @return  none
 */
static inline void plic_interrupt_disable(irq_source_e  src)
{
	reg_irq_src(src) &= (~ BIT(src%32));

}

/**
 * @brief    This function serves to clear interrupt source has completed.
 * @param[in]   src- interrupt source.
 * @return  none
 */
static inline void plic_interrupt_complete(irq_source_e  src)
{
	reg_irq_done = src;
}

/**
 * @brief    This function serves to claim  interrupt source.
 * @param[in]   src- interrupt source.
 * @return  none
 */
static inline  unsigned int plic_interrupt_claim(irq_source_e  src)
{
	return reg_irq_done;

}


#endif
