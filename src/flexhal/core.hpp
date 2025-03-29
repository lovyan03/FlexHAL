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

// 各レイヤーの名前空間定義
namespace platform {
    /**
     * @brief プラットフォームレイヤーの初期化
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    bool init();

    /**
     * @brief プラットフォームレイヤーの終了処理
     */
    void end();

    namespace esp32 {
        /**
         * @brief ESP32プラットフォーム固有の初期化
         * @return true 初期化成功
         * @return false 初期化失敗
         */
        bool initImpl();

        /**
         * @brief ESP32プラットフォーム固有の終了処理
         */
        void endImpl();
    }

    namespace desktop {
        /**
         * @brief デスクトッププラットフォーム固有の初期化
         * @return true 初期化成功
         * @return false 初期化失敗
         */
        bool initImpl();

        /**
         * @brief デスクトッププラットフォーム固有の終了処理
         */
        void endImpl();
    }
}

namespace framework {
    /**
     * @brief フレームワークレイヤーの初期化
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    bool init();

    /**
     * @brief フレームワークレイヤーの終了処理
     */
    void end();
}

namespace rtos {
    /**
     * @brief RTOSレイヤーの初期化
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    bool init();

    /**
     * @brief RTOSレイヤーの終了処理
     */
    void end();
}

/**
 * @brief FlexHALライブラリのバージョン情報
 */
struct Version {
    enum { major = FLEXHAL_VERSION_MAJOR, minor = FLEXHAL_VERSION_MINOR, patch = FLEXHAL_VERSION_PATCH };
    static const char* getString()
    {
        return FLEXHAL_VERSION_STRING;
    }
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
