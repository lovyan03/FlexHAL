/**
 * @file task.h
 * @brief FlexHAL - FreeRTOS向けタスク実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "../../internal/task.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <functional>
#include <string>

namespace flexhal {
namespace rtos {
namespace freertos {

/**
 * @brief FreeRTOS用タスク実装
 */
class FreeRTOSTask : public flexhal::ITask {
public:
    /**
     * @brief コンストラクタ
     *
     * @param name タスク名
     * @param function タスク関数
     * @param stack_size スタックサイズ（バイト）
     * @param priority タスク優先度
     * @param core_id 実行コアID（-1は自動選択）
     */
    FreeRTOSTask(const std::string& name, std::function<void()> function, size_t stack_size, flexhal::TaskPriority priority,
                 int core_id)
        : name_(name),
          function_(function),
          stack_size_(stack_size),
          priority_(priority),
          core_id_(core_id),
          handle_(nullptr),
          running_(false)
    {
    }

    /**
     * @brief デストラクタ
     */
    virtual ~FreeRTOSTask()
    {
        stop();
    }

    /**
     * @brief タスクを開始
     *
     * @return true 開始成功
     * @return false 開始失敗
     */
    bool start() override
    {
        if (running_) {
            return true;  // 既に実行中
        }

        // FreeRTOSの優先度に変換
        UBaseType_t freertos_priority;
        switch (priority_) {
            case flexhal::TaskPriority::Low:
                freertos_priority = tskIDLE_PRIORITY + 1;
                break;
            case flexhal::TaskPriority::Normal:
                freertos_priority = tskIDLE_PRIORITY + 2;
                break;
            case flexhal::TaskPriority::High:
                freertos_priority = tskIDLE_PRIORITY + 3;
                break;
            case flexhal::TaskPriority::Realtime:
                freertos_priority = configMAX_PRIORITIES - 1;
                break;
            default:
                freertos_priority = tskIDLE_PRIORITY + 2;  // デフォルトはNormal
                break;
        }

        // タスクを作成
        BaseType_t result;
        if (core_id_ >= 0) {
            // コア指定あり
            result = xTaskCreatePinnedToCore(taskFunction, name_.c_str(), stack_size_ / sizeof(StackType_t), this,
                                             freertos_priority, &handle_, core_id_);
        } else {
            // コア自動選択
            result = xTaskCreate(taskFunction, name_.c_str(), stack_size_ / sizeof(StackType_t), this,
                                 freertos_priority, &handle_);
        }

        if (result != pdPASS) {
            return false;
        }

        running_ = true;
        return true;
    }

    /**
     * @brief タスクを停止
     */
    void stop() override
    {
        if (!running_ || handle_ == nullptr) {
            return;
        }

        // タスクを削除
        vTaskDelete(handle_);
        handle_  = nullptr;
        running_ = false;
    }

    /**
     * @brief タスクが実行中か
     *
     * @return true 実行中
     * @return false 停止中
     */
    bool isRunning() const override
    {
        return running_;
    }

    /**
     * @brief タスク優先度を設定
     *
     * @param priority 設定する優先度
     */
    void setPriority(flexhal::TaskPriority priority) override
    {
        priority_ = priority;
        
        if (running_ && handle_ != nullptr) {
            // FreeRTOSの優先度に変換
            UBaseType_t freertos_priority;
            switch (priority_) {
                case flexhal::TaskPriority::Low:
                    freertos_priority = tskIDLE_PRIORITY + 1;
                    break;
                case flexhal::TaskPriority::Normal:
                    freertos_priority = tskIDLE_PRIORITY + 2;
                    break;
                case flexhal::TaskPriority::High:
                    freertos_priority = tskIDLE_PRIORITY + 3;
                    break;
                case flexhal::TaskPriority::Realtime:
                    freertos_priority = configMAX_PRIORITIES - 1;
                    break;
                default:
                    freertos_priority = tskIDLE_PRIORITY + 2;  // デフォルトはNormal
                    break;
            }
            
            // 実行中のタスクの優先度を変更
            vTaskPrioritySet(handle_, freertos_priority);
        }
    }

    /**
     * @brief タスク優先度を取得
     *
     * @return TaskPriority 現在の優先度
     */
    flexhal::TaskPriority getPriority() const override
    {
        return priority_;
    }

    /**
     * @brief タスク名を取得
     *
     * @return const std::string& タスク名
     */
    const std::string& getName() const override
    {
        return name_;
    }

private:
    /**
     * @brief FreeRTOSタスク関数（静的）
     *
     * @param pvParameters タスクパラメータ（FreeRTOSTaskインスタンス）
     */
    static void taskFunction(void* pvParameters)
    {
        auto task = static_cast<FreeRTOSTask*>(pvParameters);
        if (task) {
            task->function_();
        }

        // タスク関数が終了したら自動的にタスクを停止
        task->running_ = false;
        task->handle_  = nullptr;
        vTaskDelete(nullptr);
    }

    std::string name_;
    std::function<void()> function_;
    size_t stack_size_;
    flexhal::TaskPriority priority_;
    int core_id_;
    TaskHandle_t handle_;
    bool running_;
};

}  // namespace freertos
}  // namespace rtos
}  // namespace flexhal
