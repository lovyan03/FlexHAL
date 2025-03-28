/**
 * @file platform_detect.h
 * @brief プラットフォーム、フレームワーク、RTOSの自動検出
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

//=============================================================================
// プラットフォーム検出 (ハードウェア)
//=============================================================================

#if defined(ESP32)
#define FLEXHAL_PLATFORM_ESP32
#elif defined(ESP8266)
#define FLEXHAL_PLATFORM_ESP8266
#elif defined(__AVR__)
#define FLEXHAL_PLATFORM_AVR
#elif defined(ARDUINO_ARCH_RP2040) || defined(PICO_BOARD)
#define FLEXHAL_PLATFORM_RP2040
#elif defined(ARDUINO_ARCH_SAMD)
#define FLEXHAL_PLATFORM_SAMD
#elif defined(ARDUINO_ARCH_STM32)
#define FLEXHAL_PLATFORM_STM32
#elif defined(__EMSCRIPTEN__)
#define FLEXHAL_PLATFORM_EMSCRIPTEN
#elif defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__WINDOWS__)
#define FLEXHAL_PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define FLEXHAL_PLATFORM_MACOS
#elif defined(__linux__)
#define FLEXHAL_PLATFORM_LINUX
#else
#define FLEXHAL_PLATFORM_UNKNOWN
#endif

// デスクトップ環境の検出（SDL2サポート）
#if defined(FLEXHAL_PLATFORM_WINDOWS) || defined(FLEXHAL_PLATFORM_MACOS) || defined(FLEXHAL_PLATFORM_LINUX)
#if defined(FLEXHAL_PLATFORM_DESKTOP)
// すでに定義済み
#elif __has_include(<SDL2/SDL.h>) || __has_include(<SDL.h>)
#define FLEXHAL_PLATFORM_DESKTOP
#endif
#endif

//=============================================================================
// フレームワーク検出 (ソフトウェア)
//=============================================================================

#if defined(ARDUINO)
#define FLEXHAL_FRAMEWORK_ARDUINO
#elif defined(ESP_IDF_VERSION)
#define FLEXHAL_FRAMEWORK_ESPIDF
#elif defined(__MBED__)
#define FLEXHAL_FRAMEWORK_MBED
#elif defined(ZEPHYR_VERSION)
#define FLEXHAL_FRAMEWORK_ZEPHYR
#else
#define FLEXHAL_FRAMEWORK_UNKNOWN
#endif

//=============================================================================
// RTOSレイヤー検出
//=============================================================================

#if defined(configSUPPORT_DYNAMIC_ALLOCATION) || defined(ESP_PLATFORM)  // FreeRTOS
#define FLEXHAL_RTOS_FREERTOS
#elif defined(CONFIG_ZEPHYR_VERSION) || defined(ZEPHYR_VERSION)
#define FLEXHAL_RTOS_ZEPHYR
#elif defined(__MBED__) && defined(MBED_CONF_RTOS_PRESENT)
#define FLEXHAL_RTOS_MBED
#else
#define FLEXHAL_RTOS_NONE
#endif

//=============================================================================
// 実装選択マクロ
//=============================================================================

/**
 * @brief プラットフォーム、フレームワーク、RTOSの組み合わせに基づいて最適な実装を選択するマクロ
 *
 * 使用例:
 * #include FLEXHAL_IMPL_FILE(gpio, pin)  // impl/platforms/xxx/gpio/pin.h または impl/frameworks/xxx/gpio/pin.h を選択
 */
#if defined(FLEXHAL_PLATFORM_ESP32) && defined(FLEXHAL_FRAMEWORK_ESPIDF)
#define FLEXHAL_IMPL_FILE(category, name) "../impl/frameworks/espidf/" #category "/" #name ".h"
#elif defined(FLEXHAL_PLATFORM_ESP32) && defined(FLEXHAL_FRAMEWORK_ARDUINO)
#define FLEXHAL_IMPL_FILE(category, name) "../impl/frameworks/arduino/" #category "/" #name ".h"
#elif defined(FLEXHAL_PLATFORM_AVR) && defined(FLEXHAL_FRAMEWORK_ARDUINO)
#define FLEXHAL_IMPL_FILE(category, name) "../impl/frameworks/arduino/" #category "/" #name ".h"
#else
// デフォルトはプラットフォーム固有の実装を使用
#if defined(FLEXHAL_PLATFORM_ESP32)
#define FLEXHAL_IMPL_FILE(category, name) "../impl/platforms/esp32/" #name ".hpp"
#elif defined(FLEXHAL_PLATFORM_AVR)
#define FLEXHAL_IMPL_FILE(category, name) "../impl/platforms/avr/" #category "/" #name ".h"
#elif defined(FLEXHAL_PLATFORM_RP2040)
#define FLEXHAL_IMPL_FILE(category, name) "../impl/platforms/rp2040/" #category "/" #name ".h"
#else
#define FLEXHAL_IMPL_FILE(category, name) "../impl/internal/" #name ".h"
#endif
#endif

/**
 * @brief RTOSの実装を選択するマクロ
 */
#if defined(FLEXHAL_RTOS_FREERTOS)
#define FLEXHAL_RTOS_FILE(name) "../../impl/rtos/freertos/" #name ".h"
#elif defined(FLEXHAL_RTOS_ZEPHYR)
#define FLEXHAL_RTOS_FILE(name) "../../impl/rtos/zephyr/" #name ".h"
#elif defined(FLEXHAL_PLATFORM_DESKTOP)
#define FLEXHAL_RTOS_FILE(name) "../../impl/rtos/sdl/" #name ".h"
#else
#define FLEXHAL_RTOS_FILE(name) "../../impl/rtos/noos/" #name ".h"
#endif
