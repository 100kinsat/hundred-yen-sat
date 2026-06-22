/**
 * @file Storage.h
 * @brief Arduino SD APIを使わず、マウント済みSDをPOSIX APIで操作する。
 */
#pragma once

#include <Arduino.h>
#include <stddef.h>

namespace hundred_yen_sat {

/**
 * @brief /mnt/sd0を基準とするPOSIXストレージ。
 *
 * begin()はSDの初期化やmountを行わず、指定ディレクトリの出現だけを待つ。
 * 未認識SDへArduino SD APIを呼んだ際のハング経路を避けるための設計。
 */
class Storage {
public:
    explicit Storage(const char* root = "/mnt/sd0");

    /** rootがディレクトリとして確認できるまで待つ。 */
    bool begin(uint32_t timeoutMs = 5000);

    /** rootが現在存在するか確認する。 */
    bool mounted() const;
    const char* root() const { return root_; }

    /** rootからの相対パスが存在するか確認する。 */
    bool exists(const char* path) const;
    /** root直下のディレクトリを1階層作る。再帰作成は行わない。 */
    bool makeDirectory(const char* path) const;
    /** ファイル末尾へsize byteを追記し、fsyncして閉じる。 */
    bool append(const char* path, const void* data, size_t size) const;
    /** ファイルを切り詰め、size byteを書き込み、fsyncして閉じる。 */
    bool write(const char* path, const void* data, size_t size) const;
    /** 最大capacity byteを読み、実際に読んだbyte数を返す。 */
    size_t read(const char* path, void* data, size_t capacity) const;

private:
    bool resolve(const char* path, char* output, size_t capacity) const;
    bool writeWithFlags(const char* path, const void* data, size_t size,
                        int flags) const;

    const char* root_;
};

}  // namespace hundred_yen_sat
