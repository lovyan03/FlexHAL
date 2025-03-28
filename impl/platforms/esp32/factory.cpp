/**
 * @file factory.cpp
 * @brief FlexHAL - ESP32向けファクトリ実装
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../../../src/flexhal/gpio.hpp"
#include "../../../src/flexhal/core.hpp"
#include "gpio.hpp"
#include "core.hpp"
#include <memory>

namespace flexhal {

// ESP32環境のGPIOポートを取得
std::shared_ptr<IGPIOPort> getDefaultGPIOPort() {
    static std::shared_ptr<platform::esp32::ESP32GPIOPort> gpio_port = 
        std::make_shared<platform::esp32::ESP32GPIOPort>(40); // ESP32は最大40ピン
    
    // 初期化されていなければ初期化
    if (!gpio_port->isReady()) {
        gpio_port->begin();
    }
    
    return gpio_port;
}

// 指定したピン番号のピンを取得
std::shared_ptr<IPin> getPin(int pin_number) {
    auto port = getDefaultGPIOPort();
    if (!port) {
        return nullptr;
    }
    
    return port->getPin(pin_number);
}

// ライブラリの初期化
bool init() {
    auto& platform = platform::esp32::ESP32Platform::getInstance();
    return platform.init();
}

// ライブラリの終了処理
void end() {
    auto& platform = platform::esp32::ESP32Platform::getInstance();
    platform.end();
}

// ライブラリの更新処理
bool update() {
    auto& platform = platform::esp32::ESP32Platform::getInstance();
    return platform.update();
}

} // namespace flexhal
