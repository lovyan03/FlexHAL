/**
 * @file mutex.h
 * @brief FlexHAL - NoOS向けミューテックス実装（ダミー）
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

namespace flexhal {
namespace rtos {

/**
 * @brief ダミーミューテックスクラス（何もしない）
 */
class Mutex {
public:
    /**
     * @brief コンストラクタ
     */
    Mutex() {}

    /**
     * @brief デストラクタ
     */
    ~Mutex() {}

    /**
     * @brief ロック取得（何もしない）
     */
    void lock() {}

    /**
     * @brief ロック解放（何もしない）
     */
    void unlock() {}

    /**
     * @brief ロック試行（常に成功）
     * 
     * @return true 常にtrue
     */
    bool tryLock() { return true; }
};

/**
 * @brief ダミーロックガードクラス
 */
class LockGuard {
public:
    /**
     * @brief コンストラクタ
     * 
     * @param mutex ミューテックス
     */
    LockGuard(Mutex& mutex) : mutex_(mutex) {
        mutex_.lock();
    }

    /**
     * @brief デストラクタ
     */
    ~LockGuard() {
        mutex_.unlock();
    }

private:
    Mutex& mutex_;
};

} // namespace rtos
} // namespace flexhal
