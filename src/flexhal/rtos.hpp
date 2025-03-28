/**
 * @file rtos.hpp
 * @brief FlexHAL - RTOS機能 (C++版)
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FLEXHAL_RTOS_HPP
#define FLEXHAL_RTOS_HPP

#include "core.hpp"
#include "../../impl/internal/mutex.h"
#include "../../impl/internal/task.h"

namespace flexhal {

/**
 * @brief ミューテックスを作成
 *
 * @return std::shared_ptr<IMutex> ミューテックス
 */
std::shared_ptr<IMutex> createMutex();

/**
 * @brief タスクを作成
 *
 * @param name タスク名
 * @param function タスク関数
 * @param stack_size スタックサイズ（バイト）
 * @param priority タスク優先度
 * @param core_id 実行コアID（-1は自動選択）
 * @return std::shared_ptr<ITask> タスク
 */
std::shared_ptr<ITask> createTask(const std::string& name, std::function<void()> function, size_t stack_size,
                                  TaskPriority priority, int core_id);

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

/**
 * @brief 現在の時刻をミリ秒単位で取得
 *
 * @return uint32_t 時刻（ミリ秒）
 */
uint32_t millis();

/**
 * @brief 現在の時刻をマイクロ秒単位で取得
 *
 * @return uint32_t 時刻（マイクロ秒）
 */
uint32_t micros();

}  // namespace flexhal

#endif  // FLEXHAL_RTOS_HPP
