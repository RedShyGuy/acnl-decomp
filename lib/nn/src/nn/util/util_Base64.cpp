#include "nn/util/util_Base64.h"

namespace nn {
namespace util {
namespace {
// results (module 2; the names are ours)
const bit32 RESULT_INVALID_POINTER = 0xE0E00BF6;   // usage, invalid pointer (1014)
const bit32 RESULT_BUFFER_TOO_SMALL = 0xD8600801;  // permanent, out of resource, 1
const bit32 RESULT_INVALID_DATA = 0xD9000802;      // permanent, invalid result value, 2

// the characters of a line
const u32 LINE_LENGTH = 76;

// the values of the characters: -1 not allowed, -2 skipped (white space), -3 the padding
const s8 DECODE_TABLE_INVALID = -1;
const s8 DECODE_TABLE_SKIP = -2;
const s8 DECODE_TABLE_PADDING = -3;

// 0x008A3838
const char ENCODE_TABLE[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
// 0x008A3879
const s8 DECODE_TABLE[128] = {
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -2, -2, -1, -1, -2, -1, -1,
    -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
    -2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
    52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -3, -1, -1,
    -1, 0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14,
    15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
    -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
    41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
};

// one character of the text, a line break before it every LINE_LENGTH characters; false if the
// buffer is full (inline)
inline bool PutCharacter(char* pDst, u32 dstSize, u32* pPosition, u32* pLineLength, char c)
{
    if (*pLineLength == LINE_LENGTH) {
        if (*pPosition == dstSize) {
            return false;
        }
        pDst[(*pPosition)++] = '\r';
        if (*pPosition == dstSize) {
            return false;
        }
        pDst[(*pPosition)++] = '\n';
        *pLineLength = 0;
    }
    if (*pPosition == dstSize) {
        return false;
    }
    pDst[(*pPosition)++] = c;
    (*pLineLength)++;
    return true;
}

// the failure of DecodeBase64: the buffer is full or the text is bad (inline)
inline nn::Result GetDecodeError(u32 size, u32 dstSize)
{
    return nn::Result(size + 3 > dstSize ? RESULT_BUFFER_TOO_SMALL : RESULT_INVALID_DATA);
}
} // namespace

// 0x0047F288 (name is ours)
nn::Result EncodeBase64(const u8* pSrc, u32 srcSize, char* pDst, u32 dstSize)
{
    if (pSrc == nullptr || pDst == nullptr) {
        return nn::Result(RESULT_INVALID_POINTER);
    }
    u32 lineLength = 0;
    u32 position = 0;
    u32 bits = 0;
    s32 bitNum = 0;
    for (u32 i = 0; i < srcSize; i++) {
        bitNum += 2;
        bits = pSrc[i] | (bits << 8);
        if (!PutCharacter(pDst, dstSize, &position, &lineLength, ENCODE_TABLE[(static_cast<s32>(bits) >> bitNum) & 0x3F])) {
            return nn::Result(RESULT_BUFFER_TOO_SMALL);
        }
        if (bitNum >= 6) {
            bitNum -= 6;
            if (!PutCharacter(pDst, dstSize, &position, &lineLength, ENCODE_TABLE[(static_cast<s32>(bits) >> bitNum) & 0x3F])) {
                return nn::Result(RESULT_BUFFER_TOO_SMALL);
            }
        }
    }
    if (bitNum > 0) {
        // the rest of the bits, filled up with zeros, then the padding
        u32 rest = bits << (6 - bitNum);
        if (!PutCharacter(pDst, dstSize, &position, &lineLength, ENCODE_TABLE[rest & 0x3F])) {
            return nn::Result(RESULT_BUFFER_TOO_SMALL);
        }
        while (lineLength & 3) {
            if (position == dstSize) {
                return nn::Result(RESULT_BUFFER_TOO_SMALL);
            }
            lineLength++;
            pDst[position++] = '=';
        }
    }
    if (position == dstSize) {
        return nn::Result(RESULT_BUFFER_TOO_SMALL);
    }
    pDst[position] = '\0';
    return nn::Result();
}

// 0x0047F440 (name is ours)
nn::Result DecodeBase64(const char* pSrc, u8* pDst, u32 dstSize, u32* pSize)
{
    if (pSize == nullptr) {
        return nn::Result(RESULT_INVALID_POINTER);
    }
    if (pSrc == nullptr || pDst == nullptr) {
        *pSize = 0;
        return nn::Result(RESULT_INVALID_POINTER);
    }
    *pSize = 0;
    s8 quad[4];
    u32 num = 0;
    while (*pSrc != 0) {
        s8 c = *pSrc;
        if (c < 0) {
            return GetDecodeError(*pSize, dstSize);
        }
        s8 value = DECODE_TABLE[c];
        pSrc++;
        if (value == DECODE_TABLE_PADDING) {
            // the end: one or two bytes of the last group
            if (num == 1) {
                return GetDecodeError(*pSize, dstSize);
            }
            if (num == 2) {
                if (*pSize + 1 > dstSize) {
                    return GetDecodeError(*pSize, dstSize);
                }
                pDst[0] = (quad[0] << 2) | (quad[1] >> 4);
                *pSize += 1;
            } else if (num == 3) {
                if (*pSize + 2 > dstSize) {
                    return GetDecodeError(*pSize, dstSize);
                }
                pDst[0] = (quad[0] << 2) | (quad[1] >> 4);
                pDst[1] = (quad[1] << 4) | (quad[2] >> 2);
                *pSize += 2;
            }
            return nn::Result();
        }
        if (value == DECODE_TABLE_SKIP) {
            continue;
        }
        if (value == DECODE_TABLE_INVALID) {
            return GetDecodeError(*pSize, dstSize);
        }
        quad[num++] = value;
        if (num == 4) {
            if (*pSize + 3 > dstSize) {
                return GetDecodeError(*pSize, dstSize);
            }
            num = 0;
            pDst[0] = (quad[0] << 2) | (quad[1] >> 4);
            pDst[1] = (quad[1] << 4) | (quad[2] >> 2);
            pDst[2] = quad[3] | (quad[2] << 6);
            pDst += 3;
            *pSize += 3;
        }
    }
    if (num != 0) {
        return GetDecodeError(*pSize, dstSize);
    }
    return nn::Result();
}
} // namespace util
} // namespace nn
