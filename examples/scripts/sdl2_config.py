Import("env")
import sys
import platform
import subprocess

def get_sdl2_flags():
    """SDLのコンパイルフラグとリンクフラグを取得する"""
    try:
        # SDL2-configコマンドが利用可能かチェック
        sdl2_cflags = subprocess.check_output(["sdl2-config", "--cflags"]).decode("utf-8").strip()
        sdl2_libs = subprocess.check_output(["sdl2-config", "--libs"]).decode("utf-8").strip()
        return sdl2_cflags, sdl2_libs
    except:
        # SDL2-configが利用できない場合はシステムに応じたデフォルト値を返す
        system = platform.system()
        if system == "Darwin":  # macOS
            return "-I/opt/homebrew/include/SDL2 -D_THREAD_SAFE", "-L/opt/homebrew/lib -lSDL2"
        elif system == "Linux":
            return "-I/usr/include/SDL2 -D_REENTRANT", "-L/usr/lib -lSDL2"
        elif system == "Windows":
            # Windowsの場合はMSYS2/MinGWのパスを想定
            return "-I/mingw64/include/SDL2", "-L/mingw64/lib -lSDL2main -lSDL2"
        else:
            print("Unsupported system:", system)
            return "", ""

# SDL2のフラグを取得
sdl2_cflags, sdl2_libs = get_sdl2_flags()

# ビルド環境にフラグを追加
env.Append(CPPFLAGS=[flag for flag in sdl2_cflags.split() if flag])
env.Append(LINKFLAGS=[flag for flag in sdl2_libs.split() if flag])

# macOSの場合、追加のフレームワークをリンク
if platform.system() == "Darwin":
    env.Append(LINKFLAGS=["-framework", "CoreFoundation"])
    env.Append(LINKFLAGS=["-framework", "Cocoa"])
