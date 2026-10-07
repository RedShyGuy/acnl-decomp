#include "pead/peadHashCrc32.h"

namespace pead {
namespace {
const u32 POLYNOMIAL = 0xEDB88320;

// 0x0097FA24
bool s_IsTableInitialized;
// 0x00AF5B90
u32 s_Table[256];
} // namespace

// 0x0053DA5C (name is ours)
u32 HashCrc32::calcHash(const void* pData, u32 size)
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
    u32 hash = 0xFFFFFFFF;
    for (u32 i = 0; i < size; i++) {
        hash = s_Table[(hash ^ p[i]) & 0xFF] ^ (hash >> 8);
    }
    return ~hash;
}
} // namespace pead
