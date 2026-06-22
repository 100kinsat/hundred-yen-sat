/**
 * @file Gnss.ino
 * @brief Spresense内蔵GNSSの動作確認サンプル
 *
 * 測位が確定するまで待機し、その後は緯度・経度・高度・衛星数を周期的に出力する。
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Gnss gnss;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("GNSS initializing...");
    if (!gnss.begin()) {
        Serial.println("GNSS begin() failed");
        while (1) delay(1000);
    }

    Serial.println("GNSS started. Waiting for fix...");
    if (!gnss.waitForFix(5UL * 60UL * 1000UL)) {
        Serial.println("GNSS fix timeout");
        while (1) delay(1000);
    }
    Serial.println("GNSS fix acquired!");
}

void loop() {
    gnss.update();
    const GnssData& data = gnss.data();
    Serial.printf("lat=%.6f lon=%.6f alt=%.1fm sats=%d fix=%d utc=%s\n",
                  data.latitude, data.longitude, data.altitude, data.satellites,
                  gnss.fixed() ? 1 : 0, data.utc);
    delay(1000);
}
