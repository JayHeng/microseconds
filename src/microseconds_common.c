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
//!< ÿ΢���Ч����ֵ
static uint32_t s_tickPerMicrosecond;
//!< ��ʱʱ����Ӧϵͳ����ֵ�����ڷ���������ʱ��
static uint64_t s_timeoutTicks;
//!< ��λ������������ʹ�ܶ�ʱ����ʱ�ж�ʱ��Ч�����ڼ�¼�ж��ۼƴ���
volatile uint32_t s_highCounter;

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
//! @brief ��Ӳ����ʱ��
extern void microseconds_timer_init(void);
//! @brief �ر�Ӳ����ʱ��
extern void microseconds_timer_deinit(void);

/*******************************************************************************
 * Code
 ******************************************************************************/

//! @brief Initialize and start the timer facilities using the SysTick.
void microseconds_init(void)
{
    // �����λ������
    s_highCounter = 0;
    // ��Ӳ����ʱ��
    microseconds_timer_init();
    // ����ÿ΢��ĵ�Ч����ֵ
    s_tickPerMicrosecond = microseconds_get_clock() / 1000000UL;
    // ���趨ʱ��ʱ��Դ��С�� 1MHz
    assert(s_tickPerMicrosecond);
}

//! @brief Shutdown the microsecond timer
void microseconds_shutdown(void)
{
    // �ر�Ӳ����ʱ��
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
    // ��ȡϵͳ��ǰ����ֵ
    uint64_t currentTicks = microseconds_get_ticks();
    // ���㳬ʱʱ���ϵͳ����ֵ
    uint64_t ticksNeeded = ((uint64_t)us * s_tickPerMicrosecond) + currentTicks;
    // �ȴ�ϵͳ����ֵ���ﳬʱʱ���ϵͳ����ֵ
    while (microseconds_get_ticks() < ticksNeeded);
}

//! @brief Set delay time
//! @param us Delay time in microseconds unit
void microseconds_set_delay(uint32_t us)
{
    // ���㳬ʱʱ���Ч����ֵ
    uint64_t ticks = microseconds_convert_to_ticks(us);
    // ���ó�ʱʱ���ϵͳ����ֵ
    s_timeoutTicks = microseconds_get_ticks() + ticks;
}

//! @brief Get timeout flag
//! @retval true Timeout is reached.
//! @retval false Timeout isn't reached.
bool microseconds_is_timeout(void)
{
    // ��ȡϵͳ��ǰ����ֵ
    uint64_t currentTicks = microseconds_get_ticks();
    // �ж�ϵͳ����ֵ�Ƿ���ڳ�ʱʱ���ϵͳ����ֵ
    return (currentTicks < s_timeoutTicks) ? false : true;
}
