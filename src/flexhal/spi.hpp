/**
 * @file spi.hpp
 * @brief FlexHAL - SPI機能 (C++版)
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FLEXHAL_SPI_HPP
#define FLEXHAL_SPI_HPP

#include "core.hpp"
#include "gpio.hpp"
#include "../../impl/internal/spi.h"

namespace flexhal {

/**
 * @brief SPIバスを作成
 *
 * @param config バス設定
 * @return std::shared_ptr<ISPIBus> SPIバス
 */
std::shared_ptr<ISPIBus> createSPIBus(const SPIBusConfig& config);

/**
 * @brief デフォルトのSPIバスを取得
 *
 * プラットフォームのデフォルトSPIピンを使用したバスを返します
 *
 * @return std::shared_ptr<ISPIBus> SPIバス
 */
std::shared_ptr<ISPIBus> getDefaultSPIBus();

/**
 * @brief ソフトウェアSPI実装を作成
 *
 * ハードウェアSPIが利用できない場合や、任意のピンでSPIを使用したい場合に利用できます
 *
 * @return std::shared_ptr<SPIBusImplementation> ソフトウェアSPI実装
 */
std::shared_ptr<SPIBusImplementation> createSoftwareSPIImplementation();

}  // namespace flexhal

#endif  // FLEXHAL_SPI_HPP
