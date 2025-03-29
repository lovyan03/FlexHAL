/**
 * @file FlexHAL_Impl.hpp
 * @brief FlexHAL - 実装ファイルのエントリポイント
 * @version 0.1.0
 * @date 2025-03-29
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

// プラットフォーム検出
#include "internal/platform_detect.h"

//=============================================================================
// プラットフォーム層の実装
//=============================================================================
#if defined(FLEXHAL_PLATFORM_ESP32)
  #include "platforms/esp32/impl_includes.h"
#elif defined(FLEXHAL_PLATFORM_AVR)
  // AVR向け実装をインクルード
  // #include "platforms/avr/impl_includes.h"
#elif defined(FLEXHAL_PLATFORM_RP2040)
  // RP2040向け実装をインクルード
  // #include "platforms/rp2040/impl_includes.h"
#elif defined(FLEXHAL_PLATFORM_DESKTOP) || defined(FLEXHAL_PLATFORM_WINDOWS) || defined(FLEXHAL_PLATFORM_MACOS) || \
    defined(FLEXHAL_PLATFORM_LINUX)
  #include "platforms/desktop/impl_includes.h"
#endif

//=============================================================================
// フレームワーク層の実装
//=============================================================================
#if defined(FLEXHAL_PLATFORM_DESKTOP) || defined(FLEXHAL_PLATFORM_WINDOWS) || defined(FLEXHAL_PLATFORM_MACOS) || \
    defined(FLEXHAL_PLATFORM_LINUX)
  #include "frameworks/sdl/impl_includes.h"
#endif

//=============================================================================
// RTOSレイヤー向け実装
//=============================================================================
#if defined(FLEXHAL_RTOS_FREERTOS)
  #include "rtos/freertos/impl_includes.h"
#elif defined(FLEXHAL_RTOS_ZEPHYR)
  // Zephyr向け実装をインクルード
  // #include "rtos/zephyr/impl_includes.h"
#elif defined(FLEXHAL_PLATFORM_DESKTOP) || defined(FLEXHAL_PLATFORM_WINDOWS) || defined(FLEXHAL_PLATFORM_MACOS) || \
    defined(FLEXHAL_PLATFORM_LINUX)
  #include "rtos/sdl/impl_includes.h"
#else
  #include "rtos/noos/impl_includes.h"
#endif
