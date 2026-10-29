#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadFixedSafeString.h"
#include "sead/seadIDisposer.h"
#include "sead/seadTListNode.h"

namespace sead {
class FileHandle;
class HandleBase;
class Heap;

// RTTI N4sead10FileDeviceE @ 0x008D1364
// vtable 0x00904E38 (vptr 0x00904E40), offset_to_top 0, 29 entries
//
// A device that files are loaded from and saved to, mounted under a drive name. The public try*
// functions check the permission and the handle and call the do*_ slots of the device. The names of
// the reference symbols are tryLoad, doLoad_, tryRead, tryWrite, tryGetFileSize, isAvailable,
// getLastRawError, isMatchDevice_ and the do*_ slots they name; all other names are ours.
class FileDevice : public ::sead::TListNode<sead::FileDevice*>, public ::sead::IDisposer
{
public:
    enum FileOpenFlag
    {
        FILE_OPEN_FLAG_READ_ONLY = 0,
        FILE_OPEN_FLAG_WRITE_ONLY = 1,
    };
    enum SeekOrigin
    {
        SEEK_ORIGIN_BEGIN = 0,
        SEEK_ORIGIN_CURRENT = 1,
        SEEK_ORIGIN_END = 2,
    };

    // what tryLoad loads and what it gives back
    struct LoadArg
    {
        // (inline in DirectResourceFactoryBase::tryLoad)
        DECOMP_EMIT_COPY LoadArg() // 0x0053E8F8 (name is ours)
            : path(&SafeStringBase<char>::s_NullChar), buffer(nullptr), bufferSize(0), heap(nullptr),
              bufferSizeAlignment(0), divSize(0), field_0x1C(true), checkFileSize(true), readSize(0),
              roundupBufferSize(0), needUnload(false)
        {
        }

        SafeStringBase<char> path;      // 0x00
        u8* buffer;                     // 0x08, NULL: allocated from heap
        u32 bufferSize;                 // 0x0C, 0: the size of the file
        Heap* heap;                     // 0x10, NULL: the current heap
        s32 bufferSizeAlignment;        // 0x14, at least 32 (the sign is kept)
        u32 divSize;                    // 0x18, see FileHandle::mDivSize
        bool field_0x1C;                // 0x1C (unused here)
        bool checkFileSize;             // 0x1D, a given buffer is checked against the file size
        u32 readSize;                   // 0x20 (out)
        u32 roundupBufferSize;          // 0x24 (out)
        bool needUnload;                // 0x28 (out) the buffer was allocated
    };

    // what doSave_ writes
    struct SaveArg
    {
        SafeStringBase<char> path; // 0x00
        const u8* buffer;          // 0x08
        u32 dataSize;              // 0x0C
        u32 writeSize;             // 0x10 (out)
    };

    // an entry of a directory
    struct DirectoryEntry
    {
        FixedSafeString<256> name; // 0x000
        bool isDirectory;          // 0x10C
    };

    // (inline in the constructors of the devices)
    explicit FileDevice(const SafeStringBase<char>& driveName)
        : TListNode<FileDevice*>(this), IDisposer(), mDriveName(), mPermission(true)
    {
        mDriveName.copy(driveName);
    }
    virtual ~FileDevice(); // 0x0053F01C slot 0x00 | nintendogs:bytes
    // 0x0053EFDC slot 0x04 | nintendogs:bytes (deleting dtor)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074A19C (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074A138 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833AC (guard 0x009833A8)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    // the resolved path of a file / of a directory (the result is not kept)
    virtual void resolvePathOfFile(const SafeStringBase<char>& path) const; // 0x0074A054 slot 0x10
    virtual void resolvePathOfDirectory(const SafeStringBase<char>& path) const; // 0x0074A184 slot 0x14
    // the resolved path of a file / of a directory into out
    virtual void resolvePathOfFile(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const; // 0x0074A12C slot 0x18
    virtual void resolvePathOfDirectory(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const; // 0x0074A190 slot 0x1C
    // whether the handle belongs to this device
    virtual bool isMatchDevice_(const HandleBase* handle) const; // 0x0074A104 slot 0x20
    virtual bool doIsAvailable_() const = 0; // slot 0x24
    virtual u8* doLoad_(LoadArg& arg); // 0x0053E944 slot 0x28
    virtual bool doSave_(SaveArg& arg); // 0x0053EC10 slot 0x2C
    // the device that holds the opened file, NULL if it failed
    virtual FileDevice* doOpen_(FileHandle* handle, const SafeStringBase<char>& path, FileOpenFlag flag) = 0; // slot 0x30
    virtual bool doClose_(FileHandle* handle) = 0; // slot 0x34
    virtual bool doRead_(u32* readSize, FileHandle* handle, u8* buffer, u32 size) = 0; // slot 0x38
    virtual bool doWrite_(u32* writtenSize, FileHandle* handle, const u8* buffer, u32 size) = 0; // slot 0x3C
    virtual bool doSeek_(FileHandle* handle, s32 offset, SeekOrigin origin) = 0; // slot 0x40
    virtual bool doGetCurrentSeekPos_(u32* pos, FileHandle* handle) = 0; // slot 0x44
    virtual bool doGetFileSize_(u32* size, const SafeStringBase<char>& path) = 0; // slot 0x48
    virtual bool doGetFileSize_(u32* size, FileHandle* handle) = 0; // slot 0x4C
    virtual bool doIsExistFile_(bool* isExist, const SafeStringBase<char>& path) = 0; // slot 0x50
    virtual bool doIsExistDirectory_(bool* isExist, const SafeStringBase<char>& path) = 0; // slot 0x54
    // the device that holds the opened directory, NULL if it failed
    virtual FileDevice* doOpenDirectory_(HandleBase* handle, const SafeStringBase<char>& path) = 0; // slot 0x58
    virtual bool doCloseDirectory_(HandleBase* handle) = 0; // slot 0x5C
    virtual bool doReadDirectory_(u32* readNum, HandleBase* handle, DirectoryEntry* entries, u32 num) = 0; // slot 0x60
    virtual bool doMakeDirectory_(const SafeStringBase<char>& path, u32 permission) = 0; // slot 0x64
    virtual s32 doGetLastRawError_() const = 0; // slot 0x68
    virtual void doResolvePath_(const SafeStringBase<char>& path) const; // 0x00749FF8 slot 0x6C
    virtual void doResolvePath_(BufferedSafeStringBase<char>* out, const SafeStringBase<char>& path) const; // 0x0074A060 slot 0x70

    bool isAvailable() const; // 0x00749FD8 | nintendogs:callgraph [tier A]
    u8* tryLoad(LoadArg& arg); // 0x00132D10 | nintendogs:callgraph [tier A]
    // the device that holds the opened file, NULL if it failed
    FileDevice* tryOpen(FileHandle* handle, const SafeStringBase<char>& path, FileOpenFlag flag, u32 divSize); // 0x0053ED8C
    bool tryClose(FileHandle* handle); // 0x0013F664
    bool tryRead(u32* readSize, FileHandle* handle, u8* buffer, u32 size); // 0x0053EDD8 | nintendogs:bytes-fuzzy [tier A]
    bool tryWrite(u32* writtenSize, FileHandle* handle, const u8* buffer, u32 size); // 0x0053EF58 | nintendogs:bytes-fuzzy [tier A]
    bool trySeek(FileHandle* handle, s32 offset, SeekOrigin origin); // 0x0053EEF0
    bool tryGetCurrentSeekPos(u32* pos, FileHandle* handle); // 0x0053E88C
    bool tryGetFileSize(u32* size, const SafeStringBase<char>& path); // 0x0053E684 | nintendogs:bytes
    bool tryGetFileSize(u32* size, FileHandle* handle); // 0x0053E654
    bool tryIsExistFile(bool* isExist, const SafeStringBase<char>& path); // 0x0053E6B0
    bool tryIsExistDirectory(bool* isExist, const SafeStringBase<char>& path); // 0x0053E858
    FileDevice* tryOpenDirectory(HandleBase* handle, const SafeStringBase<char>& path); // 0x0053E704
    bool tryCloseDirectory(HandleBase* handle); // 0x0053E7E8
    bool tryReadDirectory(u32* readNum, HandleBase* handle, DirectoryEntry* entries, u32 num); // 0x0053E748
    bool tryMakeDirectory(const SafeStringBase<char>& path, u32 permission); // 0x0053E6DC
    s32 getLastRawError() const; // 0x0074A120 | nintendogs:callgraph [tier A]

    FixedSafeString<32> mDriveName; // 0x20
    // the device can be used
    bool mPermission; // 0x4C
};
ASSERT_SIZE(FileDevice, 0x50);
} // namespace sead
