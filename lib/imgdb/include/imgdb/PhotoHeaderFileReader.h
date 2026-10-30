#pragma once

#include "imgdb/FileReader.h"

namespace imgdb {

// the start of an image (the first 128 KB) for its header; larger files than the limit are not read
// RTTI N5imgdb21PhotoHeaderFileReaderE
// vtable 0x00909018 (vptr 0x00909020), 4 entries
class PhotoHeaderFileReader : public FileReader
{
public:
    static const u32 HEADER_SIZE = 0x20000;

    explicit PhotoHeaderFileReader(Allocator& allocator)
        : FileReader(allocator, HEADER_SIZE, true), m_LimitFileSize(s_DefaultLimitFileSize)
    {
    }
    virtual ~PhotoHeaderFileReader();

    virtual bool Read(StorageType storage, const wchar_t* path);
    virtual bool Read(StorageType storage, const wchar_t* path, s64 offset, u32 size);

    static void SetDefaultLimitFileSize(u32 size);

protected:
    u32 m_LimitFileSize;  // +0x30

    static bool s_IsDefaultLimitFileSizeSet;
    static u32 s_DefaultLimitFileSize;
};
ASSERT_SIZE(PhotoHeaderFileReader, 0x38);  // (8 byte aligned; derived classes use the 4 bytes after m_LimitFileSize)

} // namespace imgdb
