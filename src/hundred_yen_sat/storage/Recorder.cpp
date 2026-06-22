#include "Recorder.h"

#include <string.h>

namespace hundred_yen_sat {

bool Recorder::begin(const char* path, const char* header) {
    if (!path || !storage_.mounted()) return false;
    path_ = path;
    // 既存ログへ再起動のたびにCSVヘッダを重複追記しない。
    if (!header || storage_.exists(path_)) return true;
    return writeLine(header);
}

bool Recorder::writeLine(const char* line) {
    if (!path_ || !line) return false;
    if (!storage_.append(path_, line, strlen(line))) return false;
    return storage_.append(path_, "\n", 1);
}

bool Recorder::write(const void* data, size_t size) {
    return path_ && storage_.append(path_, data, size);
}

}  // namespace hundred_yen_sat
    // 本文と改行を分け、呼び出し側が改行コードを意識しなくてよいAPIにする。
