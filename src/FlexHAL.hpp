/**
 * @file FlexHAL.hpp
 * @brief FlexHAL - 柔軟なハードウェア抽象化レイヤー (C++版)
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef FLEXHAL_HPP
#define FLEXHAL_HPP

// プラットフォーム検出
#include "../impl/internal/platform_detect.h"

// 各機能のヘッダファイルをインクルード
#include "flexhal/core.hpp"  // コア機能
#include "flexhal/gpio.hpp"  // GPIO機能
#include "flexhal/spi.hpp"   // SPI機能
#include "flexhal/i2c.hpp"   // I2C機能
#include "flexhal/rtos.hpp"  // RTOS機能

// 以下は後方互換性のためにグローバル名前空間にエクスポート
// 新しいコードでは各機能のヘッダファイルを直接インクルードすることを推奨

// バージョン情報をグローバル名前空間にエクスポート
using flexhal::Version;

// 初期化/終了処理関数をグローバル名前空間にエクスポート
using flexhal::init;
using flexhal::end;

#endif // FLEXHAL_HPP
