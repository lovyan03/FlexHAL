/**
 * @file main.cpp
 * @brief FlexHAL - GPIOサンプル（LED点滅）
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

// 基本ヘッダー
#if defined(ARDUINO) || defined(ESP_PLATFORM) || defined(ESP32) || defined(ESP8266)
#include <Arduino.h>
#endif

// FlexHALヘッダー
#if __has_include("FlexHAL.hpp")
#include "FlexHAL.hpp"
#else
#include "../../src/FlexHAL.hpp"
#endif

// ネイティブ環境用ヘッダー
#if !defined(ARDUINO) && !defined(ESP_PLATFORM) && !defined(ESP32) && !defined(ESP8266)
#include <iostream>
#include <chrono>
#include <thread>
#endif

// ロガー関連のヘルパー関数
inline void logVersion()
{
    char version_info[64];
    snprintf(version_info, sizeof(version_info), "Version: %s", flexhal::Version::getString());
    flexhal::info(version_info);
}

// LEDのピン番号
constexpr int LED_PIN = 13;  // Arduino互換のLEDピン

// グローバル変数
std::shared_ptr<flexhal::IGPIOPort> gpio = nullptr;
std::shared_ptr<flexhal::IPin> led_pin;
bool led_state            = false;
uint32_t last_toggle_time = 0;

/**
 * @brief FlexHALの初期化とLEDピンのセットアップを行う
 * @return 初期化が成功したかどうか
 */
bool setupFlexHAL()
{
#if defined(ARDUINO)
    // Arduino環境ではSerialを初期化
    Serial.begin(115200);
#endif

    // アプリケーション情報を表示
    flexhal::info("FlexHAL GPIO Blink Example");
    logVersion();

    // FlexHALを初期化
    if (!flexhal::init()) {
        flexhal::error("FlexHAL initialization failed!");
        return false;
    }

    // GPIOポートを取得
    gpio = flexhal::getDefaultGPIOPort();
    if (!gpio) {
        flexhal::error("Failed to get GPIO port!");
        flexhal::end();
        return false;
    }

    // LEDピンを取得して出力モードに設定
    led_pin = gpio->getPin(LED_PIN);
    if (!led_pin) {
        flexhal::error("Failed to get LED pin!");
        flexhal::end();
        return false;
    }

    // LEDピンを出力モードに設定
    led_pin->setMode(flexhal::PinMode::Output);
    flexhal::info("LED pin set to OUTPUT mode");
    flexhal::info("Blinking LED...");

    // 初期時間を設定
    last_toggle_time = flexhal::millis();
    return true;
}

/**
 * @brief LEDを点滅させる処理を1回実行
 * @return 処理が正常に続行できるかどうか
 */
bool updateLED()
{
    if (!gpio || !led_pin) return false;

    // 現在時刻を取得
    uint32_t current_time = flexhal::millis();
    uint32_t elapsed      = current_time - last_toggle_time;

    // 500ミリ秒経過したらLEDの状態を切り替え
    if (elapsed >= 500) {
        // LEDの状態を切り替え
        led_state = !led_state;
        led_pin->setLevel(led_state ? flexhal::PinLevel::High : flexhal::PinLevel::Low);

        // 状態を表示
        char status_msg[32];
        snprintf(status_msg, sizeof(status_msg), "LED: %s", led_state ? "ON" : "OFF");
        flexhal::info(status_msg);

        // 次の切り替え時間を設定
        last_toggle_time = current_time;
    }

    // FlexHALの更新処理
    if (!flexhal::update()) {
        return false;
    }

    // 少し待機
    flexhal::sleep(10);
    return true;
}

#if defined(ARDUINO) || defined(ESP_PLATFORM) || defined(ESP32) || defined(ESP8266)
// Arduino環境用の関数

void setup()
{
    setupFlexHAL();
}

void loop()
{
    updateLED();
}

#else
// ネイティブ環境用のmain関数

int main()
{
    // FlexHALの初期化
    if (!setupFlexHAL()) {
        return 1;
    }

    flexhal::info("Press Ctrl+C to exit");

    // メインループ
    try {
        bool running = true;
        while (running) {
            running = updateLED();
        }
    } catch (const std::exception& e) {
        char error_msg[128];
        snprintf(error_msg, sizeof(error_msg), "Error: %s", e.what());
        flexhal::error(error_msg);
    }

    // FlexHALの終了処理
    flexhal::end();
    return 0;
}
#endif
