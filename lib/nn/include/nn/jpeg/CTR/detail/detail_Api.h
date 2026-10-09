#pragma once

#include "decomp.h"
#include "nn/jpeg/CTR/CTR_Api.h"
#include "nn/jpeg/CTR/detail/jpeg_DecoderWork.h"
#include "nn/jpeg/CTR/jpeg_Types.h"

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail {
void JpegMpDecoderAsmConvertWorkToAsm(nn::jpeg::CTR::JpegMpDecoderContext*) JPEG_ASM; // 0x00148F30 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmConvertWorkToC(nn::jpeg::CTR::JpegMpDecoderContext*) JPEG_ASM; // 0x00148F80 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmGetMatrix(nn::jpeg::CTR::JpegMpDecoderContext*, int, short*) JPEG_ASM; // 0x00148FB4 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmDecodeBlock(nn::jpeg::CTR::JpegMpDecoderContext*, unsigned char*, int) JPEG_ASM; // 0x00149414 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite11Yuyv8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x00149944 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite21Yuyv8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x001499A0 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite12Yuyv8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x00149A10 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite22Yuyv8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x00149A84 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite11CtrRgb565(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x00149B0C | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite21CtrRgb565(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x00149BEC | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite12CtrRgb565(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x00149CC4 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite22CtrRgb565(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x00149D98 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite11CtrRgb565Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x00149EAC | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite21CtrRgb565Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x00149FA4 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite12CtrRgb565Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014A0B8 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite22CtrRgb565Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014A1BC | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite11Rgb8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014A2FC | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite21Rgb8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014A3D8 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite12Rgb8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014A4A8 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite22Rgb8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014A580 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite11CtrRgb8Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014A690 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite21CtrRgb8Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014A784 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite12CtrRgb8Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014A8D4 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite22CtrRgb8Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014A9DC | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite11Rgba8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014AB38 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite21Rgba8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014AC10 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite12Rgba8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014ACD8 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite22Rgba8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014AD9C | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite11CtrRgba8Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014AEAC | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite21CtrRgba8Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014AFA8 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite12CtrRgba8Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014B0B8 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite22CtrRgba8Block8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014B1AC | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite11Bgr8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014B2E8 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite21Bgr8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014B3C4 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite12Bgr8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014B494 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite22Bgr8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014B56C | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite11Abgr8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014B67C | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite21Abgr8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014B75C | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite12Abgr8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014B824 | nintendogs:bytes [tier B]
void JpegMpDecoderAsmWrite22Abgr8(nn::jpeg::CTR::JpegMpDecoderContext*, int, int) JPEG_ASM; // 0x0014B8E8 | nintendogs:bytes [tier B]
// the decoder
bool DecodeJpegApp1(JpegMpDecoderContext* context, u32 size); // 0x00472908 | nintendogs:bytes-fuzzy [tier B]
size_t GetApp1DateTime(char* buffer, const JpegMpDecoderContext* context); // 0x004731F0 | nintendogs:bytes [tier B]
bool CopyTagWorkByITN(JpegTagWorkObj* tags, const JpegTagWorkObj* tag, const JpegMpDecoderExifTagITN* itn, u32 itnCount); // 0x0047325C | nintendogs:bytes [tier B]
bool DecodeJpegApp2Mp(JpegMpDecoderContext* context, u32 size); // 0x00473378 | nintendogs:bytes-fuzzy [tier B]
void Mel_JPEGDecodeFast(JpegMpDecoderContext* context); // 0x00474540 | nintendogs:bytes-fuzzy [tier B]
bool GetApp1SoftwarePointer(App1PointerAndSize* software, const JpegMpDecoderContext* context); // 0x00475F9C | nintendogs:bytes [tier B]
bool GetApp1MakerNotePointer(App1PointerAndSize* makerNote, const JpegMpDecoderContext* context, u32 index); // 0x00475FF0 | nintendogs:bytes [tier B]
bool InitializeJpegMpDecoderContext(JpegMpDecoderContext* context, void* dst, size_t dstSize, const u8* src, size_t srcSize, u32 maxWidth, u32 maxHeight, PixelFormat format, bool isThumbnail, bool isExifOnly, const JpegMpDecoderTemporarySettingObj* settings); // 0x00479DA0 | nintendogs:bytes-fuzzy [tier B]
void ReadSRational(s32* value, const u8* p, bool isLittleEndian); // 0x0047DB2C (name is ours)
bool GetTag(JpegTagWorkObj* tag, const u8* entry, const u8* tiffHeader, const u8* end, bool isLittleEndian); // 0x0047DB98 | nintendogs:bytes [tier B]
// the C writers (detail_DecoderWrite.cpp)
void JpegMpDecoderCWrite11Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite11Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite12Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite12Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite21Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite21Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite22Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite22Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink11Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink11Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink12Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink12Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink21Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink21Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink22Bgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink22Rgb8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite11Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite11Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite11Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite12Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite12Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite12Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite21Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite21Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite21Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite22Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite22Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite22Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink11Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink11Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink11Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink12Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink12Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink12Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink21Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink21Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink21Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink22Abgr8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink22Rgba8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink22Yuyv8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite11CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite12CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite21CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite22CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink11CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink12CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink21CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink22CtrRgb565(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite11CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite12CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite21CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite22CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink11CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink12CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink21CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink22CtrRgb8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite11CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite12CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite21CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite22CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink11CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink12CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink21CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink22CtrRgba8Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite11CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite12CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite21CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCWrite22CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink11CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink12CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink21CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
void JpegMpDecoderCShrink22CtrRgb565Block8(JpegMpDecoderContext* context, int mcuX, int mcuY);
// the decoder's default Huffman tables (blob, see extracted_data.json)
// 0x0089EAC0
extern const u8 s_DefaultHuffmanTables[];
// the encoder's Exif (APP1) and MP (APP2) segments
bool EncodeJpegApp1(JpegMpEncoderWorkObj* work); // 0x00472F84 | nintendogs:bytes [tier B]
bool EncodeJpegMpApp2(JpegMpEncoderWorkObj* work, JpegMpEncoderTemporarySettingObj* settings); // 0x00473A90
void PreEncodeJpegApp1(JpegMpEncoderWorkObj* work, JpegMpEncoderTemporarySettingObj* settings, bool isAddThumbnail); // 0x004741C8 | nintendogs:bytes [tier B]
u32 EncodeSegmentCommon(u8* dst, const u8* tiffHeader, bool isLittleEndian, const JpegTagWorkObj* tags, const JpegMpEncoderIfdWorkObj* ifds, const u8* tagCounts, u32 index, u32 endIndex, JpegMpEncoderWorkObj* work); // 0x00475618
bool PostEncodeJpegMpApp2(JpegMpEncoderWorkObj* work, bool isAbort); // 0x00475BD4
u32 CalcSegmentSizeCommon(JpegTagWorkObj* tags, JpegMpEncoderIfdWorkObj* ifds, const u8* tagCounts, u32 ifdCount, u32 index, u32 endIndex); // 0x00475ED4 | nintendogs:bytes [tier B]
bool PreEncodeJpegApp1GpsData(JpegTagWorkObj* tags, const GpsData* gps); // 0x00476048 | nintendogs:bytes-fuzzy [tier B]
u32 CalcJpegMpEncoderApp1Size(JpegMpEncoderWorkObj* work); // 0x00476244 | nintendogs:bytes [tier B]
void InitializeJpegMpEncoderApp1TagWork(JpegMpEncoderWorkObj* work); // 0x0047AC50 | nintendogs:bytes [tier B]
void InitializeJpegMpEncoderApp2IndexTagWork(JpegMpEncoderWorkObj* work); // 0x0047DAD0 | nintendogs:bytes [tier B]
void InitializeJpegMpEncoderApp2AttributeTagWork(JpegMpEncoderWorkObj* work); // 0x0047DB74 | nintendogs:bytes [tier B]

// the Huffman tables of the encoder (standard tables of JPEG Annex K): the DHT segment and the
// codes for the assembly (blobs, see extracted_data.json)
// 0x008A0AC0
extern const u8 s_HuffmanTableSegment[];
// 0x008A0C64
extern const u8 s_LumaDcCodes[];
// 0x008A1170
extern const u8 s_ChromaDcCodes[];
// 0x008A167C
extern const u8 s_LumaAcCodes[];
// 0x008A1B88
extern const u8 s_ChromaAcCodes[];
} // namespace detail
} // namespace CTR
} // namespace jpeg
} // namespace nn
