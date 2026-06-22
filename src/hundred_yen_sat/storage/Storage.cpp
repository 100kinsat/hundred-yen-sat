#include "Storage.h"

#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

namespace hundred_yen_sat {

Storage::Storage(const char* root) : root_(root) {}

bool Storage::begin(uint32_t timeoutMs) {
    const uint32_t startedAt = millis();
    do {
        // Arduino SD APIによる初期化は行わず、NuttX側のmount完了だけを確認する。
        if (mounted()) return true;
        usleep(50000);
    } while (millis() - startedAt < timeoutMs);
    return false;
}

bool Storage::mounted() const {
    struct stat info;
    return root_ && stat(root_, &info) == 0 && S_ISDIR(info.st_mode);
}

bool Storage::resolve(const char* path, char* output, size_t capacity) const {
    if (!root_ || !path || !output || capacity == 0) return false;
    const char* separator = path[0] == '/' ? "" : "/";
    const int length = snprintf(output, capacity, "%s%s%s", root_, separator, path);
    return length > 0 && static_cast<size_t>(length) < capacity;
}

bool Storage::exists(const char* path) const {
    char fullPath[128];
    struct stat info;
    return resolve(path, fullPath, sizeof(fullPath)) && stat(fullPath, &info) == 0;
}

bool Storage::makeDirectory(const char* path) const {
    char fullPath[128];
    if (!resolve(path, fullPath, sizeof(fullPath))) return false;
    if (mkdir(fullPath, 0777) == 0) return true;
    struct stat info;
    return stat(fullPath, &info) == 0 && S_ISDIR(info.st_mode);
}

bool Storage::writeWithFlags(const char* path, const void* data, size_t size,
                             int flags) const {
    // SDが抜かれた場合もopen()へ進まないよう、操作ごとにmountpointを再確認する。
    if (!mounted() || (!data && size != 0)) return false;

    char fullPath[128];
    if (!resolve(path, fullPath, sizeof(fullPath))) return false;

    const int fd = open(fullPath, flags, 0666);
    if (fd < 0) return false;

    const uint8_t* bytes = static_cast<const uint8_t*>(data);
    size_t total = 0;
    // write()は要求サイズ未満で正常復帰する場合があるため、全量まで繰り返す。
    while (total < size) {
        const ssize_t result = ::write(fd, bytes + total, size - total);
        if (result <= 0) {
            close(fd);
            return false;
        }
        total += static_cast<size_t>(result);
    }
    // 電源断前に媒体へ反映されるよう、成功時はclose前に明示的に同期する。
    fsync(fd);
    close(fd);
    return true;
}

bool Storage::append(const char* path, const void* data, size_t size) const {
    return writeWithFlags(path, data, size, O_WRONLY | O_CREAT | O_APPEND);
}

bool Storage::write(const char* path, const void* data, size_t size) const {
    return writeWithFlags(path, data, size, O_WRONLY | O_CREAT | O_TRUNC);
}

size_t Storage::read(const char* path, void* data, size_t capacity) const {
    if (!mounted() || !data || capacity == 0) return 0;

    char fullPath[128];
    if (!resolve(path, fullPath, sizeof(fullPath))) return 0;

    const int fd = open(fullPath, O_RDONLY);
    if (fd < 0) return 0;

    size_t total = 0;
    // ファイルサイズがcapacityを超える場合はcapacityまで読み、呼び出し側に総量を返す。
    while (total < capacity) {
        const ssize_t result = ::read(fd, static_cast<uint8_t*>(data) + total,
                                      capacity - total);
        if (result <= 0) break;
        total += static_cast<size_t>(result);
    }
    close(fd);
    return total;
}

}  // namespace hundred_yen_sat
    // 呼び出し側にはrootからの相対パスだけを扱わせ、mount先を一箇所へ閉じ込める。
