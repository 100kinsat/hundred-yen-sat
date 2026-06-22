/**
 * @file Motor.h
 * @brief TB6612FNGの1チャンネルへ接続したDCモーターを個別に操作する。
 */
#pragma once

#include <Arduino.h>

namespace hundred_yen_sat {

/** @brief 1個のモーターに必要なTB6612FNGの方向制御ピンとPWMピン。 */
struct MotorPins {
    uint8_t in1;
    uint8_t in2;
    uint8_t pwm;
    bool inverted;

    MotorPins(uint8_t in1Pin, uint8_t in2Pin, uint8_t pwmPin,
              bool invertDirection = false)
        : in1(in1Pin), in2(in2Pin), pwm(pwmPin), inverted(invertDirection) {}
};

/**
 * @brief 左右どちらか1個のDCモーターを独立して制御する。
 *
 * setSpeed()の範囲は-255〜255。正値を前進、負値を後退として扱う。
 * モーターの取り付け方向が逆の場合はMotorPins::invertedをtrueにする。
 */
class Motor {
public:
    explicit Motor(const MotorPins& pins) : pins_(pins), speed_(0) {}

    /** 3本の制御ピンを出力に設定し、モーターを停止する。 */
    bool begin();

    /** 速度と回転方向を-255〜255で指定する。0はstop()と同じ。 */
    void setSpeed(int speed);
    void forward(uint8_t speed);
    void backward(uint8_t speed);

    /** 出力を解放して惰性停止する。 */
    void stop();

    /** TB6612FNGのshort brake状態にして能動的に停止する。 */
    void brake();

    /** 最後にsetSpeed()へ指定した論理速度を返す。 */
    int speed() const { return speed_; }

private:
    MotorPins pins_;
    int speed_;
};

}  // namespace hundred_yen_sat

