#include "sead/seadMainFileDevice.h"
#include "sead/seadCtrFileDevice.h"
#include "sead/seadFileHandle.h"
#include "sead/seadNew.h"

namespace sead {
// 0x00132F94 (name is ours)
sead::MainFileDevice::MainFileDevice(Heap* heap) : FileDevice(SafeStringBase<char>("main")), mFileDevice(nullptr)
{
    mFileDevice = new (heap, 4) CtrFileDevice(SafeStringBase<char>("ctr"));
}

// 0x005450CC slot 0x00 | nintendogs:bytes
// 0x00545080 (deleting dtor)
sead::MainFileDevice::~MainFileDevice()
{
    if (mFileDevice != nullptr) {
        delete mFileDevice;
        mFileDevice = nullptr;
    }
}

// 0x0074C188 slot 0x10
void sead::MainFileDevice::resolvePathOfFile(const SafeStringBase<char>& path) const
{
    mFileDevice->resolvePathOfFile(path);
}

// 0x0074C204 slot 0x14
void sead::MainFileDevice::resolvePathOfDirectory(const SafeStringBase<char>& path) const
{
    mFileDevice->resolvePathOfDirectory(path);
}

// 0x0074C1A8 slot 0x18
void sead::MainFileDevice::resolvePathOfFile(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const
{
    mFileDevice->resolvePathOfFile(out, path);
}

// 0x0074C214 slot 0x1C
void sead::MainFileDevice::resolvePathOfDirectory(BufferedSafeStringBase<char>* out,
                                                  const SafeStringBase<char>& path) const
{
    mFileDevice->resolvePathOfDirectory(out, path);
}

// 0x0074C198 slot 0x20
bool sead::MainFileDevice::isMatchDevice_(const HandleBase* handle) const
{
    return mFileDevice->isMatchDevice_(handle);
}

// 0x00749FD0 slot 0x24
bool sead::MainFileDevice::doIsAvailable_() const
{
    return mFileDevice->isAvailable();
}

// 0x00545028 slot 0x30
sead::FileDevice* sead::MainFileDevice::doOpen_(FileHandle* handle, const SafeStringBase<char>& path, FileOpenFlag flag)
{
    return mFileDevice->tryOpen(handle, path, flag, handle->mDivSize);
}

// 0x00545060 slot 0x34
bool sead::MainFileDevice::doClose_(FileHandle* handle)
{
    return mFileDevice->tryClose(handle);
}

// 0x00545040 slot 0x38
bool sead::MainFileDevice::doRead_(u32* readSize, FileHandle* handle, u8* buffer, u32 size)
{
    return mFileDevice->tryRead(readSize, handle, buffer, size);
}

// 0x00545068 slot 0x3C
bool sead::MainFileDevice::doWrite_(u32* writtenSize, FileHandle* handle, const u8* buffer, u32 size)
{
    return mFileDevice->tryWrite(writtenSize, handle, buffer, size);
}

// 0x00545058 slot 0x40
bool sead::MainFileDevice::doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin)
{
    return mFileDevice->trySeek(handle, offset, origin);
}

// 0x00545020 slot 0x44
bool sead::MainFileDevice::doGetCurrentSeekPos_(u32* pos, FileHandle* handle)
{
    return mFileDevice->tryGetCurrentSeekPos(pos, handle);
}

// 0x0053E67C slot 0x48 | nintendogs:bytes
bool sead::MainFileDevice::doGetFileSize_(u32* size, const SafeStringBase<char>& path)
{
    return mFileDevice->tryGetFileSize(size, path);
}

// 0x00545000 slot 0x4C
bool sead::MainFileDevice::doGetFileSize_(u32* size, FileHandle* handle)
{
    return mFileDevice->tryGetFileSize(size, handle);
}

// 0x0053E6A8 slot 0x50 | nintendogs:bytes
bool sead::MainFileDevice::doIsExistFile_(bool* isExist, const SafeStringBase<char>& path)
{
    return mFileDevice->tryIsExistFile(isExist, path);
}

// 0x0053E850 slot 0x54
bool sead::MainFileDevice::doIsExistDirectory_(bool* isExist, const SafeStringBase<char>& path)
{
    return mFileDevice->tryIsExistDirectory(isExist, path);
}

// 0x0053E6FC slot 0x58
sead::FileDevice* sead::MainFileDevice::doOpenDirectory_(HandleBase* handle, const SafeStringBase<char>& path)
{
    return mFileDevice->tryOpenDirectory(handle, path);
}

// 0x0053E7E0 slot 0x5C
bool sead::MainFileDevice::doCloseDirectory_(HandleBase* handle)
{
    return mFileDevice->tryCloseDirectory(handle);
}

// 0x00545008 slot 0x60
bool sead::MainFileDevice::doReadDirectory_(u32* readNum, HandleBase* handle, DirectoryEntry* entries, u32 num)
{
    return mFileDevice->tryReadDirectory(readNum, handle, entries, num);
}

// 0x0053E6D4 slot 0x64
bool sead::MainFileDevice::doMakeDirectory_(const SafeStringBase<char>& path, u32 permission)
{
    return mFileDevice->tryMakeDirectory(path, permission);
}

// 0x0074A118 slot 0x68
s32 sead::MainFileDevice::doGetLastRawError_() const
{
    return mFileDevice->getLastRawError();
}

} // namespace sead
