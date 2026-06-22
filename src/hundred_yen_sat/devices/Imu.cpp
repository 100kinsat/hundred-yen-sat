#include "Imu.h"

#include <unistd.h>

namespace hundred_yen_sat {
namespace {
// BNO055のPage 0で使用するレジスタ。センサーデータは0x08〜0x1fに連続している。
constexpr uint8_t kChipId = 0x00;
constexpr uint8_t kChipIdValue = 0xA0;
constexpr uint8_t kAccelData = 0x08;
constexpr uint8_t kMagData = 0x0E;
constexpr uint8_t kGyroData = 0x14;
constexpr uint8_t kEulerData = 0x1A;
constexpr uint8_t kUnitSelect = 0x3B;
constexpr uint8_t kOperationMode = 0x3D;
constexpr uint8_t kSystemTrigger = 0x3F;
constexpr uint8_t kModeConfig = 0x00;
constexpr uint8_t kModeNdof = 0x0C;
constexpr uint8_t kModeAccelOnly = 0x01;
}

bool Imu::begin(bool sensorFusion) {
    wire_.begin();
    // 設定変更はCONFIGモードでのみ許可されるため、接続確認後に一度移行する。
    if (!connected() || !setMode(kModeConfig)) return false;

    // ソフトウェアリセット後はBNO055がI2C応答を再開するまで最大650ms程度かかる。
    if (!writeRegister(kSystemTrigger, 0x20)) return false;
    usleep(650000);
    if (!connected() || !setMode(kModeConfig)) return false;
    if (!writeRegister(kSystemTrigger, 0x00)) return false;
    // UNIT_SEL=0: 加速度=m/s^2、角速度=degree/s、姿勢角=degree。
    if (!writeRegister(kUnitSelect, 0x00)) return false;
    return setMode(sensorFusion ? kModeNdof : kModeAccelOnly);
}

bool Imu::connected() {
    uint8_t value = 0;
    return readRegisters(kChipId, &value, 1) && value == kChipIdValue;
}

bool Imu::read(ImuReading& output) {
    // 加速度からEuler角までを1トランザクションで読み、取得時刻のずれを抑える。
    uint8_t data[24];
    if (!readRegisters(kAccelData, data, sizeof(data))) return false;
    auto value = [&data](size_t offset) {
        // BNO055の16bitセンサーデータはLSB先行のlittle endian。
        return static_cast<int16_t>((static_cast<uint16_t>(data[offset + 1]) << 8) |
                                    data[offset]);
    };
    output.acceleration = {value(0) / 100.0f, value(2) / 100.0f, value(4) / 100.0f};
    output.magnetometer = {value(6) / 16.0f, value(8) / 16.0f, value(10) / 16.0f};
    output.gyroscope = {value(12) / 16.0f, value(14) / 16.0f, value(16) / 16.0f};
    output.orientation = {value(18) / 16.0f, value(20) / 16.0f, value(22) / 16.0f};
    return true;
}

Vector3 Imu::acceleration() {
    return {readInt16(kAccelData) / 100.0f, readInt16(kAccelData + 2) / 100.0f,
            readInt16(kAccelData + 4) / 100.0f};
}

Vector3 Imu::gyroscope() {
    return {readInt16(kGyroData) / 16.0f, readInt16(kGyroData + 2) / 16.0f,
            readInt16(kGyroData + 4) / 16.0f};
}

Vector3 Imu::magnetometer() {
    return {readInt16(kMagData) / 16.0f, readInt16(kMagData + 2) / 16.0f,
            readInt16(kMagData + 4) / 16.0f};
}

Orientation Imu::orientation() {
    return {readInt16(kEulerData) / 16.0f, readInt16(kEulerData + 2) / 16.0f,
            readInt16(kEulerData + 4) / 16.0f};
}

bool Imu::writeRegister(uint8_t reg, uint8_t value) {
    wire_.beginTransmission(address_);
    wire_.write(reg);
    wire_.write(value);
    return wire_.endTransmission() == 0;
}

bool Imu::readRegisters(uint8_t reg, uint8_t* output, size_t size) {
    // 読み取り開始レジスタをwriteした後、同一デバイスから連続readする。
    wire_.beginTransmission(address_);
    wire_.write(reg);
    if (wire_.endTransmission() != 0) return false;
    if (wire_.requestFrom(address_, static_cast<uint8_t>(size)) != size) return false;
    for (size_t i = 0; i < size; ++i) output[i] = wire_.read();
    return true;
}

int16_t Imu::readInt16(uint8_t reg) {
    uint8_t data[2] = {};
    if (!readRegisters(reg, data, sizeof(data))) return 0;
    return static_cast<int16_t>((static_cast<uint16_t>(data[1]) << 8) | data[0]);
}

bool Imu::setMode(uint8_t mode) {
    if (!writeRegister(kOperationMode, mode)) return false;
    // モード遷移直後のレジスタアクセスを避けるため、データシート推奨時間を待つ。
    usleep((mode == kModeConfig ? 25 : 20) * 1000);
    return true;
}

}  // namespace hundred_yen_sat
    // データシート記載のLSB/物理量で各値を実単位へ変換する。
