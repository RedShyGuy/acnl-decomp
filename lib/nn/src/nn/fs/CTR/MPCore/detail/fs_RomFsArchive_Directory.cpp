#include "nn/fs/CTR/MPCore/detail/fs_RomFsArchive_Directory.h"

namespace nn {
namespace fs {
namespace CTR {
namespace MPCore {
namespace detail {

// 0x003475BC slot 0x08
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::TrySetPriority(s32 priority)
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

// 0x003475C8 slot 0x04 | fefates:bytes [tier B]
void nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::Close()
{
    this->~Directory();
    mArchive->mDirectoryHeap.Free(this);
}

// 0x00347618 slot 0x00 | fefates:bytes [tier B]
// the subdirectories first, then the files
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::TryRead(s32* readCount, nn::fs::DirectoryEntry* entries, s32 count)
{
    s32 i = 0;
    for (; i < count; i++) {
        nn::fs::DirectoryEntry& entry = entries[i];
        nn::Result result = mArchive->mTable.FindNextDirectory(entry.name, &mFind);
        if (nn::dbm::IsFindFinished(result)) {
            break;
        }
        entry.attributes.isDirectory = true;
        entry.unknown21A = 0;
    }
    for (; i < count; i++) {
        nn::fs::DirectoryEntry& entry = entries[i];
        u32 position = mFind.nextFile;
        nn::Result result = mArchive->mTable.FindNextFile(entry.name, &mFind);
        if (nn::dbm::IsFindFinished(result)) {
            break;
        }
        FileTable::FileEntry fileEntry;
        result = mArchive->mTable.GetFileEntry(&fileEntry, position);
        if (result.IsFailure()) {
            return result;
        }
        entry.size = fileEntry.size;
        entry.attributes.isDirectory = false;
        entry.unknown21A = 0;
    }
    *readCount = i;
    return nn::Result();
}

// 0x00347930 slot 0x10
// 0x0034792C slot 0x14 (deleting dtor)
nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::~Directory()
{
    // nothing to do
}

// 0x0072696C slot 0x0C
nn::Result nn::fs::CTR::MPCore::detail::RomFsArchive::Directory::TryGetPriority(s32* priority) const
{
    return nn::Result(RESULT_UNSUPPORTED_OPERATION);
}

} // namespace detail
} // namespace MPCore
} // namespace CTR
} // namespace fs
} // namespace nn
