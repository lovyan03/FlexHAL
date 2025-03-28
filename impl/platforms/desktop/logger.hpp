/**
 * @file logger.hpp
 * @brief FlexHAL - デスクトップ環境用ロガー実装（ヘッダー）
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "../../../src/flexhal/logger.hpp"
#include <mutex>

namespace flexhal {
namespace impl {
namespace desktop {

/**
 * @brief デスクトップ環境用ロガー実装
 */
class Logger : public ILogger {
private:
    LogLevel min_level_ = LogLevel::Debug;
    bool thread_safe_ = true;  // デスクトップ環境ではデフォルトでスレッドセーフ
    std::mutex mutex_;

public:
    Logger() = default;
    ~Logger() override = default;
    
    void log(LogLevel level, const char* message) override;
    void setThreadSafe(bool enable) override;
    void setMinLogLevel(LogLevel level) override;
};

} // namespace desktop
} // namespace impl
} // namespace flexhal
