/**
 * @file IniStore.ino
 * @brief IniStoreの動作確認サンプル
 *
 * SDカードの/mnt/sd0/config.iniを読み込み、値をシリアルへ出力する。
 *
 * 事前準備: SDカードのルートに同フォルダのconfig.iniを配置する。
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Storage sd;
IniStore config(sd);

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("IniStore: start");
    if (!sd.begin(5000)) {
        Serial.println("SD mount failed");
        while (1) delay(1000);
    }

    if (!config.load("config.ini")) {
        Serial.println("config load failed: config.ini not found?");
        while (1) delay(1000);
    }
    Serial.println("config loaded");

    int standbyAlt = config.getInt("mission", "standby_alt_threshold", 20);
    int cdsThreshold = config.getInt("mission", "launch_cds_threshold", 400);
    float descentRate = config.getFloat("mission", "descent_rate_threshold", 2.5f);
    bool serialLog = config.getBool("debug", "serial_log", false);

    Serial.println("=== config.ini ===");
    Serial.printf("[mission] standby_alt_threshold = %d\n", standbyAlt);
    Serial.printf("[mission] launch_cds_threshold = %d\n", cdsThreshold);
    Serial.printf("[mission] descent_rate_threshold = %.2f\n", descentRate);
    Serial.printf("[debug] serial_log = %s\n", serialLog ? "true" : "false");

    // 存在しないキーはデフォルト値が返ることを確認
    int missing = config.getInt("mission", "nonexistent_key", 9999);
    Serial.printf("nonexistent_key (default 9999) = %d\n", missing);
}

void loop() {
    delay(10000);
}
