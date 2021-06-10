/*
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file microseconds_common.c
 * @brief Microseconds common timer driver source file
 */
#include "microseconds.h"
#include "assert.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/


/*******************************************************************************
 * Variables
 ******************************************************************************/
//!< 每微秒等效计数值
static uint32_t s_tickPerMicrosecond;
//!< 超时时间点对应系统计数值（用于非阻塞型延时）
static uint64_t s_timeoutTicks;
//!< 高位计数器，仅当使能定时器超时中断时有效，用于记录中断累计次数
volatile uint32_t s_highCounter;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//! @brief 打开硬件定时器
extern void microseconds_timer_init(void);
//! @brief 关闭硬件定时器
extern void microseconds_timer_deinit(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

//! @brief Initialize and start the timer facilities using the SysTick.
void microseconds_init(void)
{
    // 清零高位计数器
    s_highCounter = 0;
    // 打开硬件定时器
    microseconds_timer_init();
    // 计算每微秒的等效计数值
    s_tickPerMicrosecond = microseconds_get_clock() / 1000000UL;
    // 假设定时器时钟源不小于 1MHz
    assert(s_tickPerMicrosecond);
}

//! @brief Shutdown the microsecond timer
void microseconds_shutdown(void)
{
    // 关闭硬件定时器
    microseconds_timer_deinit();
}

//! @brief Returns the conversion of ticks to actual microseconds
//!        This is used to seperate any calculations from getting a timer
//!        value for timing sensitive scenarios
uint32_t microseconds_convert_to_microseconds(uint64_t ticks)
{
    return (uint32_t)(ticks / s_tickPerMicrosecond);
}

//! @brief Returns the conversion of microseconds to ticks
uint64_t microseconds_convert_to_ticks(uint32_t microseconds)
{
    return ((uint64_t)microseconds * s_tickPerMicrosecond);
}

//! @brief Delay specified time
//! @param us Delay time in microseconds unit
void microseconds_delay(uint32_t us)
{
    // 获取系统当前计数值
    uint64_t currentTicks = microseconds_get_ticks();
    // 计算超时时间点系统计数值
    uint64_t ticksNeeded = ((uint64_t)us * s_tickPerMicrosecond) + currentTicks;
    // 等待系统计数值到达超时时间点系统计数值
    while (microseconds_get_ticks() < ticksNeeded);
}

//! @brief Set delay time
//! @param us Delay time in microseconds unit
void microseconds_set_delay(uint32_t us)
{
    // 计算超时时间等效计数值
    uint64_t ticks = microseconds_convert_to_ticks(us);
    // 设置超时时间点系统计数值
    s_timeoutTicks = microseconds_get_ticks() + ticks;
}

//! @brief Get timeout flag
//! @retval true Timeout is reached.
//! @retval false Timeout isn't reached.
bool microseconds_is_timeout(void)
{
    // 获取系统当前计数值
    uint64_t currentTicks = microseconds_get_ticks();
    // 判断系统计数值是否大于超时时间点系统计数值
    return (currentTicks < s_timeoutTicks) ? false : true;
}
