# FlexHAL アーキテクチャ設計書

## 1. アーキテクチャ概要

FlexHALは3層構造のアーキテクチャを採用し、ハードウェア依存性を段階的に抽象化します。この設計により、異なるプラットフォームやフレームワーク間での移植性を高め、コードの再利用性を最大化します。

### 1.1 3層構造の基本概念

```
+---------------------------+
|      アプリケーション      |
+---------------------------+
|         FlexHAL API       |
+===========================+
| RTOS層 (タスク管理/同期)   |
+---------------------------+
| フレームワーク層 (Arduino等)|
+---------------------------+
| プラットフォーム層 (ESP32等) |
+---------------------------+
|        ハードウェア        |
+---------------------------+
```

1. **プラットフォーム層**：特定のハードウェアアーキテクチャ（ESP32、AVR、ARM等）に依存する実装
2. **フレームワーク層**：開発フレームワーク（Arduino、ESP-IDF等）に依存する実装
3. **RTOS層**：リアルタイムOS（FreeRTOS等）に依存する実装

## 2. コアコンポーネント

### 2.1 インターフェース階層

FlexHALは明確に定義されたインターフェース階層を持ち、各レイヤーの責務を分離します：

```
+------------------+     +------------------+     +------------------+
|    IDevice       |     |     IBus         |     |   ITransport     |
+------------------+     +------------------+     +------------------+
        ^                        ^                        ^
        |                        |                        |
+------------------+     +------------------+     +------------------+
|    IGPIOPin      |     |     ISPIBus      |     |  ISPITransport   |
+------------------+     +------------------+     +------------------+
        ^                        ^                        ^
        |                        |                        |
+------------------+     +------------------+     +------------------+
| ArduinoGPIOPin   |     | ArduinoSPIBus    |     | ArduinoSPITransport |
+------------------+     +------------------+     +------------------+
```

### 2.2 ファクトリパターン

各コンポーネントの生成には、ファクトリパターンを採用し、実装の詳細を隠蔽します：

```cpp
// SPIバスの作成例
std::shared_ptr<ISPIBus> bus = flexhal::spi::createBus(config);

// SPIトランスポートの作成例
std::shared_ptr<ISPITransport> transport = flexhal::spi::createTransport(bus, device_config);
```

### 2.3 プラグイン方式

新しいプラットフォームやフレームワークの追加を容易にするため、プラグイン方式を採用します：

```cpp
// 実装の追加例
bus->addImplementation(std::make_shared<framework::arduino::spi::Bus>());
```

## 3. レイヤー別設計

### 3.1 プラットフォーム層

プラットフォーム層は特定のハードウェアアーキテクチャに依存する実装を提供します。

**責務**:
- ハードウェアレジスタへの直接アクセス
- プラットフォーム固有の初期化と終了処理
- デバイス固有の最適化

**命名規則**:
- ディレクトリ: `impl/platforms/<platform_name>/`
- 名前空間: `flexhal::platform::<platform_name>`

**実装例**:
```cpp
namespace flexhal {
namespace platform {
namespace esp32 {

bool initImpl() {
    // ESP32固有の初期化処理
    return true;
}

void endImpl() {
    // ESP32固有の終了処理
}

} // namespace esp32
} // namespace platform
} // namespace flexhal
```

### 3.2 フレームワーク層

フレームワーク層は開発フレームワーク（Arduino、ESP-IDFなど）に依存する実装を提供します。

**責務**:
- フレームワークAPIの抽象化
- フレームワーク固有の機能へのアクセス
- フレームワーク間の互換性確保

**命名規則**:
- ディレクトリ: `impl/frameworks/<framework_name>/`
- 名前空間: `flexhal::framework::<framework_name>`

**実装例**:
```cpp
namespace flexhal {
namespace framework {
namespace arduino {
namespace spi {

class Bus : public SPIBusImplementation {
public:
    bool isAvailable() const override {
        return true;  // Arduinoフレームワークでは常に利用可能
    }
    
    std::shared_ptr<ISPITransport> createTransport(
        const SPIBusConfig& bus_config,
        const SPIDeviceConfig& device_config) override {
        return std::make_shared<Transport>(bus_config, device_config);
    }
};

} // namespace spi
} // namespace arduino
} // namespace framework
} // namespace flexhal
```

### 3.3 RTOS層

RTOS層はリアルタイムオペレーティングシステムに依存する機能を抽象化します。

**責務**:
- タスク管理の抽象化
- 同期プリミティブの提供
- リソース管理

**命名規則**:
- ディレクトリ: `impl/rtos/<rtos_name>/`
- 名前空間: `flexhal::rtos::<rtos_name>`

**実装例**:
```cpp
namespace flexhal {
namespace rtos {
namespace freertos {

bool initImpl() {
    // FreeRTOS固有の初期化
    return true;
}

void endImpl() {
    // FreeRTOS固有の終了処理
}

} // namespace freertos
} // namespace rtos
} // namespace flexhal
```

## 4. ディレクトリ構造

FlexHALのディレクトリ構造は、3層アーキテクチャを反映し、各コンポーネントの責務を明確に分離します：

```
flexhal/
├── docs/                     # ドキュメント
│   └── spec/                 # 仕様書
├── examples/                 # サンプルコード
│   ├── arduino/              # Arduino向けサンプル
│   ├── esp32/                # ESP32向けサンプル
│   └── native/               # デスクトップ向けサンプル
├── impl/                     # 内部実装
│   ├── internal/             # 内部インターフェース定義
│   ├── platforms/            # プラットフォーム固有実装
│   │   ├── esp32/            # ESP32プラットフォーム
│   │   └── desktop/          # デスクトップシミュレーション
│   ├── frameworks/           # フレームワーク固有実装
│   │   ├── arduino/          # Arduinoフレームワーク
│   │   └── espidf/           # ESP-IDFフレームワーク
│   └── rtos/                 # RTOS固有実装
│       ├── freertos/         # FreeRTOS実装
│       └── none/             # RTOSなし環境
└── src/                      # ユーザーAPI
    └── flexhal/              # 公開API
        ├── core.hpp          # コア定義
        ├── gpio.hpp          # GPIO API
        ├── spi/              # SPI関連API
        ├── i2c/              # I2C関連API
        └── rtos/             # RTOS関連API
```

## 5. ファイル命名規則

### 5.1 内部インターフェース定義
- 場所: `impl/internal/`
- 拡張子: `.h`
- 命名: 小文字のスネークケース
- 例: `impl/internal/spi.h`, `impl/internal/gpio.h`

### 5.2 実装ヘッダ
- 場所: `impl/frameworks/*/`, `impl/platforms/*/`, `impl/rtos/*/`
- 拡張子: `.hpp`
- 命名: 小文字のスネークケース
- 例: `impl/frameworks/arduino/spi/bus.hpp`

### 5.3 実装ソース
- 場所: 対応するヘッダと同じディレクトリ
- 拡張子: `.inl`（インライン実装）
- 命名: 対応するヘッダと同名
- 例: `impl/frameworks/arduino/spi/bus.inl`

### 5.4 ユーザーAPI
- 場所: `src/flexhal/`
- 拡張子: `.hpp`
- 命名: 小文字のスネークケース
- 例: `src/flexhal/spi.hpp`, `src/flexhal/gpio.hpp`

## 6. インターフェース設計原則

### 6.1 インターフェース分離の原則
- 各インターフェースは単一の責務を持つ
- クライアントは使用しないメソッドに依存しない

### 6.2 依存関係逆転の原則
- 上位モジュールは下位モジュールに依存しない
- 両方とも抽象化に依存する

### 6.3 リスコフの置換原則
- サブクラスはその基底クラスと置換可能である

### 6.4 開放/閉鎖の原則
- 拡張に対して開かれ、修正に対して閉じている

## 7. エラー処理戦略

FlexHALでは例外を使用せず、明示的なエラーコードと戻り値によるエラー処理を採用します：

```cpp
enum class Error : int8_t {
    None           = 0,    // エラーなし
    Timeout        = -1,   // タイムアウト
    InvalidParam   = -2,   // 不正なパラメータ
    NotSupported   = -3,   // サポートされていない機能
    NotInitialized = -4,   // 初期化されていない
    NotAvailable   = -5,   // 利用できない
    BusError       = -6,   // バスエラー
    DeviceError    = -7,   // デバイスエラー
    Unknown        = -127  // 不明なエラー
};
```

## 8. 初期化フロー

FlexHALの初期化は階層的に行われ、各レイヤーの依存関係を考慮します：

```
flexhal::init()
  ↓
  ├─ platform::init()  // プラットフォーム層の初期化
  │    ↓
  │    └─ platform::<platform_name>::initImpl()
  │
  ├─ framework::init() // フレームワーク層の初期化
  │    ↓
  │    └─ framework::<framework_name>::initImpl()
  │
  └─ rtos::init()      // RTOS層の初期化
       ↓
       └─ rtos::<rtos_name>::initImpl()
```

## 9. メモリ管理戦略

FlexHALではスマートポインタを使用して、リソース管理を安全かつ効率的に行います：

- `std::shared_ptr`：共有所有権が必要な場合
- `std::unique_ptr`：排他的所有権が必要な場合
- 生ポインタ：所有権を持たない参照の場合

## 10. 今後の拡張性

FlexHALは将来的に以下の拡張を考慮した設計となっています：

- 新しいハードウェアプラットフォームの追加
- 新しいフレームワークの統合
- 追加のデバイスドライバ（ディスプレイ、センサーなど）
- 高度なRTOS機能（タスク間通信、リソース管理）
