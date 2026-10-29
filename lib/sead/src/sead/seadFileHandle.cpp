#include "sead/seadFileHandle.h"

namespace sead {
// 0x0053F1B4 slot 0x00
// 0x0053F184 (deleting dtor)
sead::FileHandle::~FileHandle()
{
    if (mOriginalDevice != nullptr) {
        mOriginalDevice->tryClose(this);
    }
}

// 0x0053F054 (name is ours)
bool sead::FileHandle::close()
{
    if (mOriginalDevice == nullptr) {
        return false;
    }
    return mOriginalDevice->tryClose(this);
}

// 0x0053F074 (name is ours)
bool sead::FileHandle::read(u32* readSize, u8* buffer, u32 size)
{
    if (mDevice == nullptr) {
        return false;
    }
    return mDevice->tryRead(readSize, this, buffer, size);
}

// 0x0053F0A0 (name is ours)
bool sead::FileHandle::seek(s32 offset, FileDevice::SeekOrigin origin)
{
    if (mDevice == nullptr) {
        return false;
    }
    return mDevice->trySeek(this, offset, origin);
}

// 0x0053F10C (name is ours)
bool sead::FileHandle::write(u32* writtenSize, const u8* buffer, u32 size)
{
    if (mDevice == nullptr) {
        return false;
    }
    return mDevice->tryWrite(writtenSize, this, buffer, size);
}

} // namespace sead
