/**
 * @file gpio_arduino.hpp
 * @brief FlexHAL - ESP32向けArduino実装のGPIOヘッダー
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "../../frameworks/arduino/gpio.hpp"
#include <Arduino.h>

namespace flexhal {
namespace platform {
namespace esp32 {

/**
 * @brief ESP32のArduino実装ピンクラス
 */
class ESP32ArduinoPin : public framework::arduino::ArduinoPin {
public:
    /**
     * @brief コンストラクタ
     *
     * @param pin_number ピン番号
     */
    ESP32ArduinoPin(int pin_number);

    /**
     * @brief デストラクタ
     */
    virtual ~ESP32ArduinoPin() = default;

    /**
     * @brief アナログ値を設定（PWM）
     * ESP32固有のLEDCを使用
     *
     * @param value 0-255の値
     */
    void setAnalogValue(uint8_t value) override;

    /**
     * @brief アナログ値を取得（ADC）
     * ESP32固有のADC実装
     *
     * @return uint16_t 0-4095の値
     */
    uint16_t getAnalogValue() const override;
};

/**
 * @brief ESP32のArduino実装GPIOポート
 */
class ESP32ArduinoGPIOPort : public framework::arduino::ArduinoGPIOPort {
public:
    /**
     * @brief コンストラクタ
     */
    ESP32ArduinoGPIOPort();

    /**
     * @brief デストラクタ
     */
    virtual ~ESP32ArduinoGPIOPort() = default;

protected:
    /**
     * @brief ESP32固有のピンを作成
     * 
     * @param pin_number ピン番号
     * @return std::shared_ptr<IPin> ピンインスタンス
     */
    std::shared_ptr<IPin> createPin(int pin_number) override;
};

} // namespace esp32
} // namespace platform
} // namespace flexhal
