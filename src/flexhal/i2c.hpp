/**
 * @file i2c.hpp
 * @brief FlexHAL - I2C機能 (C++版)
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef FLEXHAL_I2C_HPP
#define FLEXHAL_I2C_HPP

#include "core.hpp"
#include "gpio.hpp"
#include "../../impl/internal/i2c.h"

namespace flexhal {

/**
 * @brief I2Cバスを作成
 * 
 * @param config バス設定
 * @return std::shared_ptr<II2CBus> I2Cバス
 */
std::shared_ptr<II2CBus> createI2CBus(const I2CBusConfig& config);

/**
 * @brief デフォルトのI2Cバスを取得
 * 
 * プラットフォームのデフォルトI2Cピンを使用したバスを返します
 * 
 * @return std::shared_ptr<II2CBus> I2Cバス
 */
std::shared_ptr<II2CBus> getDefaultI2CBus();

/**
 * @brief ソフトウェアI2C実装を作成
 * 
 * ハードウェアI2Cが利用できない場合や、任意のピンでI2Cを使用したい場合に利用できます
 * 
 * @return std::shared_ptr<I2CBusImplementation> ソフトウェアI2C実装
 */
std::shared_ptr<I2CBusImplementation> createSoftwareI2CImplementation();

/**
 * @brief I2Cバス上のデバイスをスキャン
 * 
 * @param bus スキャンするI2Cバス
 * @return std::vector<I2CAddress> 見つかったデバイスのアドレスリスト
 */
std::vector<I2CAddress> scanI2CDevices(std::shared_ptr<II2CBus> bus);

} // namespace flexhal

#endif // FLEXHAL_I2C_HPP
