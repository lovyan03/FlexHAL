/**
 * @file core.h
 * @brief FlexHAL コア定義
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cstdint>
#include <memory>

namespace flexhal {

/**
 * @brief エラーコード定義
 */
enum class Error : int8_t {
    None           = 0,    ///< エラーなし
    Timeout        = -1,   ///< タイムアウト
    InvalidParam   = -2,   ///< 不正なパラメータ
    NotSupported   = -3,   ///< サポートされていない機能
    NotInitialized = -4,   ///< 初期化されていない
    NotAvailable   = -5,   ///< 利用できない
    BusError       = -6,   ///< バスエラー
    DeviceError    = -7,   ///< デバイスエラー
    Unknown        = -127  ///< 不明なエラー
};

/**
 * @brief ピンモード定義
 */
enum class PinMode : uint8_t {
    Input         = 0,   ///< 入力
    Output        = 1,   ///< 出力
    InputPullUp   = 2,   ///< 入力（プルアップ）
    InputPullDown = 3,   ///< 入力（プルダウン）
    OpenDrain     = 4,   ///< オープンドレイン出力
    Analog        = 5,   ///< アナログ
    Undefined     = 255  ///< 未定義
};

/**
 * @brief ピンレベル定義
 */
enum class PinLevel : uint8_t {
    Low       = 0,   ///< Low
    High      = 1,   ///< High
    Undefined = 255  ///< 未定義
};

/**
 * @brief SPIモード定義
 */
enum class SPIMode : uint8_t {
    Mode0 = 0,  ///< CPOL=0, CPHA=0
    Mode1 = 1,  ///< CPOL=0, CPHA=1
    Mode2 = 2,  ///< CPOL=1, CPHA=0
    Mode3 = 3,  ///< CPOL=1, CPHA=1
};

/**
 * @brief SPIビットオーダー定義
 */
enum class SPIBitOrder : uint8_t {
    MSBFirst = 0,  ///< MSBファースト
    LSBFirst = 1,  ///< LSBファースト
};

/**
 * @brief I2Cアドレス定義
 */
using I2CAddress = uint16_t;

}  // namespace flexhal
