/**
 * @file framework.cpp
 * @brief FlexHAL - フレームワークレイヤーの実装
 * @version 0.1.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "core.hpp"
#include "logger.hpp"

namespace flexhal {
namespace framework {

// フレームワークレイヤーの初期化
bool init()
{
#ifdef FLEXHAL_FRAMEWORK_ARDUINO
    info("Arduino framework initialized");
    return true;
#elif defined(FLEXHAL_FRAMEWORK_ESPIDF)
    info("ESP-IDF framework initialized");
    return true;
#else
    info("No specific framework initialized");
    return true;
#endif
}

// フレームワークレイヤーの終了処理
void end()
{
#ifdef FLEXHAL_FRAMEWORK_ARDUINO
    info("Arduino framework terminated");
#elif defined(FLEXHAL_FRAMEWORK_ESPIDF)
    info("ESP-IDF framework terminated");
#else
    info("No specific framework terminated");
#endif
}

} // namespace framework
} // namespace flexhal
