/**
 * @file Storage.ino
 * @brief Storageの動作確認サンプル
 *
 * /mnt/sd0へファイルを書き込み、読み戻して内容を確認する。
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Storage sd;  // デフォルトroot: /mnt/sd0

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Storage: initializing...");
    if (!sd.begin(5000)) {
        Serial.println("SD mount failed");
        while (1) delay(1000);
    }
    Serial.println("SD mounted");

    sd.makeDirectory("logs");

    const char message[] = "hello hundred-yen-sat";
    if (!sd.write("logs/storage_test.txt", message, sizeof(message) - 1)) {
        Serial.println("write failed");
        return;
    }
    Serial.println("write ok");

    char buffer[64] = {};
    size_t bytesRead = sd.read("logs/storage_test.txt", buffer, sizeof(buffer) - 1);
    Serial.printf("read %u bytes: %s\n", static_cast<unsigned>(bytesRead), buffer);
}

void loop() {
    delay(10000);
}
