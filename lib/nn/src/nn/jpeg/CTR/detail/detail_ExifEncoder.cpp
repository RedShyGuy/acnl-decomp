#include "nn/jpeg/CTR/detail/detail_Api.h"
#include <stddef.h>
#include <string.h>
#include "nn/jpeg/CTR/detail/jpeg_EncoderWork.h"
#include "nn/jpeg/CTR/jpeg_JpegMpEncoder.h"

// Writing of the Exif (APP1) and MP (APP2) segments of the encoder. The tag tables hold the Exif
// 2.2 and CIPA DC-007 tags; slots, constant values and strings are the original's.

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail {
namespace {
const s8 ERROR_APP1_TOO_LARGE = -4;
const s8 ERROR_APP2_TOO_LARGE = -5;
const s8 ERROR_MP_TYPE = -7;
const s8 ERROR_SHORT_OF_BUFFER = -9;
const s8 ERROR_UNSUPPORTED = -127;

// tag types
const u8 TYPE_BYTE = 1;
const u8 TYPE_ASCII = 2;
const u8 TYPE_SHORT = 3;
const u8 TYPE_LONG = 4;
const u8 TYPE_RATIONAL = 5;
const u8 TYPE_UNDEFINED = 7;
const u8 TYPE_SRATIONAL = 10;

// slots of m_App1Tags (IFD0 0-10, Exif 11-21, maker note 22-28, interoperability 29-31,
// GPS 32-62, IFD1 63-68)
const s32 SLOT_MODEL = 1;
const s32 SLOT_ORIENTATION = 2;
const s32 SLOT_SOFTWARE = 6;
const s32 SLOT_DATE_TIME = 7;
const s32 SLOT_EXIF_IFD = 9;
const s32 SLOT_GPS_IFD = 10;
const s32 SLOT_DATE_TIME_ORIGINAL = 12;
const s32 SLOT_DATE_TIME_DIGITIZED = 13;
const s32 SLOT_MAKER_NOTE = 15;
const s32 SLOT_IMAGE_UNIQUE_ID = 21;
const s32 SLOT_NINTENDO_1000 = 22;
const s32 SLOT_NINTENDO_1001 = 23;
const s32 SLOT_MAKER_NOTE_DATA = 25;
const s32 TAG_COUNT_WITHOUT_THUMBNAIL = 63;

// slots of m_MpTags (index IFD 0-4, attribute IFD 5-19)
const s32 MP_SLOT_ENTRY = 2;
const s32 MP_SLOT_UNIQUE_IDS = 3;
const s32 MP_SLOT_TOTAL_FRAMES = 4;
const s32 MP_SLOT_VERSION = 5;
const s32 MP_TAG_COUNT = 20;

// 0x008B417E
const u16 s_ResolutionUnitInch[1] = { 2 };
// 0x008B4180
const u16 s_YCbCrPositioningCoSited[1] = { 2 };
// 0x008B4182
const u16 s_ColorSpaceSrgb[1] = { 1 };
// 0x008B4184
const u16 s_CompressionJpeg[1] = { 6 };
// 0x008B417C
const u8 s_MpAttributeIfdTagCounts[1] = { 20 };
// 0x008B4186
const u8 s_MpIfdTagCounts[2] = { 5, 20 };
// 0x008B4188
const char s_InteropIndex[4] = "R98";
// 0x008B418C
const char s_ExifVersion[4] = { '0', '2', '2', '0' };
// 0x008B4190
const char s_FlashpixVersion[4] = { '0', '1', '0', '0' };
// 0x008B4194
const char s_InteropVersion[4] = { '0', '1', '0', '0' };
// 0x008B4198
const u8 s_ComponentsConfiguration[4] = { 1, 2, 3, 0 };
// 0x008B419C
const char s_MpVersion[4] = { '0', '1', '0', '0' };
// 0x008B41A0
const char s_CameraSoftware[5] = "JANH";
// 0x008B41A5
const u8 s_App1IfdTagCounts[6] = { 11, 22, 29, 32, 63, 69 };
// 72 dpi
// 0x008B41AC
const u32 s_Resolution[2] = { 72, 1 };
// 0x008B41B4
const char s_Make[9] = "Nintendo";
// 0x008B41BD
const char s_Model[13] = "Nintendo 3DS";
// 0x008B41CA
const char s_CameraModel[11] = "NintendoDS";
// 0x008B41D5
const char s_RelatedImageFileFormat[18] = "JPEG Exif Ver 2.2";
// 0x008B41E7
const u8 s_CameraMakerNote1000[28] = {};
// SOI, APP1 (size to fill in), "Exif" and the TIFF header
// 0x008B4203
const u8 s_App1HeaderLittle[20] = { 0xFF, 0xD8, 0xFF, 0xE1, 0x00, 0x00, 'E', 'x', 'i', 'f', 0, 0, 'I', 'I', 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00 };
// 0x008B4217
const u8 s_App1HeaderBig[20] = { 0xFF, 0xD8, 0xFF, 0xE1, 0x00, 0x00, 'E', 'x', 'i', 'f', 0, 0, 'M', 'M', 0x00, 0x2A, 0x00, 0x00, 0x00, 0x08 };
// the size of a value of each tag type as a shift
// 0x008B422B
const u8 s_TypeSizeShift[11] = { 0, 0, 0, 1, 2, 3, 0, 0, 0, 2, 3 };

// the tags of the GPS IFD and where GpsData has them
struct GpsTagInfo
{
    u16 m_Tag;
    u8 m_Slot;
    u8 m_Type;
    u8 m_Count;         // values (0: any length for strings)
    u8 m_FlagOffset;    // the flag; the pointer of string and undefined tags
    u8 m_DataOffset;    // the value; inline strings; the size of undefined tags
    u8 m_Padding;
};

#define GPS_OFFSET(member) static_cast<u8>(offsetof(GpsData, member))
// 0x008B4236
const GpsTagInfo s_GpsTags[31] = {
    { 0x0, 32, TYPE_BYTE, 4, GPS_OFFSET(m_HasVersionId), GPS_OFFSET(m_VersionId), 0 },
    { 0x1, 33, TYPE_ASCII, 2, 0, GPS_OFFSET(m_LatitudeRef), 0 },
    { 0x2, 34, TYPE_RATIONAL, 3, GPS_OFFSET(m_HasLatitude), GPS_OFFSET(m_Latitude), 0 },
    { 0x3, 35, TYPE_ASCII, 2, 0, GPS_OFFSET(m_LongitudeRef), 0 },
    { 0x4, 36, TYPE_RATIONAL, 3, GPS_OFFSET(m_HasLongitude), GPS_OFFSET(m_Longitude), 0 },
    { 0x5, 37, TYPE_BYTE, 1, GPS_OFFSET(m_HasAltitudeRef), GPS_OFFSET(m_AltitudeRef), 0 },
    { 0x6, 38, TYPE_RATIONAL, 1, GPS_OFFSET(m_HasAltitude), GPS_OFFSET(m_Altitude), 0 },
    { 0x7, 39, TYPE_RATIONAL, 3, GPS_OFFSET(m_HasTimeStamp), GPS_OFFSET(m_TimeStamp), 0 },
    { 0x8, 40, TYPE_ASCII, 0, GPS_OFFSET(m_Satellites), 0, 0 },
    { 0x9, 41, TYPE_ASCII, 2, 0, GPS_OFFSET(m_Status), 0 },
    { 0xA, 42, TYPE_ASCII, 2, 0, GPS_OFFSET(m_MeasureMode), 0 },
    { 0xB, 43, TYPE_RATIONAL, 1, GPS_OFFSET(m_HasDop), GPS_OFFSET(m_Dop), 0 },
    { 0xC, 44, TYPE_ASCII, 2, 0, GPS_OFFSET(m_SpeedRef), 0 },
    { 0xD, 45, TYPE_RATIONAL, 1, GPS_OFFSET(m_HasSpeed), GPS_OFFSET(m_Speed), 0 },
    { 0xE, 46, TYPE_ASCII, 2, 0, GPS_OFFSET(m_TrackRef), 0 },
    { 0xF, 47, TYPE_RATIONAL, 1, GPS_OFFSET(m_HasTrack), GPS_OFFSET(m_Track), 0 },
    { 0x10, 48, TYPE_ASCII, 2, 0, GPS_OFFSET(m_ImgDirectionRef), 0 },
    { 0x11, 49, TYPE_RATIONAL, 1, GPS_OFFSET(m_HasImgDirection), GPS_OFFSET(m_ImgDirection), 0 },
    { 0x12, 50, TYPE_ASCII, 0, GPS_OFFSET(m_MapDatum), 0, 0 },
    { 0x13, 51, TYPE_ASCII, 2, 0, GPS_OFFSET(m_DestLatitudeRef), 0 },
    { 0x14, 52, TYPE_RATIONAL, 3, GPS_OFFSET(m_HasDestLatitude), GPS_OFFSET(m_DestLatitude), 0 },
    { 0x15, 53, TYPE_ASCII, 2, 0, GPS_OFFSET(m_DestLongitudeRef), 0 },
    { 0x16, 54, TYPE_RATIONAL, 3, GPS_OFFSET(m_HasDestLongitude), GPS_OFFSET(m_DestLongitude), 0 },
    { 0x17, 55, TYPE_ASCII, 2, 0, GPS_OFFSET(m_DestBearingRef), 0 },
    { 0x18, 56, TYPE_RATIONAL, 1, GPS_OFFSET(m_HasDestBearing), GPS_OFFSET(m_DestBearing), 0 },
    { 0x19, 57, TYPE_ASCII, 2, 0, GPS_OFFSET(m_DestDistanceRef), 0 },
    { 0x1A, 58, TYPE_RATIONAL, 1, GPS_OFFSET(m_HasDestDistance), GPS_OFFSET(m_DestDistance), 0 },
    { 0x1B, 59, TYPE_UNDEFINED, 0, GPS_OFFSET(m_ProcessingMethod), GPS_OFFSET(m_ProcessingMethodSize), 0 },
    { 0x1C, 60, TYPE_UNDEFINED, 0, GPS_OFFSET(m_AreaInformation), GPS_OFFSET(m_AreaInformationSize), 0 },
    { 0x1D, 61, TYPE_ASCII, 11, GPS_OFFSET(m_DateStamp), 0, 0 },
    { 0x1E, 62, TYPE_SHORT, 1, GPS_OFFSET(m_HasDifferential), GPS_OFFSET(m_Differential), 0 },
};
#undef GPS_OFFSET

// "MPF" and the TIFF header
// 0x008B432E
const u8 s_MpHeaderLittle[12] = { 'M', 'P', 'F', 0, 'I', 'I', 0x2A, 0x00, 0x08, 0x00, 0x00, 0x00 };
// 0x008B433A
const u8 s_MpHeaderBig[12] = { 'M', 'P', 'F', 0, 'M', 'M', 0x00, 0x2A, 0x00, 0x00, 0x00, 0x08 };

inline void SetTag(JpegTagWorkObj* tag, u16 id, u8 type, u32 count, const void* data)
{
    tag->m_Tag = id;
    tag->m_Type = type;
    tag->m_Count = count;
    tag->m_Data = static_cast<const u8*>(data);
}

inline void SetError(JpegMpEncoderWorkObj* work, s8 error)
{
    if (work->m_Context.m_Error == 0) {
        work->m_Context.m_Error = error;
    }
}

inline void WriteU16(u8* p, u32 value, bool isLittleEndian)
{
    if (isLittleEndian) {
        p[0] = value;
        p[1] = value >> 8;
    } else {
        p[0] = value >> 8;
        p[1] = value;
    }
}

inline void WriteU32(u8* p, u32 value, bool isLittleEndian)
{
    if (isLittleEndian) {
        p[0] = value;
        p[1] = value >> 8;
        p[2] = value >> 16;
        p[3] = value >> 24;
    } else {
        p[0] = value >> 24;
        p[1] = value >> 16;
        p[2] = value >> 8;
        p[3] = value;
    }
}
} // namespace

// 0x00472F84 | nintendogs:bytes [tier B]
bool EncodeJpegApp1(JpegMpEncoderWorkObj* work)
{
    JpegMpEncoderContext* context = &work->m_Context;
    u8* const p = context->m_Dst + context->m_DstPosition;
    JpegTagWorkObj* const tags = work->m_App1Tags;
    u32 size;
    if (work->m_IsAddThumbnail) {
        work->m_App1Ifds[0].m_NextOffset = work->m_App1Ifds[5].m_Offset + 8;
        size = work->m_App1Size + work->m_ThumbnailSize;
        work->m_ThumbnailOffset = work->m_App1Size - 8;
    } else {
        size = CalcSegmentSizeCommon(tags, work->m_App1Ifds, s_App1IfdTagCounts, JpegMpEncoderWorkObj::APP1_IFD_COUNT, 0, TAG_COUNT_WITHOUT_THUMBNAIL) + 16;
        if (size == 16) {
            // no tags: only SOI
            if (context->m_DstPosition + 2 >= context->m_DstSize) {
                SetError(work, ERROR_SHORT_OF_BUFFER);
                return false;
            }
            p[0] = 0xFF;
            p[1] = 0xD8;
            context->m_DstPosition = p + 2 - context->m_Dst;
            return true;
        }
    }
    tags[SLOT_MAKER_NOTE].m_Count = work->m_App1Ifds[2].m_Size;
    work->m_ExifIfdOffset = work->m_App1Ifds[1].m_Offset + 8;
    work->m_InteropIfdOffset = work->m_App1Ifds[3].m_Offset + 8;
    work->m_GpsIfdOffset = work->m_App1Ifds[4].m_Offset + 8;
    work->m_PixelWidth = context->m_Width;
    work->m_PixelHeight = context->m_Height;
    if (size >= 0x10000) {
        SetError(work, ERROR_APP1_TOO_LARGE);
        return false;
    }
    const u32 total = size + 4;
    if (context->m_DstPosition + total >= context->m_DstSize) {
        SetError(work, ERROR_SHORT_OF_BUFFER);
        return false;
    }
    memcpy(p, work->m_IsApp1LittleEndian ? s_App1HeaderLittle : s_App1HeaderBig, sizeof(s_App1HeaderLittle));
    u8* end = p + 20 + EncodeSegmentCommon(p + 20, p + 12, work->m_IsApp1LittleEndian, tags, work->m_App1Ifds, s_App1IfdTagCounts, 0,
                                           work->m_IsAddThumbnail ? JpegMpEncoderWorkObj::APP1_TAG_COUNT : TAG_COUNT_WITHOUT_THUMBNAIL, work);
    if (work->m_IsAddThumbnail) {
        end += work->m_ThumbnailSize;
    }
    if (end - p != static_cast<s32>(total)) {
        SetError(work, ERROR_UNSUPPORTED);
        return false;
    }
    p[4] = static_cast<u16>(size) >> 8;
    p[5] = static_cast<u16>(size);
    context->m_DstPosition = p + total - context->m_Dst;
    return true;
}

// 0x00473A90
bool EncodeJpegMpApp2(JpegMpEncoderWorkObj* work, JpegMpEncoderTemporarySettingObj* settings)
{
    JpegMpEncoderContext* context = &work->m_Context;
    u8* const p = context->m_Dst + context->m_DstPosition;
    JpegTagWorkObj* const tags = work->m_MpTags;
    if (work->m_CurrentImage != 0) {
        SetTag(&tags[MP_SLOT_VERSION], 0xB000, TYPE_UNDEFINED, 4, s_MpVersion);
    } else {
        // the first image has the index IFD
        tags[MP_SLOT_VERSION].m_Count = 0;
        if (settings->m_HasTagB204) {
            work->m_BaseViewpointNumber = settings->m_TagB204;
        }
        SetTag(&tags[MP_SLOT_ENTRY], 0xB002, TYPE_UNDEFINED, work->m_ImageCount * 16, work->m_MpEntries);
        if (work->m_HasImageUniqueIds) {
            SetTag(&tags[MP_SLOT_UNIQUE_IDS], 0xB003, TYPE_UNDEFINED, work->m_ImageCount * 33, work->m_ImageUniqueIds);
        }
        if (work->m_HasTotalFrames) {
            SetTag(&tags[MP_SLOT_TOTAL_FRAMES], 0xB004, TYPE_LONG, 4, &work->m_TotalFrames);
        }
    }

    if (settings->m_HasTagB101) {
        work->m_IndividualNumber = settings->m_TagB101;
    }
    SetTag(&tags[6], 0xB101, TYPE_LONG, 4, &work->m_IndividualNumber);
    if (work->m_TypeCode != MP_TYPE_CODE_LARGE_THUMBNAIL_VGA && work->m_TypeCode != MP_TYPE_CODE_LARGE_THUMBNAIL_FULL_HD && work->m_IndividualNumber != 0) {
        ++work->m_TotalFrames;
    }
    if (settings->m_HasTagB201) {
        SetTag(&tags[7], 0xB201, TYPE_LONG, 4, &settings->m_TagB201);
    }
    if (settings->m_HasTagB202) {
        SetTag(&tags[8], 0xB202, TYPE_RATIONAL, 8, settings->m_TagB202);
    }
    if (settings->m_HasTagB203) {
        SetTag(&tags[9], 0xB203, TYPE_RATIONAL, 8, settings->m_TagB203);
    }
    SetTag(&tags[10], 0xB204, TYPE_LONG, 4, &work->m_BaseViewpointNumber);
    if (!settings->m_HasTagB205) {
        settings->m_TagB205[1] = -1;
        settings->m_TagB205[0] = -1;
    }
    SetTag(&tags[11], 0xB205, TYPE_SRATIONAL, 8, settings->m_TagB205);
    if (!settings->m_HasTagB206) {
        settings->m_TagB206[1] = -1;
        settings->m_TagB206[0] = -1;
    }
    SetTag(&tags[12], 0xB206, TYPE_RATIONAL, 8, settings->m_TagB206);
    if (settings->m_HasTagB207) {
        SetTag(&tags[13], 0xB207, TYPE_SRATIONAL, 8, settings->m_TagB207);
    }
    if (settings->m_HasTagB208) {
        SetTag(&tags[14], 0xB208, TYPE_SRATIONAL, 8, settings->m_TagB208);
    }
    if (settings->m_HasTagB209) {
        SetTag(&tags[15], 0xB209, TYPE_SRATIONAL, 8, settings->m_TagB209);
    }
    if (settings->m_HasTagB20A) {
        SetTag(&tags[16], 0xB20A, TYPE_SRATIONAL, 8, settings->m_TagB20A);
    }
    if (settings->m_HasTagB20B) {
        SetTag(&tags[17], 0xB20B, TYPE_SRATIONAL, 8, settings->m_TagB20B);
    }
    if (settings->m_HasTagB20C) {
        SetTag(&tags[18], 0xB20C, TYPE_SRATIONAL, 8, settings->m_TagB20C);
    }
    if (settings->m_HasTagB20D) {
        SetTag(&tags[19], 0xB20D, TYPE_SRATIONAL, 8, settings->m_TagB20D);
    }

    // one image is the representative one
    if (work->m_HasRepresentativeSet) {
        work->m_IsRepresentative = 0;
    } else {
        if (!settings->m_HasRepresentative) {
            settings->m_HasRepresentative = true;
            settings->m_IsRepresentative = true;
        }
        work->m_IsRepresentative = settings->m_IsRepresentative;
        if (settings->m_IsRepresentative) {
            work->m_HasRepresentativeSet = true;
        }
    }
    work->m_IsDependentParent = settings->m_IsDependentParent;
    work->m_IsDependentChild = settings->m_IsDependentChild;
    work->m_DependentImage1 = settings->m_DependentImage1;
    work->m_DependentImage2 = settings->m_DependentImage2;

    // the attribute tags of each image type
    switch (work->m_TypeCode) {
    case MP_TYPE_CODE_MULTI_FRAME_PANORAMA:
        for (s32 i = 19; i >= 10; --i) {
            tags[i].m_Count = 0;
        }
        break;
    case MP_TYPE_CODE_MULTI_FRAME_DISPARITY:
        for (s32 i = 19; i >= 14; --i) {
            tags[i].m_Count = 0;
        }
        for (s32 i = 9; i >= 7; --i) {
            tags[i].m_Count = 0;
        }
        break;
    case MP_TYPE_CODE_MULTI_FRAME_MULTI_ANGLE:
        for (s32 i = 13; i >= 11; --i) {
            tags[i].m_Count = 0;
        }
        for (s32 i = 9; i >= 7; --i) {
            tags[i].m_Count = 0;
        }
        break;
    case MP_TYPE_CODE_BASELINE_PRIMARY:
        for (s32 i = MP_SLOT_VERSION; i < MP_TAG_COUNT; ++i) {
            tags[i].m_Count = 0;
        }
        break;
    case MP_TYPE_CODE_UNDEFINED:
        for (s32 i = 7; i < MP_TAG_COUNT; ++i) {
            tags[i].m_Count = 0;
        }
        break;
    case MP_TYPE_CODE_LARGE_THUMBNAIL_VGA:
    case MP_TYPE_CODE_LARGE_THUMBNAIL_FULL_HD:
        return true;
    default:
        SetError(work, ERROR_MP_TYPE);
        return false;
    }

    u32 size;
    if (work->m_CurrentImage != 0) {
        size = CalcSegmentSizeCommon(tags, &work->m_MpIfds[2], s_MpAttributeIfdTagCounts, 1, MP_SLOT_VERSION, MP_TAG_COUNT);
    } else {
        size = CalcSegmentSizeCommon(tags, work->m_MpIfds, s_MpIfdTagCounts, 2, 0, MP_TAG_COUNT);
        if (!work->m_IsBaselinePrimary) {
            work->m_MpIfds[0].m_NextOffset = work->m_MpIfds[1].m_Offset + 8;
        }
    }
    size += 14;
    if (size >= 0x10000 || size == 0) {
        SetError(work, ERROR_APP2_TOO_LARGE);
        return false;
    }
    const u32 total = size + 2;
    if (context->m_DstPosition + total >= context->m_DstSize) {
        SetError(work, ERROR_SHORT_OF_BUFFER);
        return false;
    }
    context->m_DstPosition += total;
    p[0] = 0xFF;
    p[1] = 0xE2;
    p[2] = static_cast<u16>(size) >> 8;
    p[3] = static_cast<u16>(size);
    if (work->m_CurrentImage == 0) {
        work->m_App2Size = size;
        work->m_App2Start = p;
    }
    memcpy(p + 4, work->m_IsApp2LittleEndian ? s_MpHeaderLittle : s_MpHeaderBig, sizeof(s_MpHeaderLittle));
    u32 written;
    if (work->m_CurrentImage != 0) {
        written = EncodeSegmentCommon(p + 16, p + 8, work->m_IsApp2LittleEndian, tags, &work->m_MpIfds[2], s_MpAttributeIfdTagCounts,
                                      MP_SLOT_VERSION, MP_TAG_COUNT, work);
    } else {
        written = EncodeSegmentCommon(p + 16, p + 8, work->m_IsApp2LittleEndian, tags, work->m_MpIfds, s_MpIfdTagCounts, 0, MP_TAG_COUNT, work);
    }
    if (p + 16 + written - p != static_cast<s32>(total)) {
        SetError(work, ERROR_UNSUPPORTED);
        return false;
    }
    return true;
}

// 0x004741C8 | nintendogs:bytes [tier B]
void PreEncodeJpegApp1(JpegMpEncoderWorkObj* work, JpegMpEncoderTemporarySettingObj* settings, bool isAddThumbnail)
{
    JpegTagWorkObj* const tags = work->m_App1Tags;
    bool hasMakerNote = false;
    bool hasExifTags = false;
    if (!isAddThumbnail && (settings->m_Flags & 0x80000000)) {
        memset(tags, 0, sizeof(work->m_App1Tags));
        return;
    }
    if (!settings->m_HasDateTime) {
        JpegMpEncoder::GetDateTimeNow(settings->m_DateTime);
        settings->m_HasDateTime = true;
    }
    SetTag(&tags[SLOT_DATE_TIME], 0x132, TYPE_ASCII, 20, settings->m_DateTime);
    SetTag(&tags[SLOT_DATE_TIME_ORIGINAL], 0x9003, TYPE_ASCII, 20, settings->m_DateTime);
    SetTag(&tags[SLOT_DATE_TIME_DIGITIZED], 0x9004, TYPE_ASCII, 20, settings->m_DateTime);
    if (settings->m_NintendoData != NULL) {
        SetTag(&tags[SLOT_MODEL], 0x110, TYPE_ASCII, sizeof(s_CameraModel), s_CameraModel);
        SetTag(&tags[SLOT_SOFTWARE], 0x131, TYPE_ASCII, sizeof(s_CameraSoftware), s_CameraSoftware);
        SetTag(&tags[SLOT_NINTENDO_1000], 0x1000, TYPE_UNDEFINED, sizeof(s_CameraMakerNote1000), s_CameraMakerNote1000);
        SetTag(&tags[SLOT_NINTENDO_1001], 0x1001, TYPE_UNDEFINED, 8, settings->m_NintendoData);
        hasMakerNote = true;
    } else if (settings->m_Software != NULL) {
        const size_t length = strlen(settings->m_Software);
        if (length != 0 && length < 0x10000) {
            SetTag(&tags[SLOT_SOFTWARE], 0x131, TYPE_ASCII, length + 1, settings->m_Software);
        }
    }
    for (u32 i = 0; i < 4; ++i) {
        if (settings->m_MakerNotes[i].m_Size != 0) {
            SetTag(&tags[SLOT_MAKER_NOTE_DATA + i], 0x1100 + i, TYPE_UNDEFINED, settings->m_MakerNotes[i].m_Size, settings->m_MakerNotes[i].m_Data);
            hasMakerNote = true;
        }
    }
    if (hasMakerNote) {
        // the maker note is an IFD; EncodeSegmentCommon writes it in place
        SetTag(&tags[SLOT_MAKER_NOTE], 0x927C, TYPE_UNDEFINED, 1, NULL);
        tags[SLOT_MAKER_NOTE].m_IsSet = true;
    }
    if (settings->m_HasImageUniqueId) {
        tags[SLOT_IMAGE_UNIQUE_ID].m_Count = 33;
        tags[SLOT_IMAGE_UNIQUE_ID].m_Tag = 0xA420;
        tags[SLOT_IMAGE_UNIQUE_ID].m_Type = TYPE_ASCII;
        tags[SLOT_IMAGE_UNIQUE_ID].m_Data = reinterpret_cast<const u8*>(settings->m_ImageUniqueId);
    }
    if (settings->m_HasOrientation) {
        SetTag(&tags[SLOT_ORIENTATION], 0x112, TYPE_SHORT, 2, &settings->m_Orientation);
    }
    if (settings->m_GpsData != NULL && PreEncodeJpegApp1GpsData(tags, settings->m_GpsData)) {
        SetTag(&tags[SLOT_GPS_IFD], 0x8825, TYPE_LONG, 4, &work->m_GpsIfdOffset);
    }
    if (!work->m_IsMp) {
        return;
    }
    if (work->m_TypeCode != MP_TYPE_CODE_LARGE_THUMBNAIL_VGA && work->m_TypeCode != MP_TYPE_CODE_LARGE_THUMBNAIL_FULL_HD) {
        return;
    }
    // large thumbnails keep only the maker note, the unique ID and the sizes
    for (s32 i = 0; i < JpegMpEncoderWorkObj::APP1_TAG_COUNT; ++i) {
        if ((i >= SLOT_NINTENDO_1000 && i < SLOT_NINTENDO_1000 + 7) || i == SLOT_MAKER_NOTE || i == SLOT_IMAGE_UNIQUE_ID) {
            if (tags[i].m_Count != 0) {
                hasExifTags = true;
            }
        } else if (i == SLOT_EXIF_IFD) {
            continue;
        } else if (i == 67 || i == 68) {
            if (work->m_IsAddThumbnail) {
                hasExifTags = true;
            }
        } else if ((i == 18 || i == 19) && !work->m_IsLastImage) {
            hasExifTags = true;
        } else {
            tags[i].m_Count = 0;
        }
    }
    if (!hasExifTags) {
        tags[SLOT_EXIF_IFD].m_Count = 0;
    }
}

// 0x00475618
u32 EncodeSegmentCommon(u8* dst, const u8* tiffHeader, bool isLittleEndian, const JpegTagWorkObj* tags, const JpegMpEncoderIfdWorkObj* ifds,
                        const u8* tagCounts, u32 index, u32 endIndex, JpegMpEncoderWorkObj* work)
{
    u8* p = dst;
    u32 ifd = 0;
    const JpegTagWorkObj* tag = &tags[index];
    u32 ifdEnd = tagCounts[0];
    work->m_ThumbnailData = NULL;
    while (index < endIndex) {
        const JpegMpEncoderIfdWorkObj* current = &ifds[ifd];
        const u16 entryCount = current->m_EntryCount;
        if (entryCount == 0) {
            // an empty IFD is left out
            while (index != ifdEnd) {
                ++index;
                ++tag;
            }
        } else {
            WriteU16(p, entryCount, isLittleEndian);
            // the values follow the entries and the offset of the next IFD
            u8* data = p + entryCount * 12 + 6;
            p += 2;
            for (; index != ifdEnd; ++index, ++tag) {
                const u32 count = tag->m_Count;
                if (count == 0) {
                    continue;
                }
                const u8* value = tag->m_Data;
                WriteU16(p, tag->m_Tag, isLittleEndian);
                WriteU16(p + 2, tag->m_Type, isLittleEndian);
                WriteU32(p + 4, count >> s_TypeSizeShift[tag->m_Type], isLittleEndian);
                if (count <= 4) {
                    // the value is in the entry
                    if (!isLittleEndian && tag->m_Type == TYPE_SHORT) {
                        p[8] = value[1];
                        p[9] = value[0];
                        if (count == 4) {
                            p[10] = value[3];
                            p[11] = value[2];
                        } else {
                            p[10] = 0;
                            p[11] = 0;
                        }
                    } else if (!isLittleEndian && (tag->m_Type == TYPE_LONG || tag->m_Type == 9)) {
                        WriteU32(p + 8, *reinterpret_cast<const u32*>(value), false);
                    } else {
                        p[11] = 0;
                        p[10] = 0;
                        p[9] = 0;
                        for (s32 i = count - 1; i >= 0; --i) {
                            p[8 + i] = value[i];
                        }
                    }
                } else {
                    const u32 offset = data - tiffHeader;
                    WriteU32(p + 8, offset, isLittleEndian);
                    u8* const dataEnd = data + count;
                    if (tag->m_IsSet) {
                        // an IFD written by itself (the maker note): its offset moves behind the image unique ID
                        for (u32 i = 1; i < ifdEnd - index; ++i) {
                            if (tag[i].m_Tag == 0xA420) {
                                const u32 uniqueIdSize = tag[i].m_Count + (tag[i].m_Count & 1);
                                WriteU32(p + 8, uniqueIdSize + offset, isLittleEndian);
                            }
                        }
                    } else if (!isLittleEndian && tag->m_Type == TYPE_SHORT) {
                        do {
                            const u16 v = *reinterpret_cast<const u16*>(value);
                            value += 2;
                            data[0] = v >> 8;
                            data[1] = v;
                            data += 2;
                        } while (data < dataEnd);
                    } else if (!isLittleEndian && (tag->m_Type == TYPE_LONG || tag->m_Type == TYPE_RATIONAL || tag->m_Type == 9 || tag->m_Type == TYPE_SRATIONAL)) {
                        do {
                            WriteU32(data, *reinterpret_cast<const u32*>(value), false);
                            value += 4;
                            data += 4;
                        } while (data < dataEnd);
                    } else {
                        // the thumbnail of IFD1 is placed by StartJpegEncoderCore
                        if (!work->m_IsMp && index == 22) {
                            work->m_ThumbnailData = data;
                        }
                        do {
                            *data++ = *value++;
                        } while (data < dataEnd);
                        if (count & 1) {
                            *data++ = 0;
                        }
                    }
                }
                p += 12;
            }
            WriteU32(p, current->m_NextOffset, isLittleEndian);
            p = data;
        }
        ++ifd;
        ifdEnd = tagCounts[ifd];
    }
    return p - dst;
}

// 0x00475BD4
bool PostEncodeJpegMpApp2(JpegMpEncoderWorkObj* work, bool isAbort)
{
    const bool isLittleEndian = work->m_IsApp2LittleEndian;
    const u32 current = work->m_CurrentImage;
    if (work->m_ImageCount <= current) {
        return true;
    }
    if (isAbort) {
        SetError(work, ERROR_UNSUPPORTED);
        return false;
    }
    // the MP entry of this image
    const u8* const tiffHeader = work->m_App2Start + 8;
    u32 attribute = work->m_TypeCode | (work->m_IsDependentParent ? 0x80000000 : 0)
        | ((work->m_IsDependentChild ? 0x40000000 : 0) | (work->m_IsRepresentative ? 0x20000000 : 0));
    u8* entry = reinterpret_cast<u8*>(&work->m_MpEntries[current]);
    WriteU32(entry, attribute, isLittleEndian);
    WriteU32(entry + 4, work->m_Context.m_DstPosition, isLittleEndian);
    WriteU32(entry + 8, current != 0 ? work->m_Context.m_Dst - tiffHeader : 0, isLittleEndian);
    WriteU16(entry + 12, work->m_DependentImage1, isLittleEndian);
    WriteU16(entry + 14, work->m_DependentImage2, isLittleEndian);
    if (work->m_HasImageUniqueIds && current != 0) {
        char* id = &work->m_ImageUniqueIds[current * 33];
        if (work->m_App1Tags[SLOT_IMAGE_UNIQUE_ID].m_Count == 33) {
            memcpy(id, work->m_App1Tags[SLOT_IMAGE_UNIQUE_ID].m_Data, 33);
        } else {
            memset(id, 0, 33);
        }
    }
    ++work->m_CurrentImage;
    InitializeJpegMpEncoderApp2AttributeTagWork(work);
    if (work->m_CurrentImage < work->m_ImageCount) {
        return true;
    }

    // all images are written: the index IFD of the first one
    work->m_NumberOfImages = work->m_ImageCount;
    const u32 written = EncodeSegmentCommon(work->m_App2Start + 16, work->m_App2Start + 8, isLittleEndian, work->m_MpTags, work->m_MpIfds,
                                            s_MpIfdTagCounts, 0, MP_SLOT_VERSION, work);
    if (work->m_App2Size < written) {
        SetError(work, ERROR_UNSUPPORTED);
        return false;
    }
    const u32 typeCode = work->m_TypeCode;
    memset(&work->m_TypeCode, 0, offsetof(JpegMpEncoderWorkObj, m_JpegStart) - offsetof(JpegMpEncoderWorkObj, m_TypeCode));
    work->m_TypeCode = typeCode;
    InitializeJpegMpEncoderApp2IndexTagWork(work);
    return true;
}

// 0x00475ED4 | nintendogs:bytes [tier B]
u32 CalcSegmentSizeCommon(JpegTagWorkObj* tags, JpegMpEncoderIfdWorkObj* ifds, const u8* tagCounts, u32 ifdCount, u32 index, u32 endIndex)
{
    u32 size = 0;
    u32 entryCount = 0;
    u32 ifdStart = 0;
    u32 ifd = 0;
    u32 ifdEnd = tagCounts[0];
    memset(ifds, 0, ifdCount * sizeof(JpegMpEncoderIfdWorkObj));
    const JpegTagWorkObj* tag = &tags[index];
    while (true) {
        if (index == ifdEnd) {
            // entry count and the offset of the next IFD
            if (entryCount != 0) {
                size += 6;
            }
            ifds[ifd].m_EntryCount = entryCount;
            ifds[ifd].m_Offset = ifdStart;
            ifds[ifd].m_Size = size - ifdStart;
            ++ifd;
            ifdEnd = tagCounts[ifd];
            entryCount = 0;
            ifdStart = size;
        }
        if (index == endIndex) {
            break;
        }
        const u32 count = tag->m_Count;
        if (count != 0) {
            size += 12;
            if (!tag->m_IsSet && count > 4) {
                size += count + (count & 1);
            }
            ++entryCount;
        }
        ++index;
        ++tag;
    }
    return size;
}

// 0x00476048 | nintendogs:bytes-fuzzy [tier B]
bool PreEncodeJpegApp1GpsData(JpegTagWorkObj* tags, const GpsData* gps)
{
    const u8* const base = reinterpret_cast<const u8*>(gps);
    bool isSet = false;
    const GpsTagInfo* info = s_GpsTags;
    for (s32 i = 31; i != 0; --i, ++info) {
        JpegTagWorkObj* tag = &tags[info->m_Slot];
        switch (info->m_Type) {
        case TYPE_BYTE:
        case TYPE_SHORT:
        case TYPE_RATIONAL:
            if (base[info->m_FlagOffset] == 0) {
                continue;
            }
            if (info->m_Type == TYPE_BYTE) {
                SetTag(tag, info->m_Tag, TYPE_BYTE, info->m_Count, base + info->m_DataOffset);
            } else if (info->m_Type == TYPE_SHORT) {
                SetTag(tag, info->m_Tag, TYPE_SHORT, info->m_Count * 2, base + info->m_DataOffset);
            } else {
                SetTag(tag, info->m_Tag, TYPE_RATIONAL, info->m_Count * 8, base + info->m_DataOffset);
            }
            break;
        case TYPE_ASCII: {
            const char* string;
            if (info->m_DataOffset != 0) {
                string = reinterpret_cast<const char*>(base + info->m_DataOffset);
                if (*string == '\0') {
                    continue;
                }
            } else {
                string = *reinterpret_cast<const char* const*>(base + info->m_FlagOffset);
                if (string == NULL) {
                    continue;
                }
            }
            const size_t length = strlen(string);
            if (length == 0 || length >= 0x10000) {
                continue;
            }
            if (info->m_Count != 0 && info->m_Count != length + 1) {
                continue;
            }
            SetTag(tag, info->m_Tag, TYPE_ASCII, length + 1, string);
            break;
        }
        case TYPE_UNDEFINED: {
            const u8* data = *reinterpret_cast<const u8* const*>(base + info->m_FlagOffset);
            const u32 size = *reinterpret_cast<const u32*>(base + info->m_DataOffset);
            if (data == NULL || size == 0) {
                continue;
            }
            SetTag(tag, info->m_Tag, TYPE_UNDEFINED, size, data);
            break;
        }
        default:
            return false;
        }
        isSet = true;
    }
    return isSet;
}

// 0x00476244 | nintendogs:bytes [tier B]
u32 CalcJpegMpEncoderApp1Size(JpegMpEncoderWorkObj* work)
{
    return CalcSegmentSizeCommon(work->m_App1Tags, work->m_App1Ifds, s_App1IfdTagCounts, JpegMpEncoderWorkObj::APP1_IFD_COUNT, 0,
                                 work->m_IsAddThumbnail ? JpegMpEncoderWorkObj::APP1_TAG_COUNT : TAG_COUNT_WITHOUT_THUMBNAIL) + 16;
}

// 0x0047AC50 | nintendogs:bytes [tier B]
void InitializeJpegMpEncoderApp1TagWork(JpegMpEncoderWorkObj* work)
{
    JpegTagWorkObj* const tags = work->m_App1Tags;
    memset(tags, 0, sizeof(work->m_App1Tags));
    // IFD0
    SetTag(&tags[0], 0x10F, TYPE_ASCII, sizeof(s_Make), s_Make);
    SetTag(&tags[1], 0x110, TYPE_ASCII, sizeof(s_Model), s_Model);
    SetTag(&tags[3], 0x11A, TYPE_RATIONAL, 8, s_Resolution);
    SetTag(&tags[4], 0x11B, TYPE_RATIONAL, 8, s_Resolution);
    SetTag(&tags[5], 0x128, TYPE_SHORT, 2, s_ResolutionUnitInch);
    SetTag(&tags[8], 0x213, TYPE_SHORT, 2, s_YCbCrPositioningCoSited);
    SetTag(&tags[SLOT_EXIF_IFD], 0x8769, TYPE_LONG, 4, &work->m_ExifIfdOffset);
    // Exif IFD
    SetTag(&tags[11], 0x9000, TYPE_UNDEFINED, 4, s_ExifVersion);
    SetTag(&tags[14], 0x9101, TYPE_UNDEFINED, 4, s_ComponentsConfiguration);
    SetTag(&tags[16], 0xA000, TYPE_UNDEFINED, 4, s_FlashpixVersion);
    SetTag(&tags[17], 0xA001, TYPE_SHORT, 2, s_ColorSpaceSrgb);
    SetTag(&tags[18], 0xA002, TYPE_LONG, 4, &work->m_PixelWidth);
    SetTag(&tags[19], 0xA003, TYPE_LONG, 4, &work->m_PixelHeight);
    SetTag(&tags[20], 0xA005, TYPE_LONG, 4, &work->m_InteropIfdOffset);
    // interoperability IFD
    SetTag(&tags[29], 0x1, TYPE_ASCII, 4, s_InteropIndex);
    SetTag(&tags[30], 0x2, TYPE_UNDEFINED, 4, s_InteropVersion);
    SetTag(&tags[31], 0x1000, TYPE_ASCII, sizeof(s_RelatedImageFileFormat), s_RelatedImageFileFormat);
    // IFD1 (the thumbnail)
    SetTag(&tags[63], 0x103, TYPE_SHORT, 2, s_CompressionJpeg);
    SetTag(&tags[64], 0x11A, TYPE_RATIONAL, 8, s_Resolution);
    SetTag(&tags[65], 0x11B, TYPE_RATIONAL, 8, s_Resolution);
    SetTag(&tags[66], 0x128, TYPE_SHORT, 2, s_ResolutionUnitInch);
    SetTag(&tags[67], 0x201, TYPE_LONG, 4, &work->m_ThumbnailOffset);
    SetTag(&tags[68], 0x202, TYPE_LONG, 4, &work->m_ThumbnailSize);
}

// 0x0047DAD0 | nintendogs:bytes [tier B]
void InitializeJpegMpEncoderApp2IndexTagWork(JpegMpEncoderWorkObj* work)
{
    work->m_BaseViewpointNumber = 1;
    SetTag(&work->m_MpTags[0], 0xB000, TYPE_UNDEFINED, 4, s_MpVersion);
    SetTag(&work->m_MpTags[1], 0xB001, TYPE_LONG, 4, &work->m_NumberOfImages);
}

// 0x0047DB74 | nintendogs:bytes [tier B]
void InitializeJpegMpEncoderApp2AttributeTagWork(JpegMpEncoderWorkObj* work)
{
    memset(&work->m_MpTags[MP_SLOT_VERSION], 0, sizeof(JpegTagWorkObj) * (MP_TAG_COUNT - MP_SLOT_VERSION));
    work->m_IndividualNumber = work->m_CurrentImage + 1;
}

} // namespace detail
} // namespace CTR
} // namespace jpeg
} // namespace nn
