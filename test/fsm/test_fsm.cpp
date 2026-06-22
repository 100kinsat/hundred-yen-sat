/**
 * @file test_fsm.cpp
 * @brief Fsm / IState のユニットテスト
 */
#include <gtest/gtest.h>

#include "hundred_yen_sat/fsm/Fsm.h"

using namespace hundred_yen_sat;

namespace {

struct CallLog {
    int enter = 0;
    int update = 0;
    int exit = 0;
    void reset() { enter = update = exit = 0; }
};

class MockState : public IState {
public:
    explicit MockState(CallLog& log) : _log(log) {}
    void onEnter() override { _log.enter++; }
    void onUpdate() override { _log.update++; }
    void onExit() override { _log.exit++; }

private:
    CallLog& _log;
};

}  // namespace

// ─────────────────────────────────────────
// begin() のテスト
// ─────────────────────────────────────────

TEST(Fsm, BeginCallsOnEnter) {
    CallLog log;
    MockState s(log);
    Fsm fsm;
    fsm.begin(&s);

    EXPECT_EQ(log.enter, 1);
    EXPECT_EQ(log.update, 0);
    EXPECT_EQ(log.exit, 0);
}

TEST(Fsm, BeginSetsCurrentState) {
    CallLog log;
    MockState s(log);
    Fsm fsm;
    fsm.begin(&s);
    EXPECT_EQ(fsm.current(), &s);
}

// ─────────────────────────────────────────
// update() のテスト
// ─────────────────────────────────────────

TEST(Fsm, UpdateCallsOnUpdateWhenNoTransition) {
    CallLog log;
    MockState s(log);
    Fsm fsm;
    fsm.addTransition(&s, &s, [] { return false; });
    fsm.begin(&s);
    log.reset();

    fsm.update();
    EXPECT_EQ(log.update, 1);
    EXPECT_EQ(log.exit, 0);
}

TEST(Fsm, UpdateDoesNotCallOnUpdateWhenTransitionOccurs) {
    CallLog logA, logB;
    MockState a(logA), b(logB);
    Fsm fsm;
    fsm.addTransition(&a, &b, [] { return true; });
    fsm.begin(&a);
    logA.reset();
    logB.reset();

    fsm.update();
    EXPECT_EQ(logA.update, 0);
    EXPECT_EQ(logB.enter, 1);
}

// ─────────────────────────────────────────
// 遷移のテスト
// ─────────────────────────────────────────

TEST(Fsm, TransitionCallsExitThenEnter) {
    CallLog logA, logB;
    MockState a(logA), b(logB);
    Fsm fsm;
    fsm.addTransition(&a, &b, [] { return true; });
    fsm.begin(&a);
    logA.reset();
    logB.reset();

    fsm.update();
    EXPECT_EQ(logA.exit, 1);
    EXPECT_EQ(logB.enter, 1);
    EXPECT_EQ(fsm.current(), &b);
}

TEST(Fsm, TransitionWithFalseConditionDoesNotTransition) {
    CallLog logA, logB;
    MockState a(logA), b(logB);
    Fsm fsm;
    fsm.addTransition(&a, &b, [] { return false; });
    fsm.begin(&a);
    logA.reset();

    fsm.update();
    EXPECT_EQ(fsm.current(), &a);
    EXPECT_EQ(logA.update, 1);
}

TEST(Fsm, TransitionWithNullConditionAlwaysOccurs) {
    CallLog logA, logB;
    MockState a(logA), b(logB);
    Fsm fsm;
    fsm.addTransition(&a, &b, nullptr);  // 常に遷移
    fsm.begin(&a);
    logA.reset();
    logB.reset();

    fsm.update();
    EXPECT_EQ(fsm.current(), &b);
    EXPECT_EQ(logB.enter, 1);
}

TEST(Fsm, FirstMatchingTransitionWins) {
    CallLog logA, logB, logC;
    MockState a(logA), b(logB), c(logC);
    Fsm fsm;
    fsm.addTransition(&a, &b, [] { return true; });
    fsm.addTransition(&a, &c, [] { return true; });  // 2番目は評価されない
    fsm.begin(&a);

    fsm.update();
    EXPECT_EQ(fsm.current(), &b);
}

TEST(Fsm, NullFromTransitionsFromAnyState) {
    CallLog logA, logB;
    MockState a(logA), b(logB);
    Fsm fsm;
    fsm.addTransition(nullptr, &b, [] { return true; });  // どこからでも
    fsm.begin(&a);
    logB.reset();

    fsm.update();
    EXPECT_EQ(fsm.current(), &b);
    EXPECT_EQ(logB.enter, 1);
}

TEST(Fsm, MultipleUpdatesProgressThroughStates) {
    CallLog logA, logB, logC;
    MockState a(logA), b(logB), c(logC);
    Fsm fsm;
    fsm.addTransition(&a, &b, [] { return true; });
    fsm.addTransition(&b, &c, [] { return true; });
    fsm.begin(&a);

    fsm.update();
    EXPECT_EQ(fsm.current(), &b);

    fsm.update();
    EXPECT_EQ(fsm.current(), &c);
}

TEST(Fsm, TransitionFromOtherStateIsSkipped) {
    CallLog logA, logB, logC;
    MockState a(logA), b(logB), c(logC);
    Fsm fsm;
    fsm.addTransition(&b, &c, [] { return true; });  // b→c（現在は a）
    fsm.addTransition(&a, &b, [] { return true; });
    fsm.begin(&a);

    fsm.update();
    EXPECT_EQ(fsm.current(), &b);
}

// ─────────────────────────────────────────
// 安全性・境界のテスト
// ─────────────────────────────────────────

TEST(Fsm, NullFromNullConditionIsRejected) {
    // from=nullptr + condition=nullptr は毎フレーム強制遷移するため登録拒否。
    CallLog logA, logB;
    MockState a(logA), b(logB);
    Fsm fsm;
    bool ok = fsm.addTransition(nullptr, &b, nullptr);
    EXPECT_FALSE(ok);

    fsm.begin(&a);
    fsm.update();
    EXPECT_EQ(fsm.current(), &a);
}

TEST(Fsm, AddTransitionReturnsFalseWhenFull) {
    CallLog logA, logB;
    MockState a(logA), b(logB);
    Fsm fsm;
    for (int i = 0; i < Fsm::kMaxTransitions; i++) {
        EXPECT_TRUE(fsm.addTransition(&a, &b, [] { return false; }));
    }
    EXPECT_FALSE(fsm.addTransition(&a, &b, [] { return false; }));
}

TEST(Fsm, TransitionToNullStopsFsm) {
    // to=nullptrへの遷移後、Fsmは停止状態になる。
    CallLog logA;
    MockState a(logA);
    Fsm fsm;
    fsm.addTransition(&a, nullptr, [] { return true; });
    fsm.begin(&a);

    fsm.update();
    EXPECT_EQ(fsm.current(), nullptr);

    logA.reset();
    fsm.update();
    EXPECT_EQ(logA.update, 0);
}
