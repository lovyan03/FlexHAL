#pragma once

#include <cstdint>
#include <chrono>

namespace flexhal {

// 時間関係の便利関数たち！
void delay(uint32_t ms);
uint32_t millis();
uint32_t micros();

// システム関連の便利機能
namespace System {
    // 初期化＆終了
    bool begin();
    void end();

    // CPU周波数の設定
    bool setCpuFrequency(uint32_t freq_mhz);
    uint32_t getCpuFrequency();

    // メモリ情報
    size_t getFreeHeap();
    size_t getMaxAllocHeap();

    // 再起動
    void restart();

    // プラットフォーム情報
    const char* getPlatformName();
    const char* getSDKVersion();
}

// タイマークラス（便利だよね！）
class Timer {
public:
    Timer() : m_start(std::chrono::steady_clock::now()) {}

    void reset() {
        m_start = std::chrono::steady_clock::now();
    }

    uint32_t elapsed() const {
        auto now = std::chrono::steady_clock::now();
        return std::chrono::duration_cast<std::chrono::milliseconds>(now - m_start).count();
    }

private:
    std::chrono::steady_clock::time_point m_start;
};

} // namespace flexhal
