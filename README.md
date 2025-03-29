# FlexHAL

> **重要なお知らせ**: このリポジトリは**Windsurf AIを使用したライブラリ開発の実験的プロジェクト**です。現時点では一般利用を推奨しておらず、APIや機能は予告なく変更される可能性があります。また、現在はIssueやプルリクエストを受け付けておりません。

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Arduino Library](https://img.shields.io/badge/Arduino-Library-blue)](https://www.arduino.cc/reference/en/libraries/)
[![Experimental](https://img.shields.io/badge/Status-Experimental-red)]()

FlexHALは柔軟なハードウェア抽象化レイヤー（HAL）ライブラリです。ESP32やRP2040などのマイコン向けの開発を効率化し、さらにSDLを使用してデスクトップ環境でのエミュレーションも可能にします。

## 特徴 ✨

- マルチプラットフォーム対応（ESP32, RP2040, SDL）
- シンプルで一貫性のあるAPI
- デスクトップ環境でのエミュレーション機能
- 豊富なドライバーライブラリ（開発中）

## 必要な環境 🛠

- C++11以上に対応したコンパイラ（C++11の機能を使用）
- CMake 3.12以上
- 各プラットフォームに応じた開発環境:
  - ESP32: ESP-IDF または Arduino ESP32
  - RP2040: Pico SDK または Arduino RP2040
  - SDL: SDL2ライブラリ（デスクトップシミュレーション用）

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

### ロガーの使用例

```cpp
#include <FlexHAL.hpp>

using namespace flexhal;

void setup() {
  // ライブラリの初期化（ロガーも自動的に初期化されます）
  init();
  
  // 各レベルのログ出力
  debug("Debugレベルのログメッセージ");
  info("Infoレベルのログメッセージ");
  warning("Warningレベルのログメッセージ");
  error("Errorレベルのログメッセージ");
  fatal("Fatalレベルのログメッセージ");
  
  // フォーマット付きログ出力の例
  char buffer[100];
  snprintf(buffer, sizeof(buffer), "バージョン: %d.%d.%d", 1, 2, 3);
  info(buffer);
}

void loop() {
  // 定期的なログ出力
  static unsigned long lastLog = 0;
  unsigned long now = millis();
  
  if (now - lastLog > 5000) {  // 5秒ごとにログ出力
    lastLog = now;
    info("定期実行中...");
  }
}
```

#### カスタムロガーの実装例

```cpp
#include <FlexHAL.hpp>

using namespace flexhal;

// 独自ロガーの実装
 class MyCustomLogger : public ILogger {
public:
  void log(LogLevel level, const char* message) override {
    // 独自のログ出力処理を実装
    // 例: SDカードや外部デバイスにログを保存するなど
    Serial.print("[カスタムロガー] ");
    
    switch (level) {
      case LogLevel::Debug:   Serial.print("DEBUG: "); break;
      case LogLevel::Info:    Serial.print("INFO: "); break;
      case LogLevel::Warning: Serial.print("WARN: "); break;
      case LogLevel::Error:   Serial.print("ERROR: "); break;
      case LogLevel::Fatal:   Serial.print("FATAL: "); break;
    }
    
    Serial.println(message);
  }
};

void setup() {
  Serial.begin(115200);
  
  // ライブラリの初期化
  init();
  
  // カスタムロガーを設定
  static MyCustomLogger customLogger;
  setLogger(&customLogger);
  
  // ログ出力テスト
  info("カスタムロガーを使用しています");
}

void loop() {
  // メイン処理
}
```

## アーキテクチャ 🏗️

### 3層構造

FlexHALは独自の3層構造を採用しています：

1. **プラットフォーム層** - ハードウェア固有の実装（ESP32, AVR, RP2040, デスクトップ）
   - 場所: `impl/platforms/*/`
   - 例: `impl/platforms/esp32/gpio_arduino.hpp` - ESP32プラットフォーム向けのArduinoフレームワーク実装
   - 例: `impl/platforms/esp32/gpio_native.hpp` - ESP32プラットフォーム向けのネイティブ実装

2. **フレームワーク層** - フレームワーク固有の実装（Arduino, ESP-IDF, SDL）
   - 場所: `impl/frameworks/*/`
   - 例: `impl/frameworks/arduino/gpio.hpp` - Arduinoフレームワーク共通の実装
   - 例: `impl/frameworks/sdl/gpio.hpp` - SDLフレームワーク向けの実装

3. **RTOS層** - RTOS固有の実装（FreeRTOS, NoOS, SDL）
   - 場所: `impl/rtos/*/`
   - 例: `impl/rtos/freertos/mutex.h` - FreeRTOS向けのミューテックス実装
   - 例: `impl/rtos/noos/task.h` - RTOSなしの環境向けのタスク実装

### 命名規則と実装の分離

FlexHALでは、各層の責任を明確にするために以下の命名規則を採用しています：

- **プラットフォーム固有の実装**: `impl/platforms/<platform>/<feature>_<framework>.hpp`
  - 例: `impl/platforms/esp32/gpio_arduino.hpp` - ESP32プラットフォーム上のArduinoフレームワーク向けGPIO実装

- **フレームワーク共通の実装**: `impl/frameworks/<framework>/<feature>.hpp`
  - 例: `impl/frameworks/arduino/gpio.hpp` - Arduinoフレームワーク共通のGPIO実装

この構造により、共通コードの再利用が容易になり、新しいプラットフォームやフレームワークの追加が簡単になります。

### エントリーポイント方式 🔗

FlexHALは「エントリーポイント方式」を採用しています。これにより、Arduino IDEでのビルドとデスクトップシミュレーションの両方に対応しています。

- **src/FlexHAL_Impl.cpp** - メインのエントリーポイントファイル
- **impl/*/impl_includes.h** - 各環境ごとの実装ファイルをまとめたインクルードファイル

## ライセンス 📜

MIT License

## 貢献について 🤝

バグ報告や機能要望、プルリクエストは大歓迎です！

[Issues](https://github.com/lovyan03/FlexHAL/issues)や[Pull Requests](https://github.com/lovyan03/FlexHAL/pulls)をお待ちしています。
