#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/enc/detail/detail_Api.h"

namespace nn {
namespace enc {
// the conversions with the line breaks as in the source (names are ours, after the detail functions)
nn::Result ConvertStringUtf16NativeToUtf8(u8* dst, s32* dstLength, const u16* src, s32* srcLength); // 0x00351B14 (name is ours)
nn::Result ConvertStringUtf8ToUtf16Native(u16* dst, s32* dstLength, const u8* src, s32* srcLength); // 0x00351B28 (name is ours)
} // namespace enc
} // namespace nn
