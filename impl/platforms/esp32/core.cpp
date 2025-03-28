/**
 * @file core.cpp
 * @brief FlexHAL - ESP32向けコア機能の実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "core.hpp"
#include <Arduino.h>

namespace flexhal {
namespace platform {
namespace esp32 {

// シングルトンインスタンス
ESP32Platform& ESP32Platform::getInstance()
{
    static ESP32Platform instance;
    return instance;
}

ESP32Platform::ESP32Platform() : initialized_(false)
{
    // コンストラクタでは何もしない
}

ESP32Platform::~ESP32Platform()
{
    end();
}

bool ESP32Platform::init()
{
    if (initialized_) {
        return true;  // 既に初期化済み
    }

    // ESP32の初期化
    // Arduinoフレームワークでは自動的に初期化されるので、
    // ここでは特に何もしない

    initialized_ = true;
    return true;
}

void ESP32Platform::end()
{
    if (!initialized_) {
        return;  // 既に終了済み
    }

    // ESP32の終了処理
    // 特に何もしない

    initialized_ = false;
}

bool ESP32Platform::update()
{
    // ESP32の更新処理
    // Arduinoフレームワークでは特に何もしない

    // 常に継続を返す
    return true;
}

}  // namespace esp32
}  // namespace platform
}  // namespace flexhal
