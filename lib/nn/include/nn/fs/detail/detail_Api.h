#pragma once

#include <cwchar>

#include "decomp.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/fs/ipc/ipc_FileSystem.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
class UserFileSystem;
}
}
}

namespace detail {
// What the user API (FileInputStream etc.) works on: nn::fs::Initialize registers the one with the
// UserFileSystem. The class name is from the symbols, the member and Initialize are ours.
class FileSystemBase
{
public:
    FileSystemBase() : mFileSystem(0) {}

    void Initialize(nn::fs::CTR::MPCore::detail::UserFileSystem* fileSystem) { mFileSystem = fileSystem; }

    // inline (e.g. in nn::ubl; names are ours): the UserFileSystem functions are static
    nn::Result TryDeleteFile(const wchar_t* path)
    {
        return nn::fs::CTR::MPCore::detail::UserFileSystem::TryDeleteFile(path);
    }
    // the path converted to UTF-16 first (inline, used by the game's save code)
    nn::Result TryDeleteFile(const char* path)
    {
        wchar_t widePath[0x105 + 3];
        ToWidePath(widePath, path);
        return TryDeleteFile(widePath);
    }
    nn::Result TryCreateFile(const char* path, s64 size)
    {
        wchar_t widePath[0x105 + 3];
        ToWidePath(widePath, path);
        return TryCreateFile(widePath, size);
    }
    nn::Result TryCreateFile(const wchar_t* path, s64 size)
    {
        return nn::fs::CTR::MPCore::detail::UserFileSystem::TryCreateFile(path, size);
    }
    nn::Result TryCreateDirectory(const wchar_t* path)
    {
        return nn::fs::CTR::MPCore::detail::UserFileSystem::TryCreateDirectory(path);
    }
    nn::Result TryDeleteDirectory(const wchar_t* path)
    {
        return nn::fs::CTR::MPCore::detail::UserFileSystem::TryDeleteDirectory(path);
    }

private:
    // (inline) the path in UTF-16
    static void ToWidePath(wchar_t* out, const char* path)
    {
        mbstate_t state = {};
        mbsinit(&state);
        for (;;) {
            size_t length = mbrlen(path, 0x105, &state);
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

    nn::fs::CTR::MPCore::detail::UserFileSystem* mFileSystem;
};

// the FS:USER session of nn::fs::Initialize; a fatal error if there is none
nn::fs::ipc::FileSystem GetIpcFileSystem(); // 0x0013AF50 | nintendogs:bytes [tier A]
// the registered FileSystemBase; stops the program if there is none (name confirmed by the code)
FileSystemBase* GetGlobalFileSystemBase(); // 0x0013057C | tier X
void RegisterGlobalFileSystemBase(FileSystemBase& fileSystem); // 0x00136244 | tier C
} // namespace detail
} // namespace fs
} // namespace nn
