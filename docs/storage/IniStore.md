# IniStore

## 概要

SD上の小さなINI形式設定ファイル（最大1023byte）を固定長バッファへ読み込み、
`section`/`key`単位で値を取得するクラス。

## API

```cpp
#include <hundred_yen_sat/storage/IniStore.h>

Storage sd;
IniStore config(sd);

void setup() {
    if (sd.begin(3000) && config.load("config.ini")) {
        long threshold = config.getInt("light", "threshold", 400);
        float gain = config.getFloat("motor", "gain", 1.0f);
        bool debug = config.getBool("system", "debug", false);

        char name[32];
        config.getString("system", "name", name, sizeof(name), "unnamed");
    }
}
```

- `load(path)` INIファイルを読み込み、内部バッファの内容を置き換える。
  `loaded()`で読み込み済みかを確認できる。
- `getString` / `getInt` / `getFloat` / `getBool` `section`と`key`を指定
  して値を取得する。キーが見つからない場合は`fallback`を返す（`getString`
  は`output`へ`fallback`をコピーして`false`を返す）。

## 設計メモ

- `load()`時に一度だけファイルI/Oを行い、以降の`getXxx()`はすべて内部
  バッファ（最大1024byte）への参照処理のみで完結する。設定値の参照を
  ループ内で頻繁に呼んでもSDアクセスが発生しない。
- 対応ファイルサイズを1023byteに制限しているのは、固定長バッファでヒープ
  確保を避けるため。大きな設定ファイルは想定していない。
- `getBool`は`true/false`、`yes/no`、`1/0`の表記を受け付ける。設定ファイルを
  手書きする運用を想定した緩いパース。
