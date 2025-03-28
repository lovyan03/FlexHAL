/**
 * @file mutex.h
 * @brief ミューテックスインターフェース定義
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <memory>
#include <cstdint>
#include "platform_detect.h"

namespace flexhal {

/**
 * @brief ミューテックスインターフェース
 */
class IMutex {
public:
    virtual ~IMutex() = default;

    /**
     * @brief ミューテックスをロック
     *
     * @param timeout_ms タイムアウト時間（ミリ秒）、0は永久待機
     * @return true ロック成功
     * @return false ロック失敗
     */
    virtual bool lock(uint32_t timeout_ms = 0) = 0;

    /**
     * @brief ミューテックスをアンロック
     */
    virtual void unlock() = 0;

    /**
     * @brief ミューテックスをトライロック（ブロックなし）
     *
     * @return true ロック成功
     * @return false ロック失敗
     */
    virtual bool tryLock() = 0;
};

/**
 * @brief ミューテックスロックガード
 *
 * スコープベースのミューテックスロック管理
 */
class MutexLockGuard {
public:
    /**
     * @brief コンストラクタ
     *
     * @param mutex ロックするミューテックス
     */
    explicit MutexLockGuard(std::shared_ptr<IMutex> mutex) : mutex_(mutex)
    {
        if (mutex_) {
            locked_ = mutex_->lock();
        }
    }

    /**
     * @brief デストラクタ
     */
    ~MutexLockGuard()
    {
        if (mutex_ && locked_) {
            mutex_->unlock();
        }
    }

    /**
     * @brief ロック成功したか確認
     *
     * @return true ロック成功
     * @return false ロック失敗
     */
    bool isLocked() const
    {
        return locked_;
    }

private:
    std::shared_ptr<IMutex> mutex_;
    bool locked_ = false;
};

/**
 * @brief ミューテックスを作成
 *
 * @return std::shared_ptr<IMutex> 作成したミューテックス
 */
std::shared_ptr<IMutex> createMutex();

}  // namespace flexhal
