#pragma once

#include <cstdint>

namespace flexhal {

class GPIO {
public:
    // シングルトンインスタンスの取得
    static GPIO& getInstance()
    {
        static GPIO instance;
        return instance;
    }

    // ピンモードの設定
    virtual void pinMode(uint8_t pin, uint8_t mode) = 0;

    // デジタル出力
    virtual void digitalWrite(uint8_t pin, uint8_t val) = 0;

    // デジタル入力
    virtual int digitalRead(uint8_t pin) = 0;

    // アナログ入力
    virtual uint16_t analogRead(uint8_t pin) = 0;

    // PWM出力
    virtual void analogWrite(uint8_t pin, uint16_t val) = 0;

    // 定数定義
    static constexpr uint8_t INPUT          = 0x0;
    static constexpr uint8_t OUTPUT         = 0x1;
    static constexpr uint8_t INPUT_PULLUP   = 0x2;
    static constexpr uint8_t INPUT_PULLDOWN = 0x3;

    static constexpr uint8_t LOW  = 0x0;
    static constexpr uint8_t HIGH = 0x1;

protected:
    GPIO()          = default;
    virtual ~GPIO() = default;

    // コピー禁止
    GPIO(const GPIO&)            = delete;
    GPIO& operator=(const GPIO&) = delete;
};

}  // namespace flexhal
