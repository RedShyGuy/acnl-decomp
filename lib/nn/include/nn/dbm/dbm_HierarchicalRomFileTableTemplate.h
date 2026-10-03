#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/dbm/dbm_KeyValueRomStorageTemplate.h"
#include "nn/dbm/dbm_Result.h"
#include "nn/dbm/dbm_RomPathTool.h"
#include "nn/dbm/dbm_RomPathTool_PathParser.h"

#include <wchar.h>

namespace nn {
namespace dbm {

// The directory and file tree of a RomFs (3dbrew "RomFS", level 3): two hash tables, one of the
// directories and one of the files, keyed by the position of the parent directory and the name.
// The positions are offsets in the entry storages; the root directory is at 0. The class name, the
// template parameters and the names RomEntryKey, EntryKey, DirectoryEntry and FileEntry are from
// the symbols; members, helpers and the fields of the entries (after 3dbrew) are ours.
template <typename DirectoryBucketStorageT, typename DirectoryEntryStorageT, typename FileBucketStorageT,
          typename FileEntryStorageT>
class HierarchicalRomFileTableTemplate
{
public:
    // the 64 bit fields of a file entry are only 4 byte aligned
    typedef s64 s64_align4 __attribute__((aligned(4)));

    static const u32 INVALID_POSITION = 0xFFFFFFFF;

    // the key in the tables (the name is the aux data)
    struct RomEntryKey
    {
        u32 parent;     // position of the parent directory

        // the parents and the UTF-16 names are equal (inline; name is ours)
        bool IsEqual(const RomEntryKey& other, const void* name, u32 nameSize, const void* otherName,
                     u32 otherNameSize) const
        {
            return parent == other.parent && nameSize == otherNameSize &&
                   wcsncmp(static_cast<const wchar_t*>(name), static_cast<const wchar_t*>(otherName),
                           nameSize / sizeof(wchar_t)) == 0;
        }
    };

    // a key with its name
    struct EntryKey
    {
        RomEntryKey key;
        RomPathTool::RomEntryName name;

        // the hash of the tables (inline; name is ours)
        u32 Hash() const
        {
            u32 hash = key.parent ^ 123456789;
            const wchar_t* end = name.path + name.length;
            for (const wchar_t* p = name.path; p < end; p++) {
                hash = ((hash >> 5) | (hash << 27)) ^ *p;
            }
            return hash;
        }
    };

    struct DirectoryEntry
    {
        u32 next;       // the next directory in the parent
        u32 directory;  // the first subdirectory
        u32 file;       // the first file
    };

    struct FileEntry
    {
        u32 next;           // the next file in the parent
        s64_align4 offset;  // of the data, from the start of the file data
        s64_align4 size;
    };

    // where a directory listing stands (name is ours)
    struct FindPosition
    {
        u32 nextDirectory;
        u32 nextFile;
    };

    typedef KeyValueRomStorageTemplate<DirectoryBucketStorageT, DirectoryEntryStorageT, RomEntryKey, DirectoryEntry, 256>
        DirectoryEntryMapTable;
    typedef KeyValueRomStorageTemplate<FileBucketStorageT, FileEntryStorageT, RomEntryKey, FileEntry, 256>
        FileEntryMapTable;

    // inline (in RomFsArchive::Initialize; name is ours); sizes in bytes
    void Initialize(DirectoryBucketStorageT* directoryBucketStorage, s64 directoryBucketOffset,
                    u32 directoryBucketSize, DirectoryEntryStorageT* directoryEntryStorage,
                    s64 directoryEntryOffset, u32 directoryEntrySize, FileBucketStorageT* fileBucketStorage,
                    s64 fileBucketOffset, u32 fileBucketSize, FileEntryStorageT* fileEntryStorage,
                    s64 fileEntryOffset, u32 fileEntrySize)
    {
        mDirectoryTable.Initialize(directoryBucketStorage, directoryBucketOffset, directoryBucketSize / sizeof(u32),
                                   directoryEntryStorage, directoryEntryOffset, directoryEntrySize);
        mFileTable.Initialize(fileBucketStorage, fileBucketOffset, fileBucketSize / sizeof(u32), fileEntryStorage,
                              fileEntryOffset, fileEntrySize);
    }

    // the file at path (inline, in RomFsArchive::OpenFile; name is ours)
    nn::Result FindFile(FileEntry* outEntry, const wchar_t* path) const
    {
        EntryKey parentKey;
        DirectoryEntry parentEntry;
        EntryKey key;
        nn::Result result = FindPathRecursive(&parentKey, &parentEntry, &key, false, path);
        if (result.IsFailure()) {
            return result;
        }
        u32 position;
        return GetFileEntry(&position, outEntry, key);
    }

    // starts a listing of the directory at path (inline, in RomFsArchive::OpenDirectory; name is ours)
    nn::Result FindOpen(FindPosition* find, const wchar_t* path) const
    {
        find->nextDirectory = INVALID_POSITION;
        find->nextFile = INVALID_POSITION;
        EntryKey parentKey;
        DirectoryEntry parentEntry;
        EntryKey key;
        nn::Result result = FindPathRecursive(&parentKey, &parentEntry, &key, true, path);
        if (result.IsFailure()) {
            return result;
        }
        u32 position;
        DirectoryEntry entry;
        result = GetDirectoryEntry(&position, &entry, key);
        if (result.IsFailure()) {
            return result;
        }
        find->nextDirectory = entry.directory;
        find->nextFile = entry.file;
        return nn::Result();
    }

    // the name of the next subdirectory, RESULT_FIND_FINISHED after the last one (inline, in
    // RomFsArchive::Directory::TryRead; name is ours)
    nn::Result FindNextDirectory(wchar_t* name, FindPosition* find) const
    {
        if (find->nextDirectory == INVALID_POSITION) {
            return nn::Result(RESULT_FIND_FINISHED);
        }
        RomEntryKey key;
        DirectoryEntry entry;
        u32 size;
        nn::Result result = mDirectoryTable.GetByPosition(&key, &entry, name, &size, find->nextDirectory);
        if (result.IsFailure()) {
            return result;
        }
        name[size / sizeof(wchar_t)] = L'\0';
        find->nextDirectory = entry.next;
        return nn::Result();
    }

    // the same for the files (inline; name is ours)
    nn::Result FindNextFile(wchar_t* name, FindPosition* find) const
    {
        if (find->nextFile == INVALID_POSITION) {
            return nn::Result(RESULT_FIND_FINISHED);
        }
        RomEntryKey key;
        FileEntry entry;
        u32 size;
        nn::Result result = mFileTable.GetByPosition(&key, &entry, name, &size, find->nextFile);
        if (result.IsFailure()) {
            return result;
        }
        name[size / sizeof(wchar_t)] = L'\0';
        find->nextFile = entry.next;
        return nn::Result();
    }

    // the file at a position (inline, in RomFsArchive::Directory::TryRead; name is ours)
    nn::Result GetFileEntry(FileEntry* outEntry, u32 position) const
    {
        RomEntryKey key;
        nn::Result result = mFileTable.GetByPosition(&key, outEntry, 0, 0, position);
        if (result.IsFailure() && IsKeyNotFound(result)) {
            DirectoryEntry entry;
            result = mDirectoryTable.GetByPosition(&key, &entry, 0, 0, position);
            if (result.IsSuccess()) {
                return nn::Result(RESULT_WRONG_ENTRY_TYPE);
            }
            if (IsKeyNotFound(result)) {
                return nn::Result(RESULT_FILE_NOT_FOUND);
            }
        }
        return result;
    }

private:
    // the directory that contains the directory at position (name: its name, path: the path it
    // points into)
    nn::Result GetGrandparent(u32* outPosition, EntryKey* outKey, DirectoryEntry* outEntry, u32 position,
                              RomPathTool::RomEntryName name, const wchar_t* path) const;

    // walks path: the directory that contains the last name (key and entry) and the key of the last
    // name (resolving "." and ".."); isDirectory: the path names a directory
    nn::Result FindPathRecursive(EntryKey* outParentKey, DirectoryEntry* outParentEntry, EntryKey* outKey,
                                 bool isDirectory, const wchar_t* path) const;

    nn::Result GetDirectoryEntry(u32* outPosition, DirectoryEntry* outEntry, const EntryKey& key) const;

    // inline (names are ours)
    template <typename TableT, typename ValueT>
    static nn::Result GetEntry(const TableT& table, u32* outPosition, ValueT* outValue, const EntryKey& key)
    {
        return table.GetInternal(outPosition, outValue, key.key, key.Hash(), key.name.path,
                                 key.name.length * sizeof(wchar_t));
    }

    nn::Result GetFileEntry(u32* outPosition, FileEntry* outEntry, const EntryKey& key) const
    {
        nn::Result result = GetEntry(mFileTable, outPosition, outEntry, key);
        if (result.IsFailure() && IsKeyNotFound(result)) {
            u32 position;
            DirectoryEntry entry;
            result = GetEntry(mDirectoryTable, &position, &entry, key);
            if (result.IsSuccess()) {
                return nn::Result(RESULT_WRONG_ENTRY_TYPE);
            }
            if (IsKeyNotFound(result)) {
                return nn::Result(RESULT_FILE_NOT_FOUND);
            }
        }
        return result;
    }

    // the directory of the names before the last one of the parser: walks down from the root
    DECOMP_ALWAYS_INLINE nn::Result FindParentDirectoryRecursive(u32* outPosition, EntryKey* outKey, DirectoryEntry* outEntry,
                                            RomPathTool::PathParser* parser, const wchar_t* path) const
    {
        EntryKey key;
        key.key.parent = 0;
        u32 position = 0;
        DirectoryEntry entry;
        nn::Result result = parser->GetNextDirectoryName(&key.name);
        if (result.IsFailure()) {
            return result;
        }
        result = GetDirectoryEntry(&position, &entry, key);
        if (result.IsFailure()) {
            return result;
        }
        u32 parentPosition = position;
        while (!parser->IsFinished()) {
            EntryKey oldKey = key;
            result = parser->GetNextDirectoryName(&key.name);
            if (result.IsFailure()) {
                return result;
            }
            if (RomPathTool::IsCurrentDirectory(key.name)) {
                key = oldKey;
                continue;
            }
            if (RomPathTool::IsParentDirectory(key.name)) {
                if (parentPosition == 0) {
                    return nn::Result(RESULT_WRONG_ENTRY_TYPE);
                }
                result = GetGrandparent(&parentPosition, &key, &entry, key.key.parent, key.name, path);
                if (result.IsFailure()) {
                    return result;
                }
                continue;
            }
            key.key.parent = parentPosition;
            result = GetDirectoryEntry(&position, &entry, key);
            if (result.IsFailure()) {
                return result;
            }
            parentPosition = position;
        }
        *outKey = key;
        *outPosition = parentPosition;
        *outEntry = entry;
        return nn::Result();
    }

    DirectoryEntryMapTable mDirectoryTable; // 0x00
    FileEntryMapTable mFileTable;           // 0x28
};

template <typename DirectoryBucketStorageT, typename DirectoryEntryStorageT, typename FileBucketStorageT,
          typename FileEntryStorageT>
nn::Result HierarchicalRomFileTableTemplate<DirectoryBucketStorageT, DirectoryEntryStorageT, FileBucketStorageT,
                                            FileEntryStorageT>::GetGrandparent(u32* outPosition, EntryKey* outKey,
                                                                               DirectoryEntry* outEntry, u32 position,
                                                                               RomPathTool::RomEntryName name,
                                                                               const wchar_t* path) const
{
    RomEntryKey parentKey;
    DirectoryEntry parentEntry;
    nn::Result result = mDirectoryTable.GetByPosition(&parentKey, &parentEntry, 0, 0, position);
    if (result.IsFailure()) {
        return result;
    }
    outKey->key = parentKey;
    result = RomPathTool::GetParentDirectoryName(&outKey->name, name, path);
    if (result.IsFailure()) {
        return result;
    }
    result = GetDirectoryEntry(outPosition, outEntry, *outKey);
    if (result.IsFailure()) {
        return result;
    }
    return nn::Result();
}

template <typename DirectoryBucketStorageT, typename DirectoryEntryStorageT, typename FileBucketStorageT,
          typename FileEntryStorageT>
nn::Result HierarchicalRomFileTableTemplate<DirectoryBucketStorageT, DirectoryEntryStorageT, FileBucketStorageT,
                                            FileEntryStorageT>::FindPathRecursive(EntryKey* outParentKey,
                                                                                  DirectoryEntry* outParentEntry,
                                                                                  EntryKey* outKey, bool isDirectory,
                                                                                  const wchar_t* path) const
{
    RomPathTool::PathParser parser;
    nn::Result result = parser.Initialize(path);
    if (result.IsFailure()) {
        return result;
    }
    u32 parentPosition;
    result = FindParentDirectoryRecursive(&parentPosition, outParentKey, outParentEntry, &parser, path);
    if (result.IsFailure()) {
        return result;
    }

    u32 position;
    if (isDirectory) {
        RomPathTool::RomEntryName name;
        result = parser.GetAsDirectoryName(&name);
        if (result.IsFailure()) {
            return result;
        }
        if (RomPathTool::IsCurrentDirectory(name)) {
            // the parent itself; its parent becomes the parent
            *outKey = *outParentKey;
            if (outKey->key.parent != 0) {
                result = GetGrandparent(&position, outParentKey, outParentEntry, outParentKey->key.parent,
                                        outParentKey->name, path);
                if (result.IsFailure()) {
                    return result;
                }
            }
        } else if (RomPathTool::IsParentDirectory(name)) {
            if (parentPosition == 0) {
                return nn::Result(RESULT_WRONG_ENTRY_TYPE);
            }
            DirectoryEntry entry;
            result = GetGrandparent(&position, outKey, &entry, outParentKey->key.parent, outParentKey->name, path);
            if (result.IsFailure()) {
                return result;
            }
            if (outKey->key.parent != 0) {
                result = GetGrandparent(&position, outParentKey, outParentEntry, outKey->key.parent, outKey->name,
                                        path);
                if (result.IsFailure()) {
                    return result;
                }
            }
        } else {
            outKey->name = name;
            outKey->key.parent = (name.length == 0) ? 0 : parentPosition;   // "/": the root
        }
    } else {
        if (parser.IsDirectoryPath()) {
            return nn::Result(RESULT_WRONG_ENTRY_TYPE);
        }
        outKey->key.parent = parentPosition;
        result = parser.GetAsFileName(&outKey->name);
        if (result.IsFailure()) {
            return result;
        }
    }
    return nn::Result();
}

template <typename DirectoryBucketStorageT, typename DirectoryEntryStorageT, typename FileBucketStorageT,
          typename FileEntryStorageT>
nn::Result HierarchicalRomFileTableTemplate<DirectoryBucketStorageT, DirectoryEntryStorageT, FileBucketStorageT,
                                            FileEntryStorageT>::GetDirectoryEntry(u32* outPosition,
                                                                                  DirectoryEntry* outEntry,
                                                                                  const EntryKey& key) const
{
    nn::Result result = GetEntry(mDirectoryTable, outPosition, outEntry, key);
    if (result.IsFailure() && IsKeyNotFound(result)) {
        u32 position;
        FileEntry entry;
        result = GetEntry(mFileTable, &position, &entry, key);
        if (result.IsSuccess()) {
            return nn::Result(RESULT_WRONG_ENTRY_TYPE);
        }
        if (IsKeyNotFound(result)) {
            return nn::Result(RESULT_DIRECTORY_NOT_FOUND);
        }
    }
    return result;
}

} // namespace dbm
} // namespace nn
