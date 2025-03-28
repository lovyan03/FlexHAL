#!/bin/bash

# ビルドディレクトリ作成
mkdir -p build/obj

# コンパイルフラグ
CXXFLAGS="-std=c++17 -Wall -Wextra -I. -DFLEXHAL_PLATFORM_DESKTOP"

# SDLフラグ
SDL_CFLAGS=$(sdl2-config --cflags)
SDL_LIBS=$(sdl2-config --libs)

# ソースファイル
SOURCES=(
    "src/FlexHAL_Impl.cpp"
    "impl/frameworks/sdl/window.cpp"
)

# サンプルアプリケーション
EXAMPLES=(
    "examples/gpio_blink/main.cpp"
)

echo "FlexHALライブラリのビルドを開始します..."

# 個別にオブジェクトファイルをビルド
OBJ_FILES=()
for source in "${SOURCES[@]}"; do
    obj_file="build/obj/$(basename "${source%.*}").o"
    echo "コンパイル中: $source"
    g++ $CXXFLAGS $SDL_CFLAGS -c "$source" -o "$obj_file"
    OBJ_FILES+=("$obj_file")
done

# サンプルアプリケーションのビルド
for example in "${EXAMPLES[@]}"; do
    echo "サンプルをビルド中: $example"
    base_name=$(basename "${example%.*}")
    g++ $CXXFLAGS $SDL_CFLAGS -o "build/$base_name" "$example" "${OBJ_FILES[@]}" $SDL_LIBS
done

echo "ビルド完了！"
echo "実行方法: ./build/gpio_blink"
