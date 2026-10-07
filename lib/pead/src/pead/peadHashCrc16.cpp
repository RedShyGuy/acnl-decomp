#include "pead/peadHashCrc16.h"

namespace pead {
namespace {
const u16 POLYNOMIAL = 0xA001;

// 0x00982EB0
bool s_IsTableInitialized;
// 0x00AF7DDC
u16 s_Table[256];
} // namespace

// 0x0053D978 (name is ours)
u16 HashCrc16::calcHash(const void* pData, u32 size)
{
    if (!s_IsTableInitialized) {
        for (u32 i = 0; i < 256; i++) {
            u32 value = i;
            for (int bit = 0; bit < 8; bit++) {
                value = (value & 1) != 0 ? (value >> 1) ^ POLYNOMIAL : value >> 1;
            }
            s_Table[i] = value;
        }
        s_IsTableInitialized = true;
    }
    const u8* p = static_cast<const u8*>(pData);
    u32 hash = 0;
    for (u32 i = 0; i < size; i++) {
        hash = s_Table[(hash ^ p[i]) & 0xFF] ^ (hash >> 8);
    }
    return hash;
}
} // namespace pead
