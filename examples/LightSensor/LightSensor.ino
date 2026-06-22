/**
 * @file LightSensor.ino
 * @brief CdS光センサーの動作確認サンプル
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

LightSensor light;  // デフォルトA0

void setup() {
    Serial.begin(115200);
    delay(1000);

    light.begin();
    Serial.println("LightSensor ready");
}

void loop() {
    Serial.printf("light: %u\n", light.read());
    delay(1000);
}
