# LightSensor

## 概要

アナログ入力に接続したCdS光センサーを読み取る最小クラス。デフォルトピンは
`A0`。

## API

```cpp
#include <hundred_yen_sat/devices/LightSensor.h>

LightSensor light;  // デフォルトA0

void setup() {
    light.begin();
}

void loop() {
    uint16_t value = light.read();  // Spresense Arduinoでは通常0〜1023
}
```

- `begin()` ピンを入力として初期化する。
- `read()` `analogRead()`の値をそのまま返す。

## 設計メモ

- センサー固有の変換（lux換算など）は持たない。ミッションごとに閾値の意味が
  変わるため、生のADC値を返してアプリ側で判断させる方針。
- ピン番号はコンストラクタ引数で受け取り、ハードコードしない（ドライバ規約と
  同じ考え方）。
