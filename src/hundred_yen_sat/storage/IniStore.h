/**
 * @file IniStore.h
 * @brief SD上の小さなINI設定ファイルを固定長バッファへ読み込む。
 */
#pragma once

#include "Storage.h"

namespace hundred_yen_sat {

/**
 * @brief section/key形式の設定値を取得する。
 *
 * 最大1023 byteのファイルに対応する。load()後の取得処理ではファイルI/Oを行わない。
 */
class IniStore {
public:
    explicit IniStore(Storage& storage) : storage_(storage), loaded_(false) {
        buffer_[0] = '\0';
    }

    /** INIファイルを読み込み、以前の内容を置き換える。 */
    bool load(const char* path);
    bool loaded() const { return loaded_; }

    /** 文字列をoutputへコピーする。キーがなければfallbackを設定してfalse。 */
    bool getString(const char* section, const char* key, char* output,
                   size_t capacity, const char* fallback = "") const;
    /** 整数値を取得する。キーがなければfallback。 */
    long getInt(const char* section, const char* key, long fallback = 0) const;
    /** 浮動小数点値を取得する。キーがなければfallback。 */
    float getFloat(const char* section, const char* key, float fallback = 0) const;
    /** true/false、yes/no、1/0をboolとして取得する。 */
    bool getBool(const char* section, const char* key, bool fallback = false) const;

private:
    bool findValue(const char* section, const char* key,
                   const char** value, size_t* length) const;

    static const size_t kBufferSize = 1024;
    Storage& storage_;
    char buffer_[kBufferSize];
    bool loaded_;
};

}  // namespace hundred_yen_sat
