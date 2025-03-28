/**
 * @file main.cpp
 * @brief FlexHAL - シンプルなテストサンプル
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <Arduino.h>

void setup()
{
    // シリアル通信初期化
    Serial.begin(115200);
    delay(1000);  // 起動を待つ

    Serial.println("FlexHAL Simple Test");
    Serial.println("This is a minimal example to test the build system");
}

void loop()
{
    // LEDを点滅させる（ほとんどのESP32ボードではピン2がLED）
    digitalWrite(2, HIGH);
    delay(500);
    digitalWrite(2, LOW);
    delay(500);

    Serial.println("LED blink");
}
