# FlexHAL

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue)](https://www.arduino.cc/reference/en/libraries/)

FlexHALは柔軟なハードウェア抽象化レイヤー（HAL）ライブラリです。ESP32やRP2040などのマイコン向けの開発を効率化し、さらにSDLを使用してデスクトップ環境でのエミュレーションも可能にします。

## 特徴 ✨

- マルチプラットフォーム対応（ESP32, RP2040, SDL）
- シンプルで一貫性のあるAPI
- デスクトップ環境でのエミュレーション機能
- 豊富なドライバーライブラリ（開発中）

## 必要な環境 🛠

- C++17対応コンパイラ
- CMake 3.12以上
- 各プラットフォームに応じた開発環境:
  - ESP32: ESP-IDF
  - RP2040: Pico SDK
  - SDL: SDL2ライブラリ

## ビルド方法 🔨

### デスクトップシミュレーション

```bash
# SDLライブラリをインストール
# macOS: brew install sdl2
# Ubuntu: sudo apt install libsdl2-dev

# ビルドと実行
chmod +x build.sh
./build.sh
./build/main
```

### Arduino ESP32

Arduino IDEでプロジェクトを開き、ボード設定でESP32を選択してビルドします。

## 使用例 💡

### GPIOの使用例

```cpp
#include <FlexHAL.hpp>

using namespace flexhal;

void setup() {
  // GPIOピンの設定
  GPIO::getInstance().setPinMode(2, GPIO::PinMode::OUTPUT);
  
  // LEDを点灯
  GPIO::getInstance().digitalWrite(2, GPIO::PinState::HIGH);
}

void loop() {
  // LEDを点滅
  GPIO::getInstance().digitalWrite(2, GPIO::PinState::HIGH);
  delay(500);
  GPIO::getInstance().digitalWrite(2, GPIO::PinState::LOW);
  delay(500);
}
```

## エントリーポイント方式について 🔗

FlexHALは「エントリーポイント方式」を採用しています。これにより、Arduino IDEでのビルドとデスクトップシミュレーションの両方に対応しています。

- **src/FlexHAL_Impl.cpp** - メインのエントリーポイントファイル
- **impl/*/impl_includes.h** - 各環境ごとの実装ファイルをまとめたインクルードファイル

## ライセンス 📜

MIT License

## 貢献について 🤝

バグ報告や機能要望、プルリクエストは大歓迎です！

[Issues](https://github.com/lovyan03/FlexHAL/issues)や[Pull Requests](https://github.com/lovyan03/FlexHAL/pulls)をお待ちしています。
