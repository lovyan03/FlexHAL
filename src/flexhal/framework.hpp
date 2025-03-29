/**
 * @file framework.hpp
 * @brief FlexHAL - フレームワークレイヤーのヘッダ
 * @version 0.1.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FLEXHAL_FRAMEWORK_HPP
#define FLEXHAL_FRAMEWORK_HPP

namespace flexhal {
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

namespace arduino {
    /**
     * @brief Arduinoフレームワーク固有の初期化
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    bool initImpl();

    /**
     * @brief Arduinoフレームワーク固有の終了処理
     */
    void endImpl();
}

namespace espidf {
    /**
     * @brief ESP-IDFフレームワーク固有の初期化
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    bool initImpl();

    /**
     * @brief ESP-IDFフレームワーク固有の終了処理
     */
    void endImpl();
}

} // namespace framework
} // namespace flexhal

#endif // FLEXHAL_FRAMEWORK_HPP
