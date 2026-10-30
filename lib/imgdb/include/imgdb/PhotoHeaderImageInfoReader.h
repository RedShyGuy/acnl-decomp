#pragma once

#include "imgdb/ImageInfo.h"
#include "imgdb/PhotoHeaderFileReader.h"

namespace imgdb {

// RTTI N5imgdb26PhotoHeaderImageInfoReaderE
// vtable 0x00909070 (vptr 0x00909078), 4 entries
// reads the start of the image file of an ImageInfo
class PhotoHeaderImageInfoReader : private PhotoHeaderFileReader
{
public:
    PhotoHeaderImageInfoReader(Allocator& allocator, StorageType storage, const ImageInfo& info, bool isLegacy);
    virtual ~PhotoHeaderImageInfoReader();

    Result ReadImage();

    using FileReader::GetData;
    using FileReader::GetDataSize;
    using FileReader::GetFileSize;

private:
    u8 m_Storage;                 // +0x34 StorageType
    const ImageInfo* m_pInfo;     // +0x38
    bool m_IsLegacy;              // +0x3C
};
ASSERT_SIZE(PhotoHeaderImageInfoReader, 0x40);

} // namespace imgdb
