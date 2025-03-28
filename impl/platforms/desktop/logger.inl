/**
 * @file logger.inl
 * @brief FlexHAL - デスクトップ環境用ロガー実装（実装）
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "logger.hpp"
#include <iostream>
#include <iomanip>
#include <ctime>
#include <chrono>

namespace flexhal {
namespace impl {
namespace desktop {

void Logger::log(LogLevel level, const char* message) {
    // 最小ログレベルより低いログはスキップ
    if (level < min_level_) {
        return;
    }

    // スレッドセーフモードが有効な場合はロック
    std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
    if (thread_safe_) {
        lock.lock();
    }

    // 現在時刻を取得
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    std::tm tm = *std::localtime(&time);

    // レベルに応じた出力先とプレフィックスを選択
    const char* prefix = "";
    std::ostream* out = &std::cout;
    
    switch (level) {
        case LogLevel::Debug:
            prefix = "[DEBUG] ";
            break;
        case LogLevel::Info:
            prefix = "[INFO] ";
            break;
        case LogLevel::Warning:
            prefix = "[WARN] ";
            break;
        case LogLevel::Error:
            prefix = "[ERROR] ";
            out = &std::cerr;
            break;
        case LogLevel::Fatal:
            prefix = "[FATAL] ";
            out = &std::cerr;
            break;
    }

    // タイムスタンプを出力
    *out << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " ";
    
    // メッセージを出力
    *out << prefix << message << std::endl;
}

void Logger::setThreadSafe(bool enable) {
    thread_safe_ = enable;
}

void Logger::setMinLogLevel(LogLevel level) {
    min_level_ = level;
}

} // namespace desktop
} // namespace impl
} // namespace flexhal
