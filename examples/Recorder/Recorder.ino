/**
 * @file Recorder.ino
 * @brief Recorderの動作確認サンプル
 *
 * SDカードにダミーのセンサーデータをCSV形式で書き込む。
 * 書き込み後PCで/mnt/sd0/logs/sensor.csvを確認する。
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Storage sd;
Recorder recorder(sd);

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Recorder: initializing...");
    if (!sd.begin(5000)) {
        Serial.println("SD mount failed");
        while (1) delay(1000);
    }
    sd.makeDirectory("logs");

    if (!recorder.begin("logs/sensor.csv", "timestamp_ms,lat,lon,alt_m,cds")) {
        Serial.println("recorder begin failed");
        while (1) delay(1000);
    }
    Serial.println("recorder ready");
}

void loop() {
    static int count = 0;

    char line[64];
    snprintf(line, sizeof(line), "%lu,35.722366,139.895361,10.5,%d", millis(), count);

    if (!recorder.writeLine(line)) {
        Serial.println("writeLine failed");
    } else {
        Serial.printf("wrote: %s\n", line);
    }

    if (++count >= 100) {
        Serial.println("done. 100 lines written to /mnt/sd0/logs/sensor.csv");
        while (1) delay(10000);
    }

    delay(200);
}
