#pragma once

#include "decomp.h"
#include "nn/ngc/ngc_RegexDfaState.h"
#include "nn/ngc/ngc_RegexNfaState.h"
#include "nn/ngc/ngc_UnitList.h"

namespace nn {
namespace ngc {
// Converts the NFA of the parser into a DFA (subset construction). Member names are ours.
class RegexDfaConverter
{
public:
    typedef UnitList<RegexDfaState>::Iterator DfaIterator;

    RegexDfaConverter(); // 0x003DF038 | fefates:bytes [tier B]
    bool Convert(nn::ngc::ProfanityFilterTemporaryPool* pool, const nn::ngc::UnitList<nn::ngc::RegexNfaState>* nfaStates,
                 nn::ngc::UnitList<nn::ngc::RegexNfaState>::ConstIterator nfaEnd); // 0x003DEDDC | fefates:bytes [tier B]

    // the DFA state for the set of NFA states (made if it does not exist); null without memory
    DfaIterator RegisterDfaState(const bool* nfaStates); // 0x003DE1C4 | fefates:bytes [tier B]
    void GatherEpsilonLinks(bool* nfaStates); // 0x003DE2B8 | fefates:bytes [tier B]
    bool CreateCharClassLink(DfaIterator state); // 0x003DE33C | fefates:bytes [tier B]
    bool CreateCharacterLink(DfaIterator state); // 0x003DE870 | fefates:bytes [tier B]
    bool CopyLinksFromNfaStates(DfaIterator state); // 0x003DEB20 | fefates:bytes [tier B]
    bool CreateAtomicZeroWidthAssersionLink(DfaIterator state); // 0x003DEBF8 | fefates:bytes [tier B]

    ProfanityFilterTemporaryPool* m_Pool;               // 0x00
    UnitList<RegexDfaState> m_DfaStates;                // 0x04, the first one is the start state
    u32 m_NfaStateCount;                                // 0x10
    UnitList<RegexNfaState>::ConstIterator* m_NfaStates;    // 0x14, by RegexNfaState::m_Index
};
ASSERT_SIZE(RegexDfaConverter, 0x18);
} // namespace ngc
} // namespace nn
