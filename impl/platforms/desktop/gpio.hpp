/**
 * @file gpio.hpp
 * @brief FlexHAL - デスクトップ向けGPIOシミュレーション
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FLEXHAL_IMPL_PLATFORMS_DESKTOP_GPIO_HPP
#define FLEXHAL_IMPL_PLATFORMS_DESKTOP_GPIO_HPP

#include "../../../src/flexhal/gpio.hpp"
#include "../../frameworks/sdl/window.hpp"
#include <map>
#include <string>
#include <memory>
#include <mutex>

namespace flexhal {
namespace platform {
namespace desktop {

/**
 * @brief GPIOピンの状態を表す列挙型
 */
enum class PinState {
    INPUT_LOW,      // 入力モード、Lowレベル
    INPUT_HIGH,     // 入力モード、Highレベル
    OUTPUT_LOW,     // 出力モード、Lowレベル
    OUTPUT_HIGH,    // 出力モード、Highレベル
    INPUT_PULLUP,   // 入力モード（プルアップ）
    INPUT_PULLDOWN  // 入力モード（プルダウン）
};

/**
 * @brief GPIOピンシミュレーションクラス
 */
class SimulatedPin : public IPin {
public:
    /**
     * @brief コンストラクタ
     *
     * @param pin_number ピン番号
     */
    SimulatedPin(int pin_number);

    /**
     * @brief デストラクタ
     */
    virtual ~SimulatedPin() = default;

    /**
     * @brief ピンモード設定
     *
     * @param mode ピンモード
     */
    virtual void setMode(PinMode mode) override;

    /**
     * @brief ピン出力レベル設定
     *
     * @param level 出力レベル
     */
    virtual void setLevel(PinLevel level) override;

    /**
     * @brief ピン入力レベル取得
     *
     * @return PinLevel 入力レベル
     */
    virtual PinLevel getLevel() const override;

    /**
     * @brief ピン状態を取得
     *
     * @return PinState ピン状態
     */
    PinState getState() const;

    /**
     * @brief 外部からピンレベルを設定（シミュレーション用）
     *
     * @param level 設定するレベル
     */
    void setExternalLevel(PinLevel level);

private:
    int pin_number_;
    PinMode mode_;
    PinLevel level_;
    mutable std::mutex mutex_;

    /**
     * @brief ピン番号を取得
     *
     * @return int ピン番号
     */
    virtual int getPinNumber() const override
    {
        return pin_number_;
    }
};

/**
 * @brief GPIOポートシミュレーションクラス
 */
class SimulatedGPIOPort : public IGPIOPort {
public:
    /**
     * @brief コンストラクタ
     *
     * @param pin_count ピン数
     * @param window_title ウィンドウタイトル
     */
    SimulatedGPIOPort(int pin_count = 40, const std::string& window_title = "FlexHAL GPIO Simulator");

    /**
     * @brief デストラクタ
     */
    virtual ~SimulatedGPIOPort();

    /**
     * @brief 指定したピン番号のピンを取得
     *
     * @param pin_number ピン番号
     * @param impl 使用するGPIO実装方法（デフォルトはArduino）
     * @return std::shared_ptr<IPin> ピンインスタンス
     */
    virtual std::shared_ptr<IPin> getPin(int pin_number, GPIOImplementation impl = GPIOImplementation::Arduino) override;

    /**
     * @brief ピンモード設定
     *
     * @param pin_number ピン番号
     * @param mode ピンモード
     */
    void pinMode(int pin_number, PinMode mode);

    /**
     * @brief ピン出力レベル設定
     *
     * @param pin_number ピン番号
     * @param level 出力レベル
     */
    void digitalWrite(int pin_number, PinLevel level);

    /**
     * @brief ピン入力レベル取得
     *
     * @param pin_number ピン番号
     * @return PinLevel 入力レベル
     */
    PinLevel digitalRead(int pin_number);

    /**
     * @brief シミュレーションウィンドウを表示
     */
    void showWindow();

    /**
     * @brief シミュレーションウィンドウを非表示
     */
    void hideWindow();

    /**
     * @brief 複数ピンのレベルを一度に設定
     *
     * @param values 設定する値（ビットマップ）
     * @param mask 設定対象のピン（ビットマップ）
     */
    virtual void setLevels(uint32_t values, uint32_t mask) override;

    /**
     * @brief 複数ピンのレベルを一度に取得
     *
     * @return uint32_t 現在のレベル（ビットマップ）
     */
    virtual uint32_t getLevels() const override;

    /**
     * @brief デバイスの初期化
     *
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    virtual bool begin() override;

    /**
     * @brief デバイスの終了処理
     */
    virtual void end() override;

    /**
     * @brief デバイスが準備完了しているか確認
     *
     * @return true 準備完了
     * @return false 準備未完了
     */
    virtual bool isReady() const override;

    /**
     * @brief シミュレーションの更新処理
     *
     * @return true 継続
     * @return false 終了要求
     */
    bool update();

private:
    /**
     * @brief SDLイベント処理コールバック
     *
     * @param event SDLイベント
     * @return true イベント処理継続
     * @return false イベント処理中断
     */
    bool handleEvent(const SDL_Event& event);

    /**
     * @brief SDLレンダリングコールバック
     *
     * @param renderer SDLレンダラー
     */
    void render(SDL_Renderer* renderer);

    /**
     * @brief ピンの描画
     *
     * @param renderer SDLレンダラー
     * @param x X座標
     * @param y Y座標
     * @param width 幅
     * @param height 高さ
     * @param state ピン状態
     * @param pin_number ピン番号
     */
    void drawPin(SDL_Renderer* renderer, int x, int y, int width, int height, PinState state, int pin_number);

    int pin_count_;
    std::map<int, std::shared_ptr<SimulatedPin>> pins_;
    std::unique_ptr<framework::sdl::Window> window_;
    mutable std::mutex mutex_;
    bool window_visible_;
};

}  // namespace desktop
}  // namespace platform
}  // namespace flexhal

#endif  // FLEXHAL_IMPL_PLATFORMS_DESKTOP_GPIO_HPP
