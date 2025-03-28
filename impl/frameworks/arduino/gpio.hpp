/**
 * @file gpio.hpp
 * @brief FlexHAL - Arduino実装のGPIOヘッダー
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "../../internal/gpio.h"
#include <Arduino.h>
#include <map>

namespace flexhal {
namespace framework {
namespace arduino {

/**
 * @brief Arduinoフレームワーク実装のピンクラス
 */
class ArduinoPin : public IPin {
public:
    /**
     * @brief コンストラクタ
     *
     * @param pin_number ピン番号
     */
    ArduinoPin(int pin_number);

    /**
     * @brief デストラクタ
     */
    virtual ~ArduinoPin() = default;

    /**
     * @brief ピン番号を取得
     *
     * @return int ピン番号
     */
    int getPinNumber() const override {
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
    virtual void setAnalogValue(uint8_t value);

    /**
     * @brief アナログ値を取得（ADC）
     *
     * @return uint16_t アナログ値
     */
    virtual uint16_t getAnalogValue() const;

protected:
    int pin_number_;
    PinMode current_mode_;
};

/**
 * @brief Arduinoフレームワーク用GPIOポート
 */
class ArduinoGPIOPort : public IGPIOPort {
public:
    /**
     * @brief コンストラクタ
     */
    ArduinoGPIOPort();

    /**
     * @brief デストラクタ
     */
    virtual ~ArduinoGPIOPort() = default;

    /**
     * @brief デバイスの初期化
     *
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    bool begin() override;

    /**
     * @brief デバイスの終了処理
     */
    void end() override;

    /**
     * @brief デバイスが準備完了しているか確認
     *
     * @return true 準備完了
     * @return false 準備未完了
     */
    bool isReady() const override;

    /**
     * @brief ピンを取得
     *
     * @param pin_number ピン番号
     * @param impl 使用するGPIO実装方法（デフォルトはArduino）
     * @return std::shared_ptr<IPin> ピンインスタンス
     */
    std::shared_ptr<IPin> getPin(int pin_number, GPIOImplementation impl = GPIOImplementation::Arduino) override;

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

protected:
    /**
     * @brief プラットフォーム固有のピンを作成
     * 
     * @param pin_number ピン番号
     * @return std::shared_ptr<IPin> ピンインスタンス
     */
    virtual std::shared_ptr<IPin> createPin(int pin_number);

    bool initialized_;
    std::map<int, std::shared_ptr<IPin>> pins_;
};

} // namespace arduino
} // namespace framework
} // namespace flexhal
