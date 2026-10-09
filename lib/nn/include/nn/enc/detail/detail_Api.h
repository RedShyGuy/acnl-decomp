#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace enc {
// how line breaks of the source are written (the type name is from the binary; the values are
// ours, after what WriteBreakType writes)
enum BreakType : u8
{
    BREAK_TYPE_KEEP = 0, // as in the source
    BREAK_TYPE_CRLF = 1,
    BREAK_TYPE_CR = 2,
    BREAK_TYPE_LF = 3,
};

namespace detail {
// the length of the line break at c (1 or 2 characters, 0 for none)
DECOMP_NOINLINE s32 CheckBreakType(u32 c, u32 next); // 0x00351B3C | fefates:bytes [tier B]
// the line break of the type in units of unitSize bytes (the character in the last byte of a
// unit); the number of units, written only with isWrite
DECOMP_NOINLINE s32 WriteBreakType(u8* buffer, u32 unitSize, nn::enc::BreakType type, bool isWrite); // 0x00351B64 | fefates:bytes [tier B]
// the remaining lengths (-1: no limit) of the destination and the source of a conversion
DECOMP_NOINLINE nn::Result CheckParameters(bool hasDst, s32* dstLength, s32* dstRemaining, bool* isDstLimited, bool hasSrc, s32* srcLength,
                           s32* srcRemaining, bool* isSrcLimited); // 0x00351BF8 | fefates:bytes [tier B]
// the lengths are in and out (characters of the type; null or negative: up to the terminator)
nn::Result ConvertStringUtf16NativeToUtf8(u8* dst, s32* dstLength, const u16* src, s32* srcLength, nn::enc::BreakType breakType); // 0x00351C84 | fefates:bytes [tier B]
nn::Result ConvertStringUtf8ToUtf16Native(u16* dst, s32* dstLength, const u8* src, s32* srcLength, nn::enc::BreakType breakType); // 0x0035201C | fefates:bytes [tier B]
} // namespace detail
} // namespace enc
} // namespace nn
