#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
class RegexNfaParser
{
public:
    void PushBackNewState(); // 0x003DC864 | fefates:bytes [tier B]
    void ParseCharClassInner(nn::ngc::CharacterRangeList*); // 0x003DC8CC | fefates:bytes [tier B]
    void ParseClass_RegisterChar(nn::ngc::CharacterRangeList*, wchar_t); // 0x003DD4D0 | fefates:bytes [tier B]
    void Parse(nn::ngc::ProfanityFilterTemporaryPool*, const nn::ngc::UnitList<nn::ngc::RegexToken>*); // 0x003DE0E0 | fefates:bytes [tier B]
    RegexNfaParser(); // 0x003DE19C | fefates:bytes [tier B]
};
} // namespace ngc
} // namespace nn
