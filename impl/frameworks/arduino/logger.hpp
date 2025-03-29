/**
 * @file logger.hpp
 * @brief FlexHAL - Arduino環境用ロガー実装（ヘッダー）
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "../../../src/flexhal/logger.hpp"

namespace flexhal {
namespace impl {
namespace arduino {

/**
 * @brief Arduino環境用ロガー実装
 */
class Logger : public ILogger {
private:
    LogLevel min_level_ = LogLevel::Debug;
    bool thread_safe_   = false;

#if defined(ESP32) || defined(ESP_PLATFORM)
    // ESP32用のミューテックス
    void* mutex_ = nullptr;
#endif

public:
    Logger();
    ~Logger() override;

    void log(LogLevel level, const char* message) override;
    void setThreadSafe(bool enable) override;
    void setMinLogLevel(LogLevel level) override;
};

}  // namespace arduino
}  // namespace impl
}  // namespace flexhal
