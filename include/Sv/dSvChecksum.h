#pragma once

#include "decomp.h"

// the CRC-32C (Castagnoli, reflected, polynomial 0x82F63B78) of the save data: crc is the start value, the result is
// xored with xorOut (the save data uses 0xFFFFFFFF for both; at least one byte is read; game code, name is ours)
DECOMP_NOINLINE u32 SvCalcCrc32C(const void* data, u32 size, u32 crc, u32 xorOut); // 0x00319568

// (inline) the checksum of a part of the save data: CRC-32C with the start value and xor 0xFFFFFFFF
inline u32 SvCalcChecksum(const void* data, u32 size)
{
    return SvCalcCrc32C(data, size, 0xFFFFFFFF, 0xFFFFFFFF);
}
