#!/bin/bash

# FlexHAL ロガーテスト用ビルドスクリプト

# ディレクトリ設定
FLEXHAL_DIR="$(cd "$(dirname "$0")/../.." && pwd)"
BUILD_DIR="${FLEXHAL_DIR}/build/logger_test"
SRC_DIR="${FLEXHAL_DIR}/tests/logger_test/src"

# ビルドディレクトリの作成
mkdir -p "${BUILD_DIR}"

# コンパイラフラグ
CXXFLAGS="-std=c++14 -Wall -Wextra -I${FLEXHAL_DIR}/src -I${FLEXHAL_DIR}"

# ソースファイル
SOURCES=(
    "${SRC_DIR}/main.cpp"
)

# FlexHALのコア実装を直接インクルード
CXXFLAGS="${CXXFLAGS} -DFLEXHAL_PLATFORM_DESKTOP -DFLEXHAL_DIRECT_INCLUDE"

# 必要な実装ファイルをインクルードするソースファイルを作成
cat > "${BUILD_DIR}/flexhal_impl.cpp" << EOF
// FlexHALの実装ファイルをインクルード
#include "${FLEXHAL_DIR}/impl/internal/platform_detect.h"

// ロガーテスト用の最小限の実装
#include "${FLEXHAL_DIR}/impl/platforms/desktop/logger.inl"

// コア関数のスタブ実装
namespace flexhal {
    bool init() { return true; }
    void end() {}
    bool update() { return true; }
    
    // デフォルトロガーのインスタンス
    static impl::desktop::Logger s_default_logger;
    
    // 現在のロガー
    static ILogger* s_current_logger = &s_default_logger;
    
    // デフォルトロガーを取得
    ILogger* getLogger() {
        return s_current_logger;
    }
    
    // ロガーを設定
    void setLogger(ILogger* logger) {
        if (logger) {
            s_current_logger = logger;
        } else {
            s_current_logger = &s_default_logger;
        }
    }
    
    // グローバルログ関数
    void debug(const char* message) {
        s_current_logger->debug(message);
    }
    
    void info(const char* message) {
        s_current_logger->info(message);
    }
    
    void warning(const char* message) {
        s_current_logger->warning(message);
    }
    
    void error(const char* message) {
        s_current_logger->error(message);
    }
    
    void fatal(const char* message) {
        s_current_logger->fatal(message);
    }
}
EOF

# 実装ファイルをソースに追加
SOURCES+=("${BUILD_DIR}/flexhal_impl.cpp")

# SDL2の依存関係を確認
if command -v sdl2-config &> /dev/null; then
    CXXFLAGS="${CXXFLAGS} $(sdl2-config --cflags)"
    LDFLAGS="$(sdl2-config --libs)"
    echo "SDL2 found, using SDL2 for desktop simulation"
else
    echo "SDL2 not found, desktop simulation may not work properly"
fi

# コンパイル
echo "Compiling logger test..."
g++ ${CXXFLAGS} ${SOURCES[@]} -o "${BUILD_DIR}/logger_test" ${LDFLAGS}

if [ $? -eq 0 ]; then
    echo "Build successful! Executable: ${BUILD_DIR}/logger_test"
    echo "Run with: ${BUILD_DIR}/logger_test"
else
    echo "Build failed!"
    exit 1
fi
