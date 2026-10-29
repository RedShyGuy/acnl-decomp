#include "sead/seadEndian.h"

namespace sead {
namespace {
// the conversions of the table (names are ours)

// 0x004DDFF8 (name is ours)
u8 noConvertU8(u8 value)
{
    return value;
}

// a byte has no order to swap
// 0x004DDFFC (name is ours)
u8 swapU8(u8 value)
{
    return value;
}

// 0x004DE000 (name is ours)
u16 noConvertU16(u16 value)
{
    return value;
}

// 0x004DE004 (name is ours)
u32 noConvertU32(u32 value)
{
    return value;
}

// 0x004DE008 (name is ours)
u64 noConvertU64(u64 value)
{
    return value;
}

// 0x004DE00C (name is ours)
u16 swapU16(u16 value)
{
    return static_cast<u16>(value << 8 | value >> 8);
}

// 0x004DE01C (name is ours)
u32 swapU32(u32 value)
{
    return __builtin_bswap32(value);
}

// 0x004DE024 (name is ours)
u64 swapU64(u64 value)
{
    return static_cast<u64>(__builtin_bswap32(static_cast<u32>(value))) << 32 |
           __builtin_bswap32(static_cast<u32>(value >> 32));
}
} // namespace

// 0x008BC35C
const Endian::ConvFuncTable Endian::s_ConvFuncTable = {
    {noConvertU8, swapU8},
    {noConvertU16, swapU16},
    {noConvertU32, swapU32},
    {noConvertU64, swapU64},
};

// 0x0097F875
Endian::Types Endian::s_HostEndian = Endian::markToEndian(0xFEFF);

// sets s_HostEndian
// 0x00790CE0 (static initializer)

} // namespace sead
