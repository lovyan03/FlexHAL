/**
 * @file device.h
 * @brief デバイスインターフェース定義
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include "core.h"

namespace flexhal {

/**
 * @brief デバイスインターフェース
 * 
 * すべてのデバイスの基底インターフェース
 */
class IDevice {
public:
    virtual ~IDevice() = default;

    /**
     * @brief デバイスの初期化
     * 
     * @return true 初期化成功
     * @return false 初期化失敗
     */
    virtual bool begin() = 0;

    /**
     * @brief デバイスの終了処理
     */
    virtual void end() = 0;

    /**
     * @brief デバイスが準備完了しているか確認
     * 
     * @return true 準備完了
     * @return false 準備未完了
     */
    virtual bool isReady() const = 0;
};

} // namespace flexhal
