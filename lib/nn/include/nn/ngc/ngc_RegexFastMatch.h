#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_RegexToken.h"
#include "nn/ngc/ngc_UnitList.h"

namespace nn {
namespace ngc {
// Matches the simple patterns (literal text with .* or ^ / $ at the ends) without building an
// automaton. Member names are ours.
class RegexFastMatch
{
public:
    typedef UnitList<RegexToken>::ConstIterator TokenIterator;

    static const int RESULT_NOT_SIMPLE = 0;
    static const int RESULT_MATCH = 1;
    static const int RESULT_NO_MATCH = 2;

    RegexFastMatch(); // 0x003DC75C | fefates:callgraph [tier C]
    bool IsMatchString(TokenIterator token, unsigned int start, unsigned int length); // 0x003DC384 | fefates:bytes [tier B]
    int MatchFast(const nn::ngc::UnitList<nn::ngc::RegexToken>* tokens, const wchar_t* text); // 0x003DC43C | fefates:bytes [tier B]

    const wchar_t* m_Text;  // 0x0
    u32 m_Length;           // 0x4
};
ASSERT_SIZE(RegexFastMatch, 0x8);
} // namespace ngc
} // namespace nn
