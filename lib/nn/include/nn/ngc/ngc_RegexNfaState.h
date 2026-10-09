#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_RegexStateLink.h"

namespace nn {
namespace ngc {
// A state of the NFA of a regular expression. Member names are ours.
class RegexNfaState
{
public:
    RegexNfaState() : m_Index(0) {}

    UnitList<RegexStateLink<RegexNfaState> > m_Links;   // 0x0
    u32 m_Index;    // 0xC, the number in the list of the parser (set after parsing)
};
ASSERT_SIZE(RegexNfaState, 0x10);

// out of line: the parser calls the original's instance (0x007E4FC0)
template <>
UnitList<RegexStateLink<RegexNfaState> >::Iterator UnitList<RegexStateLink<RegexNfaState> >::PushBackNew();
} // namespace ngc
} // namespace nn
