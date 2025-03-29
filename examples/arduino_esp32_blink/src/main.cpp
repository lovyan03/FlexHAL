/**
 * @file main.cpp
 * @brief FlexHAL - Arduino ESP32向けGPIOサンプル（LED点滅）
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

// プラットフォーム依存ヘッダー（Arduino環境の場合のみ）
#if defined(ARDUINO)
#include <Arduino.h>
#endif

// FlexHALライブラリをインクルード
#include <FlexHAL.hpp>

// LEDのピン番号
constexpr int LED_PIN = 13;  // ESP32のLEDピン（多くのボードでは内蔵LED）

// セットアップ関数
void setup()
{
#if defined(ARDUINO)
    // Arduino環境の場合のみシリアル通信初期化
    Serial.begin(115200);
    delay(1000);  // ESP32の起動を待つ
#endif

    // FlexHALを初期化
    if (!flexhal::init()) {
        flexhal::fatal("FlexHAL initialization failed!");
        return;
    }

    // ログ出力
    flexhal::info("FlexHAL GPIO Blink Example");
    char version_info[64];
    snprintf(version_info, sizeof(version_info), "Version: %s", flexhal::Version::getString());
    flexhal::info(version_info);

    // GPIOポートを取得
    auto gpio = flexhal::getDefaultGPIOPort();
    if (!gpio) {
        flexhal::error("Failed to get GPIO port!");
        flexhal::end();
        return;
    }

    // LEDピンを取得して出力モードに設定
    auto led_pin = gpio->getPin(LED_PIN);
    if (!led_pin) {
        flexhal::error("Failed to get LED pin!");
        flexhal::end();
        return;
    }
    led_pin->setMode(flexhal::PinMode::Output);
    flexhal::info("LED pin set to OUTPUT mode");

    // LEDを点滅させる
    flexhal::info("Blinking LED... Check the board's LED");
}

// LEDの状態
bool led_state                 = false;
unsigned long last_toggle_time = 0;

// ループ関数
void loop()
{
    // 現在時刻を取得
    // ArduinoのmillisではなくFlexHALのmillisを使用
    unsigned long current_time = flexhal::millis();

    // 500ミリ秒経過したらLEDの状態を切り替え
    if (current_time - last_toggle_time >= 500) {
        // LEDの状態を切り替え
        led_state = !led_state;

        // GPIOポートとピンを取得
        auto gpio    = flexhal::getDefaultGPIOPort();
        auto led_pin = gpio->getPin(LED_PIN);

        // LEDの状態を設定
        led_pin->setLevel(led_state ? flexhal::PinLevel::High : flexhal::PinLevel::Low);

        // 状態をロガーで表示
        char status_msg[32];
        snprintf(status_msg, sizeof(status_msg), "LED: %s", led_state ? "ON" : "OFF");
        flexhal::info(status_msg);

        // 次の切り替え時間を設定
        last_toggle_time = current_time;
    }

    // FlexHALの更新処理
    flexhal::update();

#if defined(ARDUINO)
    // Arduino環境の場合のみ待機処理を行う
    delay(10);  // ESP32のWDTをリセット
#endif
}
