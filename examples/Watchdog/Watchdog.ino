/**
 * @file Watchdog.ino
 * @brief Watchdogの動作確認サンプル
 *
 * 1回feedした後、意図的にfeedを止めてリセットがかかることを確認する。
 *
 * 期待ログ:
 *   Watchdog start (timeout=3000ms)
 *   feed #1
 *   --- stop feeding, waiting for reset ---
 *   （3秒後にボードがリセットされ、再び"Watchdog start"から始まる）
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Watchdog watchdog;

void setup() {
    Serial.begin(115200);
    delay(1000);

    if (!watchdog.begin(3000)) {
        Serial.println("watchdog.begin() failed");
        return;
    }
    Serial.println("Watchdog start (timeout=3000ms)");

    delay(1000);
    watchdog.feed();
    Serial.println("feed #1");

    delay(500);
    Serial.println("--- stop feeding, waiting for reset ---");
    // feedせずに待つ → タイムアウトでリセット
}

void loop() {
    // ここには来ない（setupでリセット待ちに入る）
}
