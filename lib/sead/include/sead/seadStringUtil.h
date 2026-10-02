#pragma once

#include "decomp.h"

namespace sead {
class StringUtil
{
public:
    struct CardinalNumber { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void tryParseS32(int*, const sead::SafeStringBase<char>&, sead::StringUtil::CardinalNumber); // 0x0080EF04 | nintendogs:bytes-fuzzy [tier B]
    void tryParseU32(unsigned*, const sead::SafeStringBase<char>&, sead::StringUtil::CardinalNumber); // 0x0080F1A8 | nintendogs:bytes-fuzzy [tier B]
};
} // namespace sead
