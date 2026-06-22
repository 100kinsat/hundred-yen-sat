/**
 * @file Camera.ino
 * @brief Spresense Camera BoardによるHD JPEG撮影の動作確認サンプル
 *
 * 保存先（SD書き込みのタイミング等）は未確定のため、このサンプルは
 * 撮影とバッファサイズの確認だけを行う。
 */
#include <HundredYenSAT.h>

using namespace hundred_yen_sat;

Camera camera;

void setup() {
    Serial.begin(115200);
    delay(1000);

    Serial.println("Camera initializing...");
    if (!camera.begin()) {
        Serial.println("Camera init failed");
        while (1) delay(1000);
    }
    camera.setJpegQuality(80);
    Serial.println("Camera ready");
}

void loop() {
    Image image;
    if (camera.takePicture(image)) {
        Serial.printf("captured: %u bytes\n", static_cast<unsigned>(image.size));
    } else {
        Serial.println("takePicture failed");
    }
    delay(3000);
}
