/**
 * @file Motors.h
 * @brief 左右2個のMotorをまとめて移動指示として操作する。
 */
#pragma once

#include "Motor.h"

namespace hundred_yen_sat {

/** @brief Hundred Yen SAT標準配線を含む左右モーター設定。 */
struct MotorsPins {
    MotorPins right;
    MotorPins left;

    MotorsPins(const MotorPins& rightPins = MotorPins(8, 4, 5),
               const MotorPins& leftPins = MotorPins(7, 2, 3))
        : right(rightPins), left(leftPins) {}
};

/**
 * @brief 左右モーターの一括移動APIと、各モーターへの個別アクセスを提供する。
 *
 * right()/left()で取得したMotorには、個別にsetSpeed()、stop()、brake()を
 * 呼び出せる。drive()は両方の速度を同じタイミングで設定するための補助API。
 */
class Motors {
public:
    explicit Motors(const MotorsPins& pins = MotorsPins())
        : right_(pins.right), left_(pins.left) {}

    /** 左右両方のモーターを初期化する。 */
    bool begin();

    /** 右・左の速度をそれぞれ-255〜255で指定する。 */
    void drive(int rightSpeed, int leftSpeed);
    void forward(uint8_t speed);
    void backward(uint8_t speed);
    void turnRight(uint8_t speed);
    void turnLeft(uint8_t speed);
    void stop();
    void brake();

    /** 右モーターを個別に操作する。 */
    Motor& right() { return right_; }
    const Motor& right() const { return right_; }

    /** 左モーターを個別に操作する。 */
    Motor& left() { return left_; }
    const Motor& left() const { return left_; }

private:
    Motor right_;
    Motor left_;
};

}  // namespace hundred_yen_sat

