# FlexHAL コーディングスタイルガイド

このドキュメントでは、FlexHALプロジェクトで使用するコーディングスタイルとベストプラクティスについて説明します。一貫したコードスタイルを維持することで、可読性の向上とバグの減少を目指します。

## 基本ルール

FlexHALプロジェクトでは、`.clang-format`ファイルに定義されたスタイルルールに従います。主な特徴は以下の通りです：

### 1. インデントとフォーマット

- インデントには**4スペース**を使用（タブは使用しない）
- 行の最大長は**100文字**
- 関数の波括弧は**次の行**に配置
- 制御構文（if, for, while等）の波括弧も**次の行**に配置
- プリプロセッサディレクティブは**インデントなし**

```cpp
// 正しい例
void function()
{
    if (condition)
    {
        doSomething();
    }
}

// 間違った例
void function() {
  if (condition) {
    doSomething();
  }
}
```

### 2. 命名規則

- **クラス名**: PascalCase（例: `DisplayDriver`）
- **関数名**: camelCase（例: `drawPixel`）
- **変数名**: camelCase（例: `frameBuffer`）
- **定数とマクロ**: UPPER_CASE（例: `MAX_BUFFER_SIZE`）
- **名前空間**: 小文字（例: `flexhal`）
- **プライベートメンバ変数**: m_接頭辞を使用（例: `m_counter`）

### 3. ファイル構成

- ヘッダファイルの拡張子は `.hpp`、実装ファイルの拡張子は `.cpp`
- 各ファイルの先頭には適切なライセンスヘッダとファイル説明を含める
- ヘッダガードには `#pragma once` を使用

```cpp
/**
 * @file example.hpp
 * @brief 簡単な説明
 * @version 0.1.0
 * @date 2025-03-28
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once

namespace flexhal {
// コード
}
```

## コメント

- 関数、クラス、重要な変数には**Doxygen形式**のコメントを使用
- 複雑なロジックには説明コメントを追加
- コメントは日本語または英語で記述可能（UTF-8エンコーディング）

```cpp
/**
 * @brief 関数の説明
 * @param param1 最初のパラメータの説明
 * @param param2 2番目のパラメータの説明
 * @return 戻り値の説明
 */
int function(int param1, float param2);
```

## クラス構造

- クラスメンバは以下の順序で宣言：
  1. `public` メソッド
  2. `protected` メソッド
  3. `private` メソッド
  4. `private` メンバ変数
- 継承を使用する場合は `virtual` デストラクタを定義
- 可能な限り `const` 修飾子を使用
- 必要に応じて `= default` や `= delete` を使用

```cpp
class Example
{
public:
    Example();
    virtual ~Example() = default;
    
    void publicMethod();
    
protected:
    void protectedMethod();
    
private:
    void privateMethod();
    
    int m_memberVariable;
};
```

## エラー処理

- 例外よりも戻り値によるエラー報告を優先
- エラーコードは明確に定義し、ドキュメント化する
- リソース管理にはRAIIパターンを使用

## その他のガイドライン

- マジックナンバーを避け、名前付き定数を使用
- グローバル変数の使用を最小限に抑える
- 循環依存を避ける
- 不要なインクルードを削除

## コードスタイルの確認方法

コードをコミットする前に、以下のコマンドでスタイルを確認してください：

```bash
clang-format -style=file -n path/to/your/file.cpp
```

自動的にフォーマットするには：

```bash
clang-format -style=file -i path/to/your/file.cpp
```

VSCodeを使用している場合は、保存時に自動フォーマットが適用されます。
