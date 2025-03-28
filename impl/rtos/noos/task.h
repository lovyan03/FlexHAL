/**
 * @file task.h
 * @brief FlexHAL - NoOS向けタスク実装（ダミー）
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <functional>
#include <string>

namespace flexhal {
namespace rtos {

/**
 * @brief タスク優先度
 */
enum class TaskPriority {
    Low,
    Normal,
    High,
    Realtime
};

/**
 * @brief ダミータスククラス（実際には何もしない）
 */
class Task {
public:
    /**
     * @brief コンストラクタ
     * 
     * @param name タスク名
     * @param function タスク関数
     * @param priority タスク優先度
     * @param stack_size スタックサイズ
     */
    Task(const std::string& name, std::function<void()> function, TaskPriority priority = TaskPriority::Normal, size_t stack_size = 4096)
        : name_(name), function_(function), priority_(priority), stack_size_(stack_size), running_(false) {}

    /**
     * @brief デストラクタ
     */
    ~Task() {
        stop();
    }

    /**
     * @brief タスク開始
     * 
     * @return true 成功
     * @return false 失敗
     */
    bool start() {
        if (running_) {
            return true;
        }
        
        // NoOSでは同期実行
        function_();
        running_ = true;
        return true;
    }

    /**
     * @brief タスク停止
     */
    void stop() {
        running_ = false;
    }

    /**
     * @brief タスク実行中かどうか
     * 
     * @return true 実行中
     * @return false 停止中
     */
    bool isRunning() const {
        return running_;
    }

private:
    std::string name_;
    std::function<void()> function_;
    TaskPriority priority_;
    size_t stack_size_;
    bool running_;
};

} // namespace rtos
} // namespace flexhal
