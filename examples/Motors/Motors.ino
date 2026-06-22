/**
 * @file Motors.ino
 * @brief TB6612FNGモータードライバの動作確認サンプル
 *
 * 配線（標準）: 右モーター in1=8 in2=4 pwm=5 / 左モーター in1=7 in2=2 pwm=3
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Motors motors;  // 標準配線をデフォルトで使用

void setup() {
    Serial.begin(115200);
    delay(1000);

    if (!motors.begin()) {
        Serial.println("Motors init failed");
        while (1) delay(1000);
    }
    Serial.println("Motors ready");
}

void loop() {
    Serial.println("forward");
    motors.forward(100);
    delay(1500);

    Serial.println("stop");
    motors.stop();
    delay(1000);

    Serial.println("backward");
    motors.backward(100);
    delay(1500);

    Serial.println("brake");
    motors.brake();
    delay(500);

    Serial.println("turn right");
    motors.turnRight(100);
    delay(800);

    Serial.println("turn left");
    motors.turnLeft(100);
    delay(800);

    motors.stop();
    delay(1000);
}
