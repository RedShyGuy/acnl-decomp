#pragma once

#include "decomp.h"

namespace nw {
namespace ut {
// The header of a NintendoWare binary file; the data blocks follow. Member names are ours.
struct BinaryFileHeader
{
    u32 signature;  // 0x00
    u16 byteOrder;  // 0x04 (BYTE_ORDER_MARK in the byte order of the file)
    u16 headerSize; // 0x06
    u32 version;    // 0x08 (major << 24 | minor << 16 | micro << 8 | build)
    u32 fileSize;   // 0x0C
    u16 dataBlocks; // 0x10
    u16 reserved;   // 0x12
};
ASSERT_SIZE(BinaryFileHeader, 0x14);

// The header of a data block. Member names are ours.
struct BinaryBlockHeader
{
    u32 kind; // 0x0
    u32 size; // 0x4 (with the header)
};
ASSERT_SIZE(BinaryBlockHeader, 0x8);

// (inline) the value rounded up to a multiple of the alignment, a power of two (name is ours)
template <typename T>
inline T RoundUp(T value, u32 alignment)
{
    return (value + alignment - 1) & ~static_cast<T>(alignment - 1);
}
template <typename T>
inline T* RoundUp(T* pointer, u32 alignment)
{
    return reinterpret_cast<T*>(RoundUp(reinterpret_cast<uptr>(pointer), alignment));
}

// the smallest x with 1 + x != 1 (2^-23; name is ours)
const f32 FLOAT_EPSILON = 1.1920929e-7f;

// the byte order mark in the byte order of the CPU (name is ours)
const u16 BYTE_ORDER_MARK = 0xFEFF;

// the signature and the byte order match, the major version is the same and the file is not
// newer, and it has at least minBlocks blocks
bool IsValidBinaryFile(const nw::ut::BinaryFileHeader* pHeader, u32 signature, u32 version, u16 minBlocks); // 0x0048B900 | nintendogs:bytes [tier A]
// the block behind pBlock (NULL: the first one); NULL at the end of the file
nw::ut::BinaryBlockHeader* GetNextBinaryBlockHeader(nw::ut::BinaryFileHeader* pHeader, nw::ut::BinaryBlockHeader* pBlock); // 0x0048B980 | nintendogs:bytes [tier A]

// a and b are equal up to the float precision of the larger one (out of line per type)
template <typename T>
DECOMP_NOINLINE bool FloatEqualsWeak(T a, T b)
{
    T diff = a - b;
    if (diff < 0) {
        diff = -diff;
    }
    if (a < 0) {
        a = -a;
    }
    if (a * FLOAT_EPSILON >= diff) {
        return true;
    }
    if (b < 0) {
        b = -b;
    }
    return b * FLOAT_EPSILON >= diff;
}
} // namespace ut
} // namespace nw
