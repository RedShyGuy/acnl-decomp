#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_RegexNfaState.h"
#include "nn/ngc/ngc_RegexStateLink.h"

namespace nn {
namespace ngc {
// A state of the DFA: the set of NFA states it stands for (m_NfaStates, a bool per state), its
// transitions, and the NFA transitions that are not converted yet. Member names are ours.
class RegexDfaState
{
public:
    RegexDfaState(); // 0x003DC350 | fefates:bytes [tier B]
    bool Initialize(nn::ngc::ProfanityFilterTemporaryPool* pool, u32 nfaStateCount, bool isClear); // 0x003DC2B8 | fefates:bytes [tier B]

    UnitList<RegexStateLink<RegexDfaState> > m_Links;       // 0x00
    UnitList<RegexStateLink<RegexNfaState> > m_NfaLinks;    // 0x0C
    ProfanityFilterTemporaryPool* m_Pool;                   // 0x18
    bool* m_NfaStates;                                      // 0x1C
    u32 m_NfaStateCount;                                    // 0x20
    bool m_IsAccept;                                        // 0x24, contains the end state of the NFA
};
ASSERT_SIZE(RegexDfaState, 0x28);
} // namespace ngc
} // namespace nn
