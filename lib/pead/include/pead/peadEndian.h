#pragma once

#include "decomp.h"

namespace pead {
// pia's copy of sead::Endian: the byte order of data and its conversion to the byte order of the console (all
// names are ours).
class Endian
{
public:
    enum Types : u8
    {
        BIG = 0,
        LITTLE = 1,
    };

    // the conversions for each width: [0] keeps the bytes (same order), [1] swaps them
    struct ConvFuncTable
    {
        u8 (*convU8[2])(u8);
        u16 (*convU16[2])(u16);
        u32 (*convU32[2])(u32);
        u64 (*convU64[2])(u64);
    };

    // the byte order of a byte order mark 0xFEFF as it lies in memory
    static Types markToEndian(u16 mark)
    {
        const u8* bytes = reinterpret_cast<const u8*>(&mark);
        if (bytes[0] == 0xFF) {
            return LITTLE;
        }
        if (bytes[0] == 0xFE && bytes[1] == 0xFF) {
            return BIG;
        }
        return LITTLE;
    }

    // value (in the byte order endian) in the byte order of the console, and back
    static u8 toHostU8(Types endian, u8 value) { return s_ConvFuncTable.convU8[s_HostEndian ^ endian](value); }
    static u16 toHostU16(Types endian, u16 value) { return s_ConvFuncTable.convU16[s_HostEndian ^ endian](value); }
    static u32 toHostU32(Types endian, u32 value) { return s_ConvFuncTable.convU32[s_HostEndian ^ endian](value); }
    static u64 toHostU64(Types endian, u64 value) { return s_ConvFuncTable.convU64[s_HostEndian ^ endian](value); }
    static u8 fromHostU8(Types endian, u8 value) { return toHostU8(endian, value); }
    static u16 fromHostU16(Types endian, u16 value) { return toHostU16(endian, value); }
    static u32 fromHostU32(Types endian, u32 value) { return toHostU32(endian, value); }
    static u64 fromHostU64(Types endian, u64 value) { return toHostU64(endian, value); }

    static Types s_HostEndian;
    static const ConvFuncTable s_ConvFuncTable;
};
} // namespace pead
