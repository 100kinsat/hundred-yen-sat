/**
 * @file Camera.h
 * @brief Spresense Camera BoardでJPEG静止画を撮影する。
 */
#pragma once

#include <Camera.h>

namespace hundred_yen_sat {

/**
 * @brief 撮影したJPEG画像の参照。
 *
 * dataはCameraライブラリ内部のバッファを指す。次の撮影前に保存または処理すること。
 */
struct Image {
    void* data = nullptr;
    size_t size = 0;
};

/** @brief Spresense Camera BoardをHD JPEG静止画専用で扱う。 */
class Camera {
public:
    /** カメラを初期化し、静止画形式をHD JPEGへ設定する。 */
    bool begin();

    /** カメラリソースを解放する。 */
    void end();

    /** JPEG静止画を撮影し、利用可能な画像バッファをimageへ設定する。 */
    bool takePicture(Image& image);

    /** JPEG品質を1〜100で設定する。範囲外は最寄りの値へ制限される。 */
    bool setJpegQuality(int quality);

private:
    bool started_ = false;
};

}  // namespace hundred_yen_sat
