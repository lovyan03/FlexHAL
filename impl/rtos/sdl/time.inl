/**
 * @file time.inl
 * @brief FlexHAL - SDL向け時間関連機能の実装
 * @version 0.1.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <chrono>
#include <thread>

// SDL2のヘッダーファイルのインクルード方法を条件分岐で実装
#if defined(__has_include) && __has_include(<SDL2/SDL.h>)
  #include <SDL2/SDL.h>
#elif defined(__has_include) && __has_include(<SDL.h>)
  #include <SDL.h>
#endif

namespace flexhal {

// 開始時間を記録（プログラム起動時からの経過時間を計測するため）
static std::chrono::time_point<std::chrono::steady_clock> start_time = std::chrono::steady_clock::now();

// 現在の時間をミリ秒単位で取得
unsigned long millis() {
    auto now = std::chrono::steady_clock::now();
    return static_cast<unsigned long>(std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count());
}

// 指定されたミリ秒数だけスリープ
void sleep(unsigned int ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

} // namespace flexhal
