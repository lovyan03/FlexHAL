/**
 * @file gpio.hpp
 * @brief FlexHAL - ESP32向けESP-IDF実装のGPIOヘッダー
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "../../../internal/gpio.h"
#include <driver/gpio.h>
#include <driver/ledc.h>
#include <Arduino.h>  // ADC機能のためにArduino APIを使用

namespace flexhal {
namespace framework {
namespace espidf {
namespace esp32 {

/**
 * @brief ESP32のESP-IDF実装ピンクラス
 */
class ESP32IDFPin : public IPin {
public:
    /**
     * @brief コンストラクタ
     *
     * @param pin_number ピン番号
     */
    ESP32IDFPin(int pin_number);

    /**
     * @brief デストラクタ
     */
    virtual ~ESP32IDFPin() = default;

    /**
     * @brief ピン番号を取得
     *
     * @return int ピン番号
     */
    int getPinNumber() const override
    {
        return pin_number_;
    }

    /**
     * @brief ピンモードを設定
     *
     * @param mode ピンモード
     */
    void setMode(PinMode mode) override;

    /**
     * @brief ピンレベルを設定
     *
     * @param level ピンレベル
     */
    void setLevel(PinLevel level) override;

    /**
     * @brief ピンレベルを取得
     *
     * @return PinLevel 現在のピンレベル
     */
    PinLevel getLevel() const override;

    /**
     * @brief アナログ値を設定（PWM）
     *
     * @param value 0-255の値
     */
    void setAnalogValue(uint8_t value);

    /**
     * @brief アナログ値を取得（ADC）
     *
     * @return uint16_t 0-4095の値
     */
    uint16_t getAnalogValue() const;

private:
    int pin_number_;
    PinMode current_mode_;
    int ledc_channel_;
    int adc_channel_;
};

}  // namespace esp32
}  // namespace espidf
}  // namespace framework
}  // namespace flexhal
