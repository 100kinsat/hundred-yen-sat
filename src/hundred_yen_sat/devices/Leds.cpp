#include "Leds.h"

namespace hundred_yen_sat {
namespace {
const uint8_t kLedPins[] = {LED0, LED1, LED2, LED3};
}

bool Leds::begin() {
    for (uint8_t pin : kLedPins) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, LOW);
    }
    pattern_ = 0;
    return true;
}

bool Leds::set(uint8_t index, bool enabled) {
    if (index >= 4) return false;
    digitalWrite(kLedPins[index], enabled ? HIGH : LOW);
    if (enabled) pattern_ |= static_cast<uint8_t>(1U << index);
    else pattern_ &= static_cast<uint8_t>(~(1U << index));
    return true;
}

bool Leds::toggle(uint8_t index) {
    if (index >= 4) return false;
    return set(index, (pattern_ & (1U << index)) == 0);
}

void Leds::setPattern(uint8_t pattern) {
    for (uint8_t i = 0; i < 4; ++i) set(i, (pattern & (1U << i)) != 0);
}

}  // namespace hundred_yen_sat
    // ハードウェア出力と内部patternを同じ関数で更新し、状態の不一致を防ぐ。
