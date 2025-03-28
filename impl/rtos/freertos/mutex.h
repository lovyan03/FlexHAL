/**
 * @file mutex.h
 * @brief FlexHAL - FreeRTOS向けミューテックス実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include "../../internal/mutex.h"
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace flexhal {
namespace rtos {
namespace freertos {

/**
 * @brief FreeRTOS用ミューテックス実装
 */
class FreeRTOSMutex : public IMutex {
public:
    /**
     * @brief コンストラクタ
     */
    FreeRTOSMutex()
    {
        mutex_ = xSemaphoreCreateMutex();
    }

    /**
     * @brief デストラクタ
     */
    virtual ~FreeRTOSMutex()
    {
        if (mutex_ != nullptr) {
            vSemaphoreDelete(mutex_);
            mutex_ = nullptr;
        }
    }

    /**
     * @brief ミューテックスをロック
     */
    void lock() override
    {
        if (mutex_ != nullptr) {
            xSemaphoreTake(mutex_, portMAX_DELAY);
        }
    }

    /**
     * @brief ミューテックスをアンロック
     */
    void unlock() override
    {
        if (mutex_ != nullptr) {
            xSemaphoreGive(mutex_);
        }
    }

private:
    SemaphoreHandle_t mutex_;
};

}  // namespace freertos
}  // namespace rtos
}  // namespace flexhal
