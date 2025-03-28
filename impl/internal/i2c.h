/**
 * @file i2c.h
 * @brief I2C通信インターフェース定義
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#pragma once

#include <memory>
#include <vector>
#include "core.h"
#include "transport.h"
#include "pin.h"

namespace flexhal {

/**
 * @brief I2Cバス設定
 */
struct I2CBusConfig {
    int sda_pin = -1;   ///< SDAピン番号
    int scl_pin = -1;   ///< SCLピン番号
};

/**
 * @brief I2Cデバイス設定
 */
struct I2CDeviceConfig {
    I2CAddress address = 0;     ///< I2Cアドレス
    uint32_t clock_hz = 100000; ///< クロック周波数（Hz）
};

/**
 * @brief I2Cトランスポートインターフェース
 */
class II2CTransport : public ITransport {
public:
    virtual ~II2CTransport() = default;

    /**
     * @brief I2Cアドレスを設定
     * 
     * @param address I2Cアドレス
     */
    virtual void setAddress(I2CAddress address) = 0;

    /**
     * @brief バス上のデバイスをスキャン
     * 
     * @return std::vector<I2CAddress> 見つかったデバイスのアドレスリスト
     */
    virtual std::vector<I2CAddress> scan() = 0;

    /**
     * @brief 指定アドレスのデバイスが存在するか確認
     * 
     * @param address 確認するアドレス
     * @return true デバイスが存在する
     * @return false デバイスが存在しない
     */
    virtual bool probe(I2CAddress address) = 0;
};

/**
 * @brief I2Cバス実装インターフェース
 */
class I2CBusImplementation {
public:
    virtual ~I2CBusImplementation() = default;

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
     * @return std::shared_ptr<II2CTransport> トランスポート
     */
    virtual std::shared_ptr<II2CTransport> createTransport(
        const I2CBusConfig& bus_config, 
        const I2CDeviceConfig& device_config) = 0;
};

/**
 * @brief I2Cバスインターフェース
 */
class II2CBus : public IDevice {
public:
    virtual ~II2CBus() = default;

    /**
     * @brief トランスポートを取得（自動実装選択）
     * 
     * @param device_config デバイス設定
     * @return std::shared_ptr<II2CTransport> トランスポート
     */
    virtual std::shared_ptr<II2CTransport> getTransport(const I2CDeviceConfig& device_config) = 0;

    /**
     * @brief トランスポートを取得（実装指定）
     * 
     * @param device_config デバイス設定
     * @param implementation 使用する実装
     * @return std::shared_ptr<II2CTransport> トランスポート
     */
    virtual std::shared_ptr<II2CTransport> getTransport(
        const I2CDeviceConfig& device_config,
        std::shared_ptr<I2CBusImplementation> implementation) = 0;
};

/**
 * @brief I2Cバス実装
 */
class I2CBus : public II2CBus {
public:
    /**
     * @brief コンストラクタ
     * 
     * @param config バス設定
     */
    explicit I2CBus(const I2CBusConfig& config);

    bool begin() override;
    void end() override;
    bool isReady() const override;

    std::shared_ptr<II2CTransport> getTransport(const I2CDeviceConfig& device_config) override;
    std::shared_ptr<II2CTransport> getTransport(
        const I2CDeviceConfig& device_config,
        std::shared_ptr<I2CBusImplementation> implementation) override;

    /**
     * @brief 実装を追加
     * 
     * @param implementation 追加する実装
     */
    void addImplementation(std::shared_ptr<I2CBusImplementation> implementation);

private:
    I2CBusConfig config_;
    std::vector<std::shared_ptr<I2CBusImplementation>> implementations_;
    bool initialized_ = false;
};

} // namespace flexhal
