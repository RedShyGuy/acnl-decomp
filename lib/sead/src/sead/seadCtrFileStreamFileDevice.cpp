#include "sead/seadCtrFileStreamFileDevice.h"
#include "nn/fs/detail/detail_Api.h"
#include "nn/fs/fs_Types.h"
#include "sead/seadFileHandle.h"
#include "sead/seadWFixedSafeString.h"

#include <new>

namespace sead {
namespace {
inline CtrFileStreamFileDevice::FileHandleInner* getInner(HandleBase* handle)
{
    return reinterpret_cast<CtrFileStreamFileDevice::FileHandleInner*>(handle->mHandleBuffer);
}

inline nn::fs::Directory* getDirectory(HandleBase* handle)
{
    return reinterpret_cast<nn::fs::Directory*>(handle->mHandleBuffer);
}

// a result of the fs module that tells that the path does not exist
inline bool isNotFound(nn::Result result)
{
    if (result.GetModule() != 17) {
        return false;
    }
    s32 description = result.GetDescription();
    return description >= 100 && description <= 179;
}
} // namespace

// 0x00125C5C (name is ours)
sead::CtrFileStreamFileDevice::CtrFileStreamFileDevice(const SafeStringBase<char>& driveName)
    : FileDevice(driveName), mLastRawError(), mIsFlushOnWrite(false)
{
}

// 0x0054CB40 slot 0x00
// 0x0054CB30 (deleting dtor)
sead::CtrFileStreamFileDevice::~CtrFileStreamFileDevice()
{
    // (only the bases are destroyed)
}

// 0x0074DF48 slot 0x24 | nintendogs:bytes-fuzzy
bool sead::CtrFileStreamFileDevice::doIsAvailable_() const
{
    nn::fs::Directory directory;
    nn::Result result = tryOpenDirectory_(&directory, SafeStringBase<char>(getMountName()), SafeStringBase<char>(""));
    const_cast<CtrFileStreamFileDevice*>(this)->mLastRawError = result;
    return result.IsSuccess();
}

// 0x0054C83C slot 0x30 | nintendogs:bytes-fuzzy
sead::FileDevice* sead::CtrFileStreamFileDevice::doOpen_(FileHandle* handle, const SafeStringBase<char>& path,
                                                         FileOpenFlag flag)
{
    u32 mode = nn::fs::OPEN_MODE_READ;
    switch (flag) {
    case FILE_OPEN_FLAG_READ_ONLY:
        break;
    case FILE_OPEN_FLAG_WRITE_ONLY:
        mode = nn::fs::OPEN_MODE_WRITE | nn::fs::OPEN_MODE_CREATE;
        break;
    case FILE_OPEN_FLAG_READ_WRITE:
        mode = nn::fs::OPEN_MODE_READ | nn::fs::OPEN_MODE_WRITE;
        break;
    case FILE_OPEN_FLAG_CREATE:
        mode = nn::fs::OPEN_MODE_WRITE | nn::fs::OPEN_MODE_CREATE;
        break;
    }
    bool isExist = false;
    if (!doIsExistFile_(&isExist, path)) {
        return nullptr;
    }
    // flag 3 creates a new file
    if (flag == 3 && isExist) {
        return nullptr;
    }
    FileHandleInner* inner = getInner(handle);
    new (&inner->stream) nn::fs::FileStream();
    inner->openFlag = 0;
    mLastRawError = tryOpenFile_(&inner->stream, SafeStringBase<char>(getMountName()), path, mode);
    if (mLastRawError.IsFailure()) {
        return nullptr;
    }
    inner->openFlag = flag;
    return this;
}

// 0x0054CA34 slot 0x34
bool sead::CtrFileStreamFileDevice::doClose_(FileHandle* handle)
{
    FileHandleInner* inner = getInner(handle);
    bool result = true;
    if (inner->openFlag == 1 || inner->openFlag == 2) {
        nn::Result flushResult = inner->stream.TryFlush();
        result = flushResult.IsSuccess();
        mLastRawError = flushResult;
    }
    inner->stream.Finalize();
    inner->stream.~FileStream();
    return result;
}

// 0x0054C95C slot 0x38
bool sead::CtrFileStreamFileDevice::doRead_(u32* readSize, FileHandle* handle, u8* buffer, u32 size)
{
    FileHandleInner* inner = getInner(handle);
    if (inner->openFlag == 1 || inner->openFlag == 3) {
        return false;
    }
    s32 read = 0;
    mLastRawError = inner->stream.FileBase::TryRead(&read, buffer, size);
    if (read < 0) {
        return false;
    }
    if (readSize != nullptr) {
        *readSize = read;
    }
    return mLastRawError.IsSuccess();
}

// 0x0054CAA8 slot 0x3C
bool sead::CtrFileStreamFileDevice::doWrite_(u32* writtenSize, FileHandle* handle, const u8* buffer, u32 size)
{
    FileHandleInner* inner = getInner(handle);
    if (inner->openFlag == 0) {
        return false;
    }
    s32 written = 0;
    mLastRawError = inner->stream.FileBase::TryWrite(&written, buffer, size, mIsFlushOnWrite);
    if (written < 0) {
        return false;
    }
    if (writtenSize != nullptr) {
        *writtenSize = written;
    }
    return mLastRawError.IsSuccess();
}

// 0x0054C9D4 slot 0x40 | nintendogs:bytes-fuzzy
bool sead::CtrFileStreamFileDevice::doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin)
{
    nn::fs::PositionBase base;
    switch (origin) {
    case SEEK_ORIGIN_BEGIN:
        base = nn::fs::POSITION_BASE_BEGIN;
        break;
    case SEEK_ORIGIN_CURRENT:
        base = nn::fs::POSITION_BASE_CURRENT;
        break;
    case SEEK_ORIGIN_END:
        base = nn::fs::POSITION_BASE_END;
        break;
    default:
        return false;
    }
    mLastRawError = getInner(handle)->stream.FileBase::TrySeek(offset, base);
    return mLastRawError.IsSuccess();
}

// 0x0054C6D8 slot 0x44
bool sead::CtrFileStreamFileDevice::doGetCurrentSeekPos_(u32* pos, FileHandle* handle)
{
    s64 position;
    mLastRawError = getInner(handle)->stream.FileBase::TryGetPosition(&position);
    if (position < 0) {
        return false;
    }
    *pos = static_cast<u32>(position);
    return mLastRawError.IsSuccess();
}

// 0x0054BE40 slot 0x48 | mk7dlp:bytes-fuzzy
bool sead::CtrFileStreamFileDevice::doGetFileSize_(u32* size, const SafeStringBase<char>& path)
{
    nn::fs::FileStream stream;
    mLastRawError = tryOpenFile_(&stream, SafeStringBase<char>(getMountName()), path, nn::fs::OPEN_MODE_READ);
    if (mLastRawError.IsFailure()) {
        return false;
    }
    s64 fileSize = 0;
    mLastRawError = stream.FileBase::TryGetSize(&fileSize);
    if (fileSize < 0) {
        return false;
    }
    *size = static_cast<u32>(fileSize);
    return mLastRawError.IsSuccess();
}

// 0x0054BDE8 slot 0x4C
bool sead::CtrFileStreamFileDevice::doGetFileSize_(u32* size, FileHandle* handle)
{
    s64 fileSize = 0;
    mLastRawError = getInner(handle)->stream.FileBase::TryGetSize(&fileSize);
    if (fileSize < 0) {
        return false;
    }
    *size = static_cast<u32>(fileSize);
    return mLastRawError.IsSuccess();
}

// 0x0054BF40 slot 0x50 | nintendogs:bytes-fuzzy
bool sead::CtrFileStreamFileDevice::doIsExistFile_(bool* isExist, const SafeStringBase<char>& path)
{
    // a directory of the name is no file
    nn::fs::Directory directory;
    mLastRawError = tryOpenDirectory_(&directory, SafeStringBase<char>(getMountName()), path);
    if (isNotFound(mLastRawError) || mLastRawError.IsSuccess()) {
        *isExist = false;
        return true;
    }
    nn::fs::FileStream stream;
    mLastRawError = tryOpenFile_(&stream, SafeStringBase<char>(getMountName()), path, nn::fs::OPEN_MODE_READ);
    if (mLastRawError.IsFailure()) {
        *isExist = false;
        return false;
    }
    *isExist = true;
    return true;
}

// 0x0054C48C slot 0x54
bool sead::CtrFileStreamFileDevice::doIsExistDirectory_(bool* isExist, const SafeStringBase<char>& path)
{
    // a file of the name is no directory
    nn::fs::FileStream stream;
    mLastRawError =
        tryOpenFileWithoutLastSlash_(&stream, SafeStringBase<char>(getMountName()), path, nn::fs::OPEN_MODE_READ);
    if (isNotFound(mLastRawError) || mLastRawError.IsSuccess()) {
        *isExist = false;
        return true;
    }
    nn::fs::Directory directory;
    mLastRawError = tryOpenDirectory_(&directory, SafeStringBase<char>(getMountName()), path);
    if (mLastRawError.IsFailure()) {
        *isExist = false;
        return false;
    }
    *isExist = true;
    return true;
}

// 0x0054C184 slot 0x58
sead::FileDevice* sead::CtrFileStreamFileDevice::doOpenDirectory_(HandleBase* handle, const SafeStringBase<char>& path)
{
    nn::fs::Directory* directory = new (getDirectory(handle)) nn::fs::Directory();
    mLastRawError = tryOpenDirectory_(directory, SafeStringBase<char>(getMountName()), path);
    if (mLastRawError.IsFailure()) {
        return nullptr;
    }
    return this;
}

// 0x0054C398 slot 0x5C
bool sead::CtrFileStreamFileDevice::doCloseDirectory_(HandleBase* handle)
{
    nn::fs::Directory* directory = getDirectory(handle);
    directory->Finalize();
    directory->~Directory();
    mLastRawError = nn::Result();
    return true;
}

// 0x0054C1FC slot 0x60
bool sead::CtrFileStreamFileDevice::doReadDirectory_(u32* readNum, HandleBase* handle, DirectoryEntry* entries, u32 num)
{
    nn::fs::Directory* directory = getDirectory(handle);
    for (u32 i = 0; i < num; i++) {
        nn::fs::DirectoryEntry entry;
        s32 read = 0;
        mLastRawError = directory->TryRead(&read, &entry, 1);
        if (mLastRawError.IsFailure()) {
            if (readNum != nullptr) {
                *readNum = read;
            }
            return false;
        }
        if (read == 0) {
            if (readNum != nullptr) {
                *readNum = i;
            }
            return true;
        }
        // the name in the characters of the device (the low byte of each UTF-16 character)
        DirectoryEntry& out = entries[i];
        SafeStringBase<wchar_t> name(entry.name);
        s32 length = name.getLength();
        if (length >= out.name.mBufferSize) {
            length = out.name.mBufferSize - 1;
        }
        const wchar_t* src = name.getCString();
        char* dst = out.name.getMutableStringTop();
        for (s32 j = 0; j < length; j++) {
            dst[j] = static_cast<char>(src[j]);
        }
        dst[length] = SafeStringBase<char>::s_NullChar;
        out.isDirectory = entry.attributes.isDirectory;
    }
    if (readNum != nullptr) {
        *readNum = num;
    }
    return true;
}

// 0x0054C0BC slot 0x64
bool sead::CtrFileStreamFileDevice::doMakeDirectory_(const SafeStringBase<char>& path, u32)
{
    WFixedSafeString<256> fullPath;
    fullPath.format(L"%s:/%s", getMountName(), path.getCString());
    mLastRawError = nn::fs::detail::GetGlobalFileSystemBase()->TryCreateDirectory(fullPath.getCString());
    return mLastRawError.IsSuccess();
}

// 0x0074E024 slot 0x68
s32 sead::CtrFileStreamFileDevice::doGetLastRawError_() const
{
    return mLastRawError.GetPrintableBits();
}

// 0x0074DFD0 slot 0x70
void sead::CtrFileStreamFileDevice::doResolvePath_(BufferedSafeStringBase<char>* out,
                                                   const SafeStringBase<char>& path) const
{
    out->format("%s:/%s", getMountName(), path.getCString());
}

// 0x0054C604 (name is ours)
nn::Result sead::CtrFileStreamFileDevice::tryOpenFile_(nn::fs::FileStream* stream, const SafeStringBase<char>& mount,
                                                      const SafeStringBase<char>& path, u32 mode)
{
    WFixedSafeString<256> fullPath;
    fullPath.format(L"%s:/%s", mount.getCString(), path.getCString());
    return stream->TryInitialize(fullPath.getCString(), mode);
}

// 0x0054C714 (name is ours)
nn::Result sead::CtrFileStreamFileDevice::tryOpenFileWithoutLastSlash_(nn::fs::FileStream* stream,
                                                                      const SafeStringBase<char>& mount,
                                                                      const SafeStringBase<char>& path, u32 mode)
{
    WFixedSafeString<256> fullPath;
    fullPath.format(L"%s:/%s", mount.getCString(), path.getCString());
    s32 length = fullPath.getLength();
    if (length != 0) {
        wchar_t* last = fullPath.getMutableStringTop() + length - 1;
        if (*last == L'/') {
            *last = SafeStringBase<wchar_t>::s_NullChar;
        }
    }
    return stream->TryInitialize(fullPath.getCString(), mode);
}

// 0x0054C3D4 (name is ours)
nn::Result sead::CtrFileStreamFileDevice::tryOpenDirectory_(nn::fs::Directory* directory,
                                                           const SafeStringBase<char>& mount,
                                                           const SafeStringBase<char>& path)
{
    WFixedSafeString<256> fullPath;
    fullPath.format(L"%s:/%s", mount.getCString(), path.getCString());
    return directory->TryInitialize(fullPath.getCString());
}

// 0x00829380 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::CtrFileStreamFileDevice>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;

} // namespace sead
