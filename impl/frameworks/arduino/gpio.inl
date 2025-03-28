/**
 * @file gpio.inl
 * @brief FlexHAL - Arduino実装のGPIO実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "gpio.hpp"
#include <Arduino.h>

namespace flexhal {
namespace framework {
namespace arduino {

// ArduinoPin実装

ArduinoPin::ArduinoPin(int pin_number)
    : pin_number_(pin_number), current_mode_(PinMode::Undefined)
{
    // 初期化時は何もしない
}

void ArduinoPin::setMode(PinMode mode)
{
    uint8_t arduino_mode;

    // FlexHALのピンモードをArduinoのピンモードに変換
    switch (mode) {
        case PinMode::Input:
            arduino_mode = INPUT;
            break;
        case PinMode::Output:
            arduino_mode = OUTPUT;
            break;
        case PinMode::InputPullUp:
            arduino_mode = INPUT_PULLUP;
            break;
#if defined(INPUT_PULLDOWN)
        case PinMode::InputPullDown:
            arduino_mode = INPUT_PULLDOWN;
            break;
#endif
#if defined(OUTPUT_OPEN_DRAIN)
        case PinMode::OpenDrain:
            arduino_mode = OUTPUT_OPEN_DRAIN;
            break;
#endif
        case PinMode::Analog:
            // 一般的なArduinoのADCモード
            arduino_mode = INPUT;
            break;
        default:
            return;  // 未定義のモードは無視
    }

    // Arduinoの関数でピンモードを設定
    pinMode(pin_number_, arduino_mode);
    current_mode_ = mode;
}

void ArduinoPin::setLevel(PinLevel level)
{
    // Arduinoの関数でピンレベルを設定
    digitalWrite(pin_number_, level == PinLevel::High ? HIGH : LOW);
}

PinLevel ArduinoPin::getLevel() const
{
    // Arduinoの関数でピンレベルを取得
    return digitalRead(pin_number_) == HIGH ? PinLevel::High : PinLevel::Low;
}

void ArduinoPin::setAnalogValue(uint8_t value)
{
    // 標準的なArduinoのPWM出力
    analogWrite(pin_number_, value);
}

uint16_t ArduinoPin::getAnalogValue() const
{
    // 標準的なArduinoのADC入力
    return analogRead(pin_number_);
}

// ArduinoGPIOPort実装

ArduinoGPIOPort::ArduinoGPIOPort()
    : initialized_(false)
{
}

bool ArduinoGPIOPort::begin()
{
    initialized_ = true;
    return true;
}

void ArduinoGPIOPort::end()
{
    pins_.clear();
    initialized_ = false;
}

bool ArduinoGPIOPort::isReady() const
{
    return initialized_;
}

std::shared_ptr<IPin> ArduinoGPIOPort::getPin(int pin_number, GPIOImplementation impl)
{
    // 既に作成済みのピンがあれば返す
    auto it = pins_.find(pin_number);
    if (it != pins_.end()) {
        return it->second;
    }

    // 新しいピンを作成
    auto pin = createPin(pin_number);
    if (pin) {
        pins_[pin_number] = pin;
    }
    
    return pin;
}

std::shared_ptr<IPin> ArduinoGPIOPort::createPin(int pin_number)
{
    // 基本的なArduinoピンを作成
    return std::make_shared<ArduinoPin>(pin_number);
}

void ArduinoGPIOPort::setLevels(uint32_t values, uint32_t mask)
{
    // マスクされたピンの値を設定
    for (int i = 0; i < 32; ++i) {
        if ((mask >> i) & 0x01) {
            auto pin = getPin(i);
            if (pin) {
                pin->setLevel(((values >> i) & 0x01) ? PinLevel::High : PinLevel::Low);
            }
        }
    }
}

uint32_t ArduinoGPIOPort::getLevels() const
{
    uint32_t result = 0;

    // 各ピンのレベルを取得してビットマップに変換
    for (const auto& pair : pins_) {
        int pin_number = pair.first;
        if (pin_number < 32) {
            if (pair.second->getLevel() == PinLevel::High) {
                result |= (1 << pin_number);
            }
        }
    }

    return result;
}

} // namespace arduino
} // namespace framework
} // namespace flexhal
