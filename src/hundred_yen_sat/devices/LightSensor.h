/**
 * @file LightSensor.h
 * @brief アナログ入力に接続したCdS光センサーを読み取る。
 */
#pragma once

#include <Arduino.h>

namespace hundred_yen_sat {

/** @brief CdS光センサーの簡易クラス。デフォルトではA0を使用する。 */
class LightSensor {
public:
    explicit LightSensor(uint8_t pin = A0) : pin_(pin) {}

    /** アナログピンを入力として初期化する。 */
    bool begin();

    /** analogRead()の値を返す。Spresense Arduinoでは通常0〜1023。 */
    uint16_t read() const;

private:
    uint8_t pin_;
};

}  // namespace hundred_yen_sat

