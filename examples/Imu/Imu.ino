/**
 * @file Imu.ino
 * @brief BNO055の動作確認サンプル
 *
 * I2C接続のBNO055から加速度・角速度・地磁気・姿勢角を読み、シリアルへ出力する。
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Imu imu;  // デフォルトでWireと0x28を使用

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("IMU initializing...");
    if (!imu.begin()) {
        Serial.println("BNO055 init failed");
        while (1) delay(1000);
    }
    Serial.println("IMU ready");
}

void loop() {
    ImuReading reading;
    if (!imu.read(reading)) {
        Serial.println("IMU read failed");
        delay(100);
        return;
    }

    Serial.printf("acc  x:%.4f y:%.4f z:%.4f\n", reading.acceleration.x,
                  reading.acceleration.y, reading.acceleration.z);
    Serial.printf("gyro x:%.4f y:%.4f z:%.4f\n", reading.gyroscope.x,
                  reading.gyroscope.y, reading.gyroscope.z);
    Serial.printf("mag  x:%.4f y:%.4f z:%.4f\n", reading.magnetometer.x,
                  reading.magnetometer.y, reading.magnetometer.z);
    Serial.printf("ori  heading:%.2f roll:%.2f pitch:%.2f\n",
                  reading.orientation.heading, reading.orientation.roll,
                  reading.orientation.pitch);

    delay(200);
}
