/**
 * @file arduino_esp32_blink.ino
 * @brief FlexHAL - Arduino ESP32向けGPIOサンプル（LED点滅）
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

// FlexHALライブラリをインクルード
#include "../../src/FlexHAL.hpp"

// LEDのピン番号
constexpr int LED_PIN = 13;  // ESP32のLEDピン（多くのボードでは内蔵LED）

// セットアップ関数
void setup()
{
    // シリアル通信初期化
    Serial.begin(115200);
    delay(1000);  // ESP32の起動を待つ

    Serial.println("FlexHAL GPIO Blink Example for Arduino ESP32");
    Serial.print("Version: ");
    Serial.println(flexhal::Version::string);

    // FlexHALを初期化
    if (!flexhal::init()) {
        Serial.println("FlexHAL initialization failed!");
        return;
    }

    // GPIOポートを取得
    auto gpio = flexhal::getDefaultGPIOPort();
    if (!gpio) {
        Serial.println("Failed to get GPIO port!");
        flexhal::end();
        return;
    }

    // LEDピンを取得して出力モードに設定
    auto led_pin = gpio->getPin(LED_PIN);
    if (!led_pin) {
        Serial.println("Failed to get LED pin!");
        flexhal::end();
        return;
    }
    led_pin->setMode(flexhal::PinMode::Output);
    Serial.println("LED pin set to OUTPUT mode");

    // ESP32のPWMチャンネルを設定（FlexHALのsetAnalogValueを使う場合に必要）
    ledcSetup(LED_PIN, 5000, 8);      // チャンネル、周波数5kHz、8bit解像度
    ledcAttachPin(LED_PIN, LED_PIN);  // ピン、チャンネル

    // LEDを点滅させる
    Serial.println("Blinking LED... Check the board's LED");
}

// LEDの状態
bool led_state                 = false;
unsigned long last_toggle_time = 0;

// ループ関数
void loop()
{
    // 現在時刻を取得
    unsigned long current_time = millis();

    // 500ミリ秒経過したらLEDの状態を切り替え
    if (current_time - last_toggle_time >= 500) {
        // LEDの状態を切り替え
        led_state = !led_state;

        // GPIOポートとピンを取得
        auto gpio    = flexhal::getDefaultGPIOPort();
        auto led_pin = gpio->getPin(LED_PIN);

        // LEDの状態を設定
        led_pin->setLevel(led_state ? flexhal::PinLevel::High : flexhal::PinLevel::Low);

        // 状態を表示
        Serial.print("LED: ");
        Serial.println(led_state ? "ON" : "OFF");

        // 次の切り替え時間を設定
        last_toggle_time = current_time;
    }

    // FlexHALの更新処理
    flexhal::update();

    // 少し待機（ESP32のWDTをリセット）
    delay(10);
}
