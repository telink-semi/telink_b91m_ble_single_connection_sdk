/********************************************************************************************************
 * @file     ext_misc.h
 *
 * @brief    This is the header file for BLE SDK
 *
 * @author	 BLE GROUP
 * @date         06,2022
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

#ifndef DRIVERS_B92_EXT_MISC_H_
#define DRIVERS_B92_EXT_MISC_H_

#include "nds_intrinsic.h"

#include "../analog.h"
#include "../dma.h"
#include "../gpio.h"
#include "../lib/include/pm.h"
#include "../timer.h"
#include "../flash.h"
#include "../mdec.h"
#include "../lib/include/trng.h"
#include "../lib/include/sys.h"
#include "../lib/include/plic.h"
#include "../stimer.h"
#include "../clock.h"
#include "types.h"
#include "compiler.h"
#include "common/bit.h"
#include "../reg_include/soc.h"
/******************************* stimer_start ******************************************************************/

#define reg_system_tick_irq				reg_system_irq_level

typedef enum {
	STIMER_IRQ_MASK     		=   BIT(0),
	STIMER_32K_CAL_IRQ_MASK     =   BIT(1),
}stimer_irq_mask_e;

typedef enum {
	FLD_IRQ_SYSTEM_TIMER     		=   BIT(0),
}system_timer_irq_mask_e;


typedef enum {
	STIMER_IRQ_CLR	     		=   BIT(0),
	STIMER_32K_CAL_IRQ_CLR     	=   BIT(1),
}stimer_irq_clr_e;


/**
 * @brief    This function serves to enable system timer interrupt.
 * @return  none
 */
static inline void systimer_irq_enable(void)
{
	reg_irq_src0 |= BIT(IRQ1_SYSTIMER);
	//plic_interrupt_enable(IRQ1_SYSTIMER);
}

/**
 * @brief    This function serves to disable system timer interrupt.
 * @return  none
 */
static inline void systimer_irq_disable(void)
{
	reg_irq_src0 &= ~BIT(IRQ1_SYSTIMER);
	//plic_interrupt_disable(IRQ1_SYSTIMER);
}

static inline void systimer_set_irq_mask(void)
{
	reg_system_irq_mask |= STIMER_IRQ_MASK;
}

static inline void systimer_clr_irq_mask(void)
{
	reg_system_irq_mask &= (~STIMER_IRQ_MASK);
}

static inline unsigned char systimer_get_irq_status(void)
{
	return reg_system_cal_irq & FLD_IRQ_SYSTEM_TIMER;
}

static inline void systimer_clr_irq_status(void)
{
	reg_system_cal_irq = STIMER_IRQ_CLR;
}

static inline void systimer_set_irq_capture(unsigned int tick)
{
	reg_system_irq_level = tick;
}

static inline unsigned int systimer_get_irq_capture(void)
{
	return reg_system_irq_level;
}

static inline int tick1_exceed_tick2(u32 tick1, u32 tick2)
{
	return (u32)(tick1 - tick2) < BIT(30);
}
/******************************* stimer_end ********************************************************************/




/******************************* aes_start ******************************************************************/
extern unsigned int aes_data_buff[8];
bool		aes_resolve_irk_rpa(u8 *key, u8 *addr);
/******************************* aes_end ********************************************************************/




/******************************* core_start ******************************************************************/
#define  irq_disable		core_interrupt_disable
#define	 irq_enable			core_interrupt_enable
#define  irq_restore(en)	core_restore_interrupt(en)
/******************************* core_end ********************************************************************/






/******************************* analog_start ******************************************************************/
#define analog_write	analog_write_reg8
#define analog_read		analog_read_reg8

/******************************* analog_end ********************************************************************/







/******************************* trng_start ******************************************************************/
#define rand						trng_rand
#define random_generator_init  		trng_init


/**
 * @brief     This function performs to generate a series of random numbers
 * @param[in]  len - data length
 * @param[out] data - data pointer
 * @return    none
 **/
void generateRandomNum(int len, unsigned char *data);

/******************************* trng_end ********************************************************************/





/******************************* sys_start ******************************************************************/
#define sleep_us(x)					delay_us(x)
#define sleep_ms(x)					delay_ms(x)


/******************************* sys_end ********************************************************************/

/******************************* clock_start ******************************************************************/
typedef enum{
	SYSCLK_16M    =    16,
	SYSCLK_24M    =    24,
	SYSCLK_32M    =    32,
	SYSCLK_48M    =    48,
	SYSCLK_64M    =    64,
}sys_clk_fre_t;

static inline unsigned char clock_get_system_clk()
{
	return sys_clk.cclk;
}
/******************************* clock_end ********************************************************************/


/******************************* plic_start ******************************************************************/
enum{//todo
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

unsigned int cpu_stall_WakeUp_By_RF_SystemTick(int WakeupSrc, unsigned short rf_mask, unsigned int tick);

/******************************* plic_end ********************************************************************/



/******************************* flash_start *****************************************************************/


/******************************* flash_end *******************************************************************/

/******************************* usb_end *********************************************************************/
#define reg_usb_irq	REG_ADDR8(0x100839)
/******************************* usb_end *********************************************************************/

/******************************* core_start ******************************************************************/
#define	SUPPORT_PFT_ARCH		0
/******************************* core_end ********************************************************************/


/******************************* plic_start *****************************************************************/
bool efuse_get_mac_address(u8* mac_read, int length);
/******************************* plic_end *******************************************************************/


/******************************* debug_start ******************************************************************/
void sub_wr_ana(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);
void sub_wr(unsigned int addr, unsigned char value, unsigned char e, unsigned char s);
/******************************* debug_end ********************************************************************/


/******************************* security_start ******************************************************************/
#define SECBOOT_DESC_SECTOR_NUM		2
#define SECBOOT_DESC_SIZE			0x2000  //8K for secure boot descriptor size

#define DESCRIPTOR_PUBKEY_OFFSET	0x1002

#define DESCRIPTOR_WATCHDOG_OFFSET	0x108A

typedef struct {
	unsigned char vendor_mark[4];
} sb_desc_1st_sector_t;

#define DESC_1ST_SECTOR_DATA_LEN      4
#define DESC_2ND_SECTOR_DATA_LEN	146  //16*9 + 2 = 144 + 2


typedef struct {
	unsigned short	multi_boot;
	unsigned char	public_key[64];
	unsigned char	signature[64];
	unsigned int	run_code_adr;   //4 byte
	unsigned int	run_code_size;
	unsigned char	watdog_v[4];
	unsigned char	smpi_lane[4];
} sb_desc_2nd_sector_t;


typedef struct {
	unsigned char	fw_enc_en;
	unsigned char	secboot_en;
	unsigned short  sb_desc_adr_k; //unit: 4KB
} mcu_secure_t;
extern mcu_secure_t  mcuSecur;

bool mcu_security_read_efuse(void);
bool mcu_security_read_idcode(void);
bool efuse_get_pubkey_hash(u8* pHash);
/******************************* security_end ********************************************************************/




unsigned short flash_get_lock_status_mid146085(unsigned short flash_status);
unsigned short flash_get_lock_status_mid156085(unsigned short flash_status);
unsigned short flash_get_lock_status_mid166085(unsigned short flash_status);
unsigned short flash_get_lock_status_mid186085(unsigned short flash_status);
unsigned short flash_get_lock_status_mid1560c8(unsigned short flash_status);



#endif /* DRIVERS_B92_EXT_MISC_H_ */
