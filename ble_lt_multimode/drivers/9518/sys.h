#ifndef SYS_H_
#define SYS_H_
#include "reg_include/soc.h"
#include "reg_include/stimer_reg.h"
#include "../../common/types.h"



#define FLASH_R_BASE_ADDR   		0x20000000
#define REG_RW_BASE_ADDR  			0x80000000
#define REG_ADDR8(a)				(*(volatile unsigned char*)(REG_RW_BASE_ADDR | (a)))
#define REG_ADDR16(a)				(*(volatile unsigned short*)(REG_RW_BASE_ADDR | (a)))
#define REG_ADDR32(a)				(*(volatile unsigned long*)(REG_RW_BASE_ADDR | (a)))

#define write_reg8(addr,v)			(*(volatile unsigned char*)(REG_RW_BASE_ADDR | (addr)) = (unsigned char)(v))
#define write_reg16(addr,v)			(*(volatile unsigned short*)(REG_RW_BASE_ADDR | (addr)) = (unsigned short)(v))
#define write_reg32(addr,v)			(*(volatile unsigned long*)(REG_RW_BASE_ADDR | (addr)) = (unsigned long)(v))

#define read_reg8(addr)				(*(volatile unsigned char*)(REG_RW_BASE_ADDR | (addr)))
#define read_reg16(addr)            (*(volatile unsigned short*)(REG_RW_BASE_ADDR | (addr)))
#define read_reg32(addr)            (*(volatile unsigned long*)(REG_RW_BASE_ADDR | (addr)))

#define write_sram8(addr,v)			(*(volatile unsigned char*)( (addr)) = (unsigned char)(v))
#define write_sram16(addr,v)		(*(volatile unsigned short*)( (addr)) = (unsigned short)(v))
#define write_sram32(addr,v)		(*(volatile unsigned long*)( (addr)) = (unsigned long)(v))

#define read_sram8(addr)			(*(volatile unsigned char*)((addr)))
#define read_sram16(addr)           (*(volatile unsigned short*)((addr)))
#define read_sram32(addr)           (*(volatile unsigned long*)((addr)))

/**********************************************************************************************************************
 *                                         global data type                                                           *
 *********************************************************************************************************************/

/**
 * @brief 	Power type for different application
 */
typedef enum{
	LDO_MODE 		=0x40,	/**< LDO mode */
	DCDC_LDO_MODE	=0x41,	/**< DCDC_LDO mode */
	DCDC_MODE		=0x43,	/**< DCDC mode (16pin is not suported this mode. */
}power_mode_e;

/**
 * @brief define system clock tick per us/ms/s.
 */
enum{
	CLOCK_16M_SYS_TIMER_CLK_1US = 16,
	CLOCK_16M_SYS_TIMER_CLK_1MS = 16*1000,
	CLOCK_16M_SYS_TIMER_CLK_1S =  16*1000*1000,
};


/**********************************************************************************************************************
 *                                     global variable declaration                                                    *
 *********************************************************************************************************************/


/**********************************************************************************************************************
 *                                      global function prototype                                                     *
 *********************************************************************************************************************/

/*
 * @brief     This function performs to enable system timer and 32K calibration.
 * @param[in] none.
 * @return    system timer tick value.
**/
static inline  void  sys_clock_time_en(void)
{
reg_system_ctrl|=FLD_SYSTEM_TIMER_EN|FLD_SYSTEM_32K_CAL_EN ;
}


/*
 * @brief     This function performs to get system timer tick.
 * @param[in] none.
 * @return    system timer tick value.
**/
static inline u32 clock_time(void)
{

	return reg_system_tick;
}


/**
 * @brief     This function serves to set timeout by us.
 * @param[in] ref - reference tick of system timer .
 * @param[in] us  - count by us.
 * @return    true - timeout, false - not timeout
 */
static inline _Bool clock_time_exceed(unsigned int ref, unsigned int us)
{
	return ((unsigned int)(clock_time() - ref) > us * CLOCK_16M_SYS_TIMER_CLK_1US);
}

/**
 * @brief   	This function serves to initialize system.
 * @param[in]	power_mode - power mode(LDO/DCDC/LDO_DCDC)
 * @return  	none
 */
void sys_init(power_mode_e power_mode);


/**
 * @brief     This function performs to set delay time by us.
 * @param[in] microsec - need to delay.
 * @return    none
 */
void delay_us(u32 microsec);

/**
 * @brief     This function performs to set delay time by ms.
 * @param[in] millisec - need to delay.
 * @return    none
 */
void delay_ms(u32 millisec);


#define sleep_us(x)					delay_us(x)
#define sleep_ms(x)					delay_ms(x)


#endif
