/**
 * @file version.h
 * @brief FlexHAL - バージョン情報定義
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#ifndef FLEXHAL_VERSION_H
#define FLEXHAL_VERSION_H

/**
 * @brief FlexHALのバージョン情報
 *
 * セマンティックバージョニングに従ったバージョン番号
 * MAJOR.MINOR.PATCH形式
 */
#define FLEXHAL_VERSION_MAJOR  0
#define FLEXHAL_VERSION_MINOR  1
#define FLEXHAL_VERSION_PATCH  0
#define FLEXHAL_VERSION_STRING "0.1.0"

/**
 * @brief バージョン番号を数値形式で取得
 *
 * 形式: 0xMMmmpp (MM:メジャー, mm:マイナー, pp:パッチ)
 */
#define FLEXHAL_VERSION_NUMBER ((FLEXHAL_VERSION_MAJOR << 16) | (FLEXHAL_VERSION_MINOR << 8) | FLEXHAL_VERSION_PATCH)

#endif  // FLEXHAL_VERSION_H
