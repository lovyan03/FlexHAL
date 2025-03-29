/**
 * @file logger.hpp
 * @brief FlexHAL - ロガーインターフェース
 * @version 0.1.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

namespace flexhal {

/**
 * @brief ログレベルの定義
 */
enum class LogLevel {
    Debug,   ///< デバッグ情報
    Info,    ///< 一般情報
    Warning, ///< 警告
    Error,   ///< エラー
    Fatal    ///< 致命的エラー
};

/**
 * @brief ロガーインターフェース
 */
class ILogger {
public:
    virtual ~ILogger() = default;
    
    /**
     * @brief ログメッセージを出力する
     * @param level ログレベル
     * @param message メッセージ
     */
    virtual void log(LogLevel level, const char* message) = 0;
    
    /**
     * @brief デバッグレベルのログを出力する
     * @param message メッセージ
     */
    virtual void debug(const char* message) { log(LogLevel::Debug, message); }
    
    /**
     * @brief 情報レベルのログを出力する
     * @param message メッセージ
     */
    virtual void info(const char* message) { log(LogLevel::Info, message); }
    
    /**
     * @brief 警告レベルのログを出力する
     * @param message メッセージ
     */
    virtual void warning(const char* message) { log(LogLevel::Warning, message); }
    
    /**
     * @brief エラーレベルのログを出力する
     * @param message メッセージ
     */
    virtual void error(const char* message) { log(LogLevel::Error, message); }
    
    /**
     * @brief 致命的エラーレベルのログを出力する
     * @param message メッセージ
     */
    virtual void fatal(const char* message) { log(LogLevel::Fatal, message); }
    
    /**
     * @brief スレッドセーフモードを設定する
     * @param enable trueの場合、スレッドセーフモードを有効にする
     */
    virtual void setThreadSafe(bool enable) = 0;
    
    /**
     * @brief 最小ログレベルを設定する
     * @param level 表示する最小ログレベル
     */
    virtual void setMinLogLevel(LogLevel level) = 0;
};

/**
 * @brief デフォルトロガーを初期化する
 * @note この関数は通常、init()関数から呼び出される
 */
void initDefaultLogger();

/**
 * @brief デフォルトロガーを取得する
 * @return ILogger* ロガーインスタンスへのポインタ
 */
ILogger* getLogger();

/**
 * @brief ロガーを設定する
 * @param logger ロガーインスタンスへのポインタ
 */
void setLogger(ILogger* logger);

/**
 * @brief デバッグレベルのログを出力する（グローバル関数）
 * @param message メッセージ
 */
void debug(const char* message);

/**
 * @brief 情報レベルのログを出力する（グローバル関数）
 * @param message メッセージ
 */
void info(const char* message);

/**
 * @brief 警告レベルのログを出力する（グローバル関数）
 * @param message メッセージ
 */
void warning(const char* message);

/**
 * @brief エラーレベルのログを出力する（グローバル関数）
 * @param message メッセージ
 */
void error(const char* message);

/**
 * @brief 致命的エラーレベルのログを出力する（グローバル関数）
 * @param message メッセージ
 */
void fatal(const char* message);

} // namespace flexhal
