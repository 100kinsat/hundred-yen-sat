/**
 * @file Recorder.h
 * @brief StorageへCSV行や任意データを追記する簡易レコーダー。
 */
#pragma once

#include "Storage.h"

namespace hundred_yen_sat {

/** @brief 1つの追記ファイルを管理する。 */
class Recorder {
public:
    explicit Recorder(Storage& storage) : storage_(storage), path_(nullptr) {}

    /**
     * @brief 記録先を設定する。
     * @param header ファイルが存在しない場合だけ最初の1行として書き込む。
     */
    bool begin(const char* path, const char* header = nullptr);
    /** 文字列と改行を追記する。 */
    bool writeLine(const char* line);
    /** 改行を付けずに任意のバイト列を追記する。 */
    bool write(const void* data, size_t size);
    const char* path() const { return path_; }

private:
    Storage& storage_;
    const char* path_;
};

}  // namespace hundred_yen_sat
