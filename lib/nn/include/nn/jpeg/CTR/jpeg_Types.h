#pragma once

#include "decomp.h"

// Types of the jpeg library (JPEG/Exif/MPO encoder and decoder). The type names are from the
// symbols; members, enumerators and the layouts are ours, from the code. The Exif and MP format
// values (tags, types) follow the public Exif 2.2 and CIPA DC-007 (MPF) specifications.

namespace nn {
namespace jpeg {
namespace CTR {
// layouts of the pixel data (names after the converter functions)
enum PixelFormat : u8
{
    PIXEL_FORMAT_YUYV8,
    PIXEL_FORMAT_CTR_RGB565,
    PIXEL_FORMAT_CTR_RGB565_BLOCK8,
    PIXEL_FORMAT_RGB8,
    PIXEL_FORMAT_CTR_RGB8_BLOCK8,
    PIXEL_FORMAT_RGBA8,
    PIXEL_FORMAT_CTR_RGBA8_BLOCK8,
    PIXEL_FORMAT_BGR8,
    PIXEL_FORMAT_ABGR8,
    PIXEL_FORMAT_COUNT
};

// chroma subsampling of the encoder
enum PixelSampling : u8
{
    PIXEL_SAMPLING_YUV444 = 1, // 1x1
    PIXEL_SAMPLING_YUV420 = 2, // 2x2
    PIXEL_SAMPLING_YUV422 = 3  // 2x1
};

// MP type code of an image (CIPA DC-007)
enum MpTypeCode : u32
{
    MP_TYPE_CODE_UNDEFINED = 0,
    MP_TYPE_CODE_LARGE_THUMBNAIL_VGA = 0x10001,
    MP_TYPE_CODE_LARGE_THUMBNAIL_FULL_HD = 0x10002,
    MP_TYPE_CODE_MULTI_FRAME_PANORAMA = 0x20001,
    MP_TYPE_CODE_MULTI_FRAME_DISPARITY = 0x20002,
    MP_TYPE_CODE_MULTI_FRAME_MULTI_ANGLE = 0x20003,
    MP_TYPE_CODE_BASELINE_PRIMARY = 0x30000
};

// an Exif tag to write or a tag that was read
struct JpegTagWorkObj
{
    u32 m_Count;        // 0x0, size of the data in bytes
    const u8* m_Data;   // 0x4
    u16 m_Tag;          // 0x8
    u8 m_Type;          // 0xA, 1 byte, 2 ascii, 3 short, 4 long, 5 rational, 7 undefined, 9 slong, 10 srational
    bool m_IsSet;       // 0xB, the data is written elsewhere / was found
};
ASSERT_SIZE(JpegTagWorkObj, 0xC);

// where a known tag goes when it is read
struct JpegMpDecoderExifTagITN
{
    u8 m_Index; // 0x0, slot in the tag table
    u8 m_Type;  // 0x1
    u16 m_Tag;  // 0x2
};
ASSERT_SIZE(JpegMpDecoderExifTagITN, 0x4);

// one IFD while writing (filled by CalcSegmentSizeCommon)
struct JpegMpEncoderIfdWorkObj
{
    u16 m_EntryCount;   // 0x0
    u32 m_Offset;       // 0x4, from the TIFF header
    u32 m_Size;         // 0x8
    u32 m_NextOffset;   // 0xC
};
ASSERT_SIZE(JpegMpEncoderIfdWorkObj, 0x10);

// one color component while encoding
struct JpegMpEncoderComponentStructure
{
    s16 m_Coefficients[64];     // 0x00, quantized, from JpegMpEncoderAsm_ForwardDct
    s32 m_DcPrediction;         // 0x80
    const void* m_DcTable;      // 0x84, Huffman codes
    const void* m_AcTable;      // 0x88
};
ASSERT_SIZE(JpegMpEncoderComponentStructure, 0x8C);

struct JpegMpEncoderContext;
typedef void (*JpegMpEncoderMcuFunc)(JpegMpEncoderContext* context, u32 mcuX, u32 mcuY);

struct JpegMpEncoderContext
{
    s16 m_YBlocks[4][64];                       // 0x000
    s16 m_CbBlock[64];                          // 0x200
    s16 m_CrBlock[64];                          // 0x280
    JpegMpEncoderMcuFunc m_McuFunc;             // 0x300, fills the blocks of one MCU
    u8 m_BlocksPerMcu;                          // 0x304
    u8 m_McuCountX;                             // 0x305
    u8 m_McuCountY;                             // 0x306
    u8 m_IsNoHuffmanTable;                      // 0x307, the DHT segment is left out
    const void* m_Source;                       // 0x308, pixels as the converter sees them
    u16 m_Width;                                // 0x30C
    u16 m_Height;                               // 0x30E
    u8* m_Dst;                                  // 0x310
    u32 m_DstPosition;                          // 0x314
    u32 m_DstSize;                              // 0x318
    u32 m_BitBuffer;                            // 0x31C
    u8 m_BitsLeft;                              // 0x320, 8 when the buffer is empty
    u8 m_Quality;                               // 0x321
    u8 m_SamplingH;                             // 0x322
    u8 m_SamplingV;                             // 0x323
    s8 m_Error;                                 // 0x324, first error (0 = none)
    u8 m_Sampling;                              // 0x325
    u8 m_PixelFormat;                           // 0x326
    u32 m_Stride;                               // 0x328, pixels per source line
    JpegMpEncoderMcuFunc m_ConvertFunc;         // 0x32C, after resampling
    u16* m_ResampleBuffer;                      // 0x330, one MCU as CtrRgb565 (YUYV for Yuyv8)
    const u8* m_ResampleSource;                 // 0x334
    u16 m_ResampleSourceWidth;                  // 0x338
    u16 m_ResampleSourceHeight;                 // 0x33A
    u32 m_ResampleSourceStride;                 // 0x33C
    u8 m_Unknown340[0x1C];                      // 0x340
    s32 m_LineStep2Minus16;                     // 0x35C, byte steps of the converters
    s32 m_LineStep2Minus32;                     // 0x360
    s32 m_LineStep3Minus24;                     // 0x364
    s32 m_LineStep3Minus48;                     // 0x368
    s32 m_LineStep4Minus32;                     // 0x36C
    s32 m_LineStep4Minus64;                     // 0x370
    s32 m_LineStep3;                            // 0x374
    JpegMpEncoderComponentStructure m_Y;        // 0x378
    JpegMpEncoderComponentStructure m_Cb;       // 0x404
    JpegMpEncoderComponentStructure m_Cr;       // 0x490
    u8 m_LumaQuantization[64];                  // 0x51C, kept while the quality stays the same
    u8 m_ChromaQuantization[64];                // 0x55C
    u16 m_LumaReciprocal[64];                   // 0x59C
    u16 m_ChromaReciprocal[64];                 // 0x61C
};
ASSERT_SIZE(JpegMpEncoderContext, 0x69C);

// the MP index of a file (filled by JpegMpDecoder::GetMpIndex)
struct MpIndex
{
    const u8* m_Src;            // 0x00, the file
    u32 m_SrcSize;              // 0x04
    u32 m_EntrySize;            // 0x08, of the MP entries (16 bytes each)
    u32 m_UniqueIdSize;         // 0x0C
    u32 m_EntryOffset;          // 0x10, from m_Src
    u32 m_UniqueIdOffset;       // 0x14
    u32 m_IfdOffset;            // 0x18
    u32 m_TiffHeaderOffset;     // 0x1C, MP entry offsets are relative to it
    bool m_IsLittleEndian;      // 0x20
    bool m_HasIndex;            // 0x21
    bool m_HasVersion;          // 0x22
    bool m_HasTotalFrames;      // 0x23
    u32 m_Version;              // 0x24, the four characters of tag 0xB000
    u32 m_NumberOfImages;       // 0x28
    u32 m_TotalFrames;          // 0x2C
};
ASSERT_SIZE(MpIndex, 0x30);

struct MpEntry
{
    u32 m_Attribute;    // 0x00
    u32 m_Size;         // 0x04
    u32 m_Offset;       // 0x08, from the start of the file (0 for the first image)
    u16 m_Dependent1;   // 0x0C
    u16 m_Dependent2;   // 0x0E
    const u8* m_Base;   // 0x10
};
ASSERT_SIZE(MpEntry, 0x14);

// the parts of an MP file to build a single JPEG from it
struct MpRegionsToBuildJpegData
{
    const u8* m_Data1;  // 0x0
    u32 m_Size1;        // 0x4
    const u8* m_Data2;  // 0x8
    u32 m_Size2;        // 0xC
};
ASSERT_SIZE(MpRegionsToBuildJpegData, 0x10);

// the GPS IFD of an image (tags of the Exif 2.2 GPS attribute IFD); a tag is written when its
// flag is set (strings and pointers: when they are not empty)
struct GpsData
{
    bool m_HasVersionId;            // 0x00
    bool m_HasLatitude;             // 0x01
    bool m_HasLongitude;            // 0x02
    bool m_HasAltitudeRef;          // 0x03
    bool m_HasAltitude;             // 0x04
    bool m_HasTimeStamp;            // 0x05
    bool m_HasDop;                  // 0x06
    bool m_HasSpeed;                // 0x07
    bool m_HasTrack;                // 0x08
    bool m_HasImgDirection;         // 0x09
    bool m_HasDestLatitude;         // 0x0A
    bool m_HasDestLongitude;        // 0x0B
    bool m_HasDestBearing;          // 0x0C
    bool m_HasDestDistance;         // 0x0D
    bool m_HasDifferential;         // 0x0E
    u8 m_AltitudeRef;               // 0x0F
    u8 m_VersionId[4];              // 0x10
    char m_LatitudeRef[2];          // 0x14
    char m_LongitudeRef[2];         // 0x16
    char m_Status[2];               // 0x18
    char m_MeasureMode[2];          // 0x1A
    char m_SpeedRef[2];             // 0x1C
    char m_TrackRef[2];             // 0x1E
    char m_ImgDirectionRef[2];      // 0x20
    char m_DestLatitudeRef[2];      // 0x22
    char m_DestLongitudeRef[2];     // 0x24
    char m_DestBearingRef[2];       // 0x26
    char m_DestDistanceRef[2];      // 0x28
    u16 m_Differential;             // 0x2A
    u32 m_Latitude[6];              // 0x2C, three rationals
    u32 m_Longitude[6];             // 0x44
    u32 m_Altitude[2];              // 0x5C
    u32 m_TimeStamp[6];             // 0x64
    const char* m_Satellites;       // 0x7C
    u32 m_Dop[2];                   // 0x80
    u32 m_Speed[2];                 // 0x88
    u32 m_Track[2];                 // 0x90
    u32 m_ImgDirection[2];          // 0x98
    const char* m_MapDatum;         // 0xA0
    u32 m_DestLatitude[6];          // 0xA4
    u32 m_DestLongitude[6];         // 0xBC
    u32 m_DestBearing[2];           // 0xD4
    u32 m_DestDistance[2];          // 0xDC
    const u8* m_ProcessingMethod;   // 0xE4
    u32 m_ProcessingMethodSize;     // 0xE8
    const u8* m_AreaInformation;    // 0xEC
    u32 m_AreaInformationSize;      // 0xF0
    const char* m_DateStamp;        // 0xF4
};
ASSERT_SIZE(GpsData, 0xF8);

struct JpegMpDecoderContext;
typedef void (*JpegMpDecoderWriteFunc)(JpegMpDecoderContext* context, int mcuX, int mcuY);

// the attribute IFD of an MP image (tags of slot i are present when m_Has[i] is set)
struct JpegMpDecoderMpAttribute
{
    bool m_Has[15];             // 0x00
    u8 m_Padding;               // 0x0F
    u32 m_Version;              // 0x10, tag 0xB000
    u32 m_IndividualNumber;     // 0x14, 0xB101
    u32 m_PanOrientation;       // 0x18, 0xB201
    u32 m_TagB202[2];           // 0x1C
    u32 m_TagB203[2];           // 0x24
    u32 m_TagB204;              // 0x2C
    s32 m_TagB205[2];           // 0x30
    u32 m_TagB206[2];           // 0x38
    s32 m_TagB207[2];           // 0x40
    s32 m_TagB208[2];           // 0x48
    s32 m_TagB209[2];           // 0x50
    s32 m_TagB20A[2];           // 0x58
    s32 m_TagB20B[2];           // 0x60
    s32 m_TagB20C[2];           // 0x68
    s32 m_TagB20D[2];           // 0x70
};
ASSERT_SIZE(JpegMpDecoderMpAttribute, 0x78);

// The work memory of the decoder. Offsets up to 0x80 and the sample buffers are also used by
// the assembly (jpeg_Asm.cpp). Member names are ours.
struct JpegMpDecoderContext
{
    static const s32 TAG_COUNT = 48;
    static const s32 MP_TAG_COUNT = 15;

    const u8* m_Src;                        // 0x0000
    u32 m_SrcSize;                          // 0x0004
    u32 m_DstSize;                          // 0x0008
    u32 m_RestartCounter;                   // 0x000C
    u32 m_RestartInterval;                  // 0x0010
    void* m_Dst;                            // 0x0014
    u16 m_Width;                            // 0x0018, of the image
    u16 m_Height;                           // 0x001A
    u16 m_MaxWidth;                         // 0x001C
    u16 m_MaxHeight;                        // 0x001E
    u16 m_ExpectedWidth;                    // 0x0020, 0 = any
    u16 m_ExpectedHeight;                   // 0x0022
    s16 m_DcPrediction[3];                  // 0x0024
    s8 m_BitCount;                          // 0x002A
    u8 m_LastByte;                          // 0x002B
    u8 m_SamplingH;                         // 0x002C
    u8 m_SamplingV;                         // 0x002D
    u8 m_McuWidth;                          // 0x002E
    u8 m_McuHeight;                         // 0x002F
    u8 m_BlocksPerMcu;                      // 0x0030
    bool m_IsMpAttributeWanted;             // 0x0031, read the attribute IFD behind the MP index
    bool m_HasMpIndex;                      // 0x0032
    bool m_HasMpAttribute;                  // 0x0033
    const u8* m_App2;                       // 0x0034, the first MP APP2 segment
    u32 m_Stride;                           // 0x0038, pixels per line of the output
    u32 m_AlignedHeight;                    // 0x003C
    JpegMpDecoderWriteFunc m_WriteFunc;     // 0x0040
    s8 m_Error;                             // 0x0044
    u8 m_AsmError;                          // 0x0045
    u8 m_Unknown46[2];                      // 0x0046
    u8 m_PixelFormat;                       // 0x0048
    bool m_IsLittleEndian;                  // 0x0049, of the Exif data
    bool m_HasExif;                         // 0x004A
    bool m_IsExifOnly;                      // 0x004B, stop at the first scan
    bool m_IsThumbnail;                     // 0x004C, decode the Exif thumbnail
    u8 m_Shrink;                            // 0x004D, log2 of the scale down
    u8 m_CbQuantizationTable;               // 0x004E
    u8 m_CrQuantizationTable;               // 0x004F
    u32 m_BitBuffer;                        // 0x0050, for the assembly
    u32 m_Position;                         // 0x0054, in m_Src
    const u8* m_SrcLast;                    // 0x0058
    u32 m_Flags;                            // 0x005C, segments read: 1 SOF, 0x10-0x80 Huffman tables, 0x1000 DQT, 0x10000 scan
    u32 m_RequiredFlags;                    // 0x0060
    u32 m_SettingFlags;                     // 0x0064, bit 0 default Huffman tables, bit 1 exact size
    const u8* m_TiffHeader;                 // 0x0068, of the Exif data
    u8 m_AsmWork[0x14];                     // 0x006C
    s16 m_Quantization[3][64];              // 0x0080
    s16 m_Coefficients[3][64];              // 0x0200
    u8 m_Cb[64];                            // 0x0380
    u8 m_Cr[64];                            // 0x03C0
    u8 m_Y[4][64];                          // 0x0400
    u8 m_HuffmanTables[0x1800];             // 0x0500, the DHT data (0x270) and the decoding tables
    u16 m_HuffmanCode[257];                 // 0x1D00
    s8 m_HuffmanSize[257];                  // 0x1F02
    u8 m_Padding2003;                       // 0x2003
    JpegTagWorkObj m_Tags[TAG_COUNT];       // 0x2004
    JpegTagWorkObj m_MpTags[MP_TAG_COUNT];  // 0x2244
    union                                   // 0x22F8
    {
        MpIndex m_MpIndex;
        JpegMpDecoderMpAttribute m_MpAttribute;
    };
};
ASSERT_SIZE(JpegMpDecoderContext, 0x2370);
} // namespace CTR
} // namespace jpeg
} // namespace nn
