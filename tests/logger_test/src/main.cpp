/**
 * @file main.cpp
 * @brief FlexHAL - ロガー機能テスト
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#include "FlexHAL.hpp"
#include <iostream>
#include <thread>
#include <vector>
#include <chrono>

// マルチスレッドテスト用の関数
void thread_log_function(int thread_id, int count)
{
    for (int i = 0; i < count; ++i) {
        // スレッドIDと回数を含むメッセージを作成
        char message[64];
        snprintf(message, sizeof(message), "Thread %d: Log message %d", thread_id, i);

        // ランダムなログレベルでログを出力
        switch (i % 5) {
            case 0:
                flexhal::debug(message);
                break;
            case 1:
                flexhal::info(message);
                break;
            case 2:
                flexhal::warning(message);
                break;
            case 3:
                flexhal::error(message);
                break;
            case 4:
                flexhal::fatal(message);
                break;
        }

        // 少し待機
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

// カスタムロガー実装のテスト
class CustomLogger : public flexhal::ILogger {
private:
    flexhal::LogLevel min_level_ = flexhal::LogLevel::Debug;
    bool thread_safe_            = true;
    std::mutex mutex_;

public:
    void log(flexhal::LogLevel level, const char* message) override
    {
        // 最小ログレベルより低いログはスキップ
        if (level < min_level_) {
            return;
        }

        // スレッドセーフモードが有効な場合はロック
        std::unique_lock<std::mutex> lock(mutex_, std::defer_lock);
        if (thread_safe_) {
            lock.lock();
        }

        // カスタムフォーマットでログを出力
        std::cout << "[CUSTOM] ";

        switch (level) {
            case flexhal::LogLevel::Debug:
                std::cout << "[D] ";
                break;
            case flexhal::LogLevel::Info:
                std::cout << "[I] ";
                break;
            case flexhal::LogLevel::Warning:
                std::cout << "[W] ";
                break;
            case flexhal::LogLevel::Error:
                std::cout << "[E] ";
                break;
            case flexhal::LogLevel::Fatal:
                std::cout << "[F] ";
                break;
        }

        std::cout << message << std::endl;
    }

    void setThreadSafe(bool enable) override
    {
        thread_safe_ = enable;
    }

    void setMinLogLevel(flexhal::LogLevel level) override
    {
        min_level_ = level;
    }
};

int main()
{
    std::cout << "FlexHAL Logger Test" << std::endl;
    std::cout << "Version: " << flexhal::Version::string << std::endl;

    // FlexHALを初期化
    if (!flexhal::init()) {
        std::cerr << "FlexHAL initialization failed!" << std::endl;
        return 1;
    }

    // 基本的なログ出力テスト
    std::cout << "\n=== Basic Logging Test ===" << std::endl;
    flexhal::debug("This is a debug message");
    flexhal::info("This is an info message");
    flexhal::warning("This is a warning message");
    flexhal::error("This is an error message");
    flexhal::fatal("This is a fatal message");

    // ログレベルフィルタリングテスト
    std::cout << "\n=== Log Level Filtering Test ===" << std::endl;
    flexhal::getLogger()->setMinLogLevel(flexhal::LogLevel::Warning);
    std::cout << "Setting minimum log level to Warning" << std::endl;
    flexhal::debug("This debug message should NOT be displayed");
    flexhal::info("This info message should NOT be displayed");
    flexhal::warning("This warning message should be displayed");
    flexhal::error("This error message should be displayed");
    flexhal::fatal("This fatal message should be displayed");

    // ログレベルをリセット
    flexhal::getLogger()->setMinLogLevel(flexhal::LogLevel::Debug);

    // スレッドセーフテスト
    std::cout << "\n=== Thread-Safe Logging Test ===" << std::endl;
    flexhal::getLogger()->setThreadSafe(true);

    // 複数スレッドからのログ出力
    std::vector<std::thread> threads;
    const int num_threads     = 4;
    const int logs_per_thread = 10;

    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_log_function, i, logs_per_thread);
    }

    // すべてのスレッドが終了するのを待つ
    for (auto& thread : threads) {
        thread.join();
    }

    // カスタムロガーテスト
    std::cout << "\n=== Custom Logger Test ===" << std::endl;
    CustomLogger custom_logger;
    flexhal::setLogger(&custom_logger);

    flexhal::debug("This is a debug message with custom logger");
    flexhal::info("This is an info message with custom logger");
    flexhal::warning("This is a warning message with custom logger");
    flexhal::error("This is an error message with custom logger");
    flexhal::fatal("This is a fatal message with custom logger");

    // デフォルトロガーに戻す
    flexhal::setLogger(nullptr);

    // 非スレッドセーフモードテスト
    std::cout << "\n=== Non-Thread-Safe Mode Test ===" << std::endl;
    flexhal::getLogger()->setThreadSafe(false);
    flexhal::info("Logging in non-thread-safe mode");

    // 複数スレッドからのログ出力（非スレッドセーフモード）
    threads.clear();
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_log_function, i, 5);
    }

    // すべてのスレッドが終了するのを待つ
    for (auto& thread : threads) {
        thread.join();
    }

    // スレッドセーフモードを元に戻す
    flexhal::getLogger()->setThreadSafe(true);

    // FlexHALの終了処理
    flexhal::end();
    std::cout << "\nLogger test completed successfully!" << std::endl;

    return 0;
}
