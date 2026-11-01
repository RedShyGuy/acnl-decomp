#pragma once

// Inline helpers of CFL around nn::fs (names are ours): paths in the multibyte encoding of the C library and the
// groups of results of the file system (module 17).

#include "decomp.h"
#include "nn/fs/detail/detail_Api.h"

#include <cwchar>

namespace cfl {
namespace detail {
const size_t MAX_PATH_LENGTH = 0x105;

DECOMP_ALWAYS_INLINE void ConvertPath(wchar_t* out, const char* path)
{
    mbstate_t state = {};
    mbsinit(&state);
    for (;;) {
        size_t length = mbrlen(path, MAX_PATH_LENGTH, &state);
        if (length == 0) {
            break;
        }
        if (mbrtowc(out, path, length, &state) == 0) {
            break;
        }
        out++;
        path += length;
    }
    *out = L'\0';
}

DECOMP_ALWAYS_INLINE bool IsNotFound(nn::Result result)
{
    return result.GetModule() == 17 && 100 <= static_cast<int>(result.GetDescription()) &&
           static_cast<int>(result.GetDescription()) <= 179;
}

DECOMP_ALWAYS_INLINE bool IsAlreadyExists(nn::Result result)
{
    return result.GetModule() == 17 && 180 <= static_cast<int>(result.GetDescription()) &&
           static_cast<int>(result.GetDescription()) <= 199;
}

DECOMP_ALWAYS_INLINE bool IsCorrupted(nn::Result result)
{
    return result.GetModule() == 17 && 230 <= static_cast<int>(result.GetDescription()) &&
           static_cast<int>(result.GetDescription()) < 340;
}

DECOMP_ALWAYS_INLINE nn::Result TryCreateFile(const char* path, s64 size)
{
    nn::fs::detail::FileSystemBase* fs = nn::fs::detail::GetGlobalFileSystemBase();
    wchar_t widePath[0x110];
    ConvertPath(widePath, path);
    return fs->TryCreateFile(widePath, size);
}

DECOMP_ALWAYS_INLINE nn::Result TryDeleteFile(const char* path)
{
    nn::fs::detail::FileSystemBase* fs = nn::fs::detail::GetGlobalFileSystemBase();
    wchar_t widePath[0x110];
    ConvertPath(widePath, path);
    return fs->TryDeleteFile(widePath);
}

DECOMP_ALWAYS_INLINE nn::Result TryCreateDirectory(const char* path)
{
    nn::fs::detail::FileSystemBase* fs = nn::fs::detail::GetGlobalFileSystemBase();
    wchar_t widePath[0x110];
    ConvertPath(widePath, path);
    return fs->TryCreateDirectory(widePath);
}
} // namespace detail
} // namespace cfl
