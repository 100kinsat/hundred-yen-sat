/**
 * @file Gnss.h
 * @brief Spresense内蔵GNSSの測位開始と位置情報取得を提供する。
 */
#pragma once

#include <GNSS.h>

namespace hundred_yen_sat {

/** @brief 最後に受信したGNSS測位結果。 */
struct GnssData {
    double latitude = 0;  /**< degree */
    double longitude = 0; /**< degree */
    double altitude = 0;  /**< meter */
    bool fixed = false;
    int satellites = 0;
    char utc[24] = {};
};

/** @brief GPS + QZSS（L1CA/L1S）。日本国内向けの既定構成。 */
const unsigned short kDefaultSatelliteSystems = GPS | QZ_L1CA | QZ_L1S;

/**
 * @brief Spresense ArduinoのSpGnssを簡単に扱うクラス。
 *
 * update()は非ブロッキング。測位を待つ必要がある場合だけwaitForFix()を使う。
 */
class Gnss {
public:
    /**
     * @brief 測位に使う衛星システムを選択し、測位を開始する。
     * @param satellites SpSatelliteTypeのビットOR。既定はGPS + QZSS。
     */
    bool begin(unsigned short satellites = kDefaultSatelliteSystems, bool coldStart = true);

    /** 新着データがあれば内部キャッシュを更新する。データがなければfalse。 */
    bool update();

    /** 測位確定またはtimeoutMs経過まで待つ。呼び出し中はブロックする。 */
    bool waitForFix(uint32_t timeoutMs = 300000);

    /** GNSSの測位を停止する。 */
    bool stop();

    /** 最後にupdate()で受信したデータへの参照を返す。 */
    const GnssData& data() const { return data_; }
    bool fixed() const { return data_.fixed; }

private:
    SpGnss gnss_;
    GnssData data_;
    bool started_ = false;
};

}  // namespace hundred_yen_sat
