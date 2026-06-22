/**
 * @file Imu.h
 * @brief I2C接続されたBNO055から姿勢・加速度・角速度・地磁気を取得する。
 */
#pragma once

#include <Arduino.h>
#include <Wire.h>

namespace hundred_yen_sat {

/** @brief 3軸の物理量。各フィールドの単位は利用する取得関数に従う。 */
struct Vector3 {
    float x;
    float y;
    float z;

    Vector3(float xValue = 0, float yValue = 0, float zValue = 0)
        : x(xValue), y(yValue), z(zValue) {}
};

/** @brief BNO055のセンサーフュージョンによる姿勢角。単位は度。 */
struct Orientation {
    float heading;
    float roll;
    float pitch;

    Orientation(float headingValue = 0, float rollValue = 0,
                float pitchValue = 0)
        : heading(headingValue), roll(rollValue), pitch(pitchValue) {}
};

/** @brief BNO055から一括取得したセンサーデータ。 */
struct ImuReading {
    Vector3 acceleration;   /**< m/s^2 */
    Vector3 gyroscope;      /**< degree/s */
    Vector3 magnetometer;   /**< microtesla */
    Orientation orientation; /**< degree */
};

/**
 * @brief BNO055専用IMUクラス。
 *
 * デフォルトI2Cアドレスは0x28。begin()でWireとセンサーを初期化した後に
 * read()または個別の取得関数を呼ぶ。
 */
class Imu {
public:
    explicit Imu(uint8_t address = 0x28, TwoWire& wire = Wire)
        : address_(address), wire_(wire) {}

    /**
     * @brief センサーを初期化する。
     * @param sensorFusion trueならNDOF、falseなら加速度のみのモードを使う。
     * @return BNO055を検出し、動作モードへ移行できた場合true。
     */
    bool begin(bool sensorFusion = true);

    /** BNO055のchip IDが読み取れるか確認する。 */
    bool connected();

    /** 加速度・角速度・地磁気・姿勢を1回の連続読み出しで更新する。 */
    bool read(ImuReading& output);

    /** 加速度をm/s^2で取得する。 */
    Vector3 acceleration();

    /** 角速度をdegree/sで取得する。 */
    Vector3 gyroscope();

    /** 地磁気をmicroteslaで取得する。 */
    Vector3 magnetometer();

    /** heading/roll/pitchを度で取得する。NDOFモードで使用する。 */
    Orientation orientation();

private:
    bool writeRegister(uint8_t reg, uint8_t value);
    bool readRegisters(uint8_t reg, uint8_t* output, size_t size);
    int16_t readInt16(uint8_t reg);
    bool setMode(uint8_t mode);

    uint8_t address_;
    TwoWire& wire_;
};

}  // namespace hundred_yen_sat
