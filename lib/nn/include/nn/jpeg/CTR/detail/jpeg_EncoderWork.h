#pragma once

#include "decomp.h"
#include "nn/jpeg/CTR/jpeg_Types.h"

// The work memory of JpegMpEncoder and the settings that are only kept until the next encode.
// Member names are ours.

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail {
struct JpegMpEncoderTemporarySettingObj
{
    struct MakerNote
    {
        const u8* m_Data;
        u32 m_Size;
    };

    u16 m_DependentImage1;          // 0x00, MP entry of the next image
    u16 m_DependentImage2;          // 0x02
    u16 m_Orientation;              // 0x04
    bool m_HasOrientation;          // 0x06
    u32 m_ThumbnailWidth;           // 0x08
    u32 m_ThumbnailHeight;          // 0x0C
    u32 m_Stride;                   // 0x10, pixels per line of the source
    u32 m_Flags;                    // 0x14, bit 31 no Exif, bit 30 no Huffman table
    MakerNote m_MakerNotes[4];      // 0x18
    const u8* m_NintendoData;       // 0x38, 8 bytes for tag 0x1001; also selects the camera limits
    const char* m_Software;         // 0x3C
    const GpsData* m_GpsData;       // 0x40
    char m_DateTime[20];            // 0x44, "YYYY:MM:DD hh:mm:ss"
    char m_ImageUniqueId[33];       // 0x58
    bool m_HasImageUniqueId;        // 0x79
    bool m_HasDateTime;             // 0x7A
    u8 m_ThumbnailSampling;         // 0x7B
    bool m_IsDependentParent;       // 0x7C
    bool m_IsDependentChild;        // 0x7D
    bool m_HasRepresentative;       // 0x7E
    bool m_IsRepresentative;        // 0x7F
    u8 m_Unknown80;                 // 0x80
    // MP attribute tags to write (by tag number)
    bool m_HasTagB101;              // 0x81, individual image number
    bool m_HasTagB201;              // 0x82
    bool m_HasTagB202;              // 0x83
    bool m_HasTagB203;              // 0x84
    bool m_HasTagB204;              // 0x85
    bool m_HasTagB205;              // 0x86, otherwise -1
    bool m_HasTagB206;              // 0x87, otherwise -1
    bool m_HasTagB207;              // 0x88
    bool m_HasTagB208;              // 0x89
    bool m_HasTagB209;              // 0x8A
    bool m_HasTagB20A;              // 0x8B
    bool m_HasTagB20B;              // 0x8C
    bool m_HasTagB20C;              // 0x8D
    bool m_HasTagB20D;              // 0x8E
    u8 m_Padding8F;                 // 0x8F
    u32 m_Unknown90;                // 0x90
    u32 m_TagB101;                  // 0x94
    u32 m_TagB201;                  // 0x98
    u32 m_TagB202[2];               // 0x9C, rational
    u32 m_TagB203[2];               // 0xA4, rational
    u32 m_TagB204;                  // 0xAC
    s32 m_TagB205[2];               // 0xB0, srational
    u32 m_TagB206[2];               // 0xB8, rational
    s32 m_TagB207[2];               // 0xC0, srational
    s32 m_TagB208[2];               // 0xC8
    s32 m_TagB209[2];               // 0xD0
    s32 m_TagB20A[2];               // 0xD8
    s32 m_TagB20B[2];               // 0xE0
    s32 m_TagB20C[2];               // 0xE8
    s32 m_TagB20D[2];               // 0xF0
};
ASSERT_SIZE(JpegMpEncoderTemporarySettingObj, 0xF8);

// The entries of the MP index IFD, written per image while encoding the MP file
struct JpegMpEncoderMpEntry
{
    u32 m_Attribute;
    u32 m_Size;
    u32 m_Offset;
    u16 m_Dependent1;
    u16 m_Dependent2;
};

struct JpegMpEncoderWorkObj
{
    static const s32 APP1_TAG_COUNT = 69;
    static const s32 APP1_IFD_COUNT = 6;

    JpegMpEncoderContext m_Context;                     // 0x000
    bool m_IsMp;                                        // 0x69C
    bool m_IsAddThumbnail;                              // 0x69D
    bool m_IsApp1LittleEndian;                          // 0x69E
    s8 m_IsApp2LittleEndian;                            // 0x69F
    u32 m_App1Size;                                     // 0x6A0
    u32 m_ExifIfdOffset;                                // 0x6A4, values of the IFD pointer tags
    u32 m_InteropIfdOffset;                             // 0x6A8
    u32 m_GpsIfdOffset;                                 // 0x6AC
    u32 m_PixelWidth;                                   // 0x6B0
    u32 m_PixelHeight;                                  // 0x6B4
    u32 m_ThumbnailOffset;                              // 0x6B8
    u32 m_ThumbnailSize;                                // 0x6BC
    u8* m_ThumbnailData;                                // 0x6C0
    JpegMpEncoderIfdWorkObj m_App1Ifds[APP1_IFD_COUNT]; // 0x6C4
    JpegTagWorkObj m_App1Tags[APP1_TAG_COUNT];          // 0x724
    JpegMpEncoderMpEntry* m_MpEntries;                  // 0xA60
    char* m_ImageUniqueIds;                             // 0xA64, 33 bytes per image
    u32 m_MaxImageCount;                                // 0xA68
    u32 m_TypeCode;                                     // 0xA6C
    u32 m_FirstTypeCode;                                // 0xA70
    u32 m_ImageCount;                                   // 0xA74
    u32 m_CurrentImage;                                 // 0xA78
    u8* m_App2Start;                                    // 0xA7C
    u32 m_App2Size;                                     // 0xA80
    u32 m_Unknown84;                                    // 0xA84
    u16 m_DependentImage1;                              // 0xA88
    u16 m_DependentImage2;                              // 0xA8A
    bool m_IsDependentParent;                           // 0xA8C
    bool m_IsDependentChild;                            // 0xA8D
    s8 m_IsRepresentative;                              // 0xA8E
    bool m_HasRepresentativeSet;                        // 0xA8F
    bool m_HasImageUniqueIds;                           // 0xA90
    bool m_HasTotalFrames;                              // 0xA91
    bool m_IsBaselinePrimary;                           // 0xA92
    bool m_IsLastImage;                                 // 0xA93
    u32 m_NumberOfImages;                               // 0xA94, value of tag 0xB001
    u32 m_TotalFrames;                                  // 0xA98, value of tag 0xB004
    u32 m_IndividualNumber;                             // 0xA9C, value of tag 0xB101
    u32 m_BaseViewpointNumber;                          // 0xAA0, value of tag 0xB204
    JpegMpEncoderIfdWorkObj m_MpIfds[3];                // 0xAA4
    JpegTagWorkObj m_MpTags[20];                        // 0xAD4, 5 index tags and 15 attribute tags
    u8* m_JpegStart;                                    // 0xBC4
    u32 m_JpegSize;                                     // 0xBC8
    u32 m_ImageStart;                                   // 0xBCC
    u32 m_ImageSize;                                    // 0xBD0
};
ASSERT_SIZE(JpegMpEncoderWorkObj, 0xBD4);
} // namespace detail
} // namespace CTR
} // namespace jpeg
} // namespace nn
