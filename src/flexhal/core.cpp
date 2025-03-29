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
#include "platform.hpp"
#include "framework.hpp"
#include "rtos.hpp"

namespace flexhal {

// FlexHALライブラリの初期化状態
static bool s_initialized = false;

// FlexHALライブラリの初期化
bool init()
{
    if (s_initialized) {
        return true;  // 既に初期化済み
    }

    // デフォルトロガーの初期化
    initDefaultLogger();

    // プラットフォーム初期化
    if (!platform::init()) {
        error("Platform initialization failed");
        return false;
    }

    // フレームワーク初期化
    if (!framework::init()) {
        error("Framework initialization failed");
        platform::end();
        return false;
    }

    // RTOS初期化
    if (!rtos::init()) {
        error("RTOS initialization failed");
        framework::end();
        platform::end();
        return false;
    }

    // 初期化完了
    s_initialized = true;
    info("FlexHAL initialized");
    return true;
}

// FlexHALライブラリの終了処理
void end()
{
    if (!s_initialized) {
        return;  // 初期化されていない
    }

    // 逆順で終了処理
    rtos::end();
    framework::end();
    platform::end();

    // 終了処理
    info("FlexHAL terminated");
    s_initialized = false;
}

}  // namespace flexhal
