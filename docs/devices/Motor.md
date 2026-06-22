# Motor / Motors

## 概要

TB6612FNGモータドライバに接続したDCモーターを操作するクラス。`Motor`は
1チャンネル単体、`Motors`は左右2個をまとめて扱う。Hundred Yen SATの標準
配線（右: in1=8, in2=4, pwm=5 / 左: in1=7, in2=2, pwm=3）を`Motors`の
デフォルト値として持つ。

## API

```cpp
#include <hundred_yen_sat/devices/Motors.h>

Motors motors;  // 標準配線をデフォルトで使用

void setup() {
    motors.begin();
    motors.drive(120, 80);   // 右120、左80
    motors.forward(100);
    motors.turnRight(100);
    motors.brake();

    // 個別アクセス
    motors.right().setSpeed(150);
    motors.left().stop();
}
```

1個だけ使う場合は`Motor`を直接構築できる。

```cpp
Motor rightMotor(MotorPins(8, 4, 5));
rightMotor.begin();
rightMotor.forward(120);
```

- `Motor::setSpeed(int)` `-255`〜`255`。正値が前進、負値が後退。`0`は
  `stop()`と同じ。
- `Motor::stop()` 出力を解放する惰性停止（coast stop）。
- `Motor::brake()` TB6612FNGのshort brake状態にする能動的な停止。
- `Motors::drive(right, left)` 左右の速度を同時に指定する。
- `MotorPins::inverted` モーターの取り付け向きが逆の場合に電気的な回転
  方向だけを反転する。

## 設計メモ

- `setSpeed()`の符号は「機体から見た前進方向」を表す論理値として固定し、
  配線都合による反転は`MotorPins::inverted`に閉じ込めている。呼び出し側が
  左右で符号を読み替える必要をなくすための設計。
- `stop()`（coast）と`brake()`（short brake）を別メソッドに分けているのは、
  TB6612FNGの停止特性が用途によって異なるため。急停止が必要な場面と、
  慣性で滑らせたい場面の両方を呼び出し側で選べるようにしている。
- ピン番号は`MotorPins`としてコンストラクタ引数で受け取り、デフォルト値は
  Hundred Yen SAT標準配線のみを想定する。
