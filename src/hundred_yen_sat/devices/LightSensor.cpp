#include "LightSensor.h"

namespace hundred_yen_sat {

bool LightSensor::begin() {
    // SpresenseのアナログピンはpinMode/digitalWrite系のデジタル関数を受け付けない。
    // analogRead()側で必要な初期化を遅延実行するため、ここでは何もしない。
    return true;
}

uint16_t LightSensor::read() const {
    return static_cast<uint16_t>(analogRead(pin_));
}

}  // namespace hundred_yen_sat

