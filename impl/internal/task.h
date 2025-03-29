/**
 * @file task.h
 * @brief タスクインターフェース定義
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <cstdint>
#include "platform_detect.h"

namespace flexhal {

/**
 * @brief タスク優先度
 */
enum class TaskPriority : int {
    Lowest   = 0,  ///< 最低優先度
    Low      = 1,  ///< 低優先度
    Normal   = 2,  ///< 通常優先度
    High     = 3,  ///< 高優先度
    Highest  = 4,  ///< 最高優先度
    Realtime = 5   ///< リアルタイム優先度
};

/**
 * @brief タスクインターフェース
 */
class ITask {
public:
    virtual ~ITask() = default;

    /**
     * @brief タスクを開始
     *
     * @return true 開始成功
     * @return false 開始失敗
     */
    virtual bool start() = 0;

    /**
     * @brief タスクを停止
     */
    virtual void stop() = 0;

    /**
     * @brief タスクが実行中か確認
     *
     * @return true 実行中
     * @return false 停止中
     */
    virtual bool isRunning() const = 0;

    /**
     * @brief タスク優先度を設定
     *
     * @param priority 設定する優先度
     */
    virtual void setPriority(TaskPriority priority) = 0;

    /**
     * @brief タスク優先度を取得
     *
     * @return TaskPriority 現在の優先度
     */
    virtual TaskPriority getPriority() const = 0;

    /**
     * @brief タスク名を取得
     *
     * @return const std::string& タスク名
     */
    virtual const std::string& getName() const = 0;
};

/**
 * @brief タスクを作成
 *
 * @param name タスク名
 * @param function タスク関数
 * @param stack_size スタックサイズ（バイト）
 * @param priority タスク優先度
 * @param core_id 実行コアID（-1は自動選択）
 * @return std::shared_ptr<ITask> 作成したタスク
 */
/**
 * @brief タスクを作成
 *
 * @param name タスク名
 * @param function タスク関数
 * @param stack_size スタックサイズ（バイト）
 * @param priority タスク優先度
 * @param core_id 実行コアID（-1は自動選択）
 * @return std::shared_ptr<ITask> 作成したタスク
 */
std::shared_ptr<ITask> createTask(const std::string& name, std::function<void()> function, size_t stack_size = 4096,
                                  TaskPriority priority = TaskPriority::Normal, int core_id = -1);

/**
 * @brief 現在のタスクをスリープ
 *
 * @param ms スリープ時間（ミリ秒）
 */
void sleep(uint32_t ms);

/**
 * @brief 現在のタスクを一時的に中断（他のタスクに実行を譲る）
 */
void yield();

}  // namespace flexhal
