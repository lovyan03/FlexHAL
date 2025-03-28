/**
 * @file gpio.h
 * @brief GPIOポートインターフェース定義
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

#include <memory>
#include <vector>
#include "device.h"
#include "pin.h"

namespace flexhal {

/**
 * @brief GPIOポートインターフェース
 */
class IGPIOPort : public IDevice {
public:
    virtual ~IGPIOPort() = default;

    /**
     * @brief ピンを取得
     *
     * @param pin_number ピン番号
     * @return std::shared_ptr<IPin> ピンインスタンス
     */
    virtual std::shared_ptr<IPin> getPin(int pin_number) = 0;

    /**
     * @brief 複数ピンのレベルを一度に設定
     *
     * @param values 設定する値（ビットマップ）
     * @param mask 設定対象のピン（ビットマップ）
     */
    virtual void setLevels(uint32_t values, uint32_t mask) = 0;

    /**
     * @brief 複数ピンのレベルを一度に取得
     *
     * @return uint32_t 現在のレベル（ビットマップ）
     */
    virtual uint32_t getLevels() const = 0;
};

/**
 * @brief ピン配列ポート
 *
 * 複数のピンをグループ化して管理するためのクラス
 */
class PinArrayPort : public IGPIOPort {
public:
    /**
     * @brief コンストラクタ
     */
    PinArrayPort();

    /**
     * @brief コンストラクタ
     *
     * @param pins ピンの配列
     */
    explicit PinArrayPort(const std::vector<std::shared_ptr<IPin>>& pins);

    bool begin() override;
    void end() override;
    bool isReady() const override;

    std::shared_ptr<IPin> getPin(int pin_number) override;
    void setLevels(uint32_t values, uint32_t mask) override;
    uint32_t getLevels() const override;

    /**
     * @brief ピンを追加
     *
     * @param pin 追加するピン
     */
    void addPin(std::shared_ptr<IPin> pin);

protected:
    std::vector<std::shared_ptr<IPin>> pins_;
    bool initialized_ = false;
};

/**
 * @brief SPIピンポート
 *
 * SPI通信用のピングループ
 */
class SPIPinPort : public PinArrayPort {
public:
    /**
     * @brief コンストラクタ
     *
     * @param sck SCKピン
     * @param miso MISOピン
     * @param mosi MOSIピン
     * @param cs CSピン
     */
    SPIPinPort(std::shared_ptr<IPin> sck, std::shared_ptr<IPin> miso, std::shared_ptr<IPin> mosi,
               std::shared_ptr<IPin> cs = nullptr);

    /**
     * @brief SCKピンを取得
     *
     * @return std::shared_ptr<IPin> SCKピン
     */
    std::shared_ptr<IPin> getSCK() const;

    /**
     * @brief MISOピンを取得
     *
     * @return std::shared_ptr<IPin> MISOピン
     */
    std::shared_ptr<IPin> getMISO() const;

    /**
     * @brief MOSIピンを取得
     *
     * @return std::shared_ptr<IPin> MOSIピン
     */
    std::shared_ptr<IPin> getMOSI() const;

    /**
     * @brief CSピンを取得
     *
     * @return std::shared_ptr<IPin> CSピン
     */
    std::shared_ptr<IPin> getCS() const;

private:
    enum { SCK_INDEX = 0, MISO_INDEX = 1, MOSI_INDEX = 2, CS_INDEX = 3 };
};

/**
 * @brief I2Cピンポート
 *
 * I2C通信用のピングループ
 */
class I2CPinPort : public PinArrayPort {
public:
    /**
     * @brief コンストラクタ
     *
     * @param sda SDAピン
     * @param scl SCLピン
     */
    I2CPinPort(std::shared_ptr<IPin> sda, std::shared_ptr<IPin> scl);

    /**
     * @brief SDAピンを取得
     *
     * @return std::shared_ptr<IPin> SDAピン
     */
    std::shared_ptr<IPin> getSDA() const;

    /**
     * @brief SCLピンを取得
     *
     * @return std::shared_ptr<IPin> SCLピン
     */
    std::shared_ptr<IPin> getSCL() const;

private:
    enum { SDA_INDEX = 0, SCL_INDEX = 1 };
};

}  // namespace flexhal
