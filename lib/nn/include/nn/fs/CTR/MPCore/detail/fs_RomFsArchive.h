#pragma once

#include "decomp.h"
#include "nn/dbm/dbm_HierarchicalRomFileTableTemplate.h"
#include "nn/fnd/fnd_UnitHeapTemplate.h"
#include "nn/fs/CTR/MPCore/detail/fs_IArchive.h"
#include "nn/fs/CTR/MPCore/detail/fs_IDirectory.h"
#include "nn/fs/CTR/MPCore/detail/fs_IFile.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_LockPolicy.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
// results of the RomFs archive (module fs); the names are ours
const bit32 RESULT_UNSUPPORTED_OPERATION = 0xE0C046F8;  // usage, not supported, 760: writing to the image
const bit32 RESULT_INVALID_HEADER = 0xC8A04554;         // status, invalid state, 340: the header is short

// RTTI N2nn2fs3CTR6MPCore6detail12RomFsArchiveE @ 0x008CDC90
// vtable 0x008FBE80 (vptr 0x008FBE88), offset_to_top 0, 18 entries
//
// A read-only archive on a RomFs image (3dbrew "RomFS", level 3) in an IFile. The file is opened
// once per priority group (mFiles); the metadata tables are read through RomFsStorage views,
// either from the file or from a copy in the buffer given to Initialize. Member and helper names
// are ours.
class RomFsArchive : public ::nn::fs::CTR::MPCore::detail::IArchive
{
public:
    class Directory;
    class File;
    class RomFsStorage
    {
    public:
        nn::Result ReadBytes(s64 offset, void* buffer, size_t size); // 0x00346D0C | nintendogs:bytes [tier A]

        // inline (in RomFsArchive::Initialize; names are ours)
        void InitializeOnFile(RomFsArchive* archive, u32 offset, u32 size)
        {
            mBuffer = 0;
            mSize = size;
            mArchive = archive;
            mOffset = offset;
        }
        void InitializeOnMemory(void* buffer, u32 size)
        {
            mBuffer = static_cast<u8*>(buffer);
            mSize = size;
            mArchive = 0;
            mOffset = 0;    // the original stores the archive pointer here (unused)
        }

    private:
        u8* mBuffer;            // 0x00 the table in memory, or 0: read from the file
        u32 mSize;              // 0x04
        RomFsArchive* mArchive; // 0x08
        u32 mOffset;            // 0x0C of the table in the file
    };

    typedef nn::dbm::HierarchicalRomFileTableTemplate<RomFsStorage, RomFsStorage, RomFsStorage, RomFsStorage> FileTable;
    typedef nn::fnd::UnitHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> > ObjectHeap;

    // inline (in the constructor of ContentRomFsArchive)
    RomFsArchive() : mPriority(0), mFile(0)
    {
        for (s32 i = 0; i < PRIORITY_GROUP_COUNT; i++) {
            mFiles[i] = 0;
        }
    }

    virtual nn::Result OpenFile(nn::fs::CTR::MPCore::detail::IFile** file, const ArchivePath& path, u32 mode); // 0x003472E8 slot 0x00 | fefates:bytes
    virtual nn::Result OpenDirectory(nn::fs::CTR::MPCore::detail::IDirectory** directory, const ArchivePath& path); // 0x00346DC0 slot 0x04 | fefates:bytes
    virtual nn::Result DeleteFile(const ArchivePath& path); // 0x00346CF4 slot 0x08
    virtual nn::Result RenameFile(const ArchivePath& path, const ArchivePath& newPath); // 0x00346D00 slot 0x0C
    virtual nn::Result DeleteDirectory(const ArchivePath& path); // 0x00346EA4 slot 0x10
    virtual nn::Result DeleteDirectoryRecursively(const ArchivePath& path); // 0x00347058 slot 0x14
    virtual nn::Result CreateFile(const ArchivePath& path, s64 size); // 0x00346CE8 slot 0x18
    virtual nn::Result CreateDirectory(const ArchivePath& path); // 0x00346E98 slot 0x1C
    virtual nn::Result RenameDirectory(const ArchivePath& path, const ArchivePath& newPath); // 0x00346F18 slot 0x20
    virtual nn::Result SetPriority(s32 priority); // 0x00346F34 slot 0x24
    virtual nn::Result GetPriority(s32* priority); // 0x00346F24 slot 0x28
    virtual nn::Result GetFreeBytes(s64* freeBytes); // 0x00348C24 slot 0x2C
    virtual void DeleteObject() = 0;
    virtual ~RomFsArchive(); // 0x003479BC slot 0x34, 0x00347934 slot 0x38 (deleting)
    // a file of the image as an IFile of its own (the handle is from OpenLinkHandle)
    virtual nn::Result OpenDirect(nn::fs::CTR::MPCore::detail::IFile** file, nn::Handle handle) = 0; // slot 0x3C
    // a handle of a part of the image (slot name is ours)
    virtual nn::Result OpenSubFile(nn::Handle* file, s64 offset, s64 size); // 0x00346EB0 slot 0x40
    virtual nn::Result OpenLinkHandle(nn::Handle* handle); // 0x00346E88 slot 0x44

    nn::Result Initialize(nn::fs::CTR::MPCore::detail::IFile* file, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache); // 0x0012FE88 | fefates:bytes [tier B]

    // the buffer Initialize needs, -1 if the header cannot be read (inline; name is ours)
    static s32 GetRequiredMemorySize(IFile* file, u32 maxFiles, u32 maxDirectories, bool useCache);

protected:
    // the priorities of the FS service are grouped: below -8, -8 to -1, 0, 1 to 8, above 8
    static const s32 PRIORITY_GROUP_COUNT = 5;

    // inline (names are ours)
    static s32 GetPriorityGroup(s32 priority)
    {
        if (priority == 0) {
            return 2;
        }
        if (priority < 0) {
            return priority >= -8 ? 1 : 0;
        }
        return priority <= 8 ? 3 : 4;
    }

    IFile* GetFile(s32 priority) const { return mFiles[GetPriorityGroup(priority)]; }

    // opens the image for the priority group of priority if it is not open yet
    nn::Result PrepareFile(s32 priority)
    {
        if (GetFile(priority) == 0) {
            nn::Handle handle;
            nn::Result result = OpenLinkHandle(&handle);
            if (result.IsFailure()) {
                return result;
            }
            IFile* file = 0;
            result = OpenDirect(&file, handle);
            if (result.IsFailure()) {
                return result;
            }
            result = file->TrySetPriority(priority);
            if (result.IsFailure()) {
                file->Close();
                return result;
            }
            mFiles[GetPriorityGroup(priority)] = file;
        }
        return nn::Result();
    }

    // the level 3 header of the image (3dbrew "RomFS"; offsets and sizes in bytes)
    struct Header
    {
        u32 headerSize;
        u32 directoryHashOffset;
        u32 directoryHashSize;
        u32 directoryMetaOffset;
        u32 directoryMetaSize;
        u32 fileHashOffset;
        u32 fileHashSize;
        u32 fileMetaOffset;
        u32 fileMetaSize;
        u32 fileDataOffset;
    };

    s32 mPriority;                          // 0x04
    IFile* mFile;                           // 0x08 the file given to Initialize
    IFile* mFiles[PRIORITY_GROUP_COUNT];    // 0x0C
    FileTable mTable;                       // 0x20
    RomFsStorage mDirectoryHashStorage;     // 0x70
    RomFsStorage mDirectoryMetaStorage;     // 0x80
    RomFsStorage mFileHashStorage;          // 0x90
    RomFsStorage mFileMetaStorage;          // 0xA0
    u32 mDataOffset;                        // 0xB0 of the file data in the image
    u32 mUnknownB4;                         // 0xB4 not used here
    ObjectHeap mFileHeap;                   // 0xB8 the File objects
    ObjectHeap mDirectoryHeap;              // 0xF4 the Directory objects
};
ASSERT_SIZE(RomFsArchive, 0x130);
} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn


// RTTI N2nn2fs3CTR6MPCore6detail12RomFsArchive9DirectoryE @ 0x008CDC84
// vtable 0x008FBE60 (vptr 0x008FBE68), offset_to_top 0, 6 entries
// An open directory of a RomFsArchive (allocated from its mDirectoryHeap). Member names are ours.
class nn::fs::CTR::MPCore::detail::RomFsArchive::Directory : public ::nn::fs::CTR::MPCore::detail::IDirectory
{
public:
    // inline (RomFsArchive::OpenDirectory)
    Directory(RomFsArchive* archive, const FileTable::FindPosition& find, s32 priority)
        : mArchive(archive), mFind(find), mPriority(priority)
    {
    }

    virtual nn::Result TryRead(s32* readCount, nn::fs::DirectoryEntry* entries, s32 count); // 0x00347618 slot 0x00 | fefates:bytes
    virtual void Close(); // 0x003475C8 slot 0x04 | fefates:bytes
    virtual nn::Result TrySetPriority(s32 priority); // 0x003475BC slot 0x08
    virtual nn::Result TryGetPriority(s32* priority) const; // 0x0072696C slot 0x0C
    virtual ~Directory(); // 0x00347930 slot 0x10, 0x0034792C slot 0x14 (deleting)

private:
    RomFsArchive* mArchive;         // 0x04
    FileTable::FindPosition mFind;  // 0x08
    u32 mUnknown10;                 // 0x10 not used
    s32 mPriority;                  // 0x14
};
ASSERT_SIZE(nn::fs::CTR::MPCore::detail::RomFsArchive::Directory, 0x18);

// RTTI N2nn2fs3CTR6MPCore6detail12RomFsArchive4FileE @ 0x008CDC78
// vtable 0x008FBE1C (vptr 0x008FBE24), offset_to_top 0, 15 entries
// An open file of a RomFsArchive: a range of the image (allocated from its mFileHeap). Member names
// are ours.
class nn::fs::CTR::MPCore::detail::RomFsArchive::File : public ::nn::fs::CTR::MPCore::detail::IFile
{
public:
    // inline (RomFsArchive::OpenFile)
    File(RomFsArchive* archive, s64 begin, s64 end, s32 priority)
        : mArchive(archive), mBegin(begin), mEnd(end), mPriority(priority)
    {
    }

    virtual nn::Result TryRead(s32* readSize, s64 offset, void* buffer, size_t size); // 0x00347230 slot 0x00 | fefates:bytes
    virtual nn::Result TryWrite(s32* writtenSize, s64 offset, const void* buffer, size_t size, bool flush); // 0x003472D4 slot 0x04
    virtual nn::Result TryGetAvailable(s64* available, s64 offset, s64 size); // 0x00348C18 slot 0x08
    virtual nn::Result TryGetSize(s64* size) const; // 0x00726930 slot 0x0C | fefates:bytes
    virtual nn::Result TrySetSize(s64 size); // 0x00347064 slot 0x10
    virtual nn::Result TryFlush(); // 0x003472C8 slot 0x14
    virtual nn::Result TrySetPriority(s32 priority); // 0x00347080 slot 0x18 | fefates:bytes
    virtual nn::Result TryGetPriority(s32* priority) const; // 0x00726958 slot 0x1C
    virtual nn::Result OpenSubFile(nn::Handle* file, s64 offset, s64 size); // 0x003471A8 slot 0x20
    virtual nn::Result OpenLinkHandle(nn::Handle* handle); // 0x00347070 slot 0x24
    virtual nn::Handle GetFileHandle(); // 0x00348C0C slot 0x28
    virtual void DetachFileHandle(); // 0x00348C14 slot 0x2C
    virtual void Close(); // 0x003471E0 slot 0x30 | fefates:bytes
    virtual ~File(); // 0x003472E4 slot 0x34, 0x003472E0 slot 0x38 (deleting)

private:
    typedef s64 s64_align4 __attribute__((aligned(4)));

    RomFsArchive* mArchive; // 0x04
    s64_align4 mBegin;      // 0x08 in the image
    s64_align4 mEnd;        // 0x10
    s32 mPriority;          // 0x18
    u32 mUnknown1C;         // 0x1C not used
};
ASSERT_SIZE(nn::fs::CTR::MPCore::detail::RomFsArchive::File, 0x20);

inline s32 nn::fs::CTR::MPCore::detail::RomFsArchive::GetRequiredMemorySize(IFile* file, u32 maxFiles, u32 maxDirectories, bool useCache)
{
    s32 cacheSize = 0;
    if (useCache) {
        Header header;
        s32 readSize;
        nn::Result result = file->TryRead(&readSize, 0, &header, sizeof(header));
        if (result.IsFailure() || readSize != sizeof(header)) {
            return -1;
        }
        cacheSize = (header.directoryHashSize + header.directoryMetaSize) + (header.fileHashSize + header.fileMetaSize);
        if (cacheSize < 0) {
            return -1;
        }
    }
    return maxFiles * sizeof(File) + maxDirectories * sizeof(Directory) + cacheSize;
}
