# Hundred Yen SAT

Sony Spresenseと100kinSATの実機構成に固定したArduinoライブラリです。
複数プラットフォームを抽象化することより、ミッションで使うデバイスと状態遷移を
短く、読みやすく、間違えにくいコードで扱うことを優先します。

## 対象環境

- Board: Sony Spresense
- Framework: Arduino
- Build: Arduino IDE / PlatformIO
- OS: Spresense Arduino core内部のNuttX

## ディレクトリ構成

```text
src/
├── HundredYenSAT.h
└── hundred_yen_sat/
    ├── devices/
    │   ├── Camera.*
    │   ├── Gnss.*
    │   ├── Imu.*
    │   ├── Leds.*
    │   ├── LightSensor.*
    │   ├── Motors.*
    │   ├── Speaker.*
    │   └── Watchdog.*
    ├── fsm/
    │   └── Fsm.*
    └── storage/
        ├── IniStore.*
        ├── Recorder.*
        └── Storage.*
```

デバイスはすべて`devices/`、状態遷移は`fsm/`、ファイル関連は`storage/`に配置します。
プラットフォーム抽象化用のHAL階層は設けません。

## API一覧

| API | 対象 |
|---|---|
| `Leds` | Spresense内蔵LED 0〜3 |
| `LightSensor` | CdS、デフォルトA0 |
| `Motor` | TB6612FNGの1チャンネルに接続した1個のモーター |
| `Motors` | 左右Motorの一括制御と個別アクセス |
| `Speaker` | PWMスピーカー、デフォルトpin 9 |
| `Imu` | BNO055 |
| `Gnss` | Spresense内蔵GNSS |
| `Camera` | Spresense Camera BoardによるHD JPEG静止画撮影 |
| `Watchdog` | `/dev/watchdog0` |
| `Storage` | `/mnt/sd0`へのPOSIX I/O |
| `Recorder` | CSVなどの追記記録 |
| `IniStore` | INI設定読み込み |
| `Fsm` | クラス継承による状態遷移（遷移条件は状態の外で一元管理） |

## 基本的な使い方

すべてのAPIを使う場合は公開ヘッダを1つincludeします。

```cpp
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Leds leds;
LightSensor light;

void setup() {
    leds.begin();
    light.begin();
}

void loop() {
    leds.setPattern(light.read() > 400 ? 0x0f : 0x01);
    delay(100);
}
```

必要な機能だけ読み込むこともできます。

```cpp
#include <hundred_yen_sat/devices/Imu.h>
#include <hundred_yen_sat/fsm/Fsm.h>
```

## PlatformIO

プロジェクトの`lib/hundred-yen-sat`へこのディレクトリを配置します。

```ini
[env:spresense]
platform = sonyspresense
board = spresense
framework = arduino
monitor_speed = 115200
build_unflags = -std=gnu++11
build_flags = -std=gnu++14
```

## Arduino IDE

このディレクトリをZIPにし、`スケッチ > ライブラリをインクルード >
.ZIP形式のライブラリをインストール`から追加します。

## SDカード

`Storage`はArduinoの`SD` APIを呼びません。`/mnt/sd0`が存在することを`stat()`で
確認した後、POSIXの`open/read/write/close`を使用します。

```cpp
Storage sd;
if (!sd.begin(3000)) {
    Serial.println("SD is not mounted");
}
```

`begin()`はSDの初期化やmountを行いません。SDが認識されていなければ、指定時間後に
`false`を返します。

## モーター

`Motors`は左右一括の移動APIに加え、`right()`と`left()`から各タイヤを独立して
操作できます。

```cpp
Motors motors;
motors.begin();

motors.right().setSpeed(120);
motors.left().setSpeed(80);

motors.right().brake();
motors.left().stop();
```

1個だけ使う場合は`Motor`を直接作れます。

```cpp
Motor rightMotor(MotorPins(8, 4, 5));
rightMotor.begin();
rightMotor.forward(120);
```

## FSM

状態は`IState`を継承したクラスとして実装します。`onEnter`/`onUpdate`/`onExit`
をオーバーライドし、状態固有のデータは派生クラスのメンバとして直接持てます。
遷移条件は状態の外（`Fsm::addTransition`）で一元管理するため、状態側は次の
状態を知る必要がありません。

```cpp
class StandbyState : public IState {
public:
    explicit StandbyState(Mission& ctx) : _ctx(ctx) {}
    void onEnter() override { _ctx.leds.setPattern(0x01); }
private:
    Mission& _ctx;
};

StandbyState standby(mission);
ReleasedState released(mission);
Fsm fsm;

fsm.addTransition(&standby, &released, [] { return shouldRelease(); });
fsm.begin(&standby);
```

メインループでは`fsm.update()`を呼びます。完全な例は`examples/Fsm`を
参照してください。

## pthread

初期版ではArduinoの`setup()/loop()`を標準実行モデルにします。NuttXのpthreadを
直接利用することはできますが、Arduino APIや各デバイスのスレッド安全性、優先度、
スタックサイズ、停止方法を決める必要があります。必要性が確認できたタスクから、
後続版で明示的なタスクAPIとして追加します。

## 設計方針

- Spresenseと実際に使用するデバイスへ固定する
- デバイスごとの抽象化レイヤを増やさない
- コンストラクタではハードウェアを操作せず、`begin()`で初期化する
- ヒープ確保を必須にしない（FSMの遷移条件に使う`std::function`は例外。
  経緯は`docs/fsm.md`を参照）
