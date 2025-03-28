# FlexHAL プロジェクトへの貢献ガイド

FlexHALプロジェクトへの貢献に興味を持っていただき、ありがとうございます！このガイドでは、コードの貢献方法やコードスタイルについて説明します。

## コードスタイル

FlexHALプロジェクトでは、コードの一貫性と読みやすさを確保するために、`.clang-format`ファイルを使用しています。すべてのコードは、このスタイルガイドに従う必要があります。

### 重要なスタイルのポイント

1. **プリプロセッサディレクティブ**: インデントなし
   ```cpp
   // 正しい例
   #if defined(CONDITION)
   #include <header.h>
   #else
   #include <other_header.h>
   #endif
   
   // 間違った例
   #if defined(CONDITION)
     #include <header.h>
   #else
     #include <other_header.h>
   #endif
   ```

2. **関数の波括弧**: 次の行に配置
   ```cpp
   // 正しい例
   void function()
   {
       // 処理
   }
   
   // 間違った例
   void function() {
       // 処理
   }
   ```

3. **インデント**: 4スペース
   ```cpp
   // 正しい例
   if (condition)
   {
       doSomething();
   }
   
   // 間違った例
   if (condition)
   {
     doSomething();
   }
   ```

### コードスタイルの確認方法

コードをコミットする前に、以下のコマンドでスタイルを確認してください：

```bash
clang-format -style=file -n path/to/your/file.cpp
```

自動的にフォーマットするには：

```bash
clang-format -style=file -i path/to/your/file.cpp
```

## プルリクエストの提出

1. フォークしたリポジトリで新しいブランチを作成
2. 変更を加える
3. コードスタイルが正しいことを確認
4. テストを実行
5. プルリクエストを提出

## バージョン管理

FlexHALはセマンティックバージョニング（MAJOR.MINOR.PATCH）を採用しています。破壊的変更はMAJORバージョンアップで対応します。

ご協力ありがとうございます！
