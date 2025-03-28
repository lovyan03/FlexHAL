/**
 * @file gpio.cpp
 * @brief FlexHAL - デスクトップ向けGPIOシミュレーション実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "gpio.hpp"
#include <iostream>
#include <sstream>

namespace flexhal {
namespace platform {
namespace desktop {

// SimulatedPin実装

SimulatedPin::SimulatedPin(int pin_number) : pin_number_(pin_number), mode_(PinMode::Input), level_(PinLevel::Low)
{
}

void SimulatedPin::setMode(PinMode mode)
{
    std::lock_guard<std::mutex> lock(mutex_);
    mode_ = mode;

    // モード変更時のデフォルト状態設定
    if (mode == PinMode::InputPullUp) {
        level_ = PinLevel::High;
    } else if (mode == PinMode::InputPullDown) {
        level_ = PinLevel::Low;
    }
}

void SimulatedPin::setLevel(PinLevel level)
{
    std::lock_guard<std::mutex> lock(mutex_);

    // 出力モードの場合のみレベルを変更
    if (mode_ == PinMode::Output) {
        level_ = level;
    }
}

PinLevel SimulatedPin::getLevel() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    return level_;
}

PinState SimulatedPin::getState() const
{
    if (mode_ == PinMode::Input) {
        return (level_ == PinLevel::Low) ? PinState::INPUT_LOW : PinState::INPUT_HIGH;
    } else if (mode_ == PinMode::Output) {
        return (level_ == PinLevel::Low) ? PinState::OUTPUT_LOW : PinState::OUTPUT_HIGH;
    } else if (mode_ == PinMode::InputPullUp) {
        return PinState::INPUT_PULLUP;
    } else if (mode_ == PinMode::InputPullDown) {
        return PinState::INPUT_PULLDOWN;
    }

    return PinState::INPUT_LOW;  // デフォルト
}

void SimulatedPin::setExternalLevel(PinLevel level)
{
    std::lock_guard<std::mutex> lock(mutex_);

    // 入力モードの場合のみレベルを変更
    if (mode_ == PinMode::Input || mode_ == PinMode::InputPullUp || mode_ == PinMode::InputPullDown) {
        level_ = level;
    }
}

// SimulatedGPIOPort実装

SimulatedGPIOPort::SimulatedGPIOPort(int pin_count, const std::string& window_title)
    : pin_count_(pin_count), window_visible_(false)
{
    // ウィンドウ作成
    window_ = std::make_unique<framework::sdl::Window>(window_title, 800, 600);

    // イベントとレンダリングのコールバック設定
    window_->addEventCallback([this](const SDL_Event& event) { return handleEvent(event); });
    window_->addRenderCallback([this](SDL_Renderer* renderer) { render(renderer); });

    // ピンの初期化
    for (int i = 0; i < pin_count_; ++i) {
        pins_[i] = std::make_shared<SimulatedPin>(i);
    }
}

SimulatedGPIOPort::~SimulatedGPIOPort()
{
    // ウィンドウを閉じる
    if (window_) {
        window_->close();
    }
}

std::shared_ptr<IPin> SimulatedGPIOPort::getPin(int pin_number)
{
    std::lock_guard<std::mutex> lock(mutex_);

    // 範囲外のピン番号
    if (pin_number < 0 || pin_number >= pin_count_) {
        return nullptr;
    }

    // ピンがまだ作成されていなければ作成
    if (pins_.find(pin_number) == pins_.end()) {
        pins_[pin_number] = std::make_shared<SimulatedPin>(pin_number);
    }

    return pins_[pin_number];
}

void SimulatedGPIOPort::pinMode(int pin_number, PinMode mode)
{
    auto pin = getPin(pin_number);
    if (pin) {
        pin->setMode(mode);
    }
}

void SimulatedGPIOPort::digitalWrite(int pin_number, PinLevel level)
{
    auto pin = getPin(pin_number);
    if (pin) {
        pin->setLevel(level);
    }
}

PinLevel SimulatedGPIOPort::digitalRead(int pin_number)
{
    auto pin = getPin(pin_number);
    if (!pin) {
        return PinLevel::Low;
    }

    return pin->getLevel();
}

void SimulatedGPIOPort::showWindow()
{
    if (window_) {
        window_->show();
        window_visible_ = true;
    }
}

void SimulatedGPIOPort::hideWindow()
{
    if (window_) {
        window_->hide();
        window_visible_ = false;
    }
}

void SimulatedGPIOPort::setLevels(uint32_t values, uint32_t mask)
{
    std::lock_guard<std::mutex> lock(mutex_);

    // マスクされたピンの値を設定
    for (int i = 0; i < 32; ++i) {
        if ((mask >> i) & 0x01) {
            auto pin = getPin(i);
            if (pin) {
                pin->setLevel(((values >> i) & 0x01) ? PinLevel::High : PinLevel::Low);
            }
        }
    }
}

bool SimulatedGPIOPort::update()
{
    if (!window_ || !window_visible_) {
        return true;
    }

    return window_->update();
}

uint32_t SimulatedGPIOPort::getLevels() const
{
    std::lock_guard<std::mutex> lock(mutex_);
    uint32_t result = 0;

    // 各ピンのレベルを取得してビットマップに変換
    for (int i = 0; i < 32; ++i) {
        auto it = pins_.find(i);
        if (it != pins_.end()) {
            if (it->second->getLevel() == PinLevel::High) {
                result |= (1 << i);
            }
        }
    }

    return result;
}

bool SimulatedGPIOPort::begin()
{
    // 既に初期化済みなら何もしない
    if (window_visible_) {
        return true;
    }

    // ウィンドウを表示
    showWindow();
    return true;
}

void SimulatedGPIOPort::end()
{
    // ウィンドウを非表示
    hideWindow();
}

bool SimulatedGPIOPort::isReady() const
{
    // ウィンドウが作成されていれば準備完了
    return window_ != nullptr;
}

bool SimulatedGPIOPort::handleEvent(const SDL_Event& event)
{
    // マウスクリックイベント処理
    if (event.type == SDL_MOUSEBUTTONDOWN) {
        int x = event.button.x;
        int y = event.button.y;

        // ピンの配置に基づいてクリックされたピンを特定
        int pin_width    = 80;
        int pin_height   = 40;
        int pins_per_row = 8;

        int col = x / pin_width;
        int row = y / pin_height;

        if (col >= 0 && col < pins_per_row && row >= 0) {
            int pin_number = row * pins_per_row + col;

            if (pin_number < pin_count_) {
                // ピンの状態を切り替え
                auto pin = std::dynamic_pointer_cast<SimulatedPin>(getPin(pin_number));
                if (pin) {
                    PinState state = pin->getState();

                    // 入力モードの場合はレベルを切り替え
                    if (state == PinState::INPUT_LOW || state == PinState::INPUT_PULLDOWN) {
                        pin->setExternalLevel(PinLevel::High);
                    } else if (state == PinState::INPUT_HIGH || state == PinState::INPUT_PULLUP) {
                        pin->setExternalLevel(PinLevel::Low);
                    }
                }
            }
        }
    }

    return true;
}

void SimulatedGPIOPort::render(SDL_Renderer* renderer)
{
    // 背景を暗めのグレーに
    SDL_SetRenderDrawColor(renderer, 40, 40, 40, 255);
    SDL_RenderClear(renderer);

    // ピンの配置パラメータ
    int pin_width    = 80;
    int pin_height   = 40;
    int pins_per_row = 8;
    int margin       = 5;

    // すべてのピンを描画
    for (int i = 0; i < pin_count_; ++i) {
        int row = i / pins_per_row;
        int col = i % pins_per_row;

        int x = col * pin_width + margin;
        int y = row * pin_height + margin;

        auto pin = std::dynamic_pointer_cast<SimulatedPin>(getPin(i));
        if (pin) {
            drawPin(renderer, x, y, pin_width - 2 * margin, pin_height - 2 * margin, pin->getState(), i);
        }
    }

    // タイトルとヘルプテキスト
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    // ここにテキスト描画コードを追加（SDLのテキスト描画は複雑なので省略）
}

void SimulatedGPIOPort::drawPin(SDL_Renderer* renderer, int x, int y, int width, int height, PinState state,
                                int pin_number)
{
    // ピンの背景色を状態に応じて設定
    switch (state) {
        case PinState::INPUT_LOW:
            SDL_SetRenderDrawColor(renderer, 50, 50, 150, 255);  // 暗い青
            break;
        case PinState::INPUT_HIGH:
            SDL_SetRenderDrawColor(renderer, 100, 100, 255, 255);  // 明るい青
            break;
        case PinState::OUTPUT_LOW:
            SDL_SetRenderDrawColor(renderer, 150, 50, 50, 255);  // 暗い赤
            break;
        case PinState::OUTPUT_HIGH:
            SDL_SetRenderDrawColor(renderer, 255, 100, 100, 255);  // 明るい赤
            break;
        case PinState::INPUT_PULLUP:
            SDL_SetRenderDrawColor(renderer, 100, 150, 255, 255);  // 水色
            break;
        case PinState::INPUT_PULLDOWN:
            SDL_SetRenderDrawColor(renderer, 50, 100, 200, 255);  // 濃い水色
            break;
    }

    // ピンの背景を描画
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(renderer, &rect);

    // ピンの枠線を描画
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderDrawRect(renderer, &rect);

    // ピン番号を表示（テキスト描画は省略）
}

}  // namespace desktop
}  // namespace platform
}  // namespace flexhal
