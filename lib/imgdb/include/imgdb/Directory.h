#pragma once

#include "imgdb/imgdb_Types.h"
#include "nn/fs/fs_Directory.h"
#include "nn/fs/fs_Types.h"

namespace imgdb {

// the entries of a directory, read 64 at a time
class Directory
{
public:
    static const int READ_ENTRY_NUM = 64;

    Directory();
    Directory(Allocator& allocator, StorageType storage, const wchar_t* path, int maxEntryNum);
    ~Directory();

    void Initialize(Allocator& allocator, StorageType storage, const wchar_t* path, int maxEntryNum);
    // the next entry; false at the end or after maxEntryNum entries
    bool Read();
    bool IsDirectory() const;
    bool IsFile() const;
    const nn::fs::DirectoryEntry* GetEntry() const;
    bool IsOpen() const { return m_IsOpen; }

private:
    Allocator* m_pAllocator;               // +0x00
    bool m_IsOpen;                         // +0x04
    u8 _05[3];                             // +0x05
    u8 m_Storage;                          // +0x08 StorageType
    nn::fs::Directory m_Directory;         // +0x0C
    nn::fs::DirectoryEntry* m_pEntries;    // +0x14
    s32 m_EntryNum;                        // +0x18 read into m_pEntries
    s32 m_EntryIndex;                      // +0x1C
    s32 m_MaxEntryNum;                     // +0x20
    s32 m_ReadNum;                         // +0x24
};
ASSERT_SIZE(Directory, 0x28);

} // namespace imgdb
