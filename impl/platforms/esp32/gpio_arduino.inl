/**
 * @file gpio_arduino.inl
 * @brief FlexHAL - ESP32向けArduino実装のGPIO実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "gpio_arduino.hpp"
#include <Arduino.h>

#if defined(ESP32)
// ESP32固有のヘッダー
#include "esp32-hal-ledc.h"
#endif

namespace flexhal {
namespace platform {
namespace esp32 {

// ESP32ArduinoPin実装

ESP32ArduinoPin::ESP32ArduinoPin(int pin_number)
    : framework::arduino::ArduinoPin(pin_number)
{
    // 初期化時は何もしない
}

void ESP32ArduinoPin::setAnalogValue(uint8_t value)
{
    // ESP32のPWM出力（0-255の値を0-255に変換）
    // 標準的なArduinoのanalogWrite関数を使用
    // ESP32のArduinoフレームワークでは、内部で適切なPWM出力に変換される
    analogWrite(pin_number_, value);
}

uint16_t ESP32ArduinoPin::getAnalogValue() const
{
    // ESP32のADC入力（0-4095の値を返す）
    return analogRead(pin_number_);
}

// ESP32ArduinoGPIOPort実装

ESP32ArduinoGPIOPort::ESP32ArduinoGPIOPort()
    : framework::arduino::ArduinoGPIOPort()
{
    // 初期化時は何もしない
}

std::shared_ptr<IPin> ESP32ArduinoGPIOPort::createPin(int pin_number)
{
    // ESP32固有のArduinoピンを作成
    return std::make_shared<ESP32ArduinoPin>(pin_number);
}

} // namespace esp32
} // namespace platform
} // namespace flexhal
