# Fsm

## 概要

状態をクラスとして実装する、小規模ミッション向けの有限状態機械。アプリ側は
`IState`を継承して各状態を実装し、`onEnter`/`onUpdate`/`onExit`をオーバーライド
する。状態間の遷移条件は`Fsm::addTransition()`で状態の外から登録し、各状態は
次の状態を知らない。

状態をクラス継承で実装し、遷移条件は`std::function`で状態の外から登録する
設計を採用している。

## API

```cpp
#include <hundred_yen_sat/fsm/Fsm.h>

struct Mission {
    Leds leds;
};

Mission mission;

class StandbyState : public IState {
public:
    explicit StandbyState(Mission& ctx) : _ctx(ctx) {}

    void onEnter() override {
        _ctx.leds.setPattern(0x01);
        _enteredAt = millis();
    }

    bool timedOut() const { return millis() - _enteredAt >= 3000; }

private:
    Mission& _ctx;
    unsigned long _enteredAt = 0;
};

class ReleasedState : public IState {
public:
    explicit ReleasedState(Mission& ctx) : _ctx(ctx) {}
    void onEnter() override { _ctx.leds.setPattern(0x0f); }

private:
    Mission& _ctx;
};

StandbyState standby(mission);
ReleasedState released(mission);
Fsm fsm;

void setup() {
    // 遷移条件は状態の外（このテーブル）で一元管理する。状態側は次状態を知らない。
    fsm.addTransition(&standby, &released, [] { return standby.timedOut(); });
    fsm.begin(&standby);
}

void loop() {
    fsm.update();
    delay(50);
}
```

完全な例（シリアルへの状態出力付き）は`examples/Fsm`を参照してください。

- `IState` 状態の基底クラス。`onEnter`/`onUpdate`/`onExit`をオーバーライド
  する（すべて省略可能）。状態固有のデータは派生クラスのメンバとして直接
  持てる。
- `Fsm::addTransition(from, to, condition = nullptr)` 遷移を登録する。
  `condition`が`nullptr`なら常に遷移する。`from`に`nullptr`を指定すると、
  どの状態からでも評価される（ただし`condition`も`nullptr`の組み合わせは
  毎フレーム強制遷移になるため登録を拒否する）。`to`に`nullptr`を指定する
  とFsmが停止状態になる。最大登録数は`Fsm::kMaxTransitions`（既定16）。
- `Fsm::begin(initial)` Fsmを開始する。`initial`の`onEnter`を呼ぶ。
- `Fsm::update()` 遷移テーブルを先頭から走査し、条件を満たした最初の遷移を
  実行する（`onExit`→切替→`onEnter`）。遷移が起きたフレームは`onUpdate`を
  呼ばない。マッチする遷移がなければ現在状態の`onUpdate`を呼ぶ。
- `Fsm::current()` 現在の状態を返す（停止状態なら`nullptr`）。

## 設計メモ

- 当初は関数ポインタ＋`void* context`の構造体方式を採用していたが、各
  コールバックで`static_cast<Mission*>(context)`という同じキャストが必ず
  必要になり、状態固有のデータをすべて共有contextに詰め込む必要があった。
  状態をクラスとして持つ設計に切り替え、状態固有のデータは派生クラスの
  メンバへ直接持てるようにして、このキャストを不要にした。
- 遷移条件は`std::function<bool()>`を採用した。`bool(*)(void*)`という関数
  ポインタ方式よりラムダで書きやすい。キャプチャ内容によってはヒープ確保
  される可能性があるため、hundred-yen-satの「ヒープ確保を必須にしない」
  方針には厳密には反するが、人間が書きやすいことを優先する判断とした。
- 遷移テーブル自体（`Transition transitions_[kMaxTransitions]`）は固定長配列
  で、動的確保はしない。
- `update()`は遷移テーブルを先頭から走査して最初にマッチした遷移を実行し、
  遷移が起きたフレームは`onUpdate`を呼ばない。テーブルの並び順がそのまま
  優先度になる。
