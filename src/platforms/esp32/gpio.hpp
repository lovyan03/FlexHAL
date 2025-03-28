/**
 * @file gpio.hpp
 * @brief FlexHAL - ESP32向けGPIO実装のヘッダー
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "../../internal/gpio.h"
#include "../../internal/device.h"
#include <memory>
#include <vector>
#include <Arduino.h>

namespace flexhal {
namespace platform {
namespace esp32 {

/**
 * @brief ESP32のピン実装クラス
 */
class ESP32Pin : public IPin {
public:
    /**
     * @brief コンストラクタ
     * 
     * @param pin_number ピン番号
     */
    ESP32Pin(int pin_number);
    
    /**
     * @brief デストラクタ
     */
    virtual ~ESP32Pin() = default;
    
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
    void setAnalogValue(uint8_t value) override;
    
    /**
     * @brief アナログ値を取得（ADC）
     * 
     * @return uint16_t 0-4095の値
     */
    uint16_t getAnalogValue() const override;
    
private:
    int pin_number_;
    PinMode current_mode_;
};

/**
 * @brief ESP32のGPIOポート実装クラス
 */
class ESP32GPIOPort : public IGPIOPort {
public:
    /**
     * @brief コンストラクタ
     * 
     * @param pin_count ピン数
     */
    ESP32GPIOPort(int pin_count = 40);
    
    /**
     * @brief デストラクタ
     */
    virtual ~ESP32GPIOPort() = default;
    
    /**
     * @brief ピンを取得
     * 
     * @param pin_number ピン番号
     * @return std::shared_ptr<IPin> ピンインスタンス
     */
    std::shared_ptr<IPin> getPin(int pin_number) override;
    
    /**
     * @brief 複数ピンのレベルを一度に設定
     * 
     * @param values 設定する値（ビットマップ）
     * @param mask 設定対象のピン（ビットマップ）
     */
    void setLevels(uint32_t values, uint32_t mask) override;
    
    /**
     * @brief 複数ピンのレベルを一度に取得
     * 
     * @return uint32_t 現在のレベル（ビットマップ）
     */
    uint32_t getLevels() const override;
    
    /**
     * @brief デバイスを初期化
     * 
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    bool begin() override;
    
    /**
     * @brief デバイスを終了
     */
    void end() override;
    
    /**
     * @brief デバイスが準備完了しているか
     * 
     * @return true 準備完了
     * @return false 準備未完了
     */
    bool isReady() const override;
    
private:
    int pin_count_;
    std::vector<std::shared_ptr<ESP32Pin>> pins_;
    bool initialized_;
};

} // namespace esp32
} // namespace platform
} // namespace flexhal
