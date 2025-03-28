/**
 * @file window.hpp
 * @brief FlexHAL - SDLウィンドウ管理
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FLEXHAL_IMPL_FRAMEWORKS_SDL_WINDOW_HPP
#define FLEXHAL_IMPL_FRAMEWORKS_SDL_WINDOW_HPP

// SDL.hのインクルードパスは環境によって異なるため、__has_includeマクロで分岐
#if __has_include(<SDL.h>)
#include <SDL.h>
#elif __has_include(<SDL2/SDL.h>)
#include <SDL2/SDL.h>
#else
#error "SDL.h not found. Please install SDL2 development libraries."
#endif
#include <string>
#include <memory>
#include <functional>
#include <vector>

namespace flexhal {
namespace framework {
namespace sdl {

/**
 * @brief SDLウィンドウ管理クラス
 */
class Window {
public:
    /**
     * @brief イベントコールバック関数の型
     */
    using EventCallback = std::function<bool(const SDL_Event&)>;

    /**
     * @brief レンダリングコールバック関数の型
     */
    using RenderCallback = std::function<void(SDL_Renderer*)>;

    /**
     * @brief コンストラクタ
     *
     * @param title ウィンドウタイトル
     * @param width ウィンドウ幅
     * @param height ウィンドウ高さ
     */
    Window(const std::string& title, int width, int height);

    /**
     * @brief デストラクタ
     */
    ~Window();

    /**
     * @brief ウィンドウを表示
     */
    void show();

    /**
     * @brief ウィンドウを非表示
     */
    void hide();

    /**
     * @brief ウィンドウを閉じる
     */
    void close();

    /**
     * @brief イベント処理とレンダリングを実行
     *
     * @return true 継続
     * @return false 終了要求
     */
    bool update();

    /**
     * @brief イベントコールバックを追加
     *
     * @param callback コールバック関数
     */
    void addEventCallback(EventCallback callback);

    /**
     * @brief レンダリングコールバックを追加
     *
     * @param callback コールバック関数
     */
    void addRenderCallback(RenderCallback callback);

    /**
     * @brief SDLウィンドウハンドルを取得
     *
     * @return SDL_Window* ウィンドウハンドル
     */
    SDL_Window* getSDLWindow() const
    {
        return window_;
    }

    /**
     * @brief SDLレンダラーハンドルを取得
     *
     * @return SDL_Renderer* レンダラーハンドル
     */
    SDL_Renderer* getSDLRenderer() const
    {
        return renderer_;
    }

private:
    SDL_Window* window_;
    SDL_Renderer* renderer_;
    bool running_;
    std::vector<EventCallback> eventCallbacks_;
    std::vector<RenderCallback> renderCallbacks_;
};

}  // namespace sdl
}  // namespace framework
}  // namespace flexhal

#endif  // FLEXHAL_IMPL_FRAMEWORKS_SDL_WINDOW_HPP
