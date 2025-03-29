# FlexHAL インターフェース設計書

## 1. コアインターフェース

### 1.1 IDevice

すべてのデバイスの基本インターフェース。

```cpp
namespace flexhal {

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

} // namespace flexhal
```

### 1.2 IBus

物理バスの基本インターフェース。

```cpp
namespace flexhal {

class IBus : public IDevice {
public:
    // バスがビジー状態かどうか
    virtual bool isBusy() const = 0;
    
    // バスがアイドル状態になるまで待機
    virtual void waitUntilIdle() = 0;
    
    // バスタイプの取得
    virtual BusType getType() const = 0;
};

} // namespace flexhal
```

### 1.3 ITransport

データ転送の基本インターフェース。

```cpp
namespace flexhal {

class ITransport : public IDevice {
public:
    // データの書き込み
    virtual ssize_t write(const void* data, size_t length) = 0;
    
    // データの読み取り
    virtual ssize_t read(void* data, size_t length) = 0;
    
    // 関連するバスの取得
    virtual std::shared_ptr<IBus> getBus() const = 0;
};

} // namespace flexhal
```

## 2. GPIO インターフェース

### 2.1 IGPIOPin

GPIO ピンの基本インターフェース。

```cpp
namespace flexhal {
namespace gpio {

class IPin : public IDevice {
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

} // namespace gpio
} // namespace flexhal
```

## 3. SPI インターフェース

### 3.1 ISPIBus

SPI バスの基本インターフェース。

```cpp
namespace flexhal {
namespace spi {

struct SPIBusConfig {
    int sck_pin;    // SCKピン番号
    int miso_pin;   // MISOピン番号
    int mosi_pin;   // MOSIピン番号
    uint32_t max_clock_hz = 1000000;  // 最大クロック周波数
};

class IBus : public flexhal::IBus {
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

} // namespace spi
} // namespace flexhal
```

### 3.2 ISPITransport

SPI トランスポートの基本インターフェース。

```cpp
namespace flexhal {
namespace spi {

struct SPIDeviceConfig {
    int cs_pin;             // チップセレクトピン番号
    uint32_t clock_hz;      // クロック周波数
    SPIMode mode;           // SPIモード
    SPIBitOrder bit_order;  // ビット順序
};

class ITransport : public flexhal::ITransport {
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
    virtual std::shared_ptr<IBus> getSPIBus() const = 0;
};

} // namespace spi
} // namespace flexhal
```

## 4. I2C インターフェース

### 4.1 II2CBus

I2C バスの基本インターフェース。

```cpp
namespace flexhal {
namespace i2c {

struct I2CBusConfig {
    int scl_pin;    // SCLピン番号
    int sda_pin;    // SDAピン番号
    uint32_t frequency = 100000;  // 周波数（デフォルト100kHz）
};

class IBus : public flexhal::IBus {
public:
    // I2Cバスの初期化（設定付き）
    virtual bool begin(const I2CBusConfig& config) = 0;
    
    // データ書き込み
    virtual bool write(uint8_t address, const uint8_t* data, size_t size, bool stop = true) = 0;
    
    // データ読み取り
    virtual bool read(uint8_t address, uint8_t* data, size_t size, bool stop = true) = 0;
    
    // 周波数の設定
    virtual bool setFrequency(uint32_t frequency) = 0;
    
    // 現在の設定の取得
    virtual const I2CBusConfig& getConfig() const = 0;
};

} // namespace i2c
} // namespace flexhal
```

### 4.2 II2CTransport

I2C トランスポートの基本インターフェース。

```cpp
namespace flexhal {
namespace i2c {

struct I2CDeviceConfig {
    uint8_t address;        // デバイスアドレス
    uint32_t frequency;     // 通信周波数
};

class ITransport : public flexhal::ITransport {
public:
    // I2Cトランスポートの初期化（設定付き）
    virtual bool begin(const I2CDeviceConfig& config) = 0;
    
    // レジスタ書き込み
    virtual bool writeRegister(uint8_t reg, uint8_t value) = 0;
    
    // レジスタ読み取り
    virtual bool readRegister(uint8_t reg, uint8_t* value) = 0;
    
    // 複数レジスタ書き込み
    virtual bool writeRegisters(uint8_t reg, const uint8_t* data, size_t size) = 0;
    
    // 複数レジスタ読み取り
    virtual bool readRegisters(uint8_t reg, uint8_t* data, size_t size) = 0;
    
    // 現在の設定の取得
    virtual const I2CDeviceConfig& getConfig() const = 0;
    
    // 関連するI2Cバスの取得
    virtual std::shared_ptr<IBus> getI2CBus() const = 0;
};

} // namespace i2c
} // namespace flexhal
```

## 5. UART インターフェース

### 5.1 IUARTBus

UART バスの基本インターフェース。

```cpp
namespace flexhal {
namespace uart {

struct UARTConfig {
    int tx_pin;             // TXピン番号
    int rx_pin;             // RXピン番号
    uint32_t baud_rate;     // ボーレート
    UARTDataBits data_bits; // データビット数
    UARTParity parity;      // パリティ
    UARTStopBits stop_bits; // ストップビット数
};

class IBus : public flexhal::IBus, public flexhal::ITransport {
public:
    // UARTの初期化（設定付き）
    virtual bool begin(const UARTConfig& config) = 0;
    
    // ボーレートの設定
    virtual bool setBaudRate(uint32_t baud_rate) = 0;
    
    // データフォーマットの設定
    virtual bool setDataFormat(
        UARTDataBits data_bits, 
        UARTParity parity, 
        UARTStopBits stop_bits) = 0;
    
    // 現在の設定の取得
    virtual const UARTConfig& getConfig() const = 0;
    
    // 利用可能なデータ量の取得
    virtual size_t available() const = 0;
    
    // 送信バッファのフラッシュ
    virtual void flush() = 0;
};

} // namespace uart
} // namespace flexhal
```

## 6. タイマーインターフェース

### 6.1 ITimer

タイマーの基本インターフェース。

```cpp
namespace flexhal {
namespace timer {

using TimerCallback = std::function<void()>;

class ITimer : public IDevice {
public:
    // 一定間隔で実行するタイマーの開始
    virtual bool startPeriodic(uint32_t interval_ms, TimerCallback callback) = 0;
    
    // 一度だけ実行するタイマーの開始
    virtual bool startOneShot(uint32_t delay_ms, TimerCallback callback) = 0;
    
    // タイマーの停止
    virtual void stop() = 0;
    
    // タイマーが実行中かどうか
    virtual bool isRunning() const = 0;
    
    // 現在の間隔の取得
    virtual uint32_t getInterval() const = 0;
    
    // 間隔の変更
    virtual bool setInterval(uint32_t interval_ms) = 0;
};

} // namespace timer
} // namespace flexhal
```

## 7. RTOS インターフェース

### 7.1 ITask

タスクの基本インターフェース。

```cpp
namespace flexhal {
namespace rtos {

using TaskFunction = std::function<void(void*)>;

class ITask : public IDevice {
public:
    // タスクの開始
    virtual bool start(TaskFunction function, void* arg = nullptr) = 0;
    
    // タスクの一時停止
    virtual bool suspend() = 0;
    
    // タスクの再開
    virtual bool resume() = 0;
    
    // タスクの優先度設定
    virtual bool setPriority(int priority) = 0;
    
    // タスクの優先度取得
    virtual int getPriority() const = 0;
    
    // タスクが実行中かどうか
    virtual bool isRunning() const = 0;
    
    // タスクIDの取得
    virtual TaskId getId() const = 0;
};

} // namespace rtos
} // namespace flexhal
```

### 7.2 ISemaphore

セマフォの基本インターフェース。

```cpp
namespace flexhal {
namespace rtos {

class ISemaphore : public IDevice {
public:
    // セマフォの取得（待機あり）
    virtual bool take(uint32_t timeout_ms = UINT32_MAX) = 0;
    
    // セマフォの解放
    virtual bool give() = 0;
    
    // 現在の値の取得
    virtual int getValue() const = 0;
};

} // namespace rtos
} // namespace flexhal
```

### 7.3 IMutex

ミューテックスの基本インターフェース。

```cpp
namespace flexhal {
namespace rtos {

class IMutex : public IDevice {
public:
    // ミューテックスのロック（待機あり）
    virtual bool lock(uint32_t timeout_ms = UINT32_MAX) = 0;
    
    // ミューテックスのアンロック
    virtual bool unlock() = 0;
    
    // ロックされているかどうか
    virtual bool isLocked() const = 0;
};

} // namespace rtos
} // namespace flexhal
```

### 7.4 IQueue

キューの基本インターフェース。

```cpp
namespace flexhal {
namespace rtos {

class IQueue : public IDevice {
public:
    // アイテムの送信（待機あり）
    virtual bool send(const void* item, uint32_t timeout_ms = UINT32_MAX) = 0;
    
    // アイテムの受信（待機あり）
    virtual bool receive(void* item, uint32_t timeout_ms = UINT32_MAX) = 0;
    
    // 利用可能なアイテム数の取得
    virtual size_t available() const = 0;
    
    // キューの最大サイズの取得
    virtual size_t getCapacity() const = 0;
    
    // キューのクリア
    virtual void clear() = 0;
};

} // namespace rtos
} // namespace flexhal
```

## 8. ファクトリ関数

各インターフェースのインスタンスを作成するためのファクトリ関数。

```cpp
namespace flexhal {
namespace gpio {
    // GPIOピンの作成
    std::shared_ptr<IPin> createPin(int pin_number);
    
    // デフォルトのGPIOピンの取得
    std::shared_ptr<IPin> getDefaultPin(int pin_number);
}

namespace spi {
    // SPIバスの作成
    std::shared_ptr<IBus> createBus(const SPIBusConfig& config);
    
    // デフォルトのSPIバスの取得
    std::shared_ptr<IBus> getDefaultBus();
    
    // SPIトランスポートの作成
    std::shared_ptr<ITransport> createTransport(
        std::shared_ptr<IBus> bus, 
        const SPIDeviceConfig& config);
}

namespace i2c {
    // I2Cバスの作成
    std::shared_ptr<IBus> createBus(const I2CBusConfig& config);
    
    // デフォルトのI2Cバスの取得
    std::shared_ptr<IBus> getDefaultBus();
    
    // I2Cトランスポートの作成
    std::shared_ptr<ITransport> createTransport(
        std::shared_ptr<IBus> bus, 
        const I2CDeviceConfig& config);
}

namespace uart {
    // UARTバスの作成
    std::shared_ptr<IBus> createBus(const UARTConfig& config);
    
    // デフォルトのUARTバスの取得
    std::shared_ptr<IBus> getDefaultBus(int uart_num = 0);
}

namespace timer {
    // タイマーの作成
    std::shared_ptr<ITimer> createTimer();
}

namespace rtos {
    // タスクの作成
    std::shared_ptr<ITask> createTask(const char* name, int stack_size, int priority);
    
    // セマフォの作成
    std::shared_ptr<ISemaphore> createSemaphore(int initial_count, int max_count);
    
    // ミューテックスの作成
    std::shared_ptr<IMutex> createMutex();
    
    // キューの作成
    std::shared_ptr<IQueue> createQueue(size_t item_size, size_t item_count);
}
} // namespace flexhal
```

## 9. 列挙型と定数

インターフェースで使用される列挙型と定数。

```cpp
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

// UARTデータビット
enum class UARTDataBits : uint8_t {
    Data5,
    Data6,
    Data7,
    Data8
};

// UARTパリティ
enum class UARTParity : uint8_t {
    None,
    Even,
    Odd
};

// UARTストップビット
enum class UARTStopBits : uint8_t {
    Stop1,
    Stop1_5,
    Stop2
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

} // namespace flexhal
```
