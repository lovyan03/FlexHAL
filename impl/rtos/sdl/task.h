/**
 * @file task.h
 * @brief FlexHAL - SDL向けタスク実装
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
#include <functional>
#include <string>
#include <memory>
#include <atomic>

namespace flexhal {
namespace rtos {

/**
 * @brief タスク優先度
 */
enum class TaskPriority { Low, Normal, High, Realtime };

/**
 * @brief SDL_Threadを使用したタスククラス
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
    Task(const std::string& name, std::function<void()> function, TaskPriority priority = TaskPriority::Normal,
         size_t stack_size = 4096)
        : name_(name),
          function_(function),
          priority_(priority),
          stack_size_(stack_size),
          running_(false),
          thread_(nullptr)
    {
    }

    /**
     * @brief デストラクタ
     */
    ~Task()
    {
        stop();
    }

    /**
     * @brief タスク開始
     *
     * @return true 成功
     * @return false 失敗
     */
    bool start()
    {
        if (running_) {
            return true;
        }

        // スレッド終了フラグをリセット
        should_exit_ = false;

        // スレッド作成
        thread_ = SDL_CreateThread(threadFunction, name_.c_str(), this);
        if (!thread_) {
            return false;
        }

        running_ = true;
        return true;
    }

    /**
     * @brief タスク停止
     */
    void stop()
    {
        if (!running_ || !thread_) {
            return;
        }

        // スレッド終了フラグをセット
        should_exit_ = true;

        // スレッド終了待ち
        int result;
        SDL_WaitThread(thread_, &result);
        thread_  = nullptr;
        running_ = false;
    }

    /**
     * @brief タスク実行中かどうか
     *
     * @return true 実行中
     * @return false 停止中
     */
    bool isRunning() const
    {
        return running_;
    }

private:
    /**
     * @brief スレッド関数
     *
     * @param data タスクインスタンス
     * @return int 終了コード
     */
    static int threadFunction(void* data)
    {
        Task* task = static_cast<Task*>(data);

        // タスク関数実行
        task->function_();

        // 終了処理
        task->running_ = false;
        return 0;
    }

    std::string name_;
    std::function<void()> function_;
    TaskPriority priority_;
    size_t stack_size_;
    std::atomic<bool> running_;
    std::atomic<bool> should_exit_;
    SDL_Thread* thread_;
};

}  // namespace rtos
}  // namespace flexhal
