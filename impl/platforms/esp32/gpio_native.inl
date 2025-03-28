/**
 * @file gpio_native.inl
 * @brief FlexHAL - ESP32向けネイティブ実装のGPIO実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "gpio_native.hpp"
#include <Arduino.h>

namespace flexhal {
namespace platform {
namespace esp32 {

// ESP32NativePin実装

ESP32NativePin::ESP32NativePin(int pin_number)
    : pin_number_(pin_number), current_mode_(PinMode::Undefined)
{
    // 初期化時は何もしない
}

void ESP32NativePin::setMode(PinMode mode)
{
    // ネイティブ実装では、直接レジスタアクセスを行いたいが、
    // ビルドの問題を避けるため、Arduino APIを使用する
    uint8_t arduino_mode;

    // FlexHALのピンモードをArduinoのピンモードに変換
    switch (mode) {
        case PinMode::Input: {
            arduino_mode = INPUT;
            break;
        }
            
        case PinMode::Output: {
            arduino_mode = OUTPUT;
            break;
        }
            
        case PinMode::InputPullUp: {
            arduino_mode = INPUT_PULLUP;
            break;
        }
            
        case PinMode::InputPullDown: {
            arduino_mode = INPUT_PULLDOWN;
            break;
        }
            
        case PinMode::OpenDrain: {
            arduino_mode = OUTPUT_OPEN_DRAIN;
            break;
        }
            
        case PinMode::Analog: {
            arduino_mode = INPUT;
            break;
        }
            
        default:
            return;  // 未定義のモードは無視
    }
    
    // Arduino APIを使用してピンモードを設定
    // ネイティブ実装では、将来的にはレジスタ直接アクセスを実装する
    pinMode(pin_number_, arduino_mode);
    current_mode_ = mode;
}

void ESP32NativePin::setLevel(PinLevel level)
{
    // ネイティブ実装では、直接レジスタアクセスを行いたいが、
    // ビルドの問題を避けるため、Arduino APIを使用する
    // 将来的には、以下のようなレジスタ直接アクセスを実装する予定
    // if (level == PinLevel::High) {
    //     GPIO.out_w1ts = (1ULL << pin_number_);  // ピンをHIGHに設定
    // } else {
    //     GPIO.out_w1tc = (1ULL << pin_number_);  // ピンをLOWに設定
    // }
    digitalWrite(pin_number_, level == PinLevel::High ? HIGH : LOW);
}

PinLevel ESP32NativePin::getLevel() const
{
    // ネイティブ実装では、直接レジスタアクセスを行いたいが、
    // ビルドの問題を避けるため、Arduino APIを使用する
    // 将来的には、以下のようなレジスタ直接アクセスを実装する予定
    // return ((GPIO.in >> pin_number_) & 0x1) ? PinLevel::High : PinLevel::Low;
    return digitalRead(pin_number_) == HIGH ? PinLevel::High : PinLevel::Low;
}

void ESP32NativePin::setAnalogValue(uint8_t value)
{
    // ネイティブ実装でのPWM出力
    // 将来的にはレジスタ直接アクセスを実装する
    ledcWrite(pin_number_, value);
}

uint16_t ESP32NativePin::getAnalogValue() const
{
    // ネイティブ実装でのADC入力
    // 将来的にはレジスタ直接アクセスを実装する
    return analogRead(pin_number_);
}

} // namespace esp32
} // namespace platform
} // namespace flexhal
