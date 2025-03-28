/**
 * @file FlexHAL_Impl.cpp
 * @brief FlexHAL - 実装ファイルのインクルード (Arduino IDE向け)
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// このファイルはArduino IDEでビルドするために、
// implフォルダ内の実装ファイルをインクルードするためのものです。

// プラットフォーム検出
#include "../impl/internal/platform_detect.h"

//=============================================================================
// プラットフォーム層の実装
//=============================================================================
#if defined(FLEXHAL_PLATFORM_ESP32)
  #include "../impl/platforms/esp32/impl_includes.h"
#elif defined(FLEXHAL_PLATFORM_AVR)
  // AVR向け実装をインクルード
  // #include "../impl/platforms/avr/impl_includes.h"
#elif defined(FLEXHAL_PLATFORM_RP2040)
  // RP2040向け実装をインクルード
  // #include "../impl/platforms/rp2040/impl_includes.h"
#elif defined(FLEXHAL_PLATFORM_DESKTOP)
  #include "../impl/platforms/desktop/impl_includes.h"
#endif



//=============================================================================
// RTOSレイヤー向け実装
//=============================================================================
#if defined(FLEXHAL_RTOS_FREERTOS)
  #include "../impl/rtos/freertos/impl_includes.h"
#elif defined(FLEXHAL_RTOS_ZEPHYR)
  // Zephyr向け実装をインクルード
  // #include "../impl/rtos/zephyr/impl_includes.h"
#elif defined(FLEXHAL_PLATFORM_DESKTOP)
  #include "../impl/rtos/sdl/impl_includes.h"
#else
  #include "../impl/rtos/noos/impl_includes.h"
#endif
