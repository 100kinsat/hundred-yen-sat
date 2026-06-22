/**
 * @file Fsm.h
 * @brief 状態をクラスとして実装する、小規模ミッション向け有限状態機械。
 */
#pragma once

#include <functional>

namespace hundred_yen_sat {

/**
 * @brief FSM状態の基底クラス。
 *
 * アプリ側でこのクラスを継承して各状態を実装する。遷移条件はFsmクラスの
 * addTransition()で定義するため、各状態は自分の処理だけに集中できる
 * （次の状態を知らなくてよい）。
 */
class IState {
public:
    virtual ~IState() = default;

    /** @brief 状態に入ったとき1回呼ばれる。 */
    virtual void onEnter() {}

    /** @brief 遷移が起きなかったフレームに呼ばれる。 */
    virtual void onUpdate() {}

    /** @brief 状態から出るとき1回呼ばれる。 */
    virtual void onExit() {}
};

/**
 * @brief 遷移テーブルベースのステートマシン。
 *
 * 遷移条件は状態の外（addTransition）で一元管理する。各状態は次の状態を
 * 知らなくてよい。
 */
class Fsm {
public:
    /** @brief 登録できる遷移の最大数。 */
    static constexpr int kMaxTransitions = 16;

    /**
     * @brief 遷移を登録する。
     * @param from 遷移元状態。nullptrならどの状態からでも評価される。
     * @param to 遷移先状態。nullptrを指定するとFsmが停止状態になる。
     * @param condition 遷移条件。nullptrなら常に遷移する。
     * @return 登録成功時true。最大数超過、または
     *         from=nullptr かつ condition=nullptr の禁止パターンはfalse。
     */
    bool addTransition(IState* from, IState* to, std::function<bool()> condition = nullptr);

    /** @brief Fsmを開始する。initial=nullptrなら停止状態で開始する。 */
    void begin(IState* initial);

    /**
     * @brief 1フレーム分の更新処理（メインループから毎回呼ぶ）。
     *
     * 遷移テーブルを先頭から走査し、条件を満たした最初の遷移を実行する。
     * 遷移が起きたフレームはonUpdateを呼ばない。停止状態のときは何もしない。
     */
    void update();

    /** @brief 現在の状態を返す（停止状態のときはnullptr）。 */
    IState* current() const { return current_; }

private:
    struct Transition {
        IState* from;
        IState* to;
        std::function<bool()> condition;
    };

    IState* current_ = nullptr;
    Transition transitions_[kMaxTransitions] = {};
    int count_ = 0;
};

}  // namespace hundred_yen_sat
