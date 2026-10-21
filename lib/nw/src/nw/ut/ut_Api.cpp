#include "nw/ut/ut_Api.h"

namespace nw {
namespace ut {
namespace {
// (inline; names are ours)
inline u32 ReverseEndian(u32 value)
{
    return (value << 24) | ((value >> 8 & 0xFF) << 16) | ((value >> 16 & 0xFF) << 8) | (value >> 24);
}
inline u16 ReverseEndian(u16 value)
{
    return static_cast<u16>((value >> 8) | ((value & 0xFF) << 8));
}
} // namespace

// 0x0048B900 | nintendogs:bytes [tier A]
bool IsValidBinaryFile(const nw::ut::BinaryFileHeader* pHeader, u32 signature, u32 version, u16 minBlocks)
{
    if (pHeader->signature != signature) {
        return false;
    }
    if (pHeader->byteOrder != BYTE_ORDER_MARK) {
        return false;
    }
    // the same major version, a minor version that is not newer
    if ((version >> 24) != (pHeader->version >> 24)) {
        return false;
    }
    if (((version >> 16) & 0xFF) < ((pHeader->version >> 16) & 0xFF)) {
        return false;
    }
    if ((version & 0xFF) > (pHeader->version & 0xFF)) {
        return false;
    }
    if (pHeader->fileSize < sizeof(BinaryFileHeader) + sizeof(BinaryBlockHeader) * minBlocks) {
        return false;
    }
    if (pHeader->dataBlocks < minBlocks) {
        return false;
    }
    return true;
}

// 0x0048B980 | nintendogs:bytes [tier A]
nw::ut::BinaryBlockHeader* GetNextBinaryBlockHeader(nw::ut::BinaryFileHeader* pHeader, nw::ut::BinaryBlockHeader* pBlock)
{
    u8* pNext;
    if (pHeader->byteOrder == BYTE_ORDER_MARK) {
        if (pBlock != nullptr) {
            pNext = reinterpret_cast<u8*>(pBlock) + pBlock->size;
        } else {
            if (pHeader->dataBlocks == 0) {
                return nullptr;
            }
            pNext = reinterpret_cast<u8*>(pHeader) + pHeader->headerSize;
        }
        if (reinterpret_cast<u8*>(pHeader) + pHeader->fileSize <= pNext) {
            return nullptr;
        }
    } else {
        // the file has the other byte order
        if (pBlock != nullptr) {
            pNext = reinterpret_cast<u8*>(pBlock) + ReverseEndian(pBlock->size);
        } else {
            if (pHeader->dataBlocks == 0) {
                return nullptr;
            }
            pNext = reinterpret_cast<u8*>(pHeader) + ReverseEndian(pHeader->headerSize);
        }
        if (reinterpret_cast<u8*>(pHeader) + ReverseEndian(pHeader->fileSize) <= pNext) {
            return nullptr;
        }
    }
    return reinterpret_cast<BinaryBlockHeader*>(pNext);
}

// 0x007E5D64 | nintendogs:bytes [confirmed by mk7dlp] [tier A]
template bool FloatEqualsWeak<f32>(f32 a, f32 b);

} // namespace ut
} // namespace nw
