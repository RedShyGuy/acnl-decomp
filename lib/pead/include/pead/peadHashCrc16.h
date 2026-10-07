#pragma once

// pead::HashCrc16 - the CRC-16 (polynomial 0xA001, start value 0) with a table that is built on the
// first use. The pia local messages carry it in their header. The names are ours.

#include "decomp.h"

namespace pead {
class HashCrc16
{
public:
    static u16 calcHash(const void* pData, u32 size); // 0x0053D978 (name is ours)
};
} // namespace pead
