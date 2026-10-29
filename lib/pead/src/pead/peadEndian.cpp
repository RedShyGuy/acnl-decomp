#include "pead/peadEndian.h"

namespace pead {
namespace {
// the conversions of the table (names are ours; armlink placed them, folded with identical functions, in
// the code of the game)

// 0x00187F90 (name is ours)
u8 noConvertU8(u8 value)
{
    return value;
}

// a byte has no order to swap
// 0x00187F94 (name is ours)
u8 swapU8(u8 value)
{
    return value;
}

// 0x001882BC (name is ours)
u16 noConvertU16(u16 value)
{
    return value;
}

// 0x001882C0 (name is ours)
u32 noConvertU32(u32 value)
{
    return value;
}

// 0x001882C4 (name is ours)
u64 noConvertU64(u64 value)
{
    return value;
}

// 0x001882C8 (name is ours)
u16 swapU16(u16 value)
{
    return static_cast<u16>(value << 8 | value >> 8);
}

// 0x001882D8 (name is ours)
u32 swapU32(u32 value)
{
    return __builtin_bswap32(value);
}

// 0x001882E0 (name is ours)
u64 swapU64(u64 value)
{
    return static_cast<u64>(__builtin_bswap32(static_cast<u32>(value))) << 32 |
           __builtin_bswap32(static_cast<u32>(value >> 32));
}
} // namespace

// 0x008C932C
const Endian::ConvFuncTable Endian::s_ConvFuncTable = {
    {noConvertU8, swapU8},
    {noConvertU16, swapU16},
    {noConvertU32, swapU32},
    {noConvertU64, swapU64},
};

// 0x00982FA8
Endian::Types Endian::s_HostEndian = Endian::markToEndian(0xFEFF);

// sets s_HostEndian
// 0x00790D20 (static initializer)

} // namespace pead
