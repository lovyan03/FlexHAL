/**
 * @file gpio.cpp
 * @brief FlexHAL - ESP32向けGPIO実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "gpio.hpp"
#include <Arduino.h>

namespace flexhal {
namespace platform {
namespace esp32 {

// ESP32Pin実装

ESP32Pin::ESP32Pin(int pin_number) : pin_number_(pin_number), current_mode_(PinMode::Undefined)
{
    // 初期化時は何もしない
}

void ESP32Pin::setMode(PinMode mode)
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
        case PinMode::InputPullDown:
            arduino_mode = INPUT_PULLDOWN;
            break;
        case PinMode::OpenDrain:
            arduino_mode = OUTPUT_OPEN_DRAIN;
            break;
        case PinMode::Analog:
            // ESP32のADCモード
            arduino_mode = INPUT;
            break;
        default:
            return;  // 未定義のモードは無視
    }

    // Arduinoの関数でピンモードを設定
    pinMode(pin_number_, arduino_mode);
    current_mode_ = mode;
}

void ESP32Pin::setLevel(PinLevel level)
{
    // Arduinoの関数でピンレベルを設定
    digitalWrite(pin_number_, level == PinLevel::High ? HIGH : LOW);
}

PinLevel ESP32Pin::getLevel() const
{
    // Arduinoの関数でピンレベルを取得
    return digitalRead(pin_number_) == HIGH ? PinLevel::High : PinLevel::Low;
}

void ESP32Pin::setAnalogValue(uint8_t value)
{
    // ESP32のPWM出力（0-255の値を0-255に変換）
    // ESP32はデフォルトで8ビット解像度
    ledcWrite(pin_number_, value);
}

uint16_t ESP32Pin::getAnalogValue() const
{
    // ESP32のADC入力（0-4095の値を返す）
    return analogRead(pin_number_);
}

// ESP32GPIOPort実装

ESP32GPIOPort::ESP32GPIOPort(int pin_count) : pin_count_(pin_count), initialized_(false)
{
    // ピンの配列を初期化
    pins_.reserve(pin_count);
    for (int i = 0; i < pin_count; ++i) {
        pins_.push_back(nullptr);
    }
}

std::shared_ptr<IPin> ESP32GPIOPort::getPin(int pin_number)
{
    // 範囲チェック
    if (pin_number < 0 || pin_number >= pin_count_) {
        return nullptr;
    }

    // ピンが未作成なら作成
    if (!pins_[pin_number]) {
        pins_[pin_number] = std::make_shared<ESP32Pin>(pin_number);
    }

    return pins_[pin_number];
}

void ESP32GPIOPort::setLevels(uint32_t values, uint32_t mask)
{
    // マスクされたビットに対応するピンの値を設定
    for (int i = 0; i < 32 && i < pin_count_; ++i) {
        if (mask & (1 << i)) {
            bool level = (values & (1 << i)) != 0;
            digitalWrite(i, level ? HIGH : LOW);
        }
    }
}

uint32_t ESP32GPIOPort::getLevels() const
{
    // 32ビット分のピンの値を取得
    uint32_t result = 0;
    for (int i = 0; i < 32 && i < pin_count_; ++i) {
        if (digitalRead(i) == HIGH) {
            result |= (1 << i);
        }
    }
    return result;
}

bool ESP32GPIOPort::begin()
{
    if (initialized_) {
        return true;  // 既に初期化済み
    }

    // ESP32のGPIOを初期化
    // Arduinoでは特に初期化は不要

    initialized_ = true;
    return true;
}

void ESP32GPIOPort::end()
{
    if (!initialized_) {
        return;  // 既に終了済み
    }

    // すべてのピンを入力モードに戻す
    for (int i = 0; i < pin_count_; ++i) {
        if (pins_[i]) {
            pins_[i]->setMode(PinMode::Input);
        }
    }

    initialized_ = false;
}

bool ESP32GPIOPort::isReady() const
{
    return initialized_;
}

}  // namespace esp32
}  // namespace platform
}  // namespace flexhal
