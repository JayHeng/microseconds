/*
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file microseconds_cortexm_systick.c
 * @brief Microseconds systick timer driver source file
 *
 * Notes: The driver configure systick as lifetime timer
 */
#include "microseconds.h"
#include "assert.h"
#include "fsl_device_registers.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

   
/*******************************************************************************
 * Variables
 ******************************************************************************/
//!< 高位计数器，仅当使能定时器超时中断时有效，用于记录中断累计次数
extern volatile uint32_t s_highCounter;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/


/*******************************************************************************
 * Code
 ******************************************************************************/

void microseconds_timer_init(void)
{
    // 调用 core_cmx.h 头文件里的初始化函数
    // SysTick时钟源为内核时钟，开启中断，重装值为 0xFFFFFF
    SysTick_Config(SysTick_LOAD_RELOAD_Msk + 1);
}

void microseconds_timer_deinit(void)
{
    SysTick->CTRL &= ~(SysTick_CTRL_CLKSOURCE_Msk |
                       SysTick_CTRL_TICKINT_Msk |
                       SysTick_CTRL_ENABLE_Msk);
    SysTick->VAL = 0;
}

uint32_t microseconds_get_clock(void)
{
    return SystemCoreClock;
}

//! @brief Read back the running tick count
uint64_t microseconds_get_ticks(void)
{
    uint32_t high;
    uint32_t low;
    // 这里的实现要注意确保中断发生时获取系统累计计数值的正确性
    do
    {
        // 先缓存高位计数器
        high = s_highCounter;
        // 再读定时器实际计数值
        low = ~SysTick->VAL & SysTick_LOAD_RELOAD_Msk;
    } while (high != s_highCounter); // 保证缓存高位值与读实际低位值间隙中没有发生中断


    return ((uint64_t)high << 24) + low;
}

//! @brief Interrupt handler for the SysTick timer, this will just increment
//!        the rollover counter for extended time keeping
void SysTick_Handler(void)
{
    s_highCounter++;
}
