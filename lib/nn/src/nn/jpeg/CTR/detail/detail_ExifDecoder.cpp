// The decoder's Exif (APP1) and MP (APP2) segments.

#include <string.h>
#include "nn/jpeg/CTR/detail/detail_Api.h"
#include "nn/jpeg/CTR/detail/jpeg_DecoderWork.h"

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail {
namespace {
const s8 ERROR_APP1 = -30;
const s8 ERROR_THUMBNAIL = -31;
const s8 ERROR_APP2 = -32;

// the slots of JpegMpDecoderContext::m_Tags (see the tables below)
const u32 SLOT_SOFTWARE = 1;
const u32 SLOT_DATE_TIME = 2;
const u32 SLOT_EXIF_IFD = 3;
const u32 SLOT_GPS_IFD = 4;
const u32 SLOT_MAKER_NOTE = 5;
const u32 SLOT_INTEROPERABILITY_IFD = 6;
const u32 SLOT_NINTENDO_MAKER_NOTE = 8;
const u32 SLOT_USER_MAKER_NOTE = 11;
const u32 SLOT_THUMBNAIL_OFFSET = 15;
const u32 SLOT_THUMBNAIL_LENGTH = 16;
const u32 NINTENDO_MAKER_NOTE_COUNT = 7;
const u32 USER_MAKER_NOTE_COUNT = 4;
const u32 DATE_TIME_SIZE = 20;

// the size of a value of each type, as a shift
// 0x008B3FEC
const u8 s_TagTypeSizeShift[11] = {0, 0, 0, 1, 2, 3, 0, 0, 0, 2, 3};

// the start of an MP APP2 segment and the TIFF headers
// 0x008B3FBC
const u8 s_MpFormatIdentifier[4] = {'M', 'P', 'F', 0};
// 0x008B3FC0
const u8 s_TiffHeaderLittle[4] = {'I', 'I', 0x2A, 0};
// 0x008B3FC4
const u8 s_TiffHeaderBig[4] = {'M', 'M', 0, 0x2A};
// the start of an Exif APP1 segment with the 0th IFD right behind the TIFF header
// 0x008B3FD0
const u8 s_ExifHeaderLittle[14] = {'E', 'x', 'i', 'f', 0, 0, 'I', 'I', 0x2A, 0, 8, 0, 0, 0};
// 0x008B3FDE
const u8 s_ExifHeaderBig[14] = {'E', 'x', 'i', 'f', 0, 0, 'M', 'M', 0, 0x2A, 0, 0, 0, 8};

// the tags that are read, by IFD: {slot, type, tag}
// 1st IFD: JPEGInterchangeFormat, JPEGInterchangeFormatLength
// 0x008B3FC8
const JpegMpDecoderExifTagITN s_ThumbnailIfdItn[] = {
    {15, 4, 0x0201},
    {16, 4, 0x0202},
};
// 0th IFD: Orientation, Software, DateTime, Exif IFD, GPS IFD
// 0x008B3FF8
const JpegMpDecoderExifTagITN s_PrimaryIfdItn[] = {
    {0, 3, 0x0112}, {1, 2, 0x0131}, {2, 2, 0x0132}, {3, 4, 0x8769}, {4, 4, 0x8825},
};
// Exif IFD: MakerNote, Interoperability IFD, ImageUniqueID
// 0x008B400C
const JpegMpDecoderExifTagITN s_ExifIfdItn[] = {
    {5, 7, 0x927C},
    {6, 4, 0xA005},
    {7, 2, 0xA420},
};
// the IFD of the maker note: Nintendo's data and the user maker notes
// 0x008B4018
const JpegMpDecoderExifTagITN s_MakerNoteItn[] = {
    {8, 7, 0x1000}, {9, 7, 0x1001}, {10, 7, 0x1002}, {11, 7, 0x1100}, {12, 7, 0x1101}, {13, 7, 0x1102}, {14, 7, 0x1103},
};
// GPS IFD: GPSVersionID .. GPSDifferential
// 0x008B4034
const JpegMpDecoderExifTagITN s_GpsIfdItn[] = {
    {17, 1, 0x00}, {18, 2, 0x01}, {19, 5, 0x02}, {20, 2, 0x03}, {21, 5, 0x04}, {22, 1, 0x05}, {23, 5, 0x06}, {24, 5, 0x07},
    {25, 2, 0x08}, {26, 2, 0x09}, {27, 2, 0x0A}, {28, 5, 0x0B}, {29, 2, 0x0C}, {30, 5, 0x0D}, {31, 2, 0x0E}, {32, 5, 0x0F},
    {33, 2, 0x10}, {34, 5, 0x11}, {35, 2, 0x12}, {36, 2, 0x13}, {37, 5, 0x14}, {38, 2, 0x15}, {39, 5, 0x16}, {40, 2, 0x17},
    {41, 5, 0x18}, {42, 2, 0x19}, {43, 5, 0x1A}, {44, 7, 0x1B}, {45, 7, 0x1C}, {46, 2, 0x1D}, {47, 3, 0x1E},
};
// MP index IFD: MPFVersion, NumberOfImages, MPEntry, ImageUIDList, TotalFrames
// 0x008B40B0
const JpegMpDecoderExifTagITN s_MpIndexIfdItn[] = {
    {0, 7, 0xB000}, {1, 4, 0xB001}, {2, 7, 0xB002}, {3, 7, 0xB003}, {4, 4, 0xB004},
};
// MP attribute IFD: MPFVersion, MPIndividualNum, PanOrientation .. AxisDistanceZ
// 0x008B40C4
const JpegMpDecoderExifTagITN s_MpAttributeIfdItn[] = {
    {0, 7, 0xB000}, {1, 4, 0xB101}, {2, 4, 0xB201}, {3, 5, 0xB202}, {4, 5, 0xB203},
    {5, 4, 0xB204}, {6, 10, 0xB205}, {7, 5, 0xB206}, {8, 10, 0xB207}, {9, 10, 0xB208},
    {10, 10, 0xB209}, {11, 10, 0xB20A}, {12, 10, 0xB20B}, {13, 10, 0xB20C}, {14, 10, 0xB20D},
};

inline bool SetError(JpegMpDecoderContext* context, s8 error)
{
    if (context->m_Error == 0) {
        context->m_Error = error;
    }
    return false;
}

// an IFD at an offset: its count must be inside the data
inline bool IsIfdInRange(const u8* ifd, const u8* tiffHeader, const u8* end)
{
    return ifd > tiffHeader && end > ifd && end > ifd + 2;
}

// the entries of an APP1 IFD and their count; the offset of the next IFD follows them
DECOMP_ALWAYS_INLINE bool GetIfdEntries(const u8** entries, u32* count, const u8* ifd, const u8* tiffHeader, const u8* end, bool isLittleEndian)
{
    const u32 n = ReadU16(ifd, isLittleEndian);
    if (n >= 0x10000) {
        return false;
    }
    const u8* next = ifd + 2 + n * 12 + 4;
    if (tiffHeader >= next || end < next) {
        return false;
    }
    *entries = ifd + 2;
    *count = n;
    return true;
}

// reads the known tags of an IFD into the slots; entry is left behind the entries
DECOMP_ALWAYS_INLINE bool ReadIfd(JpegTagWorkObj* tags, const u8*& entry, u32 count, const u8* tiffHeader, const u8* end, bool isLittleEndian,
                    const JpegMpDecoderExifTagITN* itn, u32 itnCount)
{
    for (; count != 0; count--) {
        JpegTagWorkObj tag;
        if (!GetTag(&tag, entry, tiffHeader, end, isLittleEndian)) {
            return false;
        }
        if (!CopyTagWorkByITN(tags, &tag, itn, itnCount)) {
            return false;
        }
        entry += 12;
    }
    return true;
}

DECOMP_ALWAYS_INLINE void ReadRational(u32* value, const u8* p, bool isLittleEndian)
{
    value[0] = ReadU32(p, isLittleEndian);
    value[1] = ReadU32(p + 4, isLittleEndian);
}
} // namespace

// 0x00472908 | nintendogs:bytes-fuzzy [tier B]
bool DecodeJpegApp1(JpegMpDecoderContext* context, u32 size)
{
    JpegTagWorkObj* tags = context->m_Tags;
    // the segment from its length
    const u8* segment = context->m_Src + context->m_Position;
    const u8* header = segment + 2;
    const u8* end = segment + size;
    const u8* tiffHeader = segment + 8;
    if (size >= context->m_SrcSize || context->m_Position + size >= context->m_SrcSize || context->m_Position + size <= size) {
        return SetError(context, ERROR_APP1);
    }
    if (header > end) {
        return SetError(context, ERROR_APP1);
    }
    // only the first Exif segment is read
    if (!context->m_HasExif && size > 14 && memcmp(header, s_ExifHeaderLittle, 6) == 0) {
        context->m_HasExif = true;
        if (header + 16 >= end) {
            return SetError(context, ERROR_APP1);
        }
        if (memcmp(header, s_ExifHeaderLittle, sizeof(s_ExifHeaderLittle)) == 0) {
            context->m_IsLittleEndian = true;
        } else if (memcmp(header, s_ExifHeaderBig, sizeof(s_ExifHeaderBig)) == 0) {
            context->m_IsLittleEndian = false;
        } else {
            return SetError(context, ERROR_APP1);
        }
        const bool isLittleEndian = context->m_IsLittleEndian;
        context->m_TiffHeader = tiffHeader;

        const u8* entry;
        u32 count;
        if (!GetIfdEntries(&entry, &count, header + 14, tiffHeader, end, isLittleEndian) ||
            !ReadIfd(tags, entry, count, tiffHeader, end, isLittleEndian, s_PrimaryIfdItn, 5)) {
            return SetError(context, ERROR_APP1);
        }
        if (context->m_IsThumbnail) {
            const u32 next = ReadU32(entry, isLittleEndian);
            if (next == 0) {
                return SetError(context, ERROR_THUMBNAIL);
            }
            const u8* ifd = tiffHeader + next;
            if (!IsIfdInRange(ifd, tiffHeader, end) || !GetIfdEntries(&entry, &count, ifd, tiffHeader, end, isLittleEndian) ||
                !ReadIfd(tags, entry, count, tiffHeader, end, isLittleEndian, s_ThumbnailIfdItn, 2)) {
                return SetError(context, ERROR_APP1);
            }
        }
        if (tags[SLOT_EXIF_IFD].m_IsSet) {
            const u8* ifd = tiffHeader + ReadU32(tags[SLOT_EXIF_IFD].m_Data, isLittleEndian);
            if (!IsIfdInRange(ifd, tiffHeader, end) || !GetIfdEntries(&entry, &count, ifd, tiffHeader, end, isLittleEndian) ||
                !ReadIfd(tags, entry, count, tiffHeader, end, isLittleEndian, s_ExifIfdItn, 3)) {
                return SetError(context, ERROR_APP1);
            }
            // the interoperability IFD is only checked
            if (tags[SLOT_INTEROPERABILITY_IFD].m_IsSet) {
                ifd = tiffHeader + ReadU32(tags[SLOT_INTEROPERABILITY_IFD].m_Data, isLittleEndian);
                if (!IsIfdInRange(ifd, tiffHeader, end) || !GetIfdEntries(&entry, &count, ifd, tiffHeader, end, isLittleEndian)) {
                    return SetError(context, ERROR_APP1);
                }
            }
            // the maker note is an IFD (without a header); a broken one is ignored
            if (tags[SLOT_MAKER_NOTE].m_IsSet) {
                const u8* makerNote = tags[SLOT_MAKER_NOTE].m_Data;
                const u8* makerNoteEnd = makerNote + tags[SLOT_MAKER_NOTE].m_Count;
                if (!IsIfdInRange(makerNote, tiffHeader, makerNoteEnd) ||
                    !GetIfdEntries(&entry, &count, makerNote, tiffHeader, makerNoteEnd, isLittleEndian) ||
                    !ReadIfd(tags, entry, count, tiffHeader, makerNoteEnd, isLittleEndian, s_MakerNoteItn, NINTENDO_MAKER_NOTE_COUNT)) {
                    memset(&tags[SLOT_NINTENDO_MAKER_NOTE], 0, NINTENDO_MAKER_NOTE_COUNT * sizeof(JpegTagWorkObj));
                }
            }
        }
        if (tags[SLOT_GPS_IFD].m_IsSet) {
            const u8* ifd = tiffHeader + ReadU32(tags[SLOT_GPS_IFD].m_Data, isLittleEndian);
            if (!IsIfdInRange(ifd, tiffHeader, end) || !GetIfdEntries(&entry, &count, ifd, tiffHeader, end, isLittleEndian) ||
                !ReadIfd(tags, entry, count, tiffHeader, end, isLittleEndian, s_GpsIfdItn, 31)) {
                return SetError(context, ERROR_APP1);
            }
        }
        if (context->m_IsThumbnail) {
            // decode the thumbnail: the source becomes its JPEG data
            if (!tags[SLOT_THUMBNAIL_OFFSET].m_IsSet || !tags[SLOT_THUMBNAIL_LENGTH].m_IsSet) {
                return SetError(context, ERROR_THUMBNAIL);
            }
            const u32 offset = ReadU32(tags[SLOT_THUMBNAIL_OFFSET].m_Data, isLittleEndian);
            const u8* src = context->m_Src;
            const u32 position = (tiffHeader - src) + offset;
            context->m_Position = position;
            if (offset == 0) {
                return SetError(context, ERROR_THUMBNAIL);
            }
            if (offset >= 0x10000 || offset >= size || offset >= position) {
                return SetError(context, ERROR_THUMBNAIL);
            }
            const u32 length = ReadU32(tags[SLOT_THUMBNAIL_LENGTH].m_Data, isLittleEndian);
            if (length == 0) {
                return SetError(context, ERROR_THUMBNAIL);
            }
            if (length >= 0x10000 || length >= size) {
                return SetError(context, ERROR_THUMBNAIL);
            }
            const u32 thumbnailEnd = position + length;
            if (length >= thumbnailEnd || thumbnailEnd >= context->m_SrcSize) {
                return SetError(context, ERROR_THUMBNAIL);
            }
            context->m_SrcSize = thumbnailEnd;
            context->m_SrcLast = src + thumbnailEnd - 1;
            return true;
        }
    }
    context->m_Position += size;
    return true;
}

// 0x004731F0 | nintendogs:bytes [tier B]
size_t GetApp1DateTime(char* buffer, const JpegMpDecoderContext* context)
{
    const JpegTagWorkObj* tags = context->m_Tags;
    if (!context->m_HasExif || !tags[SLOT_DATE_TIME].m_IsSet || tags[SLOT_DATE_TIME].m_Count != DATE_TIME_SIZE) {
        return 0;
    }
    memcpy(buffer, tags[SLOT_DATE_TIME].m_Data, DATE_TIME_SIZE);
    buffer[DATE_TIME_SIZE - 1] = '\0';
    return DATE_TIME_SIZE;
}

// 0x0047325C | nintendogs:bytes [tier B]
bool CopyTagWorkByITN(JpegTagWorkObj* tags, const JpegTagWorkObj* tag, const JpegMpDecoderExifTagITN* itn, u32 itnCount)
{
    for (; itnCount != 0; itnCount--, itn++) {
        if (itn->m_Tag != tag->m_Tag) {
            continue;
        }
        if (itn->m_Type != tag->m_Type || tags[itn->m_Index].m_IsSet) {
            return false;
        }
        const u32 size = tag->m_Count;
        if (size == 0) {
            return true;
        }
        if (tag->m_Type == 2) {
            // a string must end with its terminator
            if (tag->m_Data[size - 1] != '\0') {
                return true;
            }
        } else if (tag->m_Type == 5 || tag->m_Type == 10) {
            // no rational with a denominator of zero
            for (u32 i = 0; i < size; i += 8) {
                if ((tag->m_Data[i + 4] | tag->m_Data[i + 5] | tag->m_Data[i + 6] | tag->m_Data[i + 7]) == 0) {
                    return true;
                }
            }
        }
        tags[itn->m_Index] = *tag;
        tags[itn->m_Index].m_IsSet = true;
        return true;
    }
    return true;
}

// 0x00473378 | nintendogs:bytes-fuzzy [tier B]
bool DecodeJpegApp2Mp(JpegMpDecoderContext* context, u32 size)
{
    JpegTagWorkObj* tags = context->m_MpTags;
    const u8* segment = context->m_Src + context->m_Position;
    const u8* header = segment + 2;
    const u8* end = segment + size;
    bool isLittleEndian = false;
    if (context->m_Position + size > context->m_SrcSize) {
        return SetError(context, ERROR_APP2);
    }
    memset(tags, 0, sizeof(context->m_MpTags));
    memset(&context->m_MpAttribute, 0, sizeof(context->m_MpAttribute));
    if (header + 4 >= end || memcmp(header, s_MpFormatIdentifier, sizeof(s_MpFormatIdentifier)) != 0) {
        return true;
    }
    // one MP segment per image
    if (context->m_HasMpIndex || context->m_HasMpAttribute) {
        return SetError(context, ERROR_APP2);
    }
    const u8* tiffHeader = header + 4;
    if (context->m_App2 == NULL) {
        context->m_App2 = context->m_Src + context->m_Position - 2;
    }
    if (tiffHeader + 8 >= end) {
        return SetError(context, ERROR_APP2);
    }
    u32 offset;
    if (memcmp(tiffHeader, s_TiffHeaderLittle, sizeof(s_TiffHeaderLittle)) == 0) {
        isLittleEndian = true;
        offset = ReadU32(tiffHeader + 4, true);
    } else if (memcmp(tiffHeader, s_TiffHeaderBig, sizeof(s_TiffHeaderBig)) == 0) {
        offset = ReadU32(tiffHeader + 4, false);
    } else {
        return SetError(context, ERROR_APP2);
    }
    if (offset - 8 >= 0xFFF8) {
        return SetError(context, ERROR_APP2);
    }
    const u8* ifd = tiffHeader + offset;
    if (ifd + 2 >= end) {
        return SetError(context, ERROR_APP2);
    }
    u32 count = ReadU16(ifd, isLittleEndian);
    const u8* entry = ifd + 2;
    if (entry + count * 12 + 4 > end) {
        return SetError(context, ERROR_APP2);
    }
    if (!ReadIfd(tags, entry, count, tiffHeader, end, isLittleEndian, s_MpIndexIfdItn, 5)) {
        return SetError(context, ERROR_APP2);
    }
    const u8* attributeIfd;
    if (!tags[1].m_IsSet && !tags[2].m_IsSet && !tags[3].m_IsSet && !tags[4].m_IsSet) {
        // no index (an image after the first): the IFD is the attribute IFD
        attributeIfd = ifd;
    } else {
        if (!tags[0].m_IsSet || tags[0].m_Count != 4 || !tags[1].m_IsSet || !tags[2].m_IsSet) {
            return SetError(context, ERROR_APP2);
        }
        context->m_HasMpIndex = true;
        if (!context->m_IsMpAttributeWanted) {
            const u8* src = context->m_Src;
            MpIndex* index = &context->m_MpIndex;
            index->m_Src = src;
            index->m_SrcSize = context->m_SrcSize;
            index->m_TiffHeaderOffset = tiffHeader - src;
            index->m_IfdOffset = ifd - src;
            index->m_IsLittleEndian = isLittleEndian;
            index->m_HasIndex = true;
            index->m_Version = *reinterpret_cast<const u32*>(tags[0].m_Data);
            index->m_HasVersion = true;
            index->m_NumberOfImages = ReadU32(tags[1].m_Data, isLittleEndian);
            index->m_EntryOffset = tags[2].m_Data - src;
            index->m_EntrySize = tags[2].m_Count;
            if (tags[3].m_IsSet) {
                index->m_UniqueIdOffset = tags[3].m_Data - src;
                index->m_UniqueIdSize = tags[3].m_Count;
            }
            if (tags[4].m_IsSet) {
                index->m_HasTotalFrames = true;
                index->m_TotalFrames = ReadU32(tags[4].m_Data, isLittleEndian);
            }
            return true;
        }
        // the attribute IFD follows the index IFD
        const u32 next = ReadU32(entry, isLittleEndian);
        if (next == 0) {
            return true;
        }
        attributeIfd = tiffHeader + next;
    }

    memset(tags, 0, sizeof(context->m_MpTags));
    memset(&context->m_MpAttribute, 0, sizeof(context->m_MpAttribute));
    if (end <= attributeIfd + 2) {
        return SetError(context, ERROR_APP2);
    }
    count = ReadU16(attributeIfd, isLittleEndian);
    entry = attributeIfd + 2;
    if (entry + count * 12 + 4 > end) {
        return SetError(context, ERROR_APP2);
    }
    if (!ReadIfd(tags, entry, count, tiffHeader, end, isLittleEndian, s_MpAttributeIfdItn, 15)) {
        return SetError(context, ERROR_APP2);
    }
    context->m_HasMpAttribute = true;
    JpegMpDecoderMpAttribute* attribute = &context->m_MpAttribute;
    if (tags[0].m_IsSet && tags[0].m_Count == 4) {
        attribute->m_Has[0] = true;
        attribute->m_Version = *reinterpret_cast<const u32*>(tags[0].m_Data);
    }
    if (tags[1].m_IsSet) {
        attribute->m_Has[1] = true;
        attribute->m_IndividualNumber = ReadU32(tags[1].m_Data, isLittleEndian);
    }
    if (tags[2].m_IsSet) {
        attribute->m_Has[2] = true;
        attribute->m_PanOrientation = ReadU32(tags[2].m_Data, isLittleEndian);
    }
    if (tags[3].m_IsSet) {
        attribute->m_Has[3] = true;
        ReadRational(attribute->m_TagB202, tags[3].m_Data, isLittleEndian);
    }
    if (tags[4].m_IsSet) {
        attribute->m_Has[4] = true;
        ReadRational(attribute->m_TagB203, tags[4].m_Data, isLittleEndian);
    }
    if (tags[5].m_IsSet) {
        attribute->m_Has[5] = true;
        attribute->m_TagB204 = ReadU32(tags[5].m_Data, isLittleEndian);
    }
    if (tags[6].m_IsSet) {
        attribute->m_Has[6] = true;
        ReadSRational(attribute->m_TagB205, tags[6].m_Data, isLittleEndian);
    }
    if (tags[7].m_IsSet) {
        attribute->m_Has[7] = true;
        ReadRational(attribute->m_TagB206, tags[7].m_Data, isLittleEndian);
    }
    if (tags[8].m_IsSet) {
        attribute->m_Has[8] = true;
        ReadSRational(attribute->m_TagB207, tags[8].m_Data, isLittleEndian);
    }
    if (tags[9].m_IsSet) {
        attribute->m_Has[9] = true;
        ReadSRational(attribute->m_TagB208, tags[9].m_Data, isLittleEndian);
    }
    if (tags[10].m_IsSet) {
        attribute->m_Has[10] = true;
        ReadSRational(attribute->m_TagB209, tags[10].m_Data, isLittleEndian);
    }
    if (tags[11].m_IsSet) {
        attribute->m_Has[11] = true;
        ReadSRational(attribute->m_TagB20A, tags[11].m_Data, isLittleEndian);
    }
    if (tags[12].m_IsSet) {
        attribute->m_Has[12] = true;
        ReadSRational(attribute->m_TagB20B, tags[12].m_Data, isLittleEndian);
    }
    if (tags[13].m_IsSet) {
        attribute->m_Has[13] = true;
        ReadSRational(attribute->m_TagB20C, tags[13].m_Data, isLittleEndian);
    }
    if (tags[14].m_IsSet) {
        attribute->m_Has[14] = true;
        ReadSRational(attribute->m_TagB20D, tags[14].m_Data, isLittleEndian);
    }
    return true;
}

// 0x00475F9C | nintendogs:bytes [tier B]
bool GetApp1SoftwarePointer(App1PointerAndSize* software, const JpegMpDecoderContext* context)
{
    const JpegTagWorkObj* tags = context->m_Tags;
    if (!context->m_HasExif || !tags[SLOT_SOFTWARE].m_IsSet) {
        return false;
    }
    const u32 size = tags[SLOT_SOFTWARE].m_Count;
    const u8* data = tags[SLOT_SOFTWARE].m_Data;
    if (size <= 1 || data[size - 1] != '\0') {
        return false;
    }
    software->m_Pointer = data;
    software->m_Size = size - 1;
    return true;
}

// 0x00475FF0 | nintendogs:bytes [tier B]
bool GetApp1MakerNotePointer(App1PointerAndSize* makerNote, const JpegMpDecoderContext* context, u32 index)
{
    const JpegTagWorkObj* tags = context->m_Tags;
    if (index >= USER_MAKER_NOTE_COUNT || !context->m_HasExif) {
        return false;
    }
    const JpegTagWorkObj* tag = &tags[SLOT_USER_MAKER_NOTE + index];
    if (!tag->m_IsSet) {
        return false;
    }
    makerNote->m_Pointer = tag->m_Data;
    makerNote->m_Size = tag->m_Count;
    return makerNote->m_Size != 0;
}

// 0x0047DB2C (name is ours)
void ReadSRational(s32* value, const u8* p, bool isLittleEndian)
{
    value[0] = ReadU32(p, isLittleEndian);
    value[1] = ReadU32(p + 4, isLittleEndian);
}

// 0x0047DB98 | nintendogs:bytes [tier B]
bool GetTag(JpegTagWorkObj* tag, const u8* entry, const u8* tiffHeader, const u8* end, bool isLittleEndian)
{
    memset(tag, 0, sizeof(*tag));
    tag->m_Tag = ReadU16(entry, isLittleEndian);
    const u32 type = ReadU16(entry + 2, isLittleEndian);
    const u32 count = ReadU32(entry + 4, isLittleEndian);
    const u32 offset = ReadU32(entry + 8, isLittleEndian);
    if (type >= 11) {
        return false;
    }
    tag->m_Type = type;
    if (count >= 0x10000) {
        return false;
    }
    const u32 size = count << s_TagTypeSizeShift[type];
    tag->m_Count = size;
    // up to four bytes are in the entry itself
    const u8* data;
    if (size <= 4) {
        data = entry + 8;
    } else {
        const u32 available = end - tiffHeader;
        if (available <= offset || size > available || size + offset > available) {
            return false;
        }
        data = tiffHeader + offset;
    }
    tag->m_Data = data;
    return true;
}
} // namespace detail
} // namespace CTR
} // namespace jpeg
} // namespace nn
