#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
void GetCaseFoldingPair(wchar_t); // 0x003DF98C | fefates:bytes [tier B]
void CombineBuiltInCharClass(nn::ngc::CharacterRangeList*, nn::ngc::BuiltInCharClassType); // 0x003DFC78 | fefates:bytes [tier B]
void GetCharClassTypeFromName(const wchar_t*, unsigned int, unsigned int*); // 0x003DFCE0 | fefates:bytes [tier B]
} // namespace ngc
} // namespace nn
