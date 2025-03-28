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
class FreeRTOSMutex : public flexhal::IMutex {
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
     * 
     * @param timeout_ms タイムアウト時間（ミリ秒）、0は永久待機
     * @return true ロック成功
     * @return false ロック失敗
     */
    bool lock(uint32_t timeout_ms = 0) override
    {
        if (mutex_ != nullptr) {
            TickType_t ticks = (timeout_ms == 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
            return (xSemaphoreTake(mutex_, ticks) == pdTRUE);
        }
        return false;
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

    /**
     * @brief ミューテックスをトライロック（ブロックなし）
     * 
     * @return true ロック成功
     * @return false ロック失敗
     */
    bool tryLock() override
    {
        if (mutex_ != nullptr) {
            return (xSemaphoreTake(mutex_, 0) == pdTRUE);
        }
        return false;
    }

private:
    SemaphoreHandle_t mutex_;
};

}  // namespace freertos
}  // namespace rtos
}  // namespace flexhal
