#include "nn/enc/enc_Api.h"

namespace nn {
namespace enc {

// 0x00351B14 (name is ours)
nn::Result ConvertStringUtf16NativeToUtf8(u8* dst, s32* dstLength, const u16* src, s32* srcLength)
{
    return detail::ConvertStringUtf16NativeToUtf8(dst, dstLength, src, srcLength, BREAK_TYPE_KEEP);
}

// 0x00351B28 (name is ours)
nn::Result ConvertStringUtf8ToUtf16Native(u16* dst, s32* dstLength, const u8* src, s32* srcLength)
{
    return detail::ConvertStringUtf8ToUtf16Native(dst, dstLength, src, srcLength, BREAK_TYPE_KEEP);
}

} // namespace enc
} // namespace nn
