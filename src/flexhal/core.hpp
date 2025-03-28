/**
 * @file core.hpp
 * @brief FlexHAL - コア機能 (C++版)
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FLEXHAL_CORE_HPP
#define FLEXHAL_CORE_HPP

#include "../../impl/internal/version.h"
#include "../../impl/internal/core.h"
#include "../../impl/internal/device.h"

namespace flexhal {

/**
 * @brief FlexHALライブラリのバージョン情報
 */
struct Version {
    static constexpr int major          = FLEXHAL_VERSION_MAJOR;
    static constexpr int minor          = FLEXHAL_VERSION_MINOR;
    static constexpr int patch          = FLEXHAL_VERSION_PATCH;
    static constexpr const char* string = FLEXHAL_VERSION_STRING;
};

/**
 * @brief FlexHALライブラリの初期化
 *
 * @return true 初期化成功
 * @return false 初期化失敗
 */
bool init();

/**
 * @brief FlexHALライブラリの終了処理
 */
void end();

/**
 * @brief FlexHALライブラリの更新処理
 * メインスレッドから呼び出す必要があります
 *
 * @return true 継続
 * @return false 終了要求
 */
bool update();

}  // namespace flexhal

#endif  // FLEXHAL_CORE_HPP
