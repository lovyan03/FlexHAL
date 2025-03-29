/**
 * @file factory.inl
 * @brief FlexHAL - デスクトップ向けファクトリ実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "../../../src/flexhal/gpio.hpp"
#include "../../../src/flexhal/core.hpp"
#include "core.hpp"
#include <memory>

namespace flexhal {

// デスクトップシミュレーション環境のGPIOポートを取得
std::shared_ptr<IGPIOPort> getDefaultGPIOPort()
{
    auto& simulation = platform::desktop::DesktopSimulation::getInstance();

    // 初期化されていなければ初期化
    if (!simulation.init()) {
        return nullptr;
    }

    return simulation.getGPIOPort();
}

// 指定したピン番号のピンを取得
std::shared_ptr<IPin> getPin(int pin_number)
{
    auto port = getDefaultGPIOPort();
    if (!port) {
        return nullptr;
    }

    return port->getPin(pin_number);
}

// プラットフォーム固有の初期化
namespace platform {
    namespace desktop {
        bool initImpl()
        {
            auto& simulation = DesktopSimulation::getInstance();
            return simulation.init();
        }

        void endImpl()
        {
            auto& simulation = DesktopSimulation::getInstance();
            simulation.end();
        }
    }
}

// ライブラリの更新処理
bool update()
{
    auto& simulation = platform::desktop::DesktopSimulation::getInstance();
    return simulation.update();
}

}  // namespace flexhal
