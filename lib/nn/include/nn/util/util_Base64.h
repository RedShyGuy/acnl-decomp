#pragma once

#include "decomp.h"
#include "nn/Result.h"

namespace nn {
namespace util {
// Base64 (the alphabet of RFC 4648, a line break CR LF after every 76 characters, padding with
// '='). The names are ours.

// writes the text with a terminating 0 to pDst (dstSize bytes)
nn::Result EncodeBase64(const u8* pSrc, u32 srcSize, char* pDst, u32 dstSize); // 0x0047F288 (name is ours)
// the data of the text (white space is skipped); *pSize: the bytes written
nn::Result DecodeBase64(const char* pSrc, u8* pDst, u32 dstSize, u32* pSize); // 0x0047F440 (name is ours)
} // namespace util
} // namespace nn
