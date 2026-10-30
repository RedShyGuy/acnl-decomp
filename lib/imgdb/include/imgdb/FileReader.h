#pragma once

#include "imgdb/imgdb_Types.h"
#include "nn/util/ADLFireWall/util_NonCopyable.h"

namespace imgdb {

// RTTI N5imgdb10FileReaderE
// vtable 0x00908E94 (vptr 0x00908E9C), 4 entries
// reads a file into the buffer of the caller or into memory of the allocator
class FileReader : private nn::util::ADLFireWall::NonCopyable<FileReader>
{
public:
    FileReader(void* buffer, u32 limitSize, bool isTruncate);
    FileReader(Allocator& allocator, u32 limitSize, bool isTruncate);
    virtual ~FileReader();

    virtual bool Read(StorageType storage, const wchar_t* path);
    virtual bool Read(StorageType storage, const wchar_t* path, s64 offset, u32 size);

    s64 GetFileSize() const;
    const void* GetData() const { return m_pData; }
    u32 GetDataSize() const { return m_DataSize; }
    Result GetResult() const { return static_cast<Result>(m_Result); }

protected:
    void ReleaseData()
    {
        if (m_pData != NULL && m_pBuffer == NULL)
        {
            m_pAllocator->Free(m_pData);
        }
        m_pData = NULL;
        m_DataSize = 0;
    }

    Allocator* m_pAllocator;  // +0x04
    void* m_pBuffer;          // +0x08 the buffer of the caller (NULL: allocate)
    u32 m_LimitSize;          // +0x0C
    bool m_IsTruncate;        // +0x10 read only the first m_LimitSize bytes of a larger file
    void* m_pData;            // +0x14
    u32 m_DataSize;           // +0x18
    s64 m_FileSize;           // +0x20
    u8 m_Result;              // +0x28 Result
    nn::Result m_FsResult;    // +0x2C
};
ASSERT_SIZE(FileReader, 0x30);

} // namespace imgdb
