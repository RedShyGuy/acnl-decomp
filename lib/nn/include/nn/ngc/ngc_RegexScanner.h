#pragma once

#include "decomp.h"

namespace nn {
namespace ngc {
class RegexScanner
{
public:
    void LexicalAnalysis(nn::ngc::ProfanityFilterTemporaryPool*, const wchar_t*, unsigned int); // 0x003DB3BC | fefates:bytes [tier B]
    RegexScanner(); // 0x003DC294 | fefates:bytes [tier B]
};
} // namespace ngc
} // namespace nn
