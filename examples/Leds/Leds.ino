/**
 * @file Leds.ino
 * @brief Spresense内蔵LED（LED0〜LED3）の動作確認サンプル
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Leds leds;

void setup() {
    leds.begin();
    Serial.begin(115200);
    delay(1000);
    Serial.println("Leds ready");
}

void loop() {
    Serial.println("all on");
    leds.setPattern(0x0f);
    delay(500);

    Serial.println("all off");
    leds.setPattern(0x00);
    delay(500);

    Serial.println("sequence");
    for (uint8_t i = 0; i < 4; ++i) {
        leds.on(i);
        delay(150);
        leds.off(i);
    }

    Serial.println("toggle x4");
    for (int i = 0; i < 4; ++i) {
        leds.toggle(0);
        leds.toggle(3);
        delay(200);
        leds.toggle(1);
        leds.toggle(2);
        delay(200);
    }
    leds.setPattern(0x00);
    delay(500);
}
