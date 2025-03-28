/**
 * @file gpio.inl
 * @brief FlexHAL - ESP32向けESP-IDF実装のGPIO実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "gpio.hpp"
#include <Arduino.h>
#include <driver/gpio.h>
#include <driver/ledc.h>

namespace flexhal {
namespace framework {
namespace espidf {
namespace esp32 {

// ESP32IDFPin実装

ESP32IDFPin::ESP32IDFPin(int pin_number)
    : pin_number_(pin_number), current_mode_(PinMode::Undefined), adc_channel_(-1), ledc_channel_(-1)
{
    // 初期化時は何もしない
}

void ESP32IDFPin::setMode(PinMode mode)
{
    // ESP-IDFのAPIを使用してピンモードを設定
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << pin_number_);
    
    switch (mode) {
        case PinMode::Input:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.intr_type = GPIO_INTR_DISABLE;
            gpio_config(&io_conf);
            break;
            
        case PinMode::Output:
            io_conf.mode = GPIO_MODE_OUTPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.intr_type = GPIO_INTR_DISABLE;
            gpio_config(&io_conf);
            break;
            
        case PinMode::InputPullUp:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.intr_type = GPIO_INTR_DISABLE;
            gpio_config(&io_conf);
            break;
            
        case PinMode::InputPullDown:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
            io_conf.intr_type = GPIO_INTR_DISABLE;
            gpio_config(&io_conf);
            break;
            
        case PinMode::OpenDrain:
            io_conf.mode = GPIO_MODE_OUTPUT_OD;
            io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
            io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
            io_conf.intr_type = GPIO_INTR_DISABLE;
            gpio_config(&io_conf);
            break;
            
        case PinMode::Analog:
            // ADCモードの設定
            // ESP-IDFのADC APIは非推奨になっているため、Arduino APIを使用
            // ピンをアナログ入力モードに設定
            pinMode(pin_number_, INPUT);
            
            // ADCチャンネルを記録
            if (pin_number_ >= 32 && pin_number_ <= 39) {
                adc_channel_ = pin_number_ - 32;
            } else {
                adc_channel_ = -1;
            }
            break;
            
        default:
            return;  // 未定義のモードは無視
    }
    
    current_mode_ = mode;
}

void ESP32IDFPin::setLevel(PinLevel level)
{
    // ESP-IDFのAPIを使用してピンレベルを設定
    gpio_set_level((gpio_num_t)pin_number_, level == PinLevel::High ? 1 : 0);
}

PinLevel ESP32IDFPin::getLevel() const
{
    // ESP-IDFのAPIを使用してピンレベルを取得
    return gpio_get_level((gpio_num_t)pin_number_) ? PinLevel::High : PinLevel::Low;
}

void ESP32IDFPin::setAnalogValue(uint8_t value)
{
    // ESP-IDFのAPIを使用してPWM出力を設定
    // 初回呼び出し時にLEDCチャンネルを設定
    if (ledc_channel_ == -1) {
        // 簡略化のため、ピン番号をチャンネル番号として使用
        ledc_channel_ = pin_number_ % 8;  // LEDCは8チャンネルまでサポート
        
        ledc_timer_config_t ledc_timer = {
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .duty_resolution = LEDC_TIMER_8_BIT,
            .timer_num = LEDC_TIMER_0,
            .freq_hz = 5000,
            .clk_cfg = LEDC_AUTO_CLK
        };
        ledc_timer_config(&ledc_timer);
        
        ledc_channel_config_t ledc_channel = {
            .gpio_num = pin_number_,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .channel = (ledc_channel_t)ledc_channel_,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER_0,
            .duty = 0,
            .hpoint = 0
        };
        ledc_channel_config(&ledc_channel);
    }
    
    // PWMデューティ比を設定
    ledc_set_duty(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)ledc_channel_, value);
    ledc_update_duty(LEDC_HIGH_SPEED_MODE, (ledc_channel_t)ledc_channel_);
}

uint16_t ESP32IDFPin::getAnalogValue() const
{
    // ESP-IDFのADC APIは非推奨になっているため、Arduino APIを使用
    if (adc_channel_ >= 0) {
        return analogRead(pin_number_);
    }
    return 0;
}

} // namespace esp32
} // namespace espidf
} // namespace framework
} // namespace flexhal
