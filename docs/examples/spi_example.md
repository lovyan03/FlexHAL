# SPI通信の使用例

## 1. ハードウェアSPIを使用する場合

```cpp
// SPIバスのインスタンスを取得
auto spi_bus = hal.getSPIBus(0);  // SPI0を使用

// SPIの設定
SPIConfig config;
config.clock_hz = 40000000;       // 40MHz
config.mode = SPI_MODE0;          // MODE0
config.cs_pin = 5;               // CS: GPIO5

// トランスポートを取得（ハードウェアSPI）
auto transport = spi_bus->getTransport(config);
transport->begin();

// ST7789デバイスを作成
ST7789 lcd(transport);
lcd.begin();

// データ送信例
uint8_t cmd = 0x2A;  // Column Address Set
uint8_t data[] = {0x00, 0x00, 0x00, 0xEF};  // 0-239
lcd.writeCommand(cmd);
lcd.writeData(data, sizeof(data));
```

## 2. ソフトウェアSPI（ビットバンギング）を使用する場合

```cpp
// GPIOポートを取得してピンを設定
auto gpio = hal.getGPIOPort();
SPIPinPort pins(gpio);
pins.setSCK(18);   // SCK: GPIO18
pins.setMISO(19);  // MISO: GPIO19
pins.setMOSI(23);  // MOSI: GPIO23
pins.setCS(5);     // CS: GPIO5

// SPIバスのインスタンスを取得
auto spi_bus = hal.getSPIBus(0);

// SPIの設定（ソフトウェア実装指定）
SPIConfig config;
config.clock_hz = 2000000;        // 2MHz（ソフトウェアなので低め）
config.mode = SPI_MODE0;
config.implementation = SPI_IMPL_SOFTWARE;
config.pin_config = &pins;

// トランスポートを取得（ソフトウェアSPI）
auto transport = spi_bus->getTransport(config);
transport->begin();

// ST7789デバイスを作成（同じインターフェースで使用可能）
ST7789 lcd(transport);
lcd.begin();

// データ送信例（ハードウェアSPIと同じコードが動作）
uint8_t cmd = 0x2A;
uint8_t data[] = {0x00, 0x00, 0x00, 0xEF};
lcd.writeCommand(cmd);
lcd.writeData(data, sizeof(data));
```

## 3. デバイスクラスの実装例

```cpp
class ST7789 {
public:
    explicit ST7789(std::shared_ptr<ISPITransport> transport)
        : transport_(transport) {}

    bool begin() {
        if (!transport_->begin()) return false;
        
        // デバイス初期化シーケンス
        writeCommand(0x01);  // Software Reset
        delay(150);
        
        writeCommand(0x11);  // Sleep Out
        delay(120);
        
        writeCommand(0x29);  // Display ON
        return true;
    }

    void writeCommand(uint8_t cmd) {
        transport_->setDC(false);  // Command mode
        transport_->write(&cmd, 1);
    }

    void writeData(const uint8_t* data, size_t len) {
        transport_->setDC(true);   // Data mode
        transport_->write(data, len);
    }

private:
    std::shared_ptr<ISPITransport> transport_;
};
```

## 特徴

1. **実装の透過的な切り替え** 🔄
   - ハードウェアSPIとソフトウェアSPIを設定で切り替え可能
   - デバイス側のコードは実装方法を意識しない

2. **柔軟なピン設定** 📌
   - ハードウェアSPI: 固定ピンを使用
   - ソフトウェアSPI: 任意のピンを使用可能

3. **統一されたインターフェース** 🎯
   - `ISPITransport`を使用することで実装の詳細を隠蔽
   - デバイスドライバは実装に依存しない

4. **エラーハンドリング** ⚡
   - 各メソッドは適切なエラーチェックを実装
   - 初期化失敗時は`false`を返す
