#pragma once

#include "decomp.h"
#include "nn/jpeg/CTR/jpeg_Types.h"

namespace nn {
namespace jpeg {
namespace CTR {
namespace detail {
// settings for the next decode only (member names are ours)
struct JpegMpDecoderTemporarySettingObj
{
    u32 m_Stride;   // 0x0, pixels per output line (0: the width)
    u32 m_Flags;    // 0x4, JpegMpDecoderContext::m_SettingFlags
};
ASSERT_SIZE(JpegMpDecoderTemporarySettingObj, 0x8);

struct App1PointerAndSize
{
    const u8* m_Pointer;    // 0x0
    u32 m_Size;             // 0x4
};
ASSERT_SIZE(App1PointerAndSize, 0x8);

// The Huffman tables in JpegMpDecoderContext::m_HuffmanTables: first the DHT data per table id
// (DC: 16 counts, values from +20; AC: from +36), then the decoding tables of the assembly
// (DC 0, AC 0, DC 1, AC 1). Member names are ours.
struct JpegMpDecoderHuffmanTable
{
    s32 m_MinCode[17];          // 0x000, [length]
    s32 m_MaxCode[18];          // 0x044, [length], -1: no code; [17] = 0xFFFFF
    u16 m_ValuePointer[17];     // 0x08C, the first value of a length
    u8 m_LookNbits[256];        // 0x0AE, the length of the code starting with 8 bits
    u8 m_LookSymbol[256];       // 0x1AE, its value
    u8 m_Padding[2];            // 0x2AE
};
ASSERT_SIZE(JpegMpDecoderHuffmanTable, 0x2B0);

const u32 HUFFMAN_SPEC_SIZE = 312;            // the DHT data of a table id
const u32 HUFFMAN_SPEC_AC_OFFSET = 36;
const u32 HUFFMAN_SPEC_VALUES_OFFSET = 20;
const u32 HUFFMAN_DECODE_TABLES_OFFSET = 2 * HUFFMAN_SPEC_SIZE;

// values of the Exif/MP data in its byte order, possibly unaligned (names are ours)
DECOMP_ALWAYS_INLINE u16 ReadU16(const u8* p, bool isLittleEndian)
{
    if (isLittleEndian) {
        return p[0] | (p[1] << 8);
    }
    return (p[0] << 8) | p[1];
}

DECOMP_ALWAYS_INLINE u32 ReadU32(const u8* p, bool isLittleEndian)
{
    if (isLittleEndian) {
        return p[0] | (p[1] << 8) | (p[2] << 16) | (p[3] << 24);
    }
    return (p[0] << 24) | (p[1] << 16) | (p[2] << 8) | p[3];
}
} // namespace detail
} // namespace CTR
} // namespace jpeg
} // namespace nn
