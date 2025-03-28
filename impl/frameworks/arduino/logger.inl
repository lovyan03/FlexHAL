/**
 * @file logger.inl
 * @brief FlexHAL - Arduino環境用ロガー実装（実装）
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "logger.hpp"
#include <Arduino.h>

#if defined(ESP32) || defined(ESP_PLATFORM)
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#endif

namespace flexhal {
namespace impl {
namespace arduino {

Logger::Logger() {
#if defined(ESP32) || defined(ESP_PLATFORM)
    // ESP32環境ではミューテックスを初期化
    mutex_ = xSemaphoreCreateMutex();
#endif
}

Logger::~Logger() {
#if defined(ESP32) || defined(ESP_PLATFORM)
    // ESP32環境ではミューテックスを解放
    if (mutex_) {
        vSemaphoreDelete(static_cast<SemaphoreHandle_t>(mutex_));
        mutex_ = nullptr;
    }
#endif
}

void Logger::log(LogLevel level, const char* message) {
    // 最小ログレベルより低いログはスキップ
    if (level < min_level_) {
        return;
    }

#if defined(ESP32) || defined(ESP_PLATFORM)
    // ESP32環境でスレッドセーフが有効な場合はロック
    if (thread_safe_ && mutex_) {
        xSemaphoreTake(static_cast<SemaphoreHandle_t>(mutex_), portMAX_DELAY);
    }
#endif

    // レベルに応じたプレフィックスを設定
    const char* prefix = "";
    switch (level) {
        case LogLevel::Debug:   prefix = "[DEBUG] "; break;
        case LogLevel::Info:    prefix = "[INFO] "; break;
        case LogLevel::Warning: prefix = "[WARN] "; break;
        case LogLevel::Error:   prefix = "[ERROR] "; break;
        case LogLevel::Fatal:   prefix = "[FATAL] "; break;
    }

    // メッセージを出力
    Serial.print(prefix);
    Serial.println(message);

#if defined(ESP32) || defined(ESP_PLATFORM)
    // ESP32環境でスレッドセーフが有効な場合はアンロック
    if (thread_safe_ && mutex_) {
        xSemaphoreGive(static_cast<SemaphoreHandle_t>(mutex_));
    }
#endif
}

void Logger::setThreadSafe(bool enable) {
    thread_safe_ = enable;
#if !defined(ESP32) && !defined(ESP_PLATFORM)
    // ESP32以外の環境では警告を出す（シングルスレッドなので意味がない）
    if (enable) {
        Serial.println("[WARN] Thread-safe mode has no effect on single-threaded Arduino environments");
    }
#endif
}

void Logger::setMinLogLevel(LogLevel level) {
    min_level_ = level;
}

} // namespace arduino
} // namespace impl
} // namespace flexhal
