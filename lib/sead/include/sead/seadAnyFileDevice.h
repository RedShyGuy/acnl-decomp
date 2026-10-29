#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadFileDevice.h"
#include "sead/seadPtrArrayImpl.h"

namespace sead {
// RTTI N4sead13AnyFileDeviceE @ 0x008D15D8
// vtable 0x0090529C (vptr 0x009052A4), offset_to_top 0, 29 entries
//
// A device that asks a list of devices which one has a file. Slot names as in FileDevice.
class AnyFileDevice : public ::sead::FileDevice
{
public:
    // (inline; the devices are added with pushBack into a buffer the owner sets)
    explicit AnyFileDevice(const SafeStringBase<char>& driveName)
        : FileDevice(driveName), mDeviceList(), mLastDevice(nullptr)
    {
    }
    virtual ~AnyFileDevice(); // 0x00541754 slot 0x00 | nintendogs:bytes
    // 0x0054172C slot 0x04 | nintendogs:bytes (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074B044 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return FileDevice::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074AEA4 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833FC (guard 0x009833F8)
        static const RuntimeTypeInfo::Derive<FileDevice> s_TypeInfo;
        return &s_TypeInfo;
    }
    // the slots ask the device that has the file / directory; mLastDevice is the last one used
    virtual void resolvePathOfFile(const SafeStringBase<char>& path) const; // 0x0074AD94 slot 0x10
    virtual void resolvePathOfDirectory(const SafeStringBase<char>& path) const; // 0x0074AEF0 slot 0x14
    virtual void resolvePathOfFile(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const; // 0x0074AE58 slot 0x18
    virtual void resolvePathOfDirectory(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const; // 0x0074AF1C slot 0x1C
    virtual bool isMatchDevice_(const HandleBase* handle) const; // 0x0074AE04 slot 0x20 | nintendogs:bytes-fuzzy
    virtual bool doIsAvailable_() const; // 0x0074ADC0 slot 0x24 | nintendogs:bytes
    virtual u8* doLoad_(LoadArg& arg); // 0x0054167C slot 0x28 | nintendogs:bytes
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeStringBase<char>& path, FileOpenFlag flag); // 0x005416A8 slot 0x30 | nintendogs:bytes
    virtual bool doClose_(FileHandle* handle); // 0x0013F654 slot 0x34
    virtual bool doRead_(u32* readSize, FileHandle* handle, u8* buffer, u32 size); // 0x005416EC slot 0x38
    virtual bool doWrite_(u32* writtenSize, FileHandle* handle, const u8* buffer, u32 size); // 0x0054170C slot 0x3C
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin); // 0x0053EEE0 slot 0x40
    virtual bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle); // 0x0053E87C slot 0x44
    virtual bool doGetFileSize_(u32* size, const SafeStringBase<char>& path); // 0x0054133C slot 0x48 | nintendogs:bytes
    virtual bool doGetFileSize_(u32* size, FileHandle* handle); // 0x0053E644 slot 0x4C
    virtual bool doIsExistFile_(bool* isExist, const SafeStringBase<char>& path); // 0x00541374 slot 0x50
    virtual bool doIsExistDirectory_(bool* isExist, const SafeStringBase<char>& path); // 0x0054164C slot 0x54
    // one directory can be open at a time (its path is kept)
    virtual FileDevice* doOpenDirectory_(HandleBase* handle, const SafeStringBase<char>& path); // 0x005413AC slot 0x58
    virtual bool doCloseDirectory_(HandleBase* handle); // 0x00541620 slot 0x5C
    // goes on with the directory of the same path on the next devices
    virtual bool doReadDirectory_(u32* readNum, HandleBase* handle, DirectoryEntry* entries, u32 num); // 0x005414BC slot 0x60
    virtual bool doMakeDirectory_(const SafeStringBase<char>& path, u32 permission); // 0x005413A4 slot 0x64
    virtual s32 doGetLastRawError_() const; // 0x0074AE90 slot 0x68

    // adds a device to ask (false if the list is full)
    bool pushBack(FileDevice* device); // 0x0011D6F0 | nintendogs:bytes [tier A]
    // the first available device that has the file / the directory
    FileDevice* findFileDeviceByFile_(const SafeStringBase<char>& path) const; // 0x0074AF54 | nintendogs:callgraph [tier A]
    FileDevice* findFileDeviceByDirectory_(const SafeStringBase<char>& path) const; // 0x0074AFCC (name is ours)

    PtrArray<FileDevice> mDeviceList; // 0x50
    FileDevice* mLastDevice;          // 0x5C
};
ASSERT_SIZE(AnyFileDevice, 0x60);
} // namespace sead
