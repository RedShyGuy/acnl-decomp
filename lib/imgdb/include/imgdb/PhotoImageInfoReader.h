#pragma once

#include "imgdb/ImageInfo.h"
#include "imgdb/PhotoFileReader.h"

namespace imgdb {

// RTTI N5imgdb20PhotoImageInfoReaderE
// vtable 0x00908FC0 (vptr 0x00908FC8), 4 entries
// reads the image file of an ImageInfo
class PhotoImageInfoReader : private PhotoFileReader
{
public:
    PhotoImageInfoReader(Allocator& allocator, StorageType storage, const ImageInfo& info, bool isLegacy);
    virtual ~PhotoImageInfoReader();

    Result ReadImage();

    using FileReader::GetData;
    using FileReader::GetDataSize;
    using FileReader::GetFileSize;

private:
    u8 m_Storage;                 // +0x34 StorageType
    const ImageInfo* m_pInfo;     // +0x38
    bool m_IsLegacy;              // +0x3C
};
ASSERT_SIZE(PhotoImageInfoReader, 0x40);

} // namespace imgdb
