# FlexHAL コード実装例

## 1. コアインターフェース実装例

### 1.1 コア定義（src/flexhal/core.hpp）

```cpp
#pragma once

#include <cstdint>
#include <cstddef>

namespace flexhal {

// デバイスタイプ
enum class DeviceType : uint8_t {
    Unknown,
    GPIO,
    SPI,
    I2C,
    UART,
    Timer,
    Task,
    Semaphore,
    Mutex,
    Queue
};

// バスタイプ
enum class BusType : uint8_t {
    Unknown,
    SPI,
    I2C,
    UART
};

// ピンモード
enum class PinMode : uint8_t {
    Input,
    Output,
    InputPullUp,
    InputPullDown,
    OutputOpenDrain,
    Analog
};

// ピンレベル
enum class PinLevel : uint8_t {
    Low,
    High
};

// SPIモード
enum class SPIMode : uint8_t {
    Mode0,  // CPOL=0, CPHA=0
    Mode1,  // CPOL=0, CPHA=1
    Mode2,  // CPOL=1, CPHA=0
    Mode3   // CPOL=1, CPHA=1
};

// SPIビット順序
enum class SPIBitOrder : uint8_t {
    MSBFirst,
    LSBFirst
};

// エラーコード
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

// FlexHAL全体の初期化
bool init();

// FlexHAL全体の終了処理
void end();

} // namespace flexhal
```

### 1.2 デバイスインターフェース（src/flexhal/interfaces/idevice.hpp）

```cpp
#pragma once

#include "../core.hpp"

namespace flexhal {
namespace interfaces {

class IDevice {
public:
    virtual ~IDevice() = default;
    
    // デバイスの初期化
    virtual bool begin() = 0;
    
    // デバイスの終了処理
    virtual void end() = 0;
    
    // デバイスが初期化されているかどうか
    virtual bool isInitialized() const = 0;
    
    // デバイスタイプの取得
    virtual DeviceType getType() const = 0;
};

} // namespace interfaces
} // namespace flexhal
```

### 1.3 バスインターフェース（src/flexhal/interfaces/ibus.hpp）

```cpp
#pragma once

#include "idevice.hpp"

namespace flexhal {
namespace interfaces {

class IBus : public IDevice {
public:
    // バスがビジー状態かどうか
    virtual bool isBusy() const = 0;
    
    // バスがアイドル状態になるまで待機
    virtual void waitUntilIdle() = 0;
    
    // バスタイプの取得
    virtual BusType getType() const = 0;
};

} // namespace interfaces
} // namespace flexhal
```

### 1.4 トランスポートインターフェース（src/flexhal/interfaces/itransport.hpp）

```cpp
#pragma once

#include <memory>
#include "idevice.hpp"
#include "ibus.hpp"

namespace flexhal {
namespace interfaces {

class ITransport : public IDevice {
public:
    // データの書き込み
    virtual ssize_t write(const void* data, size_t length) = 0;
    
    // データの読み取り
    virtual ssize_t read(void* data, size_t length) = 0;
    
    // 関連するバスの取得
    virtual std::shared_ptr<IBus> getBus() const = 0;
};

} // namespace interfaces
} // namespace flexhal
```

## 2. GPIO実装例

### 2.1 GPIOインターフェース（src/flexhal/interfaces/igpio_pin.hpp）

```cpp
#pragma once

#include "idevice.hpp"
#include "../core.hpp"

namespace flexhal {
namespace interfaces {

class IGPIOPin : public IDevice {
public:
    // ピンモードの設定
    virtual bool setMode(PinMode mode) = 0;
    
    // ピンの出力レベル設定
    virtual bool setLevel(PinLevel level) = 0;
    
    // ピンの入力レベル取得
    virtual PinLevel getLevel() const = 0;
    
    // ピン番号の取得
    virtual int getPinNumber() const = 0;
    
    // 現在のピンモードの取得
    virtual PinMode getMode() const = 0;
};

} // namespace interfaces
} // namespace flexhal
```

### 2.2 GPIO API（src/flexhal/gpio.hpp）

```cpp
#pragma once

#include <memory>
#include "core.hpp"
#include "interfaces/igpio_pin.hpp"

namespace flexhal {
namespace gpio {

// GPIOピンの作成
std::shared_ptr<interfaces::IGPIOPin> createPin(int pin_number);

// デフォルトのGPIOピンの取得
std::shared_ptr<interfaces::IGPIOPin> getDefaultPin(int pin_number);

// 便利関数
bool pinMode(int pin_number, PinMode mode);
bool digitalWrite(int pin_number, PinLevel level);
PinLevel digitalRead(int pin_number);

} // namespace gpio
} // namespace flexhal
```

### 2.3 Arduino GPIO実装（impl/frameworks/arduino/gpio/pin.hpp）

```cpp
#pragma once

#include <memory>
#include "flexhal/interfaces/igpio_pin.hpp"

namespace flexhal {
namespace framework {
namespace arduino {
namespace gpio {

class Pin : public interfaces::IGPIOPin {
public:
    Pin(int pin_number);
    virtual ~Pin() = default;
    
    // IDevice実装
    bool begin() override;
    void end() override;
    bool isInitialized() const override;
    DeviceType getType() const override;
    
    // IGPIOPin実装
    bool setMode(PinMode mode) override;
    bool setLevel(PinLevel level) override;
    PinLevel getLevel() const override;
    int getPinNumber() const override;
    PinMode getMode() const override;
    
private:
    int pin_number_;
    PinMode current_mode_;
    bool initialized_;
};

// ファクトリ関数
std::shared_ptr<interfaces::IGPIOPin> createPin(int pin_number);

} // namespace gpio
} // namespace arduino
} // namespace framework
} // namespace flexhal
```

### 2.4 Arduino GPIO実装（impl/frameworks/arduino/gpio/pin.inl）

```cpp
#include "pin.hpp"
#include <Arduino.h>

namespace flexhal {
namespace framework {
namespace arduino {
namespace gpio {

Pin::Pin(int pin_number)
    : pin_number_(pin_number)
    , current_mode_(PinMode::Input)
    , initialized_(false) {
}

bool Pin::begin() {
    if (initialized_) {
        return true;
    }
    
    // デフォルトでは入力モードに設定
    if (!setMode(PinMode::Input)) {
        return false;
    }
    
    initialized_ = true;
    return true;
}

void Pin::end() {
    if (initialized_) {
        // 入力モードに戻す
        setMode(PinMode::Input);
        initialized_ = false;
    }
}

bool Pin::isInitialized() const {
    return initialized_;
}

DeviceType Pin::getType() const {
    return DeviceType::GPIO;
}

bool Pin::setMode(PinMode mode) {
    switch (mode) {
        case PinMode::Input:
            ::pinMode(pin_number_, INPUT);
            break;
        case PinMode::Output:
            ::pinMode(pin_number_, OUTPUT);
            break;
        case PinMode::InputPullUp:
            ::pinMode(pin_number_, INPUT_PULLUP);
            break;
        case PinMode::InputPullDown:
            #ifdef INPUT_PULLDOWN
            ::pinMode(pin_number_, INPUT_PULLDOWN);
            #else
            // Arduino標準ではINPUT_PULLDOWNがサポートされていない場合がある
            return false;
            #endif
            break;
        case PinMode::OutputOpenDrain:
            #ifdef OUTPUT_OPEN_DRAIN
            ::pinMode(pin_number_, OUTPUT_OPEN_DRAIN);
            #else
            // Arduino標準ではOUTPUT_OPEN_DRAINがサポートされていない場合がある
            return false;
            #endif
            break;
        case PinMode::Analog:
            // Arduino標準ではanalogWriteを使用
            ::pinMode(pin_number_, OUTPUT);
            break;
        default:
            return false;
    }
    
    current_mode_ = mode;
    return true;
}

bool Pin::setLevel(PinLevel level) {
    if (!initialized_) {
        return false;
    }
    
    if (current_mode_ != PinMode::Output && 
        current_mode_ != PinMode::OutputOpenDrain) {
        // 出力モードでない場合はエラー
        return false;
    }
    
    ::digitalWrite(pin_number_, level == PinLevel::High ? HIGH : LOW);
    return true;
}

PinLevel Pin::getLevel() const {
    if (!initialized_) {
        return PinLevel::Low;
    }
    
    return ::digitalRead(pin_number_) == HIGH ? PinLevel::High : PinLevel::Low;
}

int Pin::getPinNumber() const {
    return pin_number_;
}

PinMode Pin::getMode() const {
    return current_mode_;
}

std::shared_ptr<interfaces::IGPIOPin> createPin(int pin_number) {
    return std::make_shared<Pin>(pin_number);
}

} // namespace gpio
} // namespace arduino
} // namespace framework
} // namespace flexhal
```

## 3. 初期化実装例

### 3.1 初期化実装（src/flexhal/init.cpp）

```cpp
#include "core.hpp"
#include "platform.hpp"
#include "framework.hpp"
#include "rtos.hpp"

namespace flexhal {

bool init() {
    // プラットフォーム層の初期化
    if (!platform::init()) {
        return false;
    }
    
    // フレームワーク層の初期化
    if (!framework::init()) {
        platform::end();
        return false;
    }
    
    // RTOS層の初期化
    if (!rtos::init()) {
        framework::end();
        platform::end();
        return false;
    }
    
    return true;
}

void end() {
    // 逆順で終了処理
    rtos::end();
    framework::end();
    platform::end();
}

} // namespace flexhal
```

### 3.2 プラットフォーム層初期化（src/flexhal/platform.cpp）

```cpp
#include "platform.hpp"

// プラットフォーム固有の実装をインクルード
#if defined(ESP32)
#include "impl/platforms/esp32/init.hpp"
#elif defined(ESP8266)
#include "impl/platforms/esp8266/init.hpp"
#elif defined(ARDUINO_ARCH_AVR)
#include "impl/platforms/avr/init.hpp"
#else
#include "impl/platforms/desktop/init.hpp"
#endif

namespace flexhal {
namespace platform {

bool init() {
    // プラットフォーム固有の初期化を呼び出す
#if defined(ESP32)
    return esp32::initImpl();
#elif defined(ESP8266)
    return esp8266::initImpl();
#elif defined(ARDUINO_ARCH_AVR)
    return avr::initImpl();
#else
    return desktop::initImpl();
#endif
}

void end() {
    // プラットフォーム固有の終了処理を呼び出す
#if defined(ESP32)
    esp32::endImpl();
#elif defined(ESP8266)
    esp8266::endImpl();
#elif defined(ARDUINO_ARCH_AVR)
    avr::endImpl();
#else
    desktop::endImpl();
#endif
}

} // namespace platform
} // namespace flexhal
```

### 3.3 フレームワーク層初期化（src/flexhal/framework.cpp）

```cpp
#include "framework.hpp"

// フレームワーク固有の実装をインクルード
#if defined(ARDUINO)
#include "impl/frameworks/arduino/init.hpp"
#elif defined(ESP_IDF_VERSION)
#include "impl/frameworks/espidf/init.hpp"
#else
#include "impl/frameworks/native/init.hpp"
#endif

namespace flexhal {
namespace framework {

bool init() {
    // フレームワーク固有の初期化を呼び出す
#if defined(ARDUINO)
    return arduino::initImpl();
#elif defined(ESP_IDF_VERSION)
    return espidf::initImpl();
#else
    return native::initImpl();
#endif
}

void end() {
    // フレームワーク固有の終了処理を呼び出す
#if defined(ARDUINO)
    arduino::endImpl();
#elif defined(ESP_IDF_VERSION)
    espidf::endImpl();
#else
    native::endImpl();
#endif
}

} // namespace framework
} // namespace flexhal
```

## 4. SPI実装例

### 4.1 SPIバスインターフェース（src/flexhal/interfaces/ispi_bus.hpp）

```cpp
#pragma once

#include "ibus.hpp"
#include "../core.hpp"

namespace flexhal {
namespace interfaces {

struct SPIBusConfig {
    int sck_pin;    // SCKピン番号
    int miso_pin;   // MISOピン番号
    int mosi_pin;   // MOSIピン番号
    uint32_t max_clock_hz = 1000000;  // 最大クロック周波数
};

class ISPIBus : public IBus {
public:
    // SPIバスの初期化（設定付き）
    virtual bool begin(const SPIBusConfig& config) = 0;
    
    // データ転送
    virtual bool transfer(
        const uint8_t* tx_data, 
        uint8_t* rx_data, 
        size_t size,
        uint32_t clock_hz, 
        SPIMode mode, 
        SPIBitOrder bit_order) = 0;
    
    // 現在の設定の取得
    virtual const SPIBusConfig& getConfig() const = 0;
};

} // namespace interfaces
} // namespace flexhal
```

### 4.2 SPIトランスポートインターフェース（src/flexhal/interfaces/ispi_transport.hpp）

```cpp
#pragma once

#include <memory>
#include "itransport.hpp"
#include "ispi_bus.hpp"

namespace flexhal {
namespace interfaces {

struct SPIDeviceConfig {
    int cs_pin;             // チップセレクトピン番号
    uint32_t clock_hz;      // クロック周波数
    SPIMode mode;           // SPIモード
    SPIBitOrder bit_order;  // ビット順序
};

class ISPITransport : public ITransport {
public:
    // SPIトランスポートの初期化（設定付き）
    virtual bool begin(const SPIDeviceConfig& config) = 0;
    
    // チップセレクト制御
    virtual void select() = 0;
    virtual void deselect() = 0;
    
    // クロック周波数の設定
    virtual bool setClockFrequency(uint32_t clock_hz) = 0;
    
    // SPIモードの設定
    virtual bool setMode(SPIMode mode) = 0;
    
    // ビット順序の設定
    virtual bool setBitOrder(SPIBitOrder bit_order) = 0;
    
    // 現在の設定の取得
    virtual const SPIDeviceConfig& getConfig() const = 0;
    
    // 関連するSPIバスの取得
    virtual std::shared_ptr<ISPIBus> getSPIBus() const = 0;
};

} // namespace interfaces
} // namespace flexhal
```

### 4.3 SPI API（src/flexhal/spi.hpp）

```cpp
#pragma once

#include <memory>
#include "core.hpp"
#include "interfaces/ispi_bus.hpp"
#include "interfaces/ispi_transport.hpp"

namespace flexhal {
namespace spi {

// SPIバスの作成
std::shared_ptr<interfaces::ISPIBus> createBus(const interfaces::SPIBusConfig& config);

// デフォルトのSPIバスの取得
std::shared_ptr<interfaces::ISPIBus> getDefaultBus();

// SPIトランスポートの作成
std::shared_ptr<interfaces::ISPITransport> createTransport(
    std::shared_ptr<interfaces::ISPIBus> bus, 
    const interfaces::SPIDeviceConfig& config);

} // namespace spi
} // namespace flexhal
```

### 4.4 Arduino SPI実装（impl/frameworks/arduino/spi/bus.hpp）

```cpp
#pragma once

#include <memory>
#include "flexhal/interfaces/ispi_bus.hpp"

namespace flexhal {
namespace framework {
namespace arduino {
namespace spi {

class Bus : public interfaces::ISPIBus {
public:
    Bus();
    virtual ~Bus() = default;
    
    // IDevice実装
    bool begin() override;
    void end() override;
    bool isInitialized() const override;
    DeviceType getType() const override;
    
    // IBus実装
    bool isBusy() const override;
    void waitUntilIdle() override;
    BusType getType() const override;
    
    // ISPIBus実装
    bool begin(const interfaces::SPIBusConfig& config) override;
    bool transfer(
        const uint8_t* tx_data, 
        uint8_t* rx_data, 
        size_t size,
        uint32_t clock_hz, 
        SPIMode mode, 
        SPIBitOrder bit_order) override;
    const interfaces::SPIBusConfig& getConfig() const override;
    
private:
    interfaces::SPIBusConfig config_;
    bool initialized_;
    bool busy_;
    
    // Arduinoのハードウェア依存部分
    class SPIClass* spi_;
};

// ファクトリ関数
std::shared_ptr<interfaces::ISPIBus> createBus(const interfaces::SPIBusConfig& config);
std::shared_ptr<interfaces::ISPIBus> getDefaultBus();

} // namespace spi
} // namespace arduino
} // namespace framework
} // namespace flexhal
```

## 5. 使用例

### 5.1 GPIOの使用例

```cpp
#include <flexhal.hpp>
#include <flexhal/gpio.hpp>

void setup() {
    // FlexHALの初期化
    flexhal::init();
    
    // GPIOピンの取得
    auto led_pin = flexhal::gpio::createPin(13);
    
    // 出力モードに設定
    led_pin->begin();
    led_pin->setMode(flexhal::PinMode::Output);
    
    // LEDを点灯
    led_pin->setLevel(flexhal::PinLevel::High);
    
    // 便利関数を使用した例
    flexhal::gpio::pinMode(12, flexhal::PinMode::Output);
    flexhal::gpio::digitalWrite(12, flexhal::PinLevel::High);
}

void loop() {
    // LEDの点滅
    auto led_pin = flexhal::gpio::getDefaultPin(13);
    
    led_pin->setLevel(flexhal::PinLevel::High);
    delay(1000);
    led_pin->setLevel(flexhal::PinLevel::Low);
    delay(1000);
}
```

### 5.2 SPIの使用例

```cpp
#include <flexhal.hpp>
#include <flexhal/spi.hpp>

void setup() {
    // FlexHALの初期化
    flexhal::init();
    
    // SPIバスの設定
    flexhal::interfaces::SPIBusConfig bus_config;
    bus_config.sck_pin = 13;
    bus_config.miso_pin = 12;
    bus_config.mosi_pin = 11;
    bus_config.max_clock_hz = 1000000;
    
    // SPIバスの作成
    auto spi_bus = flexhal::spi::createBus(bus_config);
    spi_bus->begin();
    
    // SPIデバイスの設定
    flexhal::interfaces::SPIDeviceConfig device_config;
    device_config.cs_pin = 10;
    device_config.clock_hz = 500000;
    device_config.mode = flexhal::SPIMode::Mode0;
    device_config.bit_order = flexhal::SPIBitOrder::MSBFirst;
    
    // SPIトランスポートの作成
    auto spi_transport = flexhal::spi::createTransport(spi_bus, device_config);
    spi_transport->begin();
    
    // データ送信
    uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
    spi_transport->select();
    spi_transport->write(data, sizeof(data));
    spi_transport->deselect();
}

void loop() {
    // 定期的なデータ読み取り
    auto spi_transport = flexhal::spi::getDefaultTransport();
    
    uint8_t rx_buffer[4];
    spi_transport->select();
    spi_transport->read(rx_buffer, sizeof(rx_buffer));
    spi_transport->deselect();
    
    delay(1000);
}
```

### 5.3 クロスプラットフォームの例

```cpp
#include <flexhal.hpp>
#include <flexhal/gpio.hpp>
#include <flexhal/spi.hpp>

// どのプラットフォームでも同じコードで動作
void blinkLED(int pin, int count) {
    auto led = flexhal::gpio::createPin(pin);
    led->begin();
    led->setMode(flexhal::PinMode::Output);
    
    for (int i = 0; i < count; i++) {
        led->setLevel(flexhal::PinLevel::High);
        delay(200);
        led->setLevel(flexhal::PinLevel::Low);
        delay(200);
    }
}

// SPIデバイスへのデータ送信（プラットフォーム非依存）
void sendSPIData(const uint8_t* data, size_t length) {
    // デフォルトのSPIバスを使用
    auto spi_bus = flexhal::spi::getDefaultBus();
    
    // デバイス設定
    flexhal::interfaces::SPIDeviceConfig config;
    config.cs_pin = 10;
    config.clock_hz = 1000000;
    config.mode = flexhal::SPIMode::Mode0;
    config.bit_order = flexhal::SPIBitOrder::MSBFirst;
    
    // トランスポート作成
    auto transport = flexhal::spi::createTransport(spi_bus, config);
    transport->begin();
    
    // データ送信
    transport->select();
    transport->write(data, length);
    transport->deselect();
}

void setup() {
    // FlexHAL初期化
    flexhal::init();
    
    // LEDを点滅
    blinkLED(13, 3);
    
    // SPIデータ送信
    uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};
    sendSPIData(data, sizeof(data));
}

void loop() {
    // メインループ
    delay(1000);
}
```
