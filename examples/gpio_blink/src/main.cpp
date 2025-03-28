/**
 * @file main.cpp
 * @brief FlexHAL - GPIOサンプル（LED点滅）
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#if __has_include("FlexHAL.hpp")
  // PlatformIOビルド用
  #include "FlexHAL.hpp"
#else
  // 直接ビルド用
  #include "../../src/FlexHAL.hpp"
#endif
#include <iostream>
#include <chrono>
#include <thread>

// LEDのピン番号
constexpr int LED_PIN = 13;  // Arduino互換のLEDピン

int main()
{
    std::cout << "FlexHAL GPIO Blink Example" << std::endl;
    std::cout << "Version: " << flexhal::Version::string << std::endl;

    // FlexHALを初期化
    if (!flexhal::init()) {
        std::cerr << "FlexHAL initialization failed!" << std::endl;
        return 1;
    }

    // GPIOポートを取得
    auto gpio = flexhal::getDefaultGPIOPort();
    if (!gpio) {
        std::cerr << "Failed to get GPIO port!" << std::endl;
        flexhal::end();
        return 1;
    }

    // LEDピンを取得して出力モードに設定
    auto led_pin = gpio->getPin(LED_PIN);
    if (!led_pin) {
        std::cerr << "Failed to get LED pin!" << std::endl;
        flexhal::end();
        return 1;
    }
    led_pin->setMode(flexhal::PinMode::Output);
    std::cout << "LED pin set to OUTPUT mode" << std::endl;

    // LEDを点滅させる
    std::cout << "Blinking LED... Press Ctrl+C to exit" << std::endl;
    bool led_state = false;

    try {
        bool running          = true;
        auto last_toggle_time = std::chrono::steady_clock::now();

        while (running) {
            // 現在時刻を取得
            auto current_time = std::chrono::steady_clock::now();
            auto elapsed      = std::chrono::duration_cast<std::chrono::milliseconds>(current_time - last_toggle_time);

            // 500ミリ秒経過したらLEDの状態を切り替え
            if (elapsed.count() >= 500) {
                // LEDの状態を切り替え
                led_state = !led_state;
                led_pin->setLevel(led_state ? flexhal::PinLevel::High : flexhal::PinLevel::Low);

                // 状態を表示
                std::cout << "LED: " << (led_state ? "ON" : "OFF") << std::endl;

                // 次の切り替え時間を設定
                last_toggle_time = current_time;
            }

            // SDLのイベント処理を含む更新処理
            // この呼び出しがSDLのイベント処理をメインスレッドで行う
            if (!flexhal::update()) {
                running = false;
                break;
            }

            // CPUの負荷を下げるために少し待機
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // FlexHALの終了処理
    flexhal::end();
    return 0;
}
