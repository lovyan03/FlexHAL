/**
 * @file platform_detect.h
 * @brief Platform detection for FlexHAL
 * @author lovyan03
 * @date 2025/03/29
 * @version 0.1.0
 * 
 * @copyright Copyright (c) 2025 lovyan03
 * 
 * @details
 * This file provides macros to detect the platform, framework, and RTOS.
 * It is used internally by FlexHAL to select the appropriate implementation.
 */

#pragma once

#include <cstdint>

//-----------------------------------------------------------------------------
// Platform detection
//-----------------------------------------------------------------------------

// Undefine all platform macros first to avoid conflicts
#undef FLEXHAL_PLATFORM_ESP32
#undef FLEXHAL_PLATFORM_ESP8266
#undef FLEXHAL_PLATFORM_RP2040
#undef FLEXHAL_PLATFORM_STM32
#undef FLEXHAL_PLATFORM_SAMD
#undef FLEXHAL_PLATFORM_DESKTOP
#undef FLEXHAL_PLATFORM_WINDOWS
#undef FLEXHAL_PLATFORM_LINUX
#undef FLEXHAL_PLATFORM_MACOS

// ESP32 detection
#if defined(ESP32) || defined(CONFIG_IDF_TARGET_ESP32) || defined(CONFIG_IDF_TARGET_ESP32S2) || defined(CONFIG_IDF_TARGET_ESP32S3) || defined(CONFIG_IDF_TARGET_ESP32C3)
  #define FLEXHAL_PLATFORM_ESP32 1
  
// ESP8266 detection
#elif defined(ESP8266) || defined(ARDUINO_ARCH_ESP8266)
  #define FLEXHAL_PLATFORM_ESP8266 1
  
// RP2040 detection
#elif defined(ARDUINO_ARCH_RP2040) || defined(PICO_SDK_VERSION_STRING)
  #define FLEXHAL_PLATFORM_RP2040 1
  
// STM32 detection
#elif defined(ARDUINO_ARCH_STM32) || defined(STM32_CORE_VERSION)
  #define FLEXHAL_PLATFORM_STM32 1
  
// SAMD detection (SAMD21, SAMD51)
#elif defined(ARDUINO_ARCH_SAMD)
  #define FLEXHAL_PLATFORM_SAMD 1
  
// Desktop platform detection
#elif defined(_WIN32) || defined(_WIN64) || defined(__APPLE__) || defined(__linux__)
  #define FLEXHAL_PLATFORM_DESKTOP 1
  
  // Windows detection
  #if defined(_WIN32) || defined(_WIN64)
    #define FLEXHAL_PLATFORM_WINDOWS 1
  
  // macOS detection
  #elif defined(__APPLE__)
    #define FLEXHAL_PLATFORM_MACOS 1
  
  // Linux detection
  #elif defined(__linux__)
    #define FLEXHAL_PLATFORM_LINUX 1
  #endif
  
#else
  #warning "FlexHAL: Unknown platform. Using generic implementation."
#endif

//-----------------------------------------------------------------------------
// Framework detection
//-----------------------------------------------------------------------------

// Undefine all framework macros first to avoid conflicts
#undef FLEXHAL_FRAMEWORK_ARDUINO
#undef FLEXHAL_FRAMEWORK_ESP_IDF
#undef FLEXHAL_FRAMEWORK_SDL

// Arduino framework detection
#if defined(ARDUINO) || defined(ARDUINO_ARCH_ESP32) || defined(ARDUINO_ARCH_ESP8266) || defined(ARDUINO_ARCH_RP2040) || defined(ARDUINO_ARCH_STM32) || defined(ARDUINO_ARCH_SAMD)
  #define FLEXHAL_FRAMEWORK_ARDUINO 1
  
// ESP-IDF framework detection
#elif defined(ESP_PLATFORM) || defined(IDF_VER)
  #define FLEXHAL_FRAMEWORK_ESP_IDF 1
  
// SDL framework detection (for desktop simulation)
#elif defined(FLEXHAL_PLATFORM_DESKTOP) && (defined(SDL_VERSION) || defined(SDL_h_) || defined(__has_include) && __has_include(<SDL2/SDL.h>))
  #define FLEXHAL_FRAMEWORK_SDL 1
  
#endif

//-----------------------------------------------------------------------------
// RTOS detection
//-----------------------------------------------------------------------------

// Undefine all RTOS macros first to avoid conflicts
#undef FLEXHAL_RTOS_FREERTOS
#undef FLEXHAL_RTOS_DESKTOP

// FreeRTOS detection
#if defined(ESP32) || defined(ESP_PLATFORM) || defined(IDF_VER) || defined(portBASE_TYPE) || defined(configSUPPORT_DYNAMIC_ALLOCATION) || (defined(__has_include) && __has_include("freertos/FreeRTOS.h"))
  #define FLEXHAL_RTOS_FREERTOS 1
  
// Desktop OS detection
#elif defined(FLEXHAL_PLATFORM_DESKTOP)
  #define FLEXHAL_RTOS_DESKTOP 1
  
#endif

//-----------------------------------------------------------------------------
// Helper macros for implementation selection
//-----------------------------------------------------------------------------

namespace flexhal {

/**
 * @brief Platform enumeration
 */
enum class Platform : uint8_t {
  Unknown = 0,
  ESP32,
  ESP8266,
  RP2040,
  STM32,
  SAMD,
  Desktop
};

/**
 * @brief Framework enumeration
 */
enum class Framework : uint8_t {
  Unknown = 0,
  Arduino,
  EspIdf,
  SDL
};

/**
 * @brief RTOS enumeration
 */
enum class RTOS : uint8_t {
  Unknown = 0,
  FreeRTOS,
  Desktop
};

/**
 * @brief Get the current platform
 * @return Platform enum value
 */
inline Platform getPlatform() {
#if defined(FLEXHAL_PLATFORM_ESP32)
  return Platform::ESP32;
#elif defined(FLEXHAL_PLATFORM_ESP8266)
  return Platform::ESP8266;
#elif defined(FLEXHAL_PLATFORM_RP2040)
  return Platform::RP2040;
#elif defined(FLEXHAL_PLATFORM_STM32)
  return Platform::STM32;
#elif defined(FLEXHAL_PLATFORM_SAMD)
  return Platform::SAMD;
#elif defined(FLEXHAL_PLATFORM_DESKTOP)
  return Platform::Desktop;
#else
  return Platform::Unknown;
#endif
}

/**
 * @brief Check if a specific framework is available
 * @param framework Framework to check
 * @return true if the framework is available
 */
inline bool hasFramework(Framework framework) {
  switch (framework) {
    case Framework::Arduino:
#ifdef FLEXHAL_FRAMEWORK_ARDUINO
      return true;
#else
      return false;
#endif
    case Framework::EspIdf:
#ifdef FLEXHAL_FRAMEWORK_ESP_IDF
      return true;
#else
      return false;
#endif
    case Framework::SDL:
#ifdef FLEXHAL_FRAMEWORK_SDL
      return true;
#else
      return false;
#endif
    default:
      return false;
  }
}

/**
 * @brief Get the primary framework
 * @return Framework enum value representing the primary framework
 */
inline Framework getPrimaryFramework() {
#if defined(FLEXHAL_FRAMEWORK_ARDUINO)
  return Framework::Arduino;
#elif defined(FLEXHAL_FRAMEWORK_ESP_IDF)
  return Framework::EspIdf;
#elif defined(FLEXHAL_FRAMEWORK_SDL)
  return Framework::SDL;
#else
  return Framework::Unknown;
#endif
}

/**
 * @brief Get the current framework (deprecated, use getPrimaryFramework instead)
 * @return Framework enum value
 */
inline Framework getFramework() {
  return getPrimaryFramework();
}

/**
 * @brief Check if a specific RTOS is available
 * @param rtos RTOS to check
 * @return true if the RTOS is available
 */
inline bool hasRTOS(RTOS rtos) {
  switch (rtos) {
    case RTOS::FreeRTOS:
#ifdef FLEXHAL_RTOS_FREERTOS
      return true;
#else
      return false;
#endif
    case RTOS::Desktop:
#ifdef FLEXHAL_RTOS_DESKTOP
      return true;
#else
      return false;
#endif
    default:
      return false;
  }
}

/**
 * @brief Get the primary RTOS
 * @return RTOS enum value representing the primary RTOS
 */
inline RTOS getPrimaryRTOS() {
#if defined(FLEXHAL_RTOS_FREERTOS)
  return RTOS::FreeRTOS;
#elif defined(FLEXHAL_RTOS_DESKTOP)
  return RTOS::Desktop;
#else
  return RTOS::Unknown;
#endif
}

/**
 * @brief Get the current RTOS (deprecated, use getPrimaryRTOS instead)
 * @return RTOS enum value
 */
inline RTOS getRTOS() {
  return getPrimaryRTOS();
}

} // namespace flexhal
