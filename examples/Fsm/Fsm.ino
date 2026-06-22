/**
 * @file Fsm.ino
 * @brief Fsmの動作確認サンプル
 *
 * STANDBYへ入ってから3秒経過するとRELEASEDへ遷移する、時間ベースの状態遷移。
 * シリアルへ状態の出入りと残り時間を出力するので、シリアルモニタで遷移の
 * タイミングを確認できる。
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

const unsigned long kStandbyDurationMs = 3000;

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
        _lastPrintAt = 0;
        Serial.println("[STANDBY] enter");
    }

    void onUpdate() override {
        unsigned long elapsed = millis() - _enteredAt;

        // 1秒ごとに残り時間を表示する（毎ループ出力すると読みにくいため）。
        if (elapsed - _lastPrintAt >= 1000) {
            long remaining = static_cast<long>(kStandbyDurationMs) - static_cast<long>(elapsed);
            Serial.printf("[STANDBY] remaining: %ld ms\n", remaining > 0 ? remaining : 0);
            _lastPrintAt = elapsed;
        }
    }

    bool timedOut() const { return millis() - _enteredAt >= kStandbyDurationMs; }

private:
    Mission& _ctx;
    unsigned long _enteredAt = 0;
    unsigned long _lastPrintAt = 0;
};

class ReleasedState : public IState {
public:
    explicit ReleasedState(Mission& ctx) : _ctx(ctx) {}

    void onEnter() override {
        _ctx.leds.setPattern(0x0f);
        Serial.println("[RELEASED] enter");
    }

private:
    Mission& _ctx;
};

StandbyState standby(mission);
ReleasedState released(mission);
Fsm fsm;

void setup() {
    Serial.begin(115200);
    delay(1000);

    mission.leds.begin();

    // 遷移条件は状態の外（このテーブル）で一元管理する。状態側は次状態を知らない。
    fsm.addTransition(&standby, &released, [] { return standby.timedOut(); });
    fsm.begin(&standby);
}

void loop() {
    fsm.update();
    delay(50);
}
