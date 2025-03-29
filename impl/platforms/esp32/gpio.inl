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
#include "gpio_native.hpp"
#include "gpio_arduino.hpp"
#include <Arduino.h>

namespace flexhal {
namespace platform {
namespace esp32 {

// ESP32Pin実装

ESP32Pin::ESP32Pin(int pin_number, GPIOImplementation impl)
    : pin_number_(pin_number), current_mode_(PinMode::Undefined), impl_(impl)
{
    // 選択された実装方法に基づいてインスタンスを作成
    setImplementation(impl);
}

void ESP32Pin::setImplementation(GPIOImplementation impl)
{
    impl_ = impl;

    // 既存の実装を破棄
    pin_impl_.reset();

    // 新しい実装を作成
    switch (impl_) {
        case GPIOImplementation::Arduino:
            pin_impl_ = std::make_unique<ESP32ArduinoPin>(pin_number_);
            break;
        case GPIOImplementation::ESP_IDF:
            pin_impl_ = std::make_unique<framework::espidf::esp32::ESP32IDFPin>(pin_number_);
            break;
        case GPIOImplementation::Native:
            pin_impl_ = std::make_unique<ESP32NativePin>(pin_number_);
            break;
    }

    // 現在のモードを設定する（実装切り替え時にモードを維持するため）
    if (current_mode_ != PinMode::Undefined) {
        pin_impl_->setMode(current_mode_);
    }
}

void ESP32Pin::setMode(PinMode mode)
{
    // 現在のモードを保存
    current_mode_ = mode;

    // 実装クラスに処理を委託
    if (pin_impl_) {
        pin_impl_->setMode(mode);
    }
}

void ESP32Pin::setLevel(PinLevel level)
{
    // 実装クラスに処理を委託
    if (pin_impl_) {
        pin_impl_->setLevel(level);
    }
}

PinLevel ESP32Pin::getLevel() const
{
    // 実装クラスに処理を委託
    if (pin_impl_) {
        return pin_impl_->getLevel();
    }
    return PinLevel::Low;  // デフォルト値
}

void ESP32Pin::setAnalogValue(uint8_t value)
{
    // 実装クラスが存在する場合のみ処理
    if (pin_impl_) {
        // 実装クラスに応じて適切なメソッドを呼び出す
        switch (impl_) {
            case GPIOImplementation::Arduino:
                static_cast<ESP32ArduinoPin*>(pin_impl_.get())->setAnalogValue(value);
                break;
            case GPIOImplementation::ESP_IDF:
                static_cast<framework::espidf::esp32::ESP32IDFPin*>(pin_impl_.get())->setAnalogValue(value);
                break;
            case GPIOImplementation::Native:
                static_cast<ESP32NativePin*>(pin_impl_.get())->setAnalogValue(value);
                break;
        }
    }
}

uint16_t ESP32Pin::getAnalogValue() const
{
    // 実装クラスが存在する場合のみ処理
    if (pin_impl_) {
        // 実装クラスに応じて適切なメソッドを呼び出す
        switch (impl_) {
            case GPIOImplementation::Arduino:
                return static_cast<ESP32ArduinoPin*>(pin_impl_.get())->getAnalogValue();
            case GPIOImplementation::ESP_IDF:
                return static_cast<framework::espidf::esp32::ESP32IDFPin*>(pin_impl_.get())->getAnalogValue();
            case GPIOImplementation::Native:
                return static_cast<ESP32NativePin*>(pin_impl_.get())->getAnalogValue();
            default:
                break;
        }
    }
    return 0;  // デフォルト値
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

std::shared_ptr<IPin> ESP32GPIOPort::getPin(int pin_number, GPIOImplementation impl)
{
    // 範囲チェック
    if (pin_number < 0 || pin_number >= pin_count_) {
        return nullptr;
    }

    // ピンが未作成なら作成
    if (!pins_[pin_number]) {
        pins_[pin_number] = std::make_shared<ESP32Pin>(pin_number, impl);
    } else {
        // 既存のピンの実装方法を変更
        std::static_pointer_cast<ESP32Pin>(pins_[pin_number])->setImplementation(impl);
    }

    return pins_[pin_number];
}

void ESP32GPIOPort::setLevels(uint32_t values, uint32_t mask)
{
    // マスクされたビットに対応するピンの値を設定
    for (int i = 0; i < 32 && i < pin_count_; ++i) {
        if (mask & (1 << i)) {
            bool level = (values & (1 << i)) != 0;
            // 実装クラスを使用してピンレベルを設定
            auto pin = getPin(i);
            if (pin) {
                pin->setLevel(level ? PinLevel::High : PinLevel::Low);
            }
        }
    }
}

uint32_t ESP32GPIOPort::getLevels() const
{
    // 32ビット分のピンの値を取得
    uint32_t result = 0;
    for (int i = 0; i < 32 && i < pin_count_; ++i) {
        // 実装クラスを使用してピンレベルを取得
        if (pins_[i] && pins_[i]->getLevel() == PinLevel::High) {
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
