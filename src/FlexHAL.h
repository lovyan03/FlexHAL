/**
 * @file FlexHAL.h
 * @brief FlexHAL - 柔軟なハードウェア抽象化レイヤー (C/C++共通ヘッダ)
 * @version 0.1.0
 * @date 2025-03-28
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#ifndef FLEXHAL_H
#define FLEXHAL_H

// バージョン情報
#include "../impl/internal/version.h"

// C++環境の場合はC++専用ヘッダを使用
#ifdef __cplusplus
#include "FlexHAL.hpp"
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief FlexHALライブラリの初期化 (C言語インターフェース)
 * 
 * @return 1: 成功, 0: 失敗
 */
int flexhal_init(void);

/**
 * @brief FlexHALライブラリの終了処理 (C言語インターフェース)
 */
void flexhal_end(void);

// GPIO関連のC言語インターフェース
int flexhal_gpio_set_mode(int pin, int mode);
int flexhal_gpio_set_level(int pin, int level);
int flexhal_gpio_get_level(int pin);

// SPI関連のC言語インターフェース
int flexhal_spi_begin(int sck, int miso, int mosi);
void flexhal_spi_end(void);
int flexhal_spi_transfer(const void* tx_data, void* rx_data, int length);

// I2C関連のC言語インターフェース
int flexhal_i2c_begin(int sda, int scl);
void flexhal_i2c_end(void);
int flexhal_i2c_write(int address, const void* data, int length);
int flexhal_i2c_read(int address, void* data, int length);

#ifdef __cplusplus
}
#endif

#endif // FLEXHAL_H
