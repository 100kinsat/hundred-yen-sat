# Gnss

## 概要

Spresense内蔵GNSSモジュール（SpGnss）をラップし、GPS/QZSSによる測位結果を
取得するクラス。`update()`は非ブロッキングで、`loop()`を止めずに毎周期呼べる。

## API

```cpp
#include <hundred_yen_sat/devices/Gnss.h>

Gnss gnss;

void setup() {
    if (!gnss.begin()) {
        Serial.println("GNSS initialization failed");
    }
}

void loop() {
    gnss.update();
    if (gnss.fixed()) {
        const GnssData& data = gnss.data();
        // data.latitude / longitude / altitude / satellites / utc
    }
}
```

- `begin(bool coldStart = true)` GPS・QZSS（L1CA/L1S）を選択して測位を
  開始する。
- `update()` 新着データがあればキャッシュを更新する。データがなければ
  `false`（エラーではない）。
- `waitForFix(timeoutMs = 300000)` 測位確定かタイムアウトまで呼び出し元を
  ブロックする。起動直後の確認用途を想定し、通常の`loop()`内では使わない。
- `data()` / `fixed()` 最後に`update()`で取得した内容を参照する。

## 設計メモ

- データ受信（`posDataExist`）と測位確定（`posFixMode != FixInvalid`）は
  別状態として扱う。位置データ自体は受信していても未確定（fixedでない）
  ことがあるため、`fixed()`を見ずに`data()`の値を使うと誤った位置を読む
  ことになる。
- `update()`は`waitUpdate(0)`でタイムアウト即時返却にしており、ミッション
  全体のループ周期に影響しないことを優先している。測位待ちが必要な場面は
  `waitForFix()`に限定する。
- GPSに加えQZSSを選択しているのは、日本国内での運用を前提にした構成。
