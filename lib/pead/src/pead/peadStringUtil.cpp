#include "pead/peadStringUtil.h"

namespace pead {
// 0x0080EF04 | nintendogs:bytes-fuzzy [tier B]
void pead::StringUtil::tryParseS32(int*, const pead::SafeStringBase<char>&, pead::StringUtil::CardinalNumber)
{
}

// 0x0080F1A8 | nintendogs:bytes-fuzzy [tier B]
void pead::StringUtil::tryParseU32(unsigned*, const pead::SafeStringBase<char>&, pead::StringUtil::CardinalNumber)
{
}

} // namespace pead
