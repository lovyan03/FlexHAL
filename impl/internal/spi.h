/**
 * @file spi.h
 * @brief SPI通信インターフェース定義
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <memory>
#include "core.h"
#include "transport.h"
#include "pin.h"

namespace flexhal {

/**
 * @brief SPIバス設定
 */
struct SPIBusConfig {
    int sck_pin  = -1;  ///< SCKピン番号
    int miso_pin = -1;  ///< MISOピン番号
    int mosi_pin = -1;  ///< MOSIピン番号
};

/**
 * @brief SPIデバイス設定
 */
struct SPIDeviceConfig {
    int cs_pin            = -1;                     ///< CSピン番号
    uint32_t clock_hz     = 1000000;                ///< クロック周波数（Hz）
    SPIMode mode          = SPIMode::Mode0;         ///< SPIモード
    SPIBitOrder bit_order = SPIBitOrder::MSBFirst;  ///< ビットオーダー
};

/**
 * @brief SPIトランスポートインターフェース
 */
class ISPITransport : public ITransport {
public:
    virtual ~ISPITransport() = default;

    /**
     * @brief クロック周波数を設定
     *
     * @param hz クロック周波数（Hz）
     */
    virtual void setClockFrequency(uint32_t hz) = 0;

    /**
     * @brief SPIモードを設定
     *
     * @param mode SPIモード
     */
    virtual void setMode(SPIMode mode) = 0;

    /**
     * @brief ビットオーダーを設定
     *
     * @param lsb_first trueならLSBファースト、falseならMSBファースト
     */
    virtual void setLSBFirst(bool lsb_first) = 0;

    /**
     * @brief DCピンを設定（ディスプレイなどで使用）
     *
     * @param dc_level DCピンのレベル
     */
    virtual void setDC(bool dc_level) = 0;
};

/**
 * @brief SPIバス実装インターフェース
 */
class SPIBusImplementation {
public:
    virtual ~SPIBusImplementation() = default;

    /**
     * @brief 実装が利用可能か確認
     *
     * @return true 利用可能
     * @return false 利用不可
     */
    virtual bool isAvailable() const = 0;

    /**
     * @brief トランスポートを作成
     *
     * @param bus_config バス設定
     * @param device_config デバイス設定
     * @return std::shared_ptr<ISPITransport> トランスポート
     */
    virtual std::shared_ptr<ISPITransport> createTransport(const SPIBusConfig& bus_config,
                                                           const SPIDeviceConfig& device_config) = 0;
};

/**
 * @brief SPIバスインターフェース
 */
class ISPIBus : public IDevice {
public:
    virtual ~ISPIBus() = default;

    /**
     * @brief トランスポートを取得（自動実装選択）
     *
     * @param device_config デバイス設定
     * @return std::shared_ptr<ISPITransport> トランスポート
     */
    virtual std::shared_ptr<ISPITransport> getTransport(const SPIDeviceConfig& device_config) = 0;

    /**
     * @brief トランスポートを取得（実装指定）
     *
     * @param device_config デバイス設定
     * @param implementation 使用する実装
     * @return std::shared_ptr<ISPITransport> トランスポート
     */
    virtual std::shared_ptr<ISPITransport> getTransport(const SPIDeviceConfig& device_config,
                                                        std::shared_ptr<SPIBusImplementation> implementation) = 0;
};

/**
 * @brief SPIバス実装
 */
class SPIBus : public ISPIBus {
public:
    /**
     * @brief コンストラクタ
     *
     * @param config バス設定
     */
    explicit SPIBus(const SPIBusConfig& config);

    bool begin() override;
    void end() override;
    bool isReady() const override;

    std::shared_ptr<ISPITransport> getTransport(const SPIDeviceConfig& device_config) override;
    std::shared_ptr<ISPITransport> getTransport(const SPIDeviceConfig& device_config,
                                                std::shared_ptr<SPIBusImplementation> implementation) override;

    /**
     * @brief 実装を追加
     *
     * @param implementation 追加する実装
     */
    void addImplementation(std::shared_ptr<SPIBusImplementation> implementation);

private:
    SPIBusConfig config_;
    std::vector<std::shared_ptr<SPIBusImplementation>> implementations_;
    bool initialized_ = false;
};

}  // namespace flexhal
