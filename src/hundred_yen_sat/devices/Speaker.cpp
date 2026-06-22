#include "Speaker.h"

#include <unistd.h>

namespace hundred_yen_sat {

bool Speaker::begin() {
    pinMode(pin_, OUTPUT);
    noTone(pin_);
    return true;
}

void Speaker::tone(uint16_t frequencyHz, uint32_t durationMs) {
    ::tone(pin_, frequencyHz, durationMs);
}

void Speaker::stop() { noTone(pin_); }

void Speaker::beep(uint16_t frequencyHz, uint32_t durationMs) {
    tone(frequencyHz, durationMs);
    // beep()は通知音向けの同期API。非ブロッキング用途ではtone()を直接使う。
    usleep(durationMs * 1000);
}

}  // namespace hundred_yen_sat
