/**
 * @file Speaker.h
 * @brief PWM出力へ接続した圧電スピーカーを操作する。
 */
#pragma once

#include <Arduino.h>

namespace hundred_yen_sat {

class Speaker {
public:
    explicit Speaker(uint8_t pin = 9) : pin_(pin) {}

    /** スピーカーピンを出力に設定し、発音を停止する。 */
    bool begin();

    /** 指定周波数で鳴らす。durationMsが0ならstop()まで鳴り続ける。 */
    void tone(uint16_t frequencyHz, uint32_t durationMs = 0);
    void stop();

    /** 指定時間だけ鳴らし、その時間だけ呼び出し元をブロックする。 */
    void beep(uint16_t frequencyHz = 880, uint32_t durationMs = 200);

private:
    uint8_t pin_;
};

}  // namespace hundred_yen_sat

