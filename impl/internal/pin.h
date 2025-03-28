/**
 * @file pin.h
 * @brief ピンインターフェース定義
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
 * @brief ピンインターフェース
 */
class IPin {
public:
    virtual ~IPin() = default;

    /**
     * @brief ピンモードを設定
     * 
     * @param mode 設定するモード
     */
    virtual void setMode(PinMode mode) = 0;

    /**
     * @brief ピンレベルを設定
     * 
     * @param level 設定するレベル
     */
    virtual void setLevel(PinLevel level) = 0;

    /**
     * @brief ピンレベルを取得
     * 
     * @return PinLevel 現在のレベル
     */
    virtual PinLevel getLevel() const = 0;

    /**
     * @brief ピン番号を取得
     * 
     * @return int ピン番号
     */
    virtual int getPinNumber() const = 0;
};

} // namespace flexhal
