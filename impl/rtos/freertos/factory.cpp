/**
 * @file factory.cpp
 * @brief FlexHAL - FreeRTOS向けRTOS機能の実装
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "../../../src/flexhal/rtos.hpp"
#include "mutex.h"
#include "task.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

namespace flexhal {

// ミューテックスを作成
std::shared_ptr<IMutex> createMutex() {
    return std::make_shared<rtos::freertos::FreeRTOSMutex>();
}

// タスクを作成
std::shared_ptr<ITask> createTask(
    const std::string& name,
    std::function<void()> function,
    size_t stack_size,
    TaskPriority priority,
    int core_id)
{
    return std::make_shared<rtos::freertos::FreeRTOSTask>(
        name, function, stack_size, priority, core_id);
}

// 現在のタスクをスリープ
void sleep(uint32_t ms) {
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

} // namespace flexhal
