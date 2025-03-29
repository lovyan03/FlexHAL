/**
 * @file logger.cpp
 * @brief FlexHAL - ロガー実装
 * @version 0.1.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "logger.hpp"
#include <iostream>

namespace flexhal {

// デフォルトロガーの前方宣言
class DefaultLogger;

// 現在のロガー
static ILogger* s_current_logger = NULL;

// デフォルトロガーの実装
class DefaultLogger : public ILogger {
public:
    DefaultLogger() : min_level_(LogLevel::Debug), thread_safe_(true)
    {
    }

    void log(LogLevel level, const char* message)
    {
        // 最小ログレベルより低いログはスキップ
        if ((int)level < (int)min_level_) {
            return;
        }

        // レベルに応じた出力先とプレフィックスを選択
        const char* prefix = "";
        std::ostream* out  = &std::cout;

        if (level == LogLevel::Debug) {
            prefix = "[DEBUG] ";
        } else if (level == LogLevel::Info) {
            prefix = "[INFO] ";
        } else if (level == LogLevel::Warning) {
            prefix = "[WARN] ";
        } else if (level == LogLevel::Error) {
            prefix = "[ERROR] ";
            out    = &std::cerr;
        } else if (level == LogLevel::Fatal) {
            prefix = "[FATAL] ";
            out    = &std::cerr;
        }

        // ログ出力
        *out << prefix << message << std::endl;
    }

    void debug(const char* message)
    {
        log(LogLevel::Debug, message);
    }

    void info(const char* message)
    {
        log(LogLevel::Info, message);
    }

    void warning(const char* message)
    {
        log(LogLevel::Warning, message);
    }

    void error(const char* message)
    {
        log(LogLevel::Error, message);
    }

    void fatal(const char* message)
    {
        log(LogLevel::Fatal, message);
    }

    void setThreadSafe(bool enable)
    {
        thread_safe_ = enable;
    }

    void setMinLogLevel(LogLevel level)
    {
        min_level_ = level;
    }

private:
    LogLevel min_level_;
    bool thread_safe_;
};

// デフォルトロガーのインスタンス
static DefaultLogger s_default_logger;

// デフォルトロガーの初期化
void initDefaultLogger()
{
    // 初期化がまだならデフォルトロガーを設定
    if (s_current_logger == NULL) {
        s_current_logger = &s_default_logger;
    }
}

// デフォルトロガーを取得
ILogger* getLogger()
{
    return s_current_logger;
}

// ロガーを設定
void setLogger(ILogger* logger)
{
    if (logger) {
        s_current_logger = logger;
    } else {
        s_current_logger = &s_default_logger;
    }
}

// グローバルログ関数
void debug(const char* message)
{
    if (s_current_logger) {
        s_current_logger->debug(message);
    }
}

void info(const char* message)
{
    if (s_current_logger) {
        s_current_logger->info(message);
    }
}

void warning(const char* message)
{
    if (s_current_logger) {
        s_current_logger->warning(message);
    }
}

void error(const char* message)
{
    if (s_current_logger) {
        s_current_logger->error(message);
    }
}

void fatal(const char* message)
{
    if (s_current_logger) {
        s_current_logger->fatal(message);
    }
}

}  // namespace flexhal
