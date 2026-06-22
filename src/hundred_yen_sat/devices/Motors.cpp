#include "Motors.h"

namespace hundred_yen_sat {

bool Motors::begin() {
    // 片方だけ初期化に失敗した場合でも、もう片方を安全な停止状態へ初期化する。
    const bool rightReady = right_.begin();
    const bool leftReady = left_.begin();
    return rightReady && leftReady;
}

void Motors::drive(int rightSpeed, int leftSpeed) {
    // 値の制限と方向ピンの設定は各Motorへ任せる。
    right_.setSpeed(rightSpeed);
    left_.setSpeed(leftSpeed);
}

void Motors::forward(uint8_t speed) {
    drive(speed, speed);
}

void Motors::backward(uint8_t speed) {
    drive(-static_cast<int>(speed), -static_cast<int>(speed));
}

void Motors::turnRight(uint8_t speed) {
    // 右を前進、左を後退させてその場で右旋回する。
    drive(speed, -static_cast<int>(speed));
}

void Motors::turnLeft(uint8_t speed) {
    // 右を後退、左を前進させてその場で左旋回する。
    drive(-static_cast<int>(speed), speed);
}

void Motors::stop() {
    right_.stop();
    left_.stop();
}

void Motors::brake() {
    right_.brake();
    left_.brake();
}

}  // namespace hundred_yen_sat
