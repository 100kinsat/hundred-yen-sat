#include "Fsm.h"

namespace hundred_yen_sat {

bool Fsm::addTransition(IState* from, IState* to, std::function<bool()> condition) {
    // from=nullptr(任意状態) かつ condition=nullptr(常に遷移)は
    // 毎フレーム強制遷移になるため禁止する。
    if (from == nullptr && !condition) return false;
    if (count_ >= kMaxTransitions) return false;

    transitions_[count_++] = {from, to, condition};
    return true;
}

void Fsm::begin(IState* initial) {
    current_ = initial;
    if (current_) current_->onEnter();
}

void Fsm::update() {
    if (!current_) return;

    // 遷移テーブルを先頭から走査する。並び順がそのまま優先度になる。
    for (int i = 0; i < count_; ++i) {
        const Transition& t = transitions_[i];

        // from=nullptrはどの状態からでも遷移対象になる。
        if (t.from != nullptr && t.from != current_) continue;
        // condition=nullptrは常に遷移する。
        if (t.condition && !t.condition()) continue;

        // 遷移実行は onExit -> 切替 -> onEnter。このフレームはonUpdateを呼ばない。
        current_->onExit();
        current_ = t.to;
        if (current_) current_->onEnter();
        return;
    }

    current_->onUpdate();
}

}  // namespace hundred_yen_sat
