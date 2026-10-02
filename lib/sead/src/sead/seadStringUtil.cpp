#include "sead/seadStringUtil.h"

namespace sead {
// 0x0080EF04 | nintendogs:bytes-fuzzy [tier B]
void sead::StringUtil::tryParseS32(int*, const sead::SafeStringBase<char>&, sead::StringUtil::CardinalNumber)
{
}

// 0x0080F1A8 | nintendogs:bytes-fuzzy [tier B]
void sead::StringUtil::tryParseU32(unsigned*, const sead::SafeStringBase<char>&, sead::StringUtil::CardinalNumber)
{
}

} // namespace sead
