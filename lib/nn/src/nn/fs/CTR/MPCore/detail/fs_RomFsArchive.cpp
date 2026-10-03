#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive.h"
#include "nn/err/CTR/CTR_Api.h"
#include "nn/fs/CTR/MPCore/detail/fs_UserFileSystem.h"
#include "nn/fs/fs_Api.h"

#include <new>

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {
namespace {

// ArchivePath type (3dbrew "FS Path")
const u32 PATH_TYPE_UTF16 = 4;

// the text of a UTF-16 path, 0 for other paths (inline; name is ours)
const wchar_t* GetWidePath(const ArchivePath& path)
{
    return (path.type == PATH_TYPE_UTF16) ? reinterpret_cast<const wchar_t*>(path.data) : 0;
}

} // namespace

// 0x00346CE8 slot 0x18
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::CreateFile(const ArchivePath& path, s64 size)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x00346CF4 slot 0x08
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::DeleteFile(const ArchivePath& path)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x00346D00 slot 0x0C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::RenameFile(const ArchivePath& path, const ArchivePath& newPath)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x00346DC0 slot 0x04 | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::OpenDirectory(nn::fs::CTR::MPCore::detail::IDirectory** directory, const ArchivePath& path)
{
    FileTable::FindPosition find;
    nn::Result result = mTable.FindOpen(&find, GetWidePath(path));
    if (result.IsFailure()) {
        return result;
    }
    Directory* opened = new (mDirectoryHeap.Allocate()) Directory(this, find, mPriority);
    *directory = opened;
    return (opened == 0) ? nn::Result(RESULT_OUT_OF_OBJECTS) : nn::Result();
}

// 0x00346E88 slot 0x44
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::OpenLinkHandle(nn::Handle* handle)
{
    return mFile->OpenLinkHandle(handle);
}

// 0x00346E98 slot 0x1C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::CreateDirectory(const ArchivePath& path)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x00346EA4 slot 0x10
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::DeleteDirectory(const ArchivePath& path)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x00346EB0 slot 0x40 (name is ours)
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::OpenSubFile(nn::Handle* file, s64 offset, s64 size)
{
    return GetFile(mPriority)->OpenSubFile(file, offset, size);
}

// 0x00346F18 slot 0x20
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::RenameDirectory(const ArchivePath& path, const ArchivePath& newPath)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x00346F24 slot 0x28
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::GetPriority(s32* priority)
{
    *priority = mPriority;
    return nn::Result();
}

// 0x00346F34 slot 0x24
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::SetPriority(s32 priority)
{
    nn::Result result = PrepareFile(priority);
    if (result.IsFailure()) {
        return result;
    }
    mPriority = priority;
    return nn::Result();
}

// 0x00347058 slot 0x14
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::DeleteDirectoryRecursively(const ArchivePath& path)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x003472E8 slot 0x00 | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::OpenFile(nn::fs::CTR::MPCore::detail::IFile** file, const ArchivePath& path, u32 mode)
{
    FileTable::FileEntry entry;
    nn::Result result = mTable.FindFile(&entry, GetWidePath(path));
    if (result.IsFailure()) {
        return result;
    }
    File* opened = new (mFileHeap.Allocate()) File(this, mDataOffset + entry.offset, entry.offset + entry.size + mDataOffset, mPriority);
    *file = opened;
    if (opened == 0) {
        return nn::Result(RESULT_OUT_OF_OBJECTS);
    }
    result = opened->TrySetPriority(mPriority);
    if (result.IsFailure()) {
        (*file)->Close();
        *file = 0;
    }
    return result;
}

// 0x003479BC slot 0x34
// 0x00347934 slot 0x38 (deleting dtor)
nn::fs::CTR::MPCore::detail::RomFsArchive::~RomFsArchive()
{
    mFile = 0;
    for (s32 i = 0; i < PRIORITY_GROUP_COUNT; i++) {
        if (mFiles[i] != 0) {
            mFiles[i]->Close();
        }
    }
}

// 0x00348C24 slot 0x2C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::GetFreeBytes(s64* freeBytes)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x0012FE88 | fefates:bytes [tier B]
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::Initialize(nn::fs::CTR::MPCore::detail::IFile* file, u32 maxFiles, u32 maxDirectories, void* buffer, size_t bufferSize, bool useCache)
{
    Header header;
    s32 readSize;
    nn::Result result = file->TryRead(&readSize, 0, &header, sizeof(header));
    if (result.IsFailure()) {
        return result;
    }
    if (readSize != sizeof(header)) {
        return nn::Result(RESULT_INVALID_HEADER);
    }
    if (static_cast<size_t>(GetRequiredMemorySize(file, maxFiles, maxDirectories, useCache)) > bufferSize) {
        return nn::Result(RESULT_OUT_OF_OBJECTS);
    }

    u8* heapBuffer = static_cast<u8*>(buffer);
    if (useCache) {
        // the four tables follow the header: read them into the buffer
        s32 position = readSize;
        u8* directoryHash = heapBuffer;
        result = file->TryRead(&readSize, position, directoryHash, header.directoryHashSize);
        if (result.IsFailure()) {
            return result;
        }
        u8* directoryMeta = directoryHash + header.directoryHashSize;
        position += readSize;
        result = file->TryRead(&readSize, position, directoryMeta, header.directoryMetaSize);
        if (result.IsFailure()) {
            return result;
        }
        u8* fileHash = directoryMeta + header.directoryMetaSize;
        position += readSize;
        result = file->TryRead(&readSize, position, fileHash, header.fileHashSize);
        if (result.IsFailure()) {
            return result;
        }
        u8* fileMeta = fileHash + header.fileHashSize;
        position += readSize;
        result = file->TryRead(&readSize, position, fileMeta, header.fileMetaSize);
        if (result.IsFailure()) {
            return result;
        }
        heapBuffer = fileMeta + header.fileMetaSize;
        mDirectoryHashStorage.InitializeOnMemory(directoryHash, header.directoryHashSize);
        mDirectoryMetaStorage.InitializeOnMemory(directoryMeta, header.directoryMetaSize);
        mFileHashStorage.InitializeOnMemory(fileHash, header.fileHashSize);
        mFileMetaStorage.InitializeOnMemory(fileMeta, header.fileMetaSize);
    } else {
        mDirectoryHashStorage.InitializeOnFile(this, header.directoryHashOffset, header.directoryHashSize);
        mDirectoryMetaStorage.InitializeOnFile(this, header.directoryMetaOffset, header.directoryMetaSize);
        mFileHashStorage.InitializeOnFile(this, header.fileHashOffset, header.fileHashSize);
        mFileMetaStorage.InitializeOnFile(this, header.fileMetaOffset, header.fileMetaSize);
    }
    mTable.Initialize(&mDirectoryHashStorage, 0, header.directoryHashSize, &mDirectoryMetaStorage, 0,
                      header.directoryMetaSize, &mFileHashStorage, 0, header.fileHashSize, &mFileMetaStorage, 0,
                      header.fileMetaSize);

    mFileHeap.Initialize(sizeof(File), reinterpret_cast<uptr>(heapBuffer), maxFiles * sizeof(File), 4, 0);
    mDirectoryHeap.Initialize(sizeof(Directory), reinterpret_cast<uptr>(heapBuffer + maxFiles * sizeof(File)),
                              maxDirectories * sizeof(Directory), 4, 0);
    mDataOffset = header.fileDataOffset;
    nn::err::CTR::ThrowFatalErrAllIfFailure(nn::fs::GetPriority(&mPriority));
    mFiles[GetPriorityGroup(mPriority)] = file;
    mFile = file;
    return nn::Result();
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn

// the RomFs file table of RomFsArchive (out of line in the original)

// 0x00826FCC | fefates:bytes [tier B]
template nn::Result nn::dbm::KeyValueRomStorageTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::RomEntryKey, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::DirectoryEntry, 256u>::GetInternal(u32* outPosition, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::DirectoryEntry* outValue, const nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::RomEntryKey& key, u32 hash, const void* aux, u32 auxSize) const;

// 0x00827138 | fefates:bytes [tier B]
template nn::Result nn::dbm::KeyValueRomStorageTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::RomEntryKey, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::DirectoryEntry, 256u>::GetByPosition(nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::RomEntryKey* outKey, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::DirectoryEntry* outValue, void* outAux, u32* outAuxSize, u32 position) const;

// 0x008271F8 | fefates:bytes [tier B]
template nn::Result nn::dbm::KeyValueRomStorageTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::RomEntryKey, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::FileEntry, 256u>::GetInternal(u32* outPosition, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::FileEntry* outValue, const nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::RomEntryKey& key, u32 hash, const void* aux, u32 auxSize) const;

// 0x00827370 | fefates:bytes [tier B]
template nn::Result nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::GetGrandparent(u32* outPosition, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::EntryKey* outKey, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::DirectoryEntry* outEntry, u32 position, nn::dbm::RomPathTool::RomEntryName name, const wchar_t* path) const;

// 0x008273FC | fefates:bytes [tier B]
template nn::Result nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::FindPathRecursive(nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::EntryKey* outParentKey, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::DirectoryEntry* outParentEntry, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::EntryKey* outKey, bool isDirectory, const wchar_t* path) const;

// 0x008277BC | fefates:bytes [tier B]
template nn::Result nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::GetDirectoryEntry(u32* outPosition, nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::DirectoryEntry* outEntry, const nn::dbm::HierarchicalRomFileTableTemplate<nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage, nn::fs::CTR::MPCore::detail::RomFsArchive::RomFsStorage>::EntryKey& key) const;
