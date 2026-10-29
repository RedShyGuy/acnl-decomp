#include "sead/seadBase64.h"

namespace sead {
namespace {
// the 6-bit value of a character of either alphabet, 64 for anything else ('=')
inline u32 decodeChar(s32 c)
{
    if (c - 'A' <= 25u) {
        return c - 'A';
    }
    if (c - 'a' <= 25u) {
        return c - 'a' + 26;
    }
    if (c - '0' <= 9u) {
        return c - '0' + 52;
    }
    if (c == '+' || c == '-') {
        return 62;
    }
    if (c == '/' || c == '_') {
        return 63;
    }
    return 64;
}
} // namespace

// 0x0055D224 (name is ours)
bool sead::Base64::decode(void* dst, s32 dstSize, const char* src, s32 srcSize, s32* decodedSize)
{
    u8* out = static_cast<u8*>(dst);
    s32 outSize = 0;
    for (s32 i = 0; i < srcSize;) {
        if (*src < ' ') {
            // line breaks and other control characters
            i++;
            src++;
            continue;
        }
        if (i + 4 > srcSize) {
            return false;
        }
        u32 v0 = decodeChar(src[0]) & 0x3F;
        u32 v1 = decodeChar(src[1]) & 0x3F;
        u32 v2 = decodeChar(src[2]) & 0x7F;
        u32 v3 = decodeChar(src[3]) & 0x7F;
        src += 4;
        if (outSize >= dstSize) {
            return false;
        }
        out[outSize++] = v0 << 2 | v1 >> 4;
        if (v2 < 64) {
            if (outSize >= dstSize) {
                return false;
            }
            out[outSize++] = v1 << 4 | v2 >> 2;
            if (v3 < 64) {
                if (outSize >= dstSize) {
                    return false;
                }
                out[outSize++] = v2 << 6 | v3;
            }
        }
        i += 4;
    }
    if (decodedSize != nullptr) {
        *decodedSize = outSize;
    }
    return true;
}

// 0x0055D454 (name is ours)
void sead::Base64::encode(char* dst, const void* src, s32 size, bool isUrlSafe)
{
    const char* table = isUrlSafe ? "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_"
                                  : "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    const u8* in = static_cast<const u8*>(src);
    s32 fullSize = size / 3 * 3;
    for (s32 i = 0; i < fullSize; i += 3) {
        dst[0] = table[in[0] >> 2];
        dst[1] = table[(in[0] & 3) << 4 | in[1] >> 4];
        dst[2] = table[(in[1] & 0xF) << 2 | in[2] >> 6];
        dst[3] = table[in[2] & 0x3F];
        dst += 4;
        in += 3;
    }
    switch (size - fullSize) {
    case 1:
        dst[0] = table[in[0] >> 2];
        dst[1] = table[(in[0] & 3) << 4];
        dst[2] = '=';
        dst[3] = '=';
        break;
    case 2:
        dst[0] = table[in[0] >> 2];
        dst[1] = table[(in[0] & 3) << 4 | in[1] >> 4];
        dst[2] = table[(in[1] & 0xF) << 2];
        dst[3] = '=';
        break;
    }
}

} // namespace sead
