/**
 * @file transport.h
 * @brief トランスポートインターフェース定義
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <cstdint>
#include <cstddef>
#include "device.h"

namespace flexhal {

/**
 * @brief トランスポートインターフェース
 *
 * データ転送の抽象化インターフェース
 */
class ITransport : public IDevice {
public:
    virtual ~ITransport() = default;

    /**
     * @brief データ書き込み
     *
     * @param data 書き込むデータ
     * @param length データ長
     * @return ssize_t 書き込んだバイト数（負の値はエラー）
     */
    virtual ssize_t write(const void* data, size_t length) = 0;

    /**
     * @brief データ読み込み
     *
     * @param data 読み込み先バッファ
     * @param length 読み込むバイト数
     * @return ssize_t 読み込んだバイト数（負の値はエラー）
     */
    virtual ssize_t read(void* data, size_t length) = 0;

    /**
     * @brief データ送受信
     *
     * @param tx_data 送信データ
     * @param rx_data 受信データ
     * @param length データ長
     * @return ssize_t 転送したバイト数（負の値はエラー）
     */
    virtual ssize_t transfer(const void* tx_data, void* rx_data, size_t length) = 0;

    /**
     * @brief 非同期操作をサポートしているか確認
     *
     * @return true サポートしている
     * @return false サポートしていない
     */
    virtual bool supportsAsync() const = 0;
};

}  // namespace flexhal
