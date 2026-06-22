/**
 * @file Speaker.ino
 * @brief PWMスピーカーの動作確認サンプル
 *
 * 配線: スピーカーをデフォルトpin 9とGNDに接続する。
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Speaker speaker;  // デフォルトpin 9

void setup() {
    Serial.begin(115200);
    delay(1000);

    speaker.begin();
    Serial.println("Speaker ready");

    // 起動確認ビープ（ブロッキング: setup内なので問題ない）
    speaker.beep();
}

void loop() {
    Serial.println("tone 500Hz");
    speaker.tone(500, 300);
    delay(500);

    Serial.println("tone 1000Hz");
    speaker.tone(1000, 300);
    delay(2000);
}
