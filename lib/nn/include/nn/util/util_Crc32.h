#pragma once

#include "decomp.h"

namespace nn {
namespace util {

// CRC-32 (polynomial 0xEDB88320, as zlib / PNG). The table is built per object; Calculate does
// everything on the stack. Member and method names except Calculate are ours.
class Crc32
{
public:
    static const u32 POLYNOMIAL = 0xEDB88320;

    Crc32() { InitializeTable(); }

    void InitializeContext(u32 context) { mContext = context; }

    void Update(const void* data, size_t size)
    {
        const u8* bytes = static_cast<const u8*>(data);
        for (size_t i = 0; i < size; i++) {
            mContext = mTable[(bytes[i] ^ mContext) & 0xFF] ^ (mContext >> 8);
        }
    }

    u32 GetHash() const { return ~mContext; }

    // context: the start value (usually 0xFFFFFFFF); returns the inverted result
    static u32 Calculate(const void* data, size_t size, u32 context); // 0x0047F1B8 | fefates:bytes [tier B]

private:
    void InitializeTable()
    {
        for (u32 i = 0; i < 256; i++) {
            u32 value = i;
            for (s32 bit = 0; bit < 8; bit++) {
                value = (value & 1) ? (POLYNOMIAL ^ (value >> 1)) : (value >> 1);
            }
            mTable[i] = value;
        }
    }

    u32 mContext;       // 0x000
    u32 mTable[256];    // 0x004
};
ASSERT_SIZE(Crc32, 0x404);

// CRC-32 most significant bit first (polynomial 0x04C11DB7, as MPEG-2). Not named in the binary;
// it sits next to Crc32::Calculate and is built the same way, so the class and all names are ours.
class Crc32Msb
{
public:
    static const u32 POLYNOMIAL = 0x04C11DB7;

    Crc32Msb() { InitializeTable(); }

    void InitializeContext(u32 context) { mContext = context; }

    void Update(const void* data, size_t size)
    {
        const u8* bytes = static_cast<const u8*>(data);
        for (size_t i = 0; i < size; i++) {
            mContext = mTable[bytes[i] ^ (mContext >> 24)] ^ (mContext << 8);
        }
    }

    u32 GetHash() const { return ~mContext; }

    static u32 Calculate(const void* data, size_t size, u32 context); // 0x0047F0F0 (name is ours)

private:
    void InitializeTable()
    {
        for (u32 i = 0; i < 256; i++) {
            u32 value = i << 24;
            for (s32 bit = 0; bit < 8; bit++) {
                value = (value & 0x80000000) ? (POLYNOMIAL ^ (value << 1)) : (value << 1);
            }
            mTable[i] = value;
        }
    }

    u32 mContext;       // 0x000
    u32 mTable[256];    // 0x004
};
ASSERT_SIZE(Crc32Msb, 0x404);

} // namespace util
} // namespace nn
