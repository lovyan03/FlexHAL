#pragma once

#include <cstdint>
#include <memory>

namespace flexhal {

// 色の定義～♪
struct Color {
    uint8_t r, g, b, a;

    // よく使う色をプリセットしとくよ！
    static constexpr Color BLACK   = {0, 0, 0, 255};
    static constexpr Color WHITE   = {255, 255, 255, 255};
    static constexpr Color RED     = {255, 0, 0, 255};
    static constexpr Color GREEN   = {0, 255, 0, 255};
    static constexpr Color BLUE    = {0, 0, 255, 255};
    static constexpr Color YELLOW  = {255, 255, 0, 255};
    static constexpr Color MAGENTA = {255, 0, 255, 255};
    static constexpr Color CYAN    = {0, 255, 255, 255};
};

class Display {
public:
    // シングルトンでインスタンス取得！
    static Display& getInstance()
    {
        static Display instance;
        return instance;
    }

    // 初期化とか終了とか
    virtual bool begin() = 0;
    virtual void end()   = 0;

    // 基本的な描画系メソッド
    virtual void clear()                                            = 0;
    virtual void flush()                                            = 0;
    virtual void setPixel(int16_t x, int16_t y, const Color& color) = 0;

    // 図形描画
    virtual void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, const Color& color) = 0;
    virtual void drawRect(int16_t x, int16_t y, int16_t w, int16_t h, const Color& color)     = 0;
    virtual void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, const Color& color)     = 0;
    virtual void drawCircle(int16_t x, int16_t y, int16_t r, const Color& color)              = 0;
    virtual void fillCircle(int16_t x, int16_t y, int16_t r, const Color& color)              = 0;

    // テキスト描画
    virtual void setTextSize(uint8_t size)        = 0;
    virtual void setTextColor(const Color& color) = 0;
    virtual void setCursor(int16_t x, int16_t y)  = 0;
    virtual void print(const char* str)           = 0;
    virtual void println(const char* str)         = 0;

    // 画面の基本情報取得
    virtual uint16_t width() const      = 0;
    virtual uint16_t height() const     = 0;
    virtual uint8_t rotation() const    = 0;
    virtual void setRotation(uint8_t r) = 0;

protected:
    Display()          = default;
    virtual ~Display() = default;

    // コピー禁止にしとくよ！
    Display(const Display&)            = delete;
    Display& operator=(const Display&) = delete;
};

}  // namespace flexhal
