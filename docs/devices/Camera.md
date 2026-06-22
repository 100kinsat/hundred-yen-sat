# Camera

## 概要

Spresense Camera BoardでHD JPEG静止画を撮影するクラス。静止画形式を
HD（`CAM_IMGSIZE_HD_H` x `CAM_IMGSIZE_HD_V`）のJPEGに固定している。

## API

```cpp
#include <hundred_yen_sat/devices/Camera.h>

Camera camera;

void setup() {
    if (!camera.begin()) {
        Serial.println("Camera initialization failed");
    }
    camera.setJpegQuality(80);
}

void loop() {
    Image image;
    if (camera.takePicture(image)) {
        // image.data / image.size をSDへ書き込むなどに使う
    }
}
```

- `begin()` カメラを初期化し、静止画形式をHD JPEGに固定する。失敗時は
  内部で`end()`相当の解放を行ってから`false`を返す。
- `takePicture(Image&)` 静止画を1枚撮影する。`Image::data`はCameraライブラリ
  内部バッファへの参照であり、コピーされない。次の撮影前に保存または処理
  すること。
- `setJpegQuality(quality)` 1〜100。範囲外は最寄りの値に制限される。
- `end()` カメラリソースを解放する。

## 設計メモ

- 現時点では「撮影してバッファ参照を返す」最小限の機能のみを提供している。
  保存先（SD書き込みのタイミング・頻度）や撮影トリガの設計はミッション側の
  実装方針が固まっていないため未確定。バッファがCameraライブラリ所有である
  点（次撮影で内容が上書きされ得る）を踏まえて呼び出し側で扱う必要がある。
- `begin()`失敗時に`theCamera.end()`を呼んでいるのは、フォーマット設定だけ
  失敗した中途半端な初期化状態を残すと再初期化できなくなるため。
