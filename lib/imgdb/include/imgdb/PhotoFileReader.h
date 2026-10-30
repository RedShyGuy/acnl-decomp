#pragma once

#include "imgdb/FileReader.h"

namespace imgdb {

// RTTI N5imgdb15PhotoFileReaderE
// vtable 0x00908F40 (vptr 0x00908F48), 4 entries
// a FileReader for whole images (up to the default limit)
class PhotoFileReader : public FileReader
{
public:
    explicit PhotoFileReader(Allocator& allocator)
        : FileReader(allocator, s_DefaultLimitFileSize, false), m_LimitFileSize(s_DefaultLimitFileSize)
    {
    }
    virtual ~PhotoFileReader();

    static void SetDefaultLimitFileSize(u32 size);

protected:
    u32 m_LimitFileSize;  // +0x30

    static bool s_IsDefaultLimitFileSizeSet;
    static u32 s_DefaultLimitFileSize;
};
ASSERT_SIZE(PhotoFileReader, 0x38);  // (8 byte aligned; derived classes use the 4 bytes after m_LimitFileSize)

} // namespace imgdb
