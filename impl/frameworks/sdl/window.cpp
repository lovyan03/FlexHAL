/**
 * @file window.cpp
 * @brief FlexHAL - SDLウィンドウ管理の実装
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "window.hpp"
#include <iostream>

namespace flexhal {
namespace framework {
namespace sdl {

// SDLの初期化状態を追跡する静的変数
static bool sdl_initialized = false;
static int window_count = 0;

Window::Window(const std::string& title, int width, int height)
    : window_(nullptr)
    , renderer_(nullptr)
    , running_(false)
{
    // SDLが初期化されていなければ初期化
    if (!sdl_initialized) {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_EVENTS) < 0) {
            std::cerr << "SDL初期化エラー: " << SDL_GetError() << std::endl;
            return;
        }
        sdl_initialized = true;
    }

    // ウィンドウ作成
    window_ = SDL_CreateWindow(
        title.c_str(),
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width,
        height,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );

    if (!window_) {
        std::cerr << "ウィンドウ作成エラー: " << SDL_GetError() << std::endl;
        return;
    }

    // レンダラー作成
    renderer_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer_) {
        std::cerr << "レンダラー作成エラー: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        return;
    }

    // 背景色を黒に設定
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    
    running_ = true;
    window_count++;
}

Window::~Window() {
    close();
}

void Window::show() {
    if (window_) {
        SDL_ShowWindow(window_);
    }
}

void Window::hide() {
    if (window_) {
        SDL_HideWindow(window_);
    }
}

void Window::close() {
    if (renderer_) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }
    
    if (window_) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
        window_count--;
    }

    // すべてのウィンドウが閉じられたらSDLを終了
    if (sdl_initialized && window_count <= 0) {
        SDL_Quit();
        sdl_initialized = false;
    }

    running_ = false;
}

bool Window::update() {
    if (!running_ || !window_ || !renderer_) {
        return false;
    }

    // イベント処理
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        // 終了イベント処理
        if (event.type == SDL_QUIT) {
            running_ = false;
            return false;
        }

        // ウィンドウ閉じるイベント
        if (event.type == SDL_WINDOWEVENT && 
            event.window.event == SDL_WINDOWEVENT_CLOSE && 
            event.window.windowID == SDL_GetWindowID(window_)) {
            running_ = false;
            return false;
        }

        // コールバック関数を呼び出し
        for (auto& callback : eventCallbacks_) {
            if (!callback(event)) {
                break;
            }
        }
    }

    // 画面クリア
    SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
    SDL_RenderClear(renderer_);

    // レンダリングコールバック呼び出し
    for (auto& callback : renderCallbacks_) {
        callback(renderer_);
    }

    // 画面更新
    SDL_RenderPresent(renderer_);

    return running_;
}

void Window::addEventCallback(EventCallback callback) {
    eventCallbacks_.push_back(callback);
}

void Window::addRenderCallback(RenderCallback callback) {
    renderCallbacks_.push_back(callback);
}

} // namespace sdl
} // namespace framework
} // namespace flexhal
