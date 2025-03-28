/**
 * @file core.hpp
 * @brief FlexHAL - ESP32向けコア機能のヘッダー
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <memory>
#include <Arduino.h>

namespace flexhal {
namespace platform {
namespace esp32 {

/**
 * @brief ESP32プラットフォームの管理クラス
 */
class ESP32Platform {
public:
    /**
     * @brief シングルトンインスタンスを取得
     * 
     * @return ESP32Platform& シングルトンインスタンス
     */
    static ESP32Platform& getInstance();
    
    /**
     * @brief デストラクタ
     */
    ~ESP32Platform();
    
    /**
     * @brief プラットフォームを初期化
     * 
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    bool init();
    
    /**
     * @brief プラットフォームを終了
     */
    void end();
    
    /**
     * @brief プラットフォームの更新処理
     * 
     * @return true 継続
     * @return false 終了要求
     */
    bool update();
    
private:
    /**
     * @brief コンストラクタ（シングルトン）
     */
    ESP32Platform();
    
    bool initialized_;
};

} // namespace esp32
} // namespace platform
} // namespace flexhal
