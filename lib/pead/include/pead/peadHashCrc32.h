#pragma once

// pead::HashCrc32 - the CRC-32 (polynomial 0xEDB88320, start value and final xor 0xFFFFFFFF) with
// a table that is built on the first use. pia local makes its session ids with it. The names are
// ours.

#include "decomp.h"

namespace pead {
class HashCrc32
{
public:
    static u32 calcHash(const void* pData, u32 size); // 0x0053DA5C (name is ours)
};
} // namespace pead
