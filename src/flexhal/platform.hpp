/**
 * @file platform.hpp
 * @brief FlexHAL - プラットフォームレイヤーのヘッダ
 * @version 0.1.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FLEXHAL_PLATFORM_HPP
#define FLEXHAL_PLATFORM_HPP

namespace flexhal {
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

/**
 * @brief プラットフォームレイヤーの更新処理
 * @return true 継続
 * @return false 終了要求
 */
bool update();

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
}  // namespace esp32

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
}  // namespace desktop

}  // namespace platform
}  // namespace flexhal

#endif  // FLEXHAL_PLATFORM_HPP
