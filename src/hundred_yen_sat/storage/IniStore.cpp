#include "IniStore.h"

#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

namespace hundred_yen_sat {
namespace {
const char* skipSpace(const char* p, const char* end) {
    while (p < end && isspace(static_cast<unsigned char>(*p))) ++p;
    return p;
}

const char* trimEnd(const char* begin, const char* end) {
    while (end > begin && isspace(static_cast<unsigned char>(end[-1]))) --end;
    return end;
}
}

bool IniStore::load(const char* path) {
    loaded_ = false;
    const size_t size = storage_.read(path, buffer_, kBufferSize - 1);
    if (size == 0) {
        buffer_[0] = '\0';
        return false;
    }
    // パーサーはbuffer_内を直接参照するため、必ず末尾をNUL終端する。
    buffer_[size] = '\0';
    loaded_ = true;
    return true;
}

bool IniStore::findValue(const char* section, const char* key,
                         const char** value, size_t* length) const {
    if (!loaded_ || !section || !key || !value || !length) return false;

    bool inSection = false;
    const char* line = buffer_;
    // ヒープ確保を避けるため、行や値をStringへコピーせずポインタ範囲で走査する。
    while (*line) {
        const char* end = strchr(line, '\n');
        if (!end) end = line + strlen(line);
        const char* begin = skipSpace(line, end);
        const char* cleanEnd = trimEnd(begin, end);

        if (begin < cleanEnd && *begin != ';' && *begin != '#') {
            if (*begin == '[' && cleanEnd[-1] == ']') {
                // sectionが変わるたびに対象section内かを更新する。
                ++begin;
                --cleanEnd;
                const size_t sectionLength = static_cast<size_t>(cleanEnd - begin);
                inSection = strlen(section) == sectionLength &&
                            strncmp(begin, section, sectionLength) == 0;
            } else if (inSection) {
                // 対象section内だけkey=valueを解析し、最初に一致した値を返す。
                const char* equals = static_cast<const char*>(memchr(begin, '=', cleanEnd - begin));
                if (equals) {
                    const char* keyEnd = trimEnd(begin, equals);
                    const size_t keyLength = static_cast<size_t>(keyEnd - begin);
                    if (strlen(key) == keyLength && strncmp(begin, key, keyLength) == 0) {
                        const char* valueBegin = skipSpace(equals + 1, cleanEnd);
                        *value = valueBegin;
                        *length = static_cast<size_t>(trimEnd(valueBegin, cleanEnd) - valueBegin);
                        return true;
                    }
                }
            }
        }
        line = *end ? end + 1 : end;
    }
    return false;
}

bool IniStore::getString(const char* section, const char* key, char* output,
                         size_t capacity, const char* fallback) const {
    if (!output || capacity == 0) return false;
    const char* value = nullptr;
    size_t length = 0;
    const bool found = findValue(section, key, &value, &length);
    // 値がなくてもoutputはfallbackで必ずNUL終端し、呼び出し側を単純にする。
    if (!found) {
        value = fallback ? fallback : "";
        length = strlen(value);
    }
    if (length >= capacity) length = capacity - 1;
    memcpy(output, value, length);
    output[length] = '\0';
    return found;
}

long IniStore::getInt(const char* section, const char* key, long fallback) const {
    char value[32];
    if (!getString(section, key, value, sizeof(value))) return fallback;
    return strtol(value, nullptr, 0);
}

float IniStore::getFloat(const char* section, const char* key, float fallback) const {
    char value[32];
    if (!getString(section, key, value, sizeof(value))) return fallback;
    return strtof(value, nullptr);
}

bool IniStore::getBool(const char* section, const char* key, bool fallback) const {
    char value[16];
    if (!getString(section, key, value, sizeof(value))) return fallback;
    if (!strcasecmp(value, "true") || !strcasecmp(value, "yes") || !strcmp(value, "1")) return true;
    if (!strcasecmp(value, "false") || !strcasecmp(value, "no") || !strcmp(value, "0")) return false;
    return fallback;
}

}  // namespace hundred_yen_sat
