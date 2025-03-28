cmake_minimum_required(VERSION 3.12)
project(FlexHAL VERSION 0.1.0)

# C++17を使うよ！
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ビルドタイプが設定されてなかったらDebugにするね！
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# プラットフォーム選択オプション追加！
set(FLEXHAL_PLATFORM "desktop" CACHE STRING "Target platform (desktop, esp32, rp2040)")

# インクルードディレクトリの設定
include_directories(${PROJECT_SOURCE_DIR})

# ソースファイルを集めるよ～
file(GLOB CORE_SOURCES
    "src/FlexHAL.h"
    "src/FlexHAL.hpp"
    "src/flexhal/*.hpp"
)

# 実装ファイルを集める
file(GLOB_RECURSE IMPL_SOURCES
    "impl/platforms/${FLEXHAL_PLATFORM}/*.cpp"
    "impl/frameworks/*/*.cpp"
    "impl/rtos/*/*.cpp"
)

# ライブラリを作成！
add_library(flexhal
    ${CORE_SOURCES}
    ${IMPL_SOURCES}
)

# プラットフォーム依存の設定
if(FLEXHAL_PLATFORM STREQUAL "desktop")
    # SDLの依存関係を追加
    find_package(SDL2 REQUIRED)
    target_include_directories(flexhal PUBLIC ${SDL2_INCLUDE_DIRS})
    target_link_libraries(flexhal PUBLIC ${SDL2_LIBRARIES})
    
    # SDLのコンパイルフラグを設定
    target_compile_definitions(flexhal PUBLIC FLEXHAL_PLATFORM_DESKTOP)
    
elseif(FLEXHAL_PLATFORM STREQUAL "esp32")
    # ESP-IDF の設定をここに追加予定！
    target_compile_definitions(flexhal PUBLIC FLEXHAL_PLATFORM_ESP32)
    
elseif(FLEXHAL_PLATFORM STREQUAL "rp2040")
    # Pico SDKの設定をここに追加予定！
    target_compile_definitions(flexhal PUBLIC FLEXHAL_PLATFORM_RP2040)
endif()

# インストール設定
install(TARGETS flexhal
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

# ヘッダーファイルのインストール
install(DIRECTORY src/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# サンプルアプリケーションのビルド設定
option(BUILD_EXAMPLES "Build example applications" ON)

if(BUILD_EXAMPLES)
    # GPIO Blinkサンプル
    add_executable(gpio_blink examples/gpio_blink/main.cpp)
    target_link_libraries(gpio_blink flexhal)
    
    # プラットフォーム固有の設定
    if(FLEXHAL_PLATFORM STREQUAL "desktop")
        # デスクトップ向けの設定
        if(WIN32)
            # Windowsの場合、SDL2.dllを実行ファイルと同じディレクトリにコピー
            add_custom_command(TARGET gpio_blink POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy_if_different
                $<TARGET_FILE:SDL2::SDL2>
                $<TARGET_FILE_DIR:gpio_blink>)
        endif()
    endif()
endif()
