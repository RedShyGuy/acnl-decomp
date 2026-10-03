#include "nn/fs/detail/fs_FileBase.h"

#include <wchar.h>

namespace nn {
namespace fs {
namespace detail {
namespace {

// results (module fs); the names are ours
const bit32 RESULT_INVALID_POSITION_BASE = 0xE0E046BD;  // usage, invalid argument, 701
const bit32 RESULT_INVALID_POSITION = 0xE0E046C1;       // usage, invalid argument, 705: negative or past the end

// the most UTF-16 characters TryInitialize converts a path to
const size_t MAX_PATH_LENGTH = 0x105;

} // namespace

using nn::fs::CTR::MPCore::detail::UserFileSystem;

// 0x0013E638 (name is ours)
nn::Result nn::fs::detail::FileBase::TryInitialize(const char* path, u32 mode)
{
    wchar_t widePath[MAX_PATH_LENGTH + 3];
    wchar_t* out = widePath;
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
    mSize = 0;
    mPosition = 0;
    return UserFileSystem::TryOpenFile(reinterpret_cast<void**>(&mHandle), widePath, mode);
}

// 0x0013E6E0 | nintendogs:bytes [tier A]
nn::Result nn::fs::detail::FileBase::TryRead(s32* readSize, void* buffer, size_t size)
{
    s32 total = 0;
    while (size != 0) {
        s32 read;
        nn::Result result = UserFileSystem::TryReadFile(&read, GetHandle(), mPosition, buffer, size);
        if (result.IsFailure()) {
            return result;
        }
        mPosition += read;
        total += read;
        if (static_cast<size_t>(read) == size || read == 0) {
            break;
        }
        buffer = static_cast<u8*>(buffer) + read;
        size -= read;
    }
    *readSize = total;
    return nn::Result();
}

// 0x0013E768 | fefates:bytes [tier B]
nn::Result nn::fs::detail::FileBase::TryWrite(s32* writtenSize, const void* buffer, size_t size, bool flush)
{
    s32 total = 0;
    while (size != 0) {
        if (flush) {
            mHandle &= ~HANDLE_NOT_FLUSHED;
        } else {
            mHandle |= HANDLE_NOT_FLUSHED;
        }
        s32 written;
        nn::Result result = UserFileSystem::TryWriteFile(&written, GetHandle(), mPosition, buffer, size, flush);
        if (result.IsFailure()) {
            return result;
        }
        mPosition += written;
        total += written;
        if (static_cast<size_t>(written) == size || written == 0) {
            break;
        }
        buffer = static_cast<const u8*>(buffer) + written;
        size -= written;
    }
    *writtenSize = total;
    return nn::Result();
}

// 0x0013F9B8 | nintendogs:bytes [tier A]
nn::Result nn::fs::detail::FileBase::TryGetSize(s64* size) const
{
    s64 fileSize;
    nn::Result result = UserFileSystem::TryGetFileSize(&fileSize, GetHandle());
    if (result.IsSuccess()) {
        mSize = fileSize;
        *size = fileSize;
    } else {
        mSize = 0;
    }
    return result;
}

// 0x0034971C | fefates:bytes [tier B]
nn::Result nn::fs::detail::FileBase::TrySetSize(s64 size)
{
    nn::Result result = UserFileSystem::TrySetFileSize(GetHandle(), size);
    if (result.IsFailure()) {
        return result;
    }
    mSize = size;
    if (size < mPosition) {
        mPosition = size;
    }
    return result;
}

// 0x0034976C | nintendogs:bytes [tier A]
nn::Result nn::fs::detail::FileBase::TrySetPosition(s64 position)
{
    if (position < 0) {
        return nn::Result(RESULT_INVALID_POSITION);
    }
    if (position >= mSize) {
        // the file may have grown
        s64 size;
        nn::Result result = TryGetSize(&size);
        if (result.IsFailure()) {
            return result;
        }
        if (size < position) {
            return nn::Result(RESULT_INVALID_POSITION);
        }
    }
    mPosition = position;
    return nn::Result();
}

// 0x0034980C | nintendogs:bytes [tier A]
nn::Result nn::fs::detail::FileBase::TrySeek(s64 offset, nn::fs::PositionBase base)
{
    switch (base) {
    case POSITION_BASE_BEGIN:
        break;
    case POSITION_BASE_CURRENT:
        offset += mPosition;
        break;
    case POSITION_BASE_END: {
        s64 size;
        nn::Result result = TryGetSize(&size);
        if (result.IsFailure()) {
            return result;
        }
        offset += size;
        break;
    }
    default:
        return nn::Result(RESULT_INVALID_POSITION_BASE);
    }
    return TrySetPosition(offset);
}

// 0x003498C4 | tier C (confirmed by the code)
nn::Result nn::fs::detail::FileBase::TryFlush()
{
    mHandle &= ~HANDLE_NOT_FLUSHED;
    return UserFileSystem::TryFlush(GetHandle());
}

} // namespace detail
} // namespace fs
} // namespace nn
