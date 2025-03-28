/**
 * @file logger.cpp
 * @brief FlexHAL - ロガー実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "logger.hpp"

// 環境に応じたロガー実装をインクルード
#if defined(ARDUINO) || defined(ESP_PLATFORM) || defined(ESP32) || defined(ESP8266)
#include "../../impl/frameworks/arduino/logger.inl"
#else
#include "../../impl/platforms/desktop/logger.inl"
#endif

namespace flexhal {

// デフォルトロガーのインスタンス
#if defined(ARDUINO) || defined(ESP_PLATFORM) || defined(ESP32) || defined(ESP8266)
static impl::arduino::Logger s_default_logger;
#else
static impl::desktop::Logger s_default_logger;
#endif

// 現在のロガー
static ILogger* s_current_logger = &s_default_logger;

// デフォルトロガーを取得
ILogger* getLogger() {
    return s_current_logger;
}

// ロガーを設定
void setLogger(ILogger* logger) {
    if (logger) {
        s_current_logger = logger;
    } else {
        s_current_logger = &s_default_logger;
    }
}

// グローバルログ関数
void debug(const char* message) {
    s_current_logger->debug(message);
}

void info(const char* message) {
    s_current_logger->info(message);
}

void warning(const char* message) {
    s_current_logger->warning(message);
}

void error(const char* message) {
    s_current_logger->error(message);
}

void fatal(const char* message) {
    s_current_logger->fatal(message);
}

} // namespace flexhal
