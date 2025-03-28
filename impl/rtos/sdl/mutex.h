/**
 * @file mutex.h
 * @brief FlexHAL - SDL向けミューテックス実装
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

// SDL.hのインクルードパスは環境によって異なるため、__has_includeマクロで分岐
#if __has_include(<SDL.h>)
#include <SDL.h>
#elif __has_include(<SDL2/SDL.h>)
#include <SDL2/SDL.h>
#else
#error "SDL.h not found. Please install SDL2 development libraries."
#endif

namespace flexhal {
namespace rtos {

/**
 * @brief SDL_mutexを使用したミューテックスクラス
 */
class Mutex {
public:
    /**
     * @brief コンストラクタ
     */
    Mutex() : mutex_(SDL_CreateMutex())
    {
    }

    /**
     * @brief デストラクタ
     */
    ~Mutex()
    {
        if (mutex_) {
            SDL_DestroyMutex(mutex_);
        }
    }

    /**
     * @brief ロック取得
     */
    void lock()
    {
        if (mutex_) {
            SDL_LockMutex(mutex_);
        }
    }

    /**
     * @brief ロック解放
     */
    void unlock()
    {
        if (mutex_) {
            SDL_UnlockMutex(mutex_);
        }
    }

    /**
     * @brief ロック試行
     *
     * @return true 成功
     * @return false 失敗
     */
    bool tryLock()
    {
        return mutex_ ? (SDL_TryLockMutex(mutex_) == 0) : false;
    }

private:
    SDL_mutex* mutex_;
};

/**
 * @brief ロックガードクラス
 */
class LockGuard {
public:
    /**
     * @brief コンストラクタ
     *
     * @param mutex ミューテックス
     */
    LockGuard(Mutex& mutex) : mutex_(mutex)
    {
        mutex_.lock();
    }

    /**
     * @brief デストラクタ
     */
    ~LockGuard()
    {
        mutex_.unlock();
    }

private:
    Mutex& mutex_;
};

}  // namespace rtos
}  // namespace flexhal
