/**
 * @file Leds.h
 * @brief Spresense本体に搭載された4個のLEDを操作する。
 */
#pragma once

#include <Arduino.h>

namespace hundred_yen_sat {

/**
 * @brief Spresense内蔵LED（LED0〜LED3）の制御クラス。
 *
 * begin()後、LED番号を0〜3で指定して操作する。setPattern()ではbit 0が
 * LED0、bit 3がLED3に対応するため、ミッション状態の簡易表示にも使える。
 */
class Leds {
public:
    /** 全LEDピンを出力に設定し、消灯する。 */
    bool begin();

    /** 指定したLEDを点灯または消灯する。indexが0〜3以外ならfalse。 */
    bool set(uint8_t index, bool enabled);
    bool on(uint8_t index) { return set(index, true); }
    bool off(uint8_t index) { return set(index, false); }

    /** 指定したLEDの状態を反転する。 */
    bool toggle(uint8_t index);

    /** 下位4bitをLED0〜LED3へ一括反映する。 */
    void setPattern(uint8_t pattern);

    /** 現在の点灯状態を下位4bitで返す。 */
    uint8_t pattern() const { return pattern_; }

private:
    uint8_t pattern_ = 0;
};

}  // namespace hundred_yen_sat

