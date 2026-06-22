/**
 * @file Watchdog.h
 * @brief NuttXの/dev/watchdog0をPOSIX APIで制御する。
 */
#pragma once

#include <stdint.h>

namespace hundred_yen_sat {

/**
 * @brief Spresenseのハードウェアwatchdogを開始・更新する。
 *
 * begin()後はtimeoutMs以内にfeed()を繰り返すこと。停止不能な設定もあり得るため、
 * 実機投入前にリセット挙動を確認する。
 */
class Watchdog {
public:
    explicit Watchdog(const char* device = "/dev/watchdog0")
        : device_(device), fd_(-1), running_(false) {}
    ~Watchdog();

    /** watchdogを開き、timeoutを設定して開始する。 */
    bool begin(uint32_t timeoutMs);

    /** watchdogの期限を更新する。 */
    bool feed();

    /** watchdog停止を要求する。NuttX側が拒否した場合false。 */
    bool stop();
    bool running() const { return running_; }

private:
    const char* device_;
    int fd_;
    bool running_;
};

}  // namespace hundred_yen_sat
