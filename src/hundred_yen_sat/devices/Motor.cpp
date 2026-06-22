#include "Motor.h"

namespace hundred_yen_sat {

bool Motor::begin() {
    // TB6612FNGのIN1/IN2は回転方向、PWMは出力強度を決める。
    pinMode(pins_.in1, OUTPUT);
    pinMode(pins_.in2, OUTPUT);
    pinMode(pins_.pwm, OUTPUT);
    stop();
    return true;
}

void Motor::setSpeed(int speed) {
    // 公開APIでは機体から見た前進方向を正として保持する。
    speed_ = constrain(speed, -255, 255);
    if (speed_ == 0) {
        stop();
        return;
    }

    // 左右でモーターの取り付け向きが異なる場合だけ、電気的な回転方向を反転する。
    const int outputSpeed = pins_.inverted ? -speed_ : speed_;

    // TB6612FNG: IN1=H/IN2=Lで正転、IN1=L/IN2=Hで逆転。
    digitalWrite(pins_.in1, outputSpeed > 0 ? HIGH : LOW);
    digitalWrite(pins_.in2, outputSpeed > 0 ? LOW : HIGH);
    analogWrite(pins_.pwm, abs(outputSpeed));
}

void Motor::forward(uint8_t speed) {
    setSpeed(speed);
}

void Motor::backward(uint8_t speed) {
    setSpeed(-static_cast<int>(speed));
}

void Motor::stop() {
    speed_ = 0;
    // IN1/IN2をともにLOW、PWMを0にして出力を解放する（coast stop）。
    digitalWrite(pins_.in1, LOW);
    digitalWrite(pins_.in2, LOW);
    analogWrite(pins_.pwm, 0);
}

void Motor::brake() {
    speed_ = 0;
    // IN1/IN2をともにHIGHにするとTB6612FNGはshort brake状態になる。
    digitalWrite(pins_.in1, HIGH);
    digitalWrite(pins_.in2, HIGH);
    analogWrite(pins_.pwm, 255);
}

}  // namespace hundred_yen_sat

