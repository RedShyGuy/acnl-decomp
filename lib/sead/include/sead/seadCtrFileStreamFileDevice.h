#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/fs/fs_Directory.h"
#include "nn/fs/fs_FileStream.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadFileDevice.h"

namespace sead {
// RTTI N4sead23CtrFileStreamFileDeviceE @ 0x008D1F2C
// vtable 0x00906738 (vptr 0x00906740), offset_to_top 0, 30 entries
//
// A device on an archive of nn::fs ("<mount name>:/<path>", the mount name comes from the derived
// class). An opened file keeps an nn::fs::FileStream and the open flag in the handle buffer, an
// opened directory an nn::fs::Directory. The do*_ names are those of the reference symbols (mk7dlp,
// nintendogs); the others are ours.
class CtrFileStreamFileDevice : public ::sead::FileDevice
{
public:
    // what an opened file keeps in FileHandle::mHandleBuffer
    struct FileHandleInner
    {
        nn::fs::FileStream stream; // 0x00
        u8 openFlag;               // 0x1C, the FileOpenFlag
    };

    explicit CtrFileStreamFileDevice(const SafeStringBase<char>& driveName); // 0x00125C5C (name is ours)
    virtual ~CtrFileStreamFileDevice(); // 0x0054CB40 slot 0x00
    // 0x0054CB30 slot 0x04 (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074E078 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return FileDevice::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074E02C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098355C (guard 0x00983558)
        static const RuntimeTypeInfo::Derive<FileDevice> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual bool doIsAvailable_() const; // 0x0074DF48 slot 0x24 | nintendogs:bytes-fuzzy
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeStringBase<char>& path, FileOpenFlag flag); // 0x0054C83C slot 0x30 | nintendogs:bytes-fuzzy
    virtual bool doClose_(FileHandle* handle); // 0x0054CA34 slot 0x34
    virtual bool doRead_(u32* readSize, FileHandle* handle, u8* buffer, u32 size); // 0x0054C95C slot 0x38
    virtual bool doWrite_(u32* writtenSize, FileHandle* handle, const u8* buffer, u32 size); // 0x0054CAA8 slot 0x3C
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin); // 0x0054C9D4 slot 0x40 | nintendogs:bytes-fuzzy
    virtual bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle); // 0x0054C6D8 slot 0x44
    virtual bool doGetFileSize_(u32* size, const SafeStringBase<char>& path); // 0x0054BE40 slot 0x48 | mk7dlp:bytes-fuzzy
    virtual bool doGetFileSize_(u32* size, FileHandle* handle); // 0x0054BDE8 slot 0x4C
    virtual bool doIsExistFile_(bool* isExist, const SafeStringBase<char>& path); // 0x0054BF40 slot 0x50 | nintendogs:bytes-fuzzy
    virtual bool doIsExistDirectory_(bool* isExist, const SafeStringBase<char>& path); // 0x0054C48C slot 0x54
    virtual FileDevice* doOpenDirectory_(HandleBase* handle, const SafeStringBase<char>& path); // 0x0054C184 slot 0x58
    virtual bool doCloseDirectory_(HandleBase* handle); // 0x0054C398 slot 0x5C
    virtual bool doReadDirectory_(u32* readNum, HandleBase* handle, DirectoryEntry* entries, u32 num); // 0x0054C1FC slot 0x60
    virtual bool doMakeDirectory_(const SafeStringBase<char>& path, u32 permission); // 0x0054C0BC slot 0x64
    virtual s32 doGetLastRawError_() const; // 0x0074E024 slot 0x68
    // "<mount name>:/<path>"
    virtual void doResolvePath_(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const; // 0x0074DFD0 slot 0x70
    using FileDevice::doResolvePath_;
    // the name of the archive in nn::fs
    virtual const char* getMountName() const = 0; // slot 0x74

    // opens the file mount:/path into stream
    static nn::Result tryOpenFile_(nn::fs::FileStream* stream, const SafeStringBase<char>& mount,
                                   const SafeStringBase<char>& path, u32 mode); // 0x0054C604
    // the same without a slash at the end of the path
    static nn::Result tryOpenFileWithoutLastSlash_(nn::fs::FileStream* stream, const SafeStringBase<char>& mount,
                                                   const SafeStringBase<char>& path, u32 mode); // 0x0054C714
    // opens the directory mount:/path into directory
    static nn::Result tryOpenDirectory_(nn::fs::Directory* directory, const SafeStringBase<char>& mount,
                                        const SafeStringBase<char>& path); // 0x0054C3D4

    nn::Result mLastRawError; // 0x50
    // written data is flushed at once
    bool mIsFlushOnWrite; // 0x54
};
ASSERT_SIZE(CtrFileStreamFileDevice, 0x58);
} // namespace sead
