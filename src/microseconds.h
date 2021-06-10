/*
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*
 * @file microseconds.h
 * @brief Microseconds timer driver
 */

#ifndef ___MICROSECONDS_H__
#define ___MICROSECONDS_H__

#include <stdint.h>
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * API
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif // __cplusplus

/********************************************************************/
//! @brief Initialize timer facilities.
//! @brief 初始化计时
void microseconds_init(void);

//! @brief Shutdown the microsecond timer
//! @brief 关闭计时
void microseconds_shutdown(void);

//! @brief Gets the clock value used for microseconds driver
//! @brief 获取定时器时钟源数值
uint32_t microseconds_get_clock(void);

//! @brief Read back the running tick count
//! @brief 获取系统累计计数值
uint64_t microseconds_get_ticks(void);

//! @brief Returns the conversion of ticks to actual microseconds
//! @brief 将计数值转换为时间值(微秒)
uint32_t microseconds_convert_to_microseconds(uint64_t ticks);

//! @brief Returns the conversion of microseconds to ticks
//! @brief 将时间值(微秒)转换为计数值
uint64_t microseconds_convert_to_ticks(uint32_t microseconds);

//! @brief Delay specified time
//! @brief 阻塞型延时(微秒级)
void microseconds_delay(uint32_t us);

//! @brief Set delay time
//! @brief 设置超时时间（用于非阻塞型延时）
void microseconds_set_delay(uint32_t us);

//! @brief Get timeout flag
//! @brief 判断是否超时（用于非阻塞型延时）
bool microseconds_is_timeout(void);

#if defined(__cplusplus)
}
#endif // __cplusplus

#endif /* ___MICROSECONDS_H__ */

