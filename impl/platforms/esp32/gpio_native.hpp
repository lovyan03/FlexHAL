/**
 * @file gpio_native.hpp
 * @brief FlexHAL - ESP32向けネイティブ実装のGPIOヘッダー
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "../../internal/gpio.h"
#include <esp_rom_gpio.h>
#include <soc/gpio_reg.h>
#include <soc/gpio_struct.h>

namespace flexhal {
namespace platform {
namespace esp32 {

/**
 * @brief ESP32のネイティブ実装ピンクラス
 */
class ESP32NativePin : public IPin {
public:
    /**
     * @brief コンストラクタ
     *
     * @param pin_number ピン番号
     */
    ESP32NativePin(int pin_number);

    /**
     * @brief デストラクタ
     */
    virtual ~ESP32NativePin() = default;

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
    uint32_t gpio_num_;
};

}  // namespace esp32
}  // namespace platform
}  // namespace flexhal
