# Leds

## 概要

Spresense本体に搭載されたLED0〜LED3を操作するクラス。

## API

```cpp
#include <hundred_yen_sat/devices/Leds.h>

Leds leds;

void setup() {
    leds.begin();
    leds.on(0);
    leds.toggle(1);
    leds.setPattern(0x0f);  // 全点灯
}
```

- `set(index, enabled)` / `on(index)` / `off(index)` 指定LEDの点灯状態を
  変更する。`index`は0〜3。範囲外は`false`を返す。
- `toggle(index)` 現在の状態を反転する。
- `setPattern(pattern)` 下位4bitをLED0〜LED3へ一括反映する。`pattern()`で
  現在のパターンを取得できる。

## 設計メモ

- `setPattern()`はミッション状態の簡易表示用途を想定している（例: 状態ごとに
  異なるビットパターンを割り当てる）。状態とLED表示の対応はアプリ側が決める。
