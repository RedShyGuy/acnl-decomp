#pragma once

#include "decomp.h"

namespace pead {
class StringUtil
{
public:
    struct CardinalNumber { u32 _unknown; }; // TODO: real type unknown (placeholder)
    void tryParseS32(int*, const pead::SafeStringBase<char>&, pead::StringUtil::CardinalNumber); // 0x0080EF04 | nintendogs:bytes-fuzzy [tier B]
    void tryParseU32(unsigned*, const pead::SafeStringBase<char>&, pead::StringUtil::CardinalNumber); // 0x0080F1A8 | nintendogs:bytes-fuzzy [tier B]
};
} // namespace pead
