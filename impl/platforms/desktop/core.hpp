/**
 * @file core.hpp
 * @brief FlexHAL - デスクトップ向けコア機能
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef FLEXHAL_IMPL_PLATFORMS_DESKTOP_CORE_HPP
#define FLEXHAL_IMPL_PLATFORMS_DESKTOP_CORE_HPP

#include "../../../src/flexhal/core.hpp"
#include "gpio.hpp"
#include <memory>
#include <thread>
#include <atomic>

namespace flexhal {
namespace platform {
namespace desktop {

/**
 * @brief デスクトップシミュレーション環境
 */
class DesktopSimulation {
public:
    /**
     * @brief シングルトンインスタンスを取得
     * 
     * @return DesktopSimulation& シングルトンインスタンス
     */
    static DesktopSimulation& getInstance();

    /**
     * @brief シミュレーション環境を初期化
     * 
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    bool init();

    /**
     * @brief シミュレーション環境を終了
     */
    void end();

    /**
     * @brief GPIOポートを取得
     * 
     * @return std::shared_ptr<SimulatedGPIOPort> GPIOポート
     */
    std::shared_ptr<SimulatedGPIOPort> getGPIOPort();

    /**
     * @brief シミュレーションの更新処理
     * 
     * @return true 継続
     * @return false 終了要求
     */
    bool update();

    /**
     * @brief シミュレーションウィンドウを表示
     */
    void showWindows();

    /**
     * @brief シミュレーションウィンドウを非表示
     */
    void hideWindows();

private:
    /**
     * @brief コンストラクタ（シングルトン）
     */
    DesktopSimulation();

    /**
     * @brief デストラクタ
     */
    ~DesktopSimulation();

    /**
     * @brief 更新スレッド関数
     */
    void updateThread();

    std::shared_ptr<SimulatedGPIOPort> gpio_port_;
    std::atomic<bool> running_;
    std::unique_ptr<std::thread> update_thread_;
};

} // namespace desktop
} // namespace platform
} // namespace flexhal

#endif // FLEXHAL_IMPL_PLATFORMS_DESKTOP_CORE_HPP
