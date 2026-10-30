#pragma once

#include "imgdb/ImageInfo.h"
#include "imgdb/imgdb_Internal.h"
#include "imgdb/imgdb_Types.h"

namespace imgdb {

// an image file of the collection of ImageCollector (names are ours)
class ImageCollectionTableRecord
{
public:
    enum State : u8
    {
        STATE_EMPTY = 0,
        STATE_KNOWN = 1,             // to be checked by the cleanup
        STATE_NEW = 2,               // found by the collection
        STATE_DELETED = 3,
        STATE_REPLACED_BY_JPEG = 4,  // the MPO is gone, its JPEG is there
        STATE_REPLACED_BY_MP = 5,    // the JPEG has an MPO now
        STATE_CHANGED = 6,           // another file size
    };

    // in upper case (the length of the string is measured but not used)
    static void CopyStrToBuf(char* buffer, int size, const wchar_t* string);
    // "<root>/<directory>/<file>.<ext>"; the length
    int CreateImagePath(wchar_t* buffer, int size, StorageType storage, bool isLegacy) const;
    // with the extension of another kind
    int CreateImagePathWithChangeExt(wchar_t* buffer, int size, StorageType storage, ImageKind kind,
                                     bool isLegacy) const;

    ImageKind GetImageKind() const
    {
        wchar_t ext[sizeof(m_Ext) + 1];
        detail::CopyBufToStr(ext, m_Ext, sizeof(m_Ext));
        return ImageInfo::GetImageKindFromExt(ext);
    }

    DateTimeSeconds m_DateTime;  // +0x00
    char m_Ext[4];               // +0x08
    char m_FileName[8];          // +0x0C
    char m_DirectoryName[8];     // +0x14
    u32 m_State : 3;             // +0x1C State
    u32 m_IsOfficial : 1;        // a directory of a Nintendo console
    u32 : 28;
    u32 m_FileSize;              // +0x20
};
ASSERT_SIZE(ImageCollectionTableRecord, 0x24);

} // namespace imgdb
