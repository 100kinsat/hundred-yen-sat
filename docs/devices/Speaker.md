# Speaker

## 概要

PWM出力に接続した圧電スピーカーを操作するクラス。デフォルトピンは9。

## API

```cpp
#include <hundred_yen_sat/devices/Speaker.h>

Speaker speaker;  // デフォルトpin 9

void setup() {
    speaker.begin();
    speaker.beep();              // 880Hz, 200ms（ブロッキング）
    speaker.tone(440, 500);      // 440Hzを500ms鳴らす
    speaker.tone(1000);          // stop()まで鳴り続ける
    speaker.stop();
}
```

- `tone(frequencyHz, durationMs = 0)` 指定周波数で鳴らす。`durationMs`が
  `0`なら`stop()`まで鳴り続ける（非ブロッキング）。
- `beep(frequencyHz = 880, durationMs = 200)` 指定時間だけ鳴らし、その間
  呼び出し元をブロックする。

## 設計メモ

- `tone()`は非ブロッキングAPIとして用意し、`beep()`はその場で完結させたい
  単発の通知音（起動完了音など）向けの簡易APIとして分けている。`loop()`内で
  他処理を止めたくない場面は`tone()`を使う。
