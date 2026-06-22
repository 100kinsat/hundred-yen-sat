# Watchdog

## 概要

NuttXの`/dev/watchdog0`をPOSIX API（`open`/`ioctl`）で直接操作するクラス。

## API

```cpp
#include <hundred_yen_sat/devices/Watchdog.h>

Watchdog watchdog;  // デフォルトで/dev/watchdog0

void setup() {
    watchdog.begin(5000);  // 5秒タイムアウトで開始
}

void loop() {
    watchdog.feed();  // タイムアウト前に必ず呼ぶ
    delay(1000);
}
```

- `begin(timeoutMs)` watchdogを開き、タイムアウトを設定して開始する。
  タイムアウト設定または開始のどちらかが失敗した場合はfdを閉じて`false`を
  返す。
- `feed()` 期限をリセットする。`begin()`前の呼び出しは失敗する。
- `stop()` 停止を要求する。NuttX側の設定によっては拒否され`false`になる
  ことがある。

## 設計メモ

- 外部に見せる機能ではなく「フリーズ時に確実にリセットさせる」ための最終
  防衛線として、最小限のPOSIX実装にとどめている。Arduino向けのラッパー等は
  設けていない。
- 停止できない設定もあり得るため、実機投入前に`stop()`の挙動とリセット
  タイミングを必ず確認すること。
