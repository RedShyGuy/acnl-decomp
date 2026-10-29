#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadFileDevice.h"

namespace sead {
class Heap;
}

namespace sead {
// RTTI N4sead14MainFileDeviceE @ 0x008D1710
// vtable 0x00905858 (vptr 0x00905860), offset_to_top 0, 29 entries
//
// The device "main": forwards everything to a CtrFileDevice. Slot names as in FileDevice.
class MainFileDevice : public ::sead::FileDevice
{
public:
    explicit MainFileDevice(Heap* heap); // 0x00132F94 (name is ours)
    virtual ~MainFileDevice(); // 0x005450CC slot 0x00 | nintendogs:bytes
    // 0x00545080 slot 0x04 | nintendogs:bytes (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074C224 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return FileDevice::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074C1B8 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098347C (guard 0x00983478)
        static const RuntimeTypeInfo::Derive<FileDevice> s_TypeInfo;
        return &s_TypeInfo;
    }
    // the slots forward to mFileDevice
    virtual void resolvePathOfFile(const SafeStringBase<char>& path) const; // 0x0074C188 slot 0x10
    virtual void resolvePathOfDirectory(const SafeStringBase<char>& path) const; // 0x0074C204 slot 0x14
    virtual void resolvePathOfFile(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const; // 0x0074C1A8 slot 0x18
    virtual void resolvePathOfDirectory(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const; // 0x0074C214 slot 0x1C
    virtual bool isMatchDevice_(const HandleBase* handle) const; // 0x0074C198 slot 0x20
    virtual bool doIsAvailable_() const; // 0x00749FD0 slot 0x24
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeStringBase<char>& path, FileOpenFlag flag); // 0x00545028 slot 0x30
    virtual bool doClose_(FileHandle* handle); // 0x00545060 slot 0x34
    virtual bool doRead_(u32* readSize, FileHandle* handle, u8* buffer, u32 size); // 0x00545040 slot 0x38
    virtual bool doWrite_(u32* writtenSize, FileHandle* handle, const u8* buffer, u32 size); // 0x00545068 slot 0x3C
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin); // 0x00545058 slot 0x40
    virtual bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle); // 0x00545020 slot 0x44
    virtual bool doGetFileSize_(u32* size, const SafeStringBase<char>& path); // 0x0053E67C slot 0x48
    virtual bool doGetFileSize_(u32* size, FileHandle* handle); // 0x00545000 slot 0x4C
    virtual bool doIsExistFile_(bool* isExist, const SafeStringBase<char>& path); // 0x0053E6A8 slot 0x50
    virtual bool doIsExistDirectory_(bool* isExist, const SafeStringBase<char>& path); // 0x0053E850 slot 0x54
    virtual FileDevice* doOpenDirectory_(HandleBase* handle, const SafeStringBase<char>& path); // 0x0053E6FC slot 0x58
    virtual bool doCloseDirectory_(HandleBase* handle); // 0x0053E7E0 slot 0x5C
    virtual bool doReadDirectory_(u32* readNum, HandleBase* handle, DirectoryEntry* entries, u32 num); // 0x00545008 slot 0x60
    virtual bool doMakeDirectory_(const SafeStringBase<char>& path, u32 permission); // 0x0053E6D4 slot 0x64
    virtual s32 doGetLastRawError_() const; // 0x0074A118 slot 0x68

    // the device of the files (a CtrFileDevice "ctr")
    FileDevice* mFileDevice; // 0x50
};
ASSERT_SIZE(MainFileDevice, 0x54);
} // namespace sead
