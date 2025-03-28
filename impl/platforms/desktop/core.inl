/**
 * @file core.cpp
 * @brief FlexHAL - デスクトップ向けコア機能の実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "core.hpp"
#include <iostream>
#include <chrono>

namespace flexhal {
namespace platform {
namespace desktop {

// シングルトンインスタンス
DesktopSimulation& DesktopSimulation::getInstance()
{
    static DesktopSimulation instance;
    return instance;
}

DesktopSimulation::DesktopSimulation() : running_(false)
{
    // GPIOポート作成
    gpio_port_ = std::make_shared<SimulatedGPIOPort>(40, "FlexHAL GPIO Simulator");
}

DesktopSimulation::~DesktopSimulation()
{
    end();
}

bool DesktopSimulation::init()
{
    if (running_.load()) {
        return true;  // 既に初期化済み
    }

    // GPIOウィンドウを表示
    if (gpio_port_) {
        gpio_port_->showWindow();
    }

    // 更新スレッドは使用せず、メインスレッドでの更新に切り替え
    running_.store(true);

    std::cout << "FlexHAL Desktop Simulation initialized" << std::endl;
    return true;
}

void DesktopSimulation::end()
{
    if (!running_.load()) {
        return;  // 既に終了済み
    }

    // 実行フラグをオフに
    running_.store(false);

    // GPIOウィンドウを閉じる
    if (gpio_port_) {
        gpio_port_->hideWindow();
    }

    std::cout << "FlexHAL Desktop Simulation terminated" << std::endl;
}

std::shared_ptr<SimulatedGPIOPort> DesktopSimulation::getGPIOPort()
{
    return gpio_port_;
}

bool DesktopSimulation::update()
{
    bool result = true;

    // GPIOポート更新
    if (gpio_port_) {
        result = gpio_port_->update() && result;
    }

    return result;
}

void DesktopSimulation::showWindows()
{
    // GPIOウィンドウを表示
    if (gpio_port_) {
        gpio_port_->showWindow();
    }
}

void DesktopSimulation::hideWindows()
{
    // GPIOウィンドウを非表示
    if (gpio_port_) {
        gpio_port_->hideWindow();
    }
}

// メインスレッドから呼び出される更新関数に置き換えたため、updateThreadは不要

}  // namespace desktop
}  // namespace platform
}  // namespace flexhal
