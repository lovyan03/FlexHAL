/**
 * @file gpio_esp_idf.inl
 * @brief FlexHAL - ESP32向けESP-IDF実装のGPIO実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "gpio_esp_idf.hpp"
#include <Arduino.h>
#include <driver/gpio.h>
#include <driver/ledc.h>

namespace flexhal {
namespace platform {
namespace esp32 {

// ESP32IDFPin実装

ESP32IDFPin::ESP32IDFPin(int pin_number)
    : pin_number_(pin_number), current_mode_(PinMode::Undefined)
{
    // 初期化時は何もしない
}

void ESP32IDFPin::setMode(PinMode mode)
{
    gpio_config_t io_conf = {};
    
    // FlexHALのピンモードをESP-IDFのピンモードに変換
    switch (mode) {
        case PinMode::Input:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
        case PinMode::Output:
            io_conf.mode = GPIO_MODE_OUTPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
        case PinMode::InputPullUp:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
        case PinMode::InputPullDown:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
            break;
        case PinMode::OpenDrain:
            io_conf.mode = GPIO_MODE_OUTPUT_OD;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            break;
        case PinMode::Analog:
            // ESP32のADCモード
            // ADCの設定はESP-IDFでは別途必要
            return;
        default:
            return;  // 未定義のモードは無視
    }
    
    // ピン番号を設定
    io_conf.pin_bit_mask = (1ULL << pin_number_);
    io_conf.intr_type = GPIO_INTR_DISABLE;
    
    // ESP-IDFの関数でピンモードを設定
    gpio_config(&io_conf);
    current_mode_ = mode;
}

void ESP32IDFPin::setLevel(PinLevel level)
{
    // ESP-IDFの関数でピンレベルを設定
    gpio_set_level((gpio_num_t)pin_number_, level == PinLevel::High ? 1 : 0);
}

PinLevel ESP32IDFPin::getLevel() const
{
    // ESP-IDFの関数でピンレベルを取得
    return gpio_get_level((gpio_num_t)pin_number_) == 1 ? PinLevel::High : PinLevel::Low;
}

void ESP32IDFPin::setAnalogValue(uint8_t value)
{
    // ESP-IDFのLEDCを使用してPWM出力
    static bool ledc_initialized = false;
    static const ledc_timer_t LEDC_TIMER_NUM = LEDC_TIMER_0;
    static const ledc_channel_t LEDC_CHANNEL_NUM = (ledc_channel_t)(pin_number_ % 8);  // チャンネルはピン番号から割り当て
    
    if (!ledc_initialized) {
        // LEDCタイマーの設定
        ledc_timer_config_t ledc_timer = {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .duty_resolution = LEDC_TIMER_8_BIT,
            .timer_num = LEDC_TIMER_NUM,
            .freq_hz = 5000,
            .clk_cfg = LEDC_AUTO_CLK
        };
        ledc_timer_config(&ledc_timer);
        ledc_initialized = true;
    }
    
    // LEDCチャンネルの設定
    ledc_channel_config_t ledc_channel = {
        .gpio_num = (gpio_num_t)pin_number_,
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .channel = LEDC_CHANNEL_NUM,
        .intr_type = LEDC_INTR_DISABLE,
        .timer_sel = LEDC_TIMER_NUM,
        .duty = value,
        .hpoint = 0
    };
    ledc_channel_config(&ledc_channel);
    
    // デューティ比の設定
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_NUM, value);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, LEDC_CHANNEL_NUM);
}

uint16_t ESP32IDFPin::getAnalogValue() const
{
    // ESP-IDFのADC機能ではなく、Arduino APIを使用してアナログ入力を読み取り
    // これにより、非推奨のADC APIの使用を避ける
    if (pin_number_ >= 32 && pin_number_ <= 39) {
        return analogRead(pin_number_);
    }
    
    return 0;
}

} // namespace esp32
} // namespace platform
} // namespace flexhal
