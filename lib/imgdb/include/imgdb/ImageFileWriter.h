#pragma once

#include "imgdb/FileWriter.h"
#include "imgdb/ImageInfo.h"

namespace imgdb {

// RTTI N5imgdb15ImageFileWriterE
// vtable 0x00908F1C (vptr 0x00908F24), 3 entries
// writes the image file of an ImageInfo (and its directory)
class ImageFileWriter : private FileWriter
{
public:
    ImageFileWriter(Allocator& allocator, StorageType storage, const ImageInfo& info, bool isLegacy, const void* data,
                    u32 size);
    virtual ~ImageFileWriter();

    Result WriteImage();

private:
    u8 m_Storage;              // +0x10 StorageType
    const ImageInfo* m_pInfo;  // +0x14
    bool m_IsLegacy;           // +0x18
    const void* m_pData;       // +0x1C
    u32 m_Size;                // +0x20
};
ASSERT_SIZE(ImageFileWriter, 0x24);

} // namespace imgdb
