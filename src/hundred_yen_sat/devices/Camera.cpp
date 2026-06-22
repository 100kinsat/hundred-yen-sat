#include "Camera.h"

namespace hundred_yen_sat {

bool Camera::begin() {
    // Camera Boardを通常モードで起動し、用途をHD JPEG静止画だけに固定する。
    if (theCamera.begin() != CAM_ERR_SUCCESS) return false;

    const CamErr result = theCamera.setStillPictureImageFormat(
        CAM_IMGSIZE_HD_H, CAM_IMGSIZE_HD_V, CAM_IMAGE_PIX_FMT_JPG);
    if (result != CAM_ERR_SUCCESS) {
        // begin()だけ成功した状態を残すと再初期化できないため、失敗時に解放する。
        theCamera.end();
        return false;
    }

    started_ = true;
    return true;
}

void Camera::end() {
    if (started_) theCamera.end();
    started_ = false;
}

bool Camera::takePicture(Image& image) {
    if (!started_) return false;
    CamImage captured = theCamera.takePicture();
    if (!captured.isAvailable()) return false;
    // getImgBuff()はCameraライブラリ所有領域。ここではコピーせず参照だけ返す。
    image.data = captured.getImgBuff();
    image.size = captured.getImgSize();
    return true;
}

bool Camera::setJpegQuality(int quality) {
    if (quality < 1) quality = 1;
    if (quality > 100) quality = 100;
    return started_ && theCamera.setJPEGQuality(quality) == CAM_ERR_SUCCESS;
}

}  // namespace hundred_yen_sat
