#include "sead/seadFileDevice.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadFileDeviceMgr.h"
#include "sead/seadFileHandle.h"
#include "sead/seadHeap.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadNew.h"

namespace sead {
// 0x0053F01C slot 0x00 | nintendogs:bytes
// 0x0053EFDC (deleting dtor)
sead::FileDevice::~FileDevice()
{
    if (FileDeviceMgr::s_pInstance != nullptr) {
        FileDeviceMgr::s_pInstance->unmount(this);
    }
}

// 0x0074A054 slot 0x10
void sead::FileDevice::resolvePathOfFile(const SafeStringBase<char>& path) const
{
    doResolvePath_(path);
}

// 0x0074A184 slot 0x14
void sead::FileDevice::resolvePathOfDirectory(const SafeStringBase<char>& path) const
{
    doResolvePath_(path);
}

// 0x0074A12C slot 0x18
void sead::FileDevice::resolvePathOfFile(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const
{
    doResolvePath_(out, path);
}

// 0x0074A190 slot 0x1C
void sead::FileDevice::resolvePathOfDirectory(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const
{
    doResolvePath_(out, path);
}

// 0x0074A104 slot 0x20
bool sead::FileDevice::isMatchDevice_(const HandleBase* handle) const
{
    return handle->mDevice == this;
}

// 0x0053E944 slot 0x28 | nintendogs:bytes
u8* sead::FileDevice::doLoad_(LoadArg& arg)
{
    if (arg.buffer != nullptr && arg.bufferSize == 0) {
        return nullptr;
    }
    FileHandle handle;
    if (tryOpen(&handle, arg.path, FILE_OPEN_FLAG_READ_ONLY, arg.divSize) == nullptr) {
        return nullptr;
    }
    u32 bufferSize = arg.bufferSize;
    if (arg.buffer == nullptr || arg.checkFileSize) {
        u32 fileSize = 0;
        if (!tryGetFileSize(&fileSize, &handle)) {
            return nullptr;
        }
        if (fileSize == 0) {
            return nullptr;
        }
        if (bufferSize == 0) {
            bufferSize = (fileSize + 31) & ~31u;
        } else if (bufferSize < fileSize) {
            return nullptr;
        }
    }
    u8* buffer = arg.buffer;
    bool isAllocated = false;
    if (buffer == nullptr) {
        s32 alignment = arg.bufferSizeAlignment;
        s32 sign = alignment >= 0 ? 1 : -1;
        s32 absAlignment = alignment < 0 ? -alignment : alignment;
        if (absAlignment <= 32) {
            absAlignment = 32;
        }
        Heap* heap = arg.heap;
        if (heap == nullptr) {
            heap = HeapMgr::s_pInstance->getCurrentHeap();
        }
        buffer = static_cast<u8*>(heap->tryAlloc(bufferSize, sign * absAlignment));
        if (buffer == nullptr) {
            return nullptr;
        }
        isAllocated = true;
    }
    u32 readSize = 0;
    if (!tryRead(&readSize, &handle, buffer, bufferSize)) {
        if (isAllocated) {
            delete[] buffer;
        }
        return nullptr;
    }
    if (!tryClose(&handle)) {
        if (isAllocated) {
            delete[] buffer;
        }
        return nullptr;
    }
    arg.readSize = readSize;
    arg.roundupBufferSize = bufferSize;
    arg.needUnload = isAllocated;
    return buffer;
}

// 0x0053EC10 slot 0x2C (name is ours)
bool sead::FileDevice::doSave_(SaveArg& arg)
{
    if (arg.buffer == nullptr) {
        return false;
    }
    FileHandle handle;
    if (tryOpen(&handle, arg.path, FILE_OPEN_FLAG_WRITE_ONLY, 0) == nullptr) {
        return false;
    }
    bool result = true;
    if (arg.dataSize != 0) {
        result = tryWrite(&arg.writeSize, &handle, arg.buffer, arg.dataSize);
    }
    if (!tryClose(&handle)) {
        return false;
    }
    return result;
}

// 0x00749FF8 slot 0x6C (name is ours)
void sead::FileDevice::doResolvePath_(const SafeStringBase<char>& path) const
{
    FixedSafeString<512> resolved;
    doResolvePath_(&resolved, path);
}

// 0x0074A060 slot 0x70 (name is ours)
void sead::FileDevice::doResolvePath_(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const
{
    out->copy(path);
}

// 0x00749FD8 | nintendogs:callgraph [tier A]
bool sead::FileDevice::isAvailable() const
{
    if (!mPermission) {
        return false;
    }
    return doIsAvailable_();
}

// 0x00132D10 | nintendogs:callgraph [tier A]
u8* sead::FileDevice::tryLoad(LoadArg& arg)
{
    if (!mPermission) {
        return nullptr;
    }
    return doLoad_(arg);
}

// 0x0053ED8C (name is ours)
sead::FileDevice* sead::FileDevice::tryOpen(FileHandle* handle, const SafeStringBase<char>& path, FileOpenFlag flag,
                                            u32 divSize)
{
    if (!mPermission || handle == nullptr) {
        return nullptr;
    }
    handle->mDivSize = divSize;
    FileDevice* device = doOpen_(handle, path, flag);
    handle->mDevice = device;
    if (device != nullptr) {
        handle->mOriginalDevice = this;
    }
    return device;
}

// 0x0013F664 (name is ours)
bool sead::FileDevice::tryClose(FileHandle* handle)
{
    if (!mPermission) {
        return false;
    }
    if (handle == nullptr) {
        return false;
    }
    if (!isMatchDevice_(handle)) {
        return false;
    }
    bool result = doClose_(handle);
    if (result) {
        handle->mDevice = nullptr;
        handle->mOriginalDevice = nullptr;
    }
    return result;
}

// 0x0053EDD8 | nintendogs:bytes-fuzzy [tier A]
bool sead::FileDevice::tryRead(u32* readSize, FileHandle* handle, u8* buffer, u32 size)
{
    if (!mPermission) {
        return false;
    }
    if (handle == nullptr) {
        return false;
    }
    if (!isMatchDevice_(handle)) {
        return false;
    }
    if (buffer == nullptr) {
        return false;
    }
    if (handle->mDivSize == 0) {
        return doRead_(readSize, handle, buffer, size);
    }
    // in pieces of mDivSize bytes
    u32 total = 0;
    do {
        u32 pieceSize = static_cast<s32>(handle->mDivSize) > static_cast<s32>(size) ? size : handle->mDivSize;
        u32 pieceRead = 0;
        if (!doRead_(&pieceRead, handle, buffer, pieceSize)) {
            if (readSize != nullptr) {
                *readSize = total;
            }
            return false;
        }
        total += pieceRead;
        if (pieceSize > pieceRead) {
            break;
        }
        size -= pieceSize;
        buffer += pieceRead;
    } while (size != 0);
    if (readSize != nullptr) {
        *readSize = total;
    }
    return true;
}

// 0x0053EF58 | nintendogs:bytes-fuzzy [tier A]
bool sead::FileDevice::tryWrite(u32* writtenSize, FileHandle* handle, const u8* buffer, u32 size)
{
    if (!mPermission) {
        return false;
    }
    if (handle == nullptr || buffer == nullptr) {
        return false;
    }
    if (!isMatchDevice_(handle)) {
        return false;
    }
    return doWrite_(writtenSize, handle, buffer, size);
}

// 0x0053EEF0 (name is ours)
bool sead::FileDevice::trySeek(FileHandle* handle, s32 offset, SeekOrigin origin)
{
    if (!mPermission) {
        return false;
    }
    if (handle == nullptr) {
        return false;
    }
    if (!isMatchDevice_(handle)) {
        return false;
    }
    return doSeek_(handle, offset, origin);
}

// 0x0053E88C (name is ours)
bool sead::FileDevice::tryGetCurrentSeekPos(u32* pos, FileHandle* handle)
{
    if (!mPermission) {
        return false;
    }
    if (handle == nullptr) {
        return false;
    }
    if (!isMatchDevice_(handle)) {
        return false;
    }
    if (pos == nullptr) {
        return false;
    }
    return doGetCurrentSeekPos_(pos, handle);
}

// 0x0053E684 | nintendogs:bytes
bool sead::FileDevice::tryGetFileSize(u32* size, const SafeStringBase<char>& path)
{
    if (!mPermission || size == nullptr) {
        return false;
    }
    return doGetFileSize_(size, path);
}

// 0x0053E654 (name is ours)
bool sead::FileDevice::tryGetFileSize(u32* size, FileHandle* handle)
{
    if (!mPermission || handle == nullptr || size == nullptr) {
        return false;
    }
    return doGetFileSize_(size, handle);
}

// 0x0053E6B0 (name is ours)
bool sead::FileDevice::tryIsExistFile(bool* isExist, const SafeStringBase<char>& path)
{
    if (!mPermission || isExist == nullptr) {
        return false;
    }
    return doIsExistFile_(isExist, path);
}

// 0x0053E858 (name is ours)
bool sead::FileDevice::tryIsExistDirectory(bool* isExist, const SafeStringBase<char>& path)
{
    if (!mPermission || isExist == nullptr) {
        return false;
    }
    return doIsExistDirectory_(isExist, path);
}

// 0x0053E704 (name is ours)
sead::FileDevice* sead::FileDevice::tryOpenDirectory(HandleBase* handle, const SafeStringBase<char>& path)
{
    if (!mPermission) {
        return nullptr;
    }
    if (handle == nullptr) {
        return nullptr;
    }
    FileDevice* device = doOpenDirectory_(handle, path);
    handle->mDevice = device;
    if (device != nullptr) {
        handle->mOriginalDevice = this;
    }
    return device;
}

// 0x0053E7E8 (name is ours)
bool sead::FileDevice::tryCloseDirectory(HandleBase* handle)
{
    if (!mPermission) {
        return false;
    }
    if (handle == nullptr) {
        return false;
    }
    if (!isMatchDevice_(handle)) {
        return false;
    }
    bool result = doCloseDirectory_(handle);
    if (result) {
        handle->mDevice = nullptr;
        handle->mOriginalDevice = nullptr;
    }
    return result;
}

// 0x0053E748 (name is ours)
bool sead::FileDevice::tryReadDirectory(u32* readNum, HandleBase* handle, DirectoryEntry* entries, u32 num)
{
    if (!mPermission) {
        return false;
    }
    if (handle == nullptr) {
        return false;
    }
    if (!isMatchDevice_(handle)) {
        return false;
    }
    u32 read = 0;
    bool result = doReadDirectory_(&read, handle, entries, num);
    if (readNum != nullptr) {
        *readNum = read;
    }
    if (read > num) {
        return false;
    }
    return result;
}

// 0x0053E6DC (name is ours)
bool sead::FileDevice::tryMakeDirectory(const SafeStringBase<char>& path, u32 permission)
{
    if (!mPermission) {
        return false;
    }
    return doMakeDirectory_(path, permission);
}

// 0x0074A120 | nintendogs:callgraph [tier A]
s32 sead::FileDevice::getLastRawError() const
{
    return doGetLastRawError_();
}

// 0x008288A4 (name is ours)
template bool sead::RuntimeTypeInfo::Derive<sead::FileDevice>::isDerived(const sead::RuntimeTypeInfo::Interface*) const;

} // namespace sead
