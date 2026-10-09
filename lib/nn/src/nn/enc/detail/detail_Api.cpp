#include "nn/enc/detail/detail_Api.h"
#include <string.h>

namespace nn {
namespace enc {
namespace detail {
namespace {
// results (module 81; the names are ours)
const bit32 RESULT_INVALID_ARGUMENT = 0xE0E14402;  // usage, invalid argument, 2
const bit32 RESULT_INVALID_CHARACTER = 0xD8E14403; // permanent, invalid argument, 3
const bit32 RESULT_BUFFER_TOO_SMALL = 0xD9014401;  // permanent, wrong argument, 1

const u32 CHARACTER_LF = '\n';
const u32 CHARACTER_CR = '\r';

// byte order marks
const u16 UTF16_BOM = 0xFEFF;
const u16 UTF16_BOM_SWAPPED = 0xFFFE;
const u8 UTF8_BOM[] = {0xEF, 0xBB, 0xBF};
} // namespace

// 0x00351B3C | fefates:bytes [tier B]
s32 CheckBreakType(u32 c, u32 next)
{
    if (c == CHARACTER_LF) {
        return 1;
    }
    if (c != CHARACTER_CR) {
        return 0;
    }
    if (next == CHARACTER_LF) {
        return 2;
    }
    return 1;
}

// 0x00351B64 | fefates:bytes [tier B]
s32 WriteBreakType(u8* buffer, u32 unitSize, nn::enc::BreakType type, bool isWrite)
{
    if (!isWrite) {
        if (type == BREAK_TYPE_CRLF) {
            return 2;
        }
        if (type == BREAK_TYPE_CR || type == BREAK_TYPE_LF) {
            return 1;
        }
        return 0;
    }
    memset(buffer, 0, unitSize - 1);
    switch (type) {
    case BREAK_TYPE_CRLF:
        buffer[unitSize - 1] = CHARACTER_CR;
        memset(buffer + unitSize, 0, unitSize - 1);
        buffer[unitSize * 2 - 1] = CHARACTER_LF;
        return 2;
    case BREAK_TYPE_CR:
        buffer[unitSize - 1] = CHARACTER_CR;
        return 1;
    case BREAK_TYPE_LF:
        buffer[unitSize - 1] = CHARACTER_LF;
        return 1;
    default:
        return 0;
    }
}

// 0x00351BF8 | fefates:bytes [tier B]
nn::Result CheckParameters(bool hasDst, s32* dstLength, s32* dstRemaining, bool* isDstLimited, bool hasSrc, s32* srcLength,
                           s32* srcRemaining, bool* isSrcLimited)
{
    nn::Result result;
    if (srcLength != 0) {
        *srcRemaining = *srcLength;
    } else {
        *srcRemaining = -1;
    }
    if (dstLength != 0) {
        *dstRemaining = *dstLength;
    } else {
        result = RESULT_INVALID_ARGUMENT;
        *dstRemaining = -1;
    }
    if (!hasSrc) {
        result = RESULT_INVALID_ARGUMENT;
    }
    if (!hasDst) {
        // only counted
        *isDstLimited = false;
        *dstRemaining = -1;
    }
    if (*srcRemaining < 0) {
        *isSrcLimited = false;
    }
    if (result.IsFailure()) {
        if (dstLength != 0) {
            *dstLength = 0;
        }
        if (srcLength != 0) {
            *srcLength = 0;
        }
    }
    return result;
}

// 0x00351C84 | fefates:bytes [tier B]
nn::Result ConvertStringUtf16NativeToUtf8(u8* dst, s32* dstLength, const u16* src, s32* srcLength, nn::enc::BreakType breakType)
{
    bool isDstLimited = true;
    bool isSrcLimited = true;
    s32 srcRemaining = -1;
    s32 dstRemaining = -1;
    s32 srcCount = 0;
    s32 dstCount = 0;
    nn::Result result = CheckParameters(dst != 0, dstLength, &dstRemaining, &isDstLimited, src != 0, srcLength, &srcRemaining, &isSrcLimited);
    if (result.IsFailure()) {
        return result;
    }
    // no byte order mark
    if ((srcRemaining > 0 || !isSrcLimited) && (*src == UTF16_BOM_SWAPPED || *src == UTF16_BOM)) {
        if (dstLength != 0) {
            *dstLength = 0;
        }
        *srcLength = 0;
        return RESULT_INVALID_CHARACTER;
    }
    while (*src != 0 && (srcCount < srcRemaining || !isSrcLimited)) {
        u32 c = *src;
        if (dstCount >= dstRemaining && isDstLimited) {
            result = RESULT_BUFFER_TOO_SMALL;
            break;
        }
        if (breakType != BREAK_TYPE_KEEP) {
            u32 next = (srcRemaining - srcCount > 1 || !isSrcLimited) ? src[1] : 0;
            s32 length = CheckBreakType(c, next);
            if (length > 0) {
                s32 written = WriteBreakType(dst, sizeof(u8), breakType, isDstLimited);
                if (dstRemaining - dstCount < written && isDstLimited) {
                    result = RESULT_BUFFER_TOO_SMALL;
                    break;
                }
                dstCount += written;
                src += length;
                srcCount += length;
                if (isDstLimited) {
                    dst += written;
                }
                continue;
            }
        }
        if (c < 0x80) {
            dstCount++;
            src++;
            srcCount++;
            if (isDstLimited) {
                *dst++ = c;
            }
            continue;
        }
        s32 units;
        s32 bytes;
        if (c < 0x800) {
            units = 1;
            bytes = 2;
        } else if ((c & 0xF800) == 0xD800) {
            // a surrogate pair
            units = 2;
            bytes = 4;
        } else {
            units = 1;
            bytes = 3;
        }
        if (srcRemaining - srcCount < units && isSrcLimited) {
            break;
        }
        if (isDstLimited) {
            if (bytes > dstRemaining - dstCount) {
                result = RESULT_BUFFER_TOO_SMALL;
                break;
            }
            u32 codePoint;
            if (units == 1) {
                codePoint = src[0];
            } else if (units == 2) {
                // (the original also clears bit 4 of the low surrogate)
                codePoint = ((src[0] - 0xD800) << 10) + 0x10000 + (src[1] & ~0xFC10);
            } else {
                codePoint = 0;
            }
            switch (bytes) {
            case 1:
                dst[0] = codePoint;
                break;
            case 2:
                dst[0] = 0xC0 + (codePoint >> 6);
                break;
            case 3:
                dst[0] = 0xE0 + (codePoint >> 12);
                break;
            case 4:
                dst[0] = 0xF0 + (codePoint >> 18);
                break;
            }
            for (s32 i = 1; i < bytes; i++) {
                dst[i] = 0x80 + ((codePoint >> (6 * (bytes - 1 - i))) & 0x3F);
            }
            dst += bytes;
        }
        src += units;
        srcCount += units;
        dstCount += bytes;
    }
    if (srcLength != 0) {
        *srcLength = srcCount;
    }
    if (dstLength != 0) {
        *dstLength = dstCount;
    }
    return result;
}

// 0x0035201C | fefates:bytes [tier B]
nn::Result ConvertStringUtf8ToUtf16Native(u16* dst, s32* dstLength, const u8* src, s32* srcLength, nn::enc::BreakType breakType)
{
    bool isDstLimited = true;
    bool isSrcLimited = true;
    s32 srcRemaining = -1;
    s32 dstRemaining = -1;
    s32 srcCount = 0;
    s32 dstCount = 0;
    nn::Result result = CheckParameters(dst != 0, dstLength, &dstRemaining, &isDstLimited, src != 0, srcLength, &srcRemaining, &isSrcLimited);
    if (result.IsFailure()) {
        return result;
    }
    // a byte order mark is skipped
    if ((srcRemaining >= static_cast<s32>(sizeof(UTF8_BOM)) || !isSrcLimited) && src[0] == UTF8_BOM[0] && src[1] == UTF8_BOM[1] &&
        src[2] == UTF8_BOM[2]) {
        src += sizeof(UTF8_BOM);
        srcCount = sizeof(UTF8_BOM);
    }
    while (*src != 0 && (srcCount < srcRemaining || !isSrcLimited)) {
        u32 c = *src;
        if (dstCount >= dstRemaining && isDstLimited) {
            result = RESULT_BUFFER_TOO_SMALL;
            break;
        }
        if (breakType != BREAK_TYPE_KEEP) {
            u32 next = (srcRemaining - srcCount > 1 || !isSrcLimited) ? src[1] : 0;
            s32 length = CheckBreakType(c, next);
            if (length > 0) {
                s32 written = WriteBreakType(reinterpret_cast<u8*>(dst), sizeof(u16), breakType, isDstLimited);
                if (dstRemaining - dstCount < written && isDstLimited) {
                    result = RESULT_BUFFER_TOO_SMALL;
                    break;
                }
                dstCount += written;
                src += length;
                srcCount += length;
                if (isDstLimited) {
                    dst += written;
                }
                continue;
            }
        }
        if (c < 0x80) {
            dstCount++;
            src++;
            srcCount++;
            if (isDstLimited) {
                *dst++ = c;
            }
            continue;
        }
        s32 bytes;
        s32 units;
        if ((c & 0xE0) == 0xC0) {
            bytes = 2;
            units = 1;
        } else if ((c & 0xF0) == 0xE0) {
            bytes = 3;
            units = 1;
        } else if ((c & 0xF8) == 0xF0) {
            bytes = 4;
            units = 2;
        } else {
            result = RESULT_INVALID_CHARACTER;
            break;
        }
        if (srcRemaining - srcCount < bytes && isSrcLimited) {
            break;
        }
        u32 codePoint;
        switch (bytes) {
        case 1:
            codePoint = src[0];
            break;
        case 2:
            codePoint = src[0] & 0x1F;
            break;
        case 3:
            codePoint = src[0] & 0x0F;
            break;
        case 4:
            codePoint = src[0] & 0x07;
            break;
        default:
            result = RESULT_INVALID_CHARACTER;
            goto end;
        }
        for (s32 i = 1; i < bytes; i++) {
            if ((src[i] & 0xC0) != 0x80) {
                result = RESULT_INVALID_CHARACTER;
                goto end;
            }
            codePoint = (codePoint << 6) + (src[i] & 0x3F);
        }
        if (codePoint == 0) {
            result = RESULT_INVALID_CHARACTER;
            break;
        }
        if (isDstLimited) {
            if (units > dstRemaining - dstCount) {
                result = RESULT_BUFFER_TOO_SMALL;
                break;
            }
            if (units == 1) {
                dst[0] = codePoint;
            } else if (units == 2) {
                dst[0] = 0xD7C0 + (codePoint >> 10);
                dst[1] = 0xDC00 + (codePoint & 0x3FF);
            }
            dst += units;
        }
        dstCount += units;
        src += bytes;
        srcCount += bytes;
    }
end:
    if (srcLength != 0) {
        *srcLength = srcCount;
    }
    if (dstLength != 0) {
        *dstLength = dstCount;
    }
    return result;
}

} // namespace detail
} // namespace enc
} // namespace nn
