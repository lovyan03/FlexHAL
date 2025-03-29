/**
 * @file platform.cpp
 * @brief FlexHAL - プラットフォームレイヤーの実装
 * @version 0.1.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "core.hpp"
#include "logger.hpp"

#ifdef FLEXHAL_PLATFORM_ESP32
#include "../../impl/platforms/esp32/core.hpp"
#elif defined(FLEXHAL_PLATFORM_DESKTOP)
#include "../../impl/platforms/desktop/core.hpp"
#endif

namespace flexhal {
namespace platform {

// プラットフォームレイヤーの初期化
bool init()
{
#ifdef FLEXHAL_PLATFORM_ESP32
    return esp32::initImpl();
#elif defined(FLEXHAL_PLATFORM_DESKTOP)
    return desktop::initImpl();
#else
    error("Unsupported platform");
    return false;
#endif
}

// プラットフォームレイヤーの終了処理
void end()
{
#ifdef FLEXHAL_PLATFORM_ESP32
    esp32::endImpl();
#elif defined(FLEXHAL_PLATFORM_DESKTOP)
    desktop::endImpl();
#endif
}

}  // namespace platform
}  // namespace flexhal
