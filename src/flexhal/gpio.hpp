/**
 * @file gpio.hpp
 * @brief FlexHAL - GPIO機能 (C++版)
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef FLEXHAL_GPIO_HPP
#define FLEXHAL_GPIO_HPP

#include "core.hpp"
#include "../../impl/internal/pin.h"
#include "../../impl/internal/gpio.h"

namespace flexhal {

/**
 * @brief デフォルトのGPIOポートを取得
 * 
 * @return std::shared_ptr<IGPIOPort> GPIOポート
 */
std::shared_ptr<IGPIOPort> getDefaultGPIOPort();

/**
 * @brief 指定したピン番号のピンを取得
 * 
 * @param pin_number ピン番号
 * @return std::shared_ptr<IPin> ピンインスタンス
 */
std::shared_ptr<IPin> getPin(int pin_number);

} // namespace flexhal

#endif // FLEXHAL_GPIO_HPP
