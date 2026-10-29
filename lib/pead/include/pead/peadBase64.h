#pragma once

#include "decomp.h"

namespace pead {
// Base64 of the text stream format (memory blocks; pia's copy of our sead::Base64). The class and all names are
// ours.
class Base64
{
public:
    // size bytes of src as text in dst (4 characters per 3 bytes, padded with '='; not terminated);
    // isUrlSafe: '-' and '_' instead of '+' and '/'
    static void encode(char* dst, const void* src, s32 size, bool isUrlSafe); // 0x0053BDA8 (name is ours)
    // the bytes of the text src (characters below ' ' are skipped, both alphabets are accepted) into
    // dst; false if dst is too small or the text is cut
    static bool decode(void* dst, s32 dstSize, const char* src, s32 srcSize, s32* decodedSize); // 0x0053BB78 (name is ours)
};
} // namespace pead
