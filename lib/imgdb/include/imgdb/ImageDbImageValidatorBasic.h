#pragma once

#include "imgdb/imgdb_Types.h"

namespace imgdb {

class ImageInfo;

// checks that an image file of a storage can be shown: its thumbnail or the image must fit the limits of the
// decoders; the date of the Exif data and the file size (names are ours)
class ImageDbImageValidatorBasic
{
public:
    ImageDbImageValidatorBasic();

    bool ValidateMp(const void* data, u32 size, s64 fileSize);
    bool ValidateJpeg(const void* data, u32 size, s64 fileSize);
    // reads the header of the file
    bool ValidateImage(StorageType storage, const ImageInfo& info, bool isLegacy);

    bool m_IsValid;              // +0x00
    DateTimeSeconds m_DateTime;  // +0x08
    s64 m_FileSize;              // +0x10
    bool m_HasThumbnail;         // +0x18
};
ASSERT_SIZE(ImageDbImageValidatorBasic, 0x20);

} // namespace imgdb
