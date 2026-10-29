#include "sead/seadAnyFileDevice.h"
#include "sead/seadFileHandle.h"

namespace sead {
namespace {
// the path of the open directory (empty if none is open; name is ours)
// 0x00AE62D4
FixedSafeString<512> s_AnyFileDeviceDirectoryPath;
} // namespace

// constructs s_AnyFileDeviceDirectoryPath
// 0x0079C2BC (static initializer)

// 0x00541754 slot 0x00 | nintendogs:bytes
// 0x0054172C (deleting dtor)
sead::AnyFileDevice::~AnyFileDevice()
{
    mDeviceList.freeBuffer();
}

// 0x0074AD94 slot 0x10
void sead::AnyFileDevice::resolvePathOfFile(const SafeStringBase<char>& path) const
{
    FileDevice* device = findFileDeviceByFile_(path);
    if (device != nullptr) {
        device->resolvePathOfFile(path);
    }
}

// 0x0074AEF0 slot 0x14
void sead::AnyFileDevice::resolvePathOfDirectory(const SafeStringBase<char>& path) const
{
    FileDevice* device = findFileDeviceByDirectory_(path);
    if (device != nullptr) {
        device->resolvePathOfDirectory(path);
    }
}

// 0x0074AE58 slot 0x18
void sead::AnyFileDevice::resolvePathOfFile(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const
{
    FileDevice* device = findFileDeviceByFile_(path);
    if (device != nullptr) {
        device->resolvePathOfFile(out, path);
    }
}

// 0x0074AF1C slot 0x1C
void sead::AnyFileDevice::resolvePathOfDirectory(BufferedSafeStringBase<char>* out,
                                                 const SafeStringBase<char>& path) const
{
    FileDevice* device = findFileDeviceByDirectory_(path);
    if (device != nullptr) {
        device->resolvePathOfDirectory(out, path);
    }
}

// 0x0074AE04 slot 0x20 | nintendogs:bytes-fuzzy
bool sead::AnyFileDevice::isMatchDevice_(const HandleBase* handle) const
{
    for (FileDevice** it = mDeviceList.begin(); it != mDeviceList.end(); it++) {
        if ((*it)->isMatchDevice_(handle)) {
            return true;
        }
    }
    return false;
}

// 0x0074ADC0 slot 0x24 | nintendogs:bytes
bool sead::AnyFileDevice::doIsAvailable_() const
{
    for (FileDevice** it = mDeviceList.begin(); it != mDeviceList.end(); it++) {
        if ((*it)->isAvailable()) {
            return true;
        }
    }
    return false;
}

// 0x0054167C slot 0x28 | nintendogs:bytes
u8* sead::AnyFileDevice::doLoad_(LoadArg& arg)
{
    FileDevice* device = findFileDeviceByFile_(arg.path);
    if (device == nullptr) {
        return nullptr;
    }
    mLastDevice = device;
    return device->tryLoad(arg);
}

// 0x005416A8 slot 0x30 | nintendogs:bytes
sead::FileDevice* sead::AnyFileDevice::doOpen_(FileHandle* handle, const SafeStringBase<char>& path, FileOpenFlag flag)
{
    FileDevice* device = findFileDeviceByFile_(path);
    if (device == nullptr) {
        return nullptr;
    }
    mLastDevice = device;
    return device->tryOpen(handle, path, flag, handle->mDivSize);
}

// 0x0013F654 slot 0x34
bool sead::AnyFileDevice::doClose_(FileHandle* handle)
{
    mLastDevice = handle->mDevice;
    return mLastDevice->tryClose(handle);
}

// 0x005416EC slot 0x38
bool sead::AnyFileDevice::doRead_(u32* readSize, FileHandle* handle, u8* buffer, u32 size)
{
    mLastDevice = handle->mDevice;
    return mLastDevice->tryRead(readSize, handle, buffer, size);
}

// 0x0054170C slot 0x3C
bool sead::AnyFileDevice::doWrite_(u32* writtenSize, FileHandle* handle, const u8* buffer, u32 size)
{
    mLastDevice = handle->mDevice;
    return mLastDevice->tryWrite(writtenSize, handle, buffer, size);
}

// 0x0053EEE0 slot 0x40
bool sead::AnyFileDevice::doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin)
{
    mLastDevice = handle->mDevice;
    return mLastDevice->trySeek(handle, offset, origin);
}

// 0x0053E87C slot 0x44
bool sead::AnyFileDevice::doGetCurrentSeekPos_(u32* pos, FileHandle* handle)
{
    mLastDevice = handle->mDevice;
    return mLastDevice->tryGetCurrentSeekPos(pos, handle);
}

// 0x0054133C slot 0x48 | nintendogs:bytes
bool sead::AnyFileDevice::doGetFileSize_(u32* size, const SafeStringBase<char>& path)
{
    FileDevice* device = findFileDeviceByFile_(path);
    if (device == nullptr) {
        return false;
    }
    mLastDevice = device;
    return device->tryGetFileSize(size, path);
}

// 0x0053E644 slot 0x4C
bool sead::AnyFileDevice::doGetFileSize_(u32* size, FileHandle* handle)
{
    mLastDevice = handle->mDevice;
    return mLastDevice->tryGetFileSize(size, handle);
}

// 0x00541374 slot 0x50
bool sead::AnyFileDevice::doIsExistFile_(bool* isExist, const SafeStringBase<char>& path)
{
    FileDevice* device = findFileDeviceByFile_(path);
    mLastDevice = device;
    *isExist = device != nullptr;
    return true;
}

// 0x0054164C slot 0x54
bool sead::AnyFileDevice::doIsExistDirectory_(bool* isExist, const SafeStringBase<char>& path)
{
    FileDevice* device = findFileDeviceByDirectory_(path);
    mLastDevice = device;
    *isExist = device != nullptr;
    return true;
}

// 0x005413AC slot 0x58
sead::FileDevice* sead::AnyFileDevice::doOpenDirectory_(HandleBase* handle, const SafeStringBase<char>& path)
{
    if (s_AnyFileDeviceDirectoryPath.getMutableStringTop()[0] != SafeStringBase<char>::s_NullChar) {
        return nullptr;
    }
    FileDevice* opened = nullptr;
    for (FileDevice** it = mDeviceList.begin(); it != mDeviceList.end(); it++) {
        opened = (*it)->tryOpenDirectory(handle, path);
        if (opened != nullptr) {
            mLastDevice = opened;
            s_AnyFileDeviceDirectoryPath.copy(path);
            break;
        }
    }
    return opened;
}

// 0x00541620 slot 0x5C
bool sead::AnyFileDevice::doCloseDirectory_(HandleBase* handle)
{
    mLastDevice = handle->mDevice;
    s_AnyFileDeviceDirectoryPath.clear();
    return mLastDevice->tryCloseDirectory(handle);
}

// 0x005414BC slot 0x60
bool sead::AnyFileDevice::doReadDirectory_(u32* readNum, HandleBase* handle, DirectoryEntry* entries, u32 num)
{
    u32 read = 0;
    if (!handle->mDevice->tryReadDirectory(&read, handle, entries, num)) {
        if (readNum != nullptr) {
            *readNum = read;
        }
        return false;
    }
    u32 total = read;
    if (num <= total) {
        if (readNum != nullptr) {
            *readNum = total;
        }
        return true;
    }
    u32 rest = num - total;
    DirectoryEntry* next = entries + total;
    // the device after the one of the handle
    FileDevice** it = mDeviceList.begin();
    while (it != mDeviceList.end()) {
        if (handle->mDevice == *it++) {
            break;
        }
    }
    while (it != mDeviceList.end()) {
        FileDevice* opened = (*it)->tryOpenDirectory(handle, s_AnyFileDeviceDirectoryPath);
        if (opened == nullptr) {
            if (readNum != nullptr) {
                *readNum = total;
            }
            return false;
        }
        mLastDevice = opened;
        if (!handle->mDevice->tryReadDirectory(&read, handle, next, rest)) {
            if (readNum != nullptr) {
                *readNum = total;
            }
            return false;
        }
        if (rest <= read) {
            if (readNum != nullptr) {
                *readNum = read + total;
            }
            return true;
        }
        next += read;
        it++;
        rest -= read;
        total += read;
    }
    if (readNum != nullptr) {
        *readNum = total;
    }
    return true;
}

// 0x005413A4 slot 0x64
bool sead::AnyFileDevice::doMakeDirectory_(const SafeStringBase<char>&, u32)
{
    return false;
}

// 0x0074AE90 slot 0x68
s32 sead::AnyFileDevice::doGetLastRawError_() const
{
    if (mLastDevice == nullptr) {
        return 0;
    }
    return mLastDevice->getLastRawError();
}

// 0x0011D6F0 | nintendogs:bytes [tier A]
bool sead::AnyFileDevice::pushBack(FileDevice* device)
{
    return mDeviceList.pushBack(device);
}

// 0x0074AF54 | nintendogs:callgraph [tier A]
sead::FileDevice* sead::AnyFileDevice::findFileDeviceByFile_(const SafeStringBase<char>& path) const
{
    for (FileDevice** it = mDeviceList.begin(); it != mDeviceList.end(); it++) {
        FileDevice* device = *it;
        if (device->isAvailable()) {
            bool isExist = false;
            if (device->tryIsExistFile(&isExist, path) && isExist) {
                return device;
            }
        }
    }
    return nullptr;
}

// 0x0074AFCC (name is ours)
sead::FileDevice* sead::AnyFileDevice::findFileDeviceByDirectory_(const SafeStringBase<char>& path) const
{
    for (FileDevice** it = mDeviceList.begin(); it != mDeviceList.end(); it++) {
        FileDevice* device = *it;
        if (device->isAvailable()) {
            bool isExist = false;
            if (device->tryIsExistDirectory(&isExist, path) && isExist) {
                return device;
            }
        }
    }
    return nullptr;
}

} // namespace sead
