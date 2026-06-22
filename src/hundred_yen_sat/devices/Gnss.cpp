#include "Gnss.h"

#include <Arduino.h>
#include <stdio.h>
#include <unistd.h>

namespace hundred_yen_sat {

bool Gnss::begin(unsigned short satellites, bool coldStart) {
    if (gnss_.begin() != 0) return false;
    // select()はSpSatelliteTypeを1個ずつ受け取るため、ビットごとに呼び出す。
    for (unsigned short bit = 1; bit != 0; bit = static_cast<unsigned short>(bit << 1)) {
        if (satellites & bit) gnss_.select(static_cast<SpSatelliteType>(bit));
    }
    if (gnss_.start(coldStart ? COLD_START : HOT_START) != 0) return false;
    started_ = true;
    return true;
}

bool Gnss::update() {
    // timeout=0にしてArduinoのloop()を止めない非ブロッキング更新とする。
    if (!started_ || !gnss_.waitUpdate(0)) return false;
    SpNavData nav;
    gnss_.getNavData(&nav);
    // データ受信と測位確定は別状態。FixInvalidなら位置データがあっても未確定扱い。
    data_.fixed = nav.posDataExist && nav.posFixMode != FixInvalid;
    if (nav.posDataExist) {
        data_.latitude = nav.latitude;
        data_.longitude = nav.longitude;
        data_.altitude = nav.altitude;
        data_.satellites = nav.numSatellites;
        snprintf(data_.utc, sizeof(data_.utc), "%04d-%02d-%02dT%02d:%02d:%02dZ",
                 nav.time.year, nav.time.month, nav.time.day,
                 nav.time.hour, nav.time.minute, nav.time.sec);
    }
    return true;
}

bool Gnss::waitForFix(uint32_t timeoutMs) {
    const uint32_t startedAt = millis();
    while (millis() - startedAt < timeoutMs) {
        update();
        if (fixed()) return true;
        // GNSS更新周期より十分短いsleepを入れ、待機中のビジーループを避ける。
        usleep(50000);
    }
    return false;
}

bool Gnss::stop() {
    if (!started_) return true;
    const bool success = gnss_.stop() == 0;
    if (success) started_ = false;
    return success;
}

}  // namespace hundred_yen_sat
