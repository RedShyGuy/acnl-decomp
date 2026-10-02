#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
namespace CTR {
// RTTI N2nn3ngc3CTR19ProfanityFilterBaseE @ 0x008CF7BC
class ProfanityFilterBase
{
public:
    ProfanityFilterBase(); // ctor address unknown
    void IsIncludesAtSign(const wchar_t*, int); // 0x003E1640 | fefates:bytes [tier B]
    void ConvertUserInputForWord(wchar_t*, int, const wchar_t*); // 0x003E192C | fefates:bytes [tier B]
    void GetPatternListsFromRegion(nn::ngc::CTR::ProfanityFilterPatternList*, int*, bool); // 0x003E1BA8 | fefates:bytes-fuzzy [tier B]
};
} // namespace CTR
} // namespace ngc
} // namespace nn
