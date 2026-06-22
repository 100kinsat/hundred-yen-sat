#include "Watchdog.h"

#include <fcntl.h>
#include <nuttx/timers/watchdog.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace hundred_yen_sat {

Watchdog::~Watchdog() {
    // デストラクタではfdだけ解放する。通常運用ではグローバル寿命の利用を想定する。
    if (fd_ >= 0) close(fd_);
}

bool Watchdog::begin(uint32_t timeoutMs) {
    if (!device_ || timeoutMs == 0) return false;
    if (fd_ >= 0) close(fd_);
    fd_ = open(device_, O_RDWR);
    if (fd_ < 0) return false;
    // timeout設定が成功してから開始する。片方でも失敗したら不完全なfdを残さない。
    if (ioctl(fd_, WDIOC_SETTIMEOUT, static_cast<unsigned long>(timeoutMs)) < 0 ||
        ioctl(fd_, WDIOC_START, 0) < 0) {
        close(fd_);
        fd_ = -1;
        return false;
    }
    running_ = true;
    return true;
}

bool Watchdog::feed() {
    // KEEPALIVEはカウンタを初期値へ戻す。開始前の呼び出しは拒否する。
    return running_ && fd_ >= 0 && ioctl(fd_, WDIOC_KEEPALIVE, 0) >= 0;
}

bool Watchdog::stop() {
    if (fd_ < 0) return false;
    const bool success = ioctl(fd_, WDIOC_STOP, 0) >= 0;
    if (success) running_ = false;
    return success;
}

}  // namespace hundred_yen_sat
