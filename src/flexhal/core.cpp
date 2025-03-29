/**
 * @file core.cpp
 * @brief FlexHAL - コア機能の実装
 * @version 0.1.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "core.hpp"
#include "logger.hpp"

namespace flexhal {

// FlexHALライブラリの初期化状態
static bool s_initialized = false;

// FlexHALライブラリの初期化
bool init() {
    if (s_initialized) {
        return true;  // 既に初期化済み
    }

    // デフォルトロガーの初期化
    initDefaultLogger();

    // 初期化完了
    s_initialized = true;
    info("FlexHAL initialized");
    return true;
}

// FlexHALライブラリの終了処理
void end() {
    if (!s_initialized) {
        return;  // 初期化されていない
    }

    // 終了処理
    info("FlexHAL terminated");
    s_initialized = false;
}

} // namespace flexhal
