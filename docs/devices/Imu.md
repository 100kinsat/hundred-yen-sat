# Imu

## 概要

I2C接続のBNO055から加速度・角速度・地磁気・姿勢角を取得するクラス。
デフォルトI2Cアドレスは`0x28`。`begin(sensorFusion)`でNDOFモード（true、
姿勢角まで使う）か加速度のみのモード（false）かを選べる。

## API

```cpp
#include <hundred_yen_sat/devices/Imu.h>

Imu imu;  // デフォルトでWireと0x28を使用

void setup() {
    if (!imu.begin()) {
        Serial.println("BNO055 initialization failed");
    }
}

void loop() {
    ImuReading reading;
    if (imu.read(reading)) {
        // reading.acceleration / gyroscope / magnetometer / orientation
    }

    // 個別取得も可能
    Vector3 accel = imu.acceleration();
}
```

- `begin(bool sensorFusion = true)` センサーを初期化する。`connected()`で
  chip IDの読み取り可否だけを確認できる。
- `read(ImuReading&)` 加速度・角速度・地磁気・姿勢を1回の連続読み出しで
  まとめて取得する。個別に呼ぶより通信回数が少ない。
- `acceleration()` / `gyroscope()` / `magnetometer()` / `orientation()`
  個別取得用。`orientation()`はNDOFモードでのみ意味のある値になる。

## 設計メモ

- `II2C`のようなHAL抽象化は持たず、`TwoWire&`をコンストラクタで直接受け取る。
  Spresense + BNO055固定のため、抽象化レイヤを増やさない方針による。
- センサー値の取得関数は、読み取りに失敗した場合は前回有効値を保持して返す
  想定。`0`や`-1`をエラー値として返さないことで、呼び出し側のnullチェック
  漏れによる誤動作を防ぐ。
- `read()`による一括取得を優先APIとしているのは、個別取得関数を複数回呼ぶと
  I2Cトランザクションが増え、ループ周期に影響するため。
