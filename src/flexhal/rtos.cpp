/**
 * @file rtos.cpp
 * @brief FlexHAL - RTOSレイヤーの実装
 * @version 0.1.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "core.hpp"
#include "logger.hpp"

namespace flexhal {
namespace rtos {

// RTOSレイヤーの初期化
bool init()
{
#ifdef FLEXHAL_RTOS_FREERTOS
    info("FreeRTOS initialized");
    return true;
#elif defined(FLEXHAL_RTOS_SDL)
    info("SDL RTOS initialized");
    return true;
#else
    info("No specific RTOS initialized");
    return true;
#endif
}

// RTOSレイヤーの終了処理
void end()
{
#ifdef FLEXHAL_RTOS_FREERTOS
    info("FreeRTOS terminated");
#elif defined(FLEXHAL_RTOS_SDL)
    info("SDL RTOS terminated");
#else
    info("No specific RTOS terminated");
#endif
}

} // namespace rtos
} // namespace flexhal
