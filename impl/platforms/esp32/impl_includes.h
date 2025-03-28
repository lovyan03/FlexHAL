/**
 * @file impl_includes.h
 * @brief FlexHAL - ESP32プラットフォーム向け実装ファイルのインクルード
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

// フレームワーク実装ファイルをインクルード
#include "../../frameworks/arduino/gpio.inl"
#include "gpio_arduino.inl"
#include "../../frameworks/espidf/esp32/gpio.inl"

// ESP32プラットフォーム向け実装ファイルをインクルード
#include "gpio_native.inl"
#include "gpio.inl"
#include "core.inl"
#include "factory.inl"

// 将来的に追加される実装ファイルもここに追加
// #include "spi.cpp"
// #include "i2c.cpp"
// など
